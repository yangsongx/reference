/**
 * This code is originally selected from:
 *
 * http://stackoverflow.com/questions/30904560/libwebsocket-client-example
 * 
 */
#include <stdio.h>
#include <assert.h>
#include <libwebsockets.h>

struct per_session_data {
    int fd;
};

static int ws_service_callback(struct lws *wsi,
        enum lws_callback_reasons reason, void *user,
        void *in, size_t len)
{
    switch(reason){
        case LWS_CALLBACK_ESTABLISHED:
            char buf[32];
            printf("the buf:%s\n", lws_wsi_user(wsi));
            lws_hdr_copy_fragment(wsi, buf, sizeof(buf), WSI_TOKEN_HTTP_URI_ARGS, 20);
            printf("the buf:%s\n", lws_wsi_user(wsi));

            printf("SERVER CONN ESTABLISHED\n");
            if (len > 0){
                printf("coming with in(%s), len=%ld\n",
                    (char *)in, len);
            }
            break;

        case LWS_CALLBACK_RECEIVE:
            printf("LWS cb recv\n");
            printf("coming with %s, %s, %ld\n",
                    user, in, len);
            break;

        case LWS_CALLBACK_CLOSED:
            printf("closed\n");
            printf("coming with (%s) (%s), %ld\n",
                    user, in, len);
            break;

        default:
            break;
    }
    return 0;
}

int main(int argc, char **argv)
{
    const char *interface = NULL;

    struct lws_context_creation_info info;
    struct lws_protocols protocol;
    struct lws_context *context;


    /* FIXME - how did this defined? */
    protocol.name = "my-echo-protocol";
    protocol.callback = ws_service_callback;
    protocol.per_session_data_size=sizeof(struct per_session_data);
    protocol.rx_buffer_size = 0;

    memset(&info, 0, sizeof info);
    info.port = 9191; // default port
    info.iface = interface;
    info.extensions = lws_get_internal_extensions();
    info.protocols = &protocol;
    /* FIXME below for HTTPS?
    info.ssl_cert_filepath = cert_path;
    info.ssl_private_key_filepath = key_path;
    */

    context = lws_create_context(&info);
    assert(context != NULL);

    while(1){
        lws_service(context, 50);
    }

    lws_context_destroy(context);

    return 0;
}
