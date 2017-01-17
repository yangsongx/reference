#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <libwebsockets.h>

struct session_data {
    int fd;
};

struct pthread_routine_tool {
    struct lws_context *context;
    struct lws *wsi;
};

static int websocket_write_back(struct lws *wsi_in, char *str, int str_size_in) 
{
    if (str == NULL || wsi_in == NULL)
        return -1;

    int n;
    int len;
    unsigned char *out = NULL;

    if (str_size_in < 1) 
        len = strlen(str);
    else
        len = str_size_in;

    out = (unsigned char *)malloc(sizeof(char)*(LWS_SEND_BUFFER_PRE_PADDING + len + LWS_SEND_BUFFER_POST_PADDING));

    memcpy (out + LWS_SEND_BUFFER_PRE_PADDING, str, len );
    n = lws_write(wsi_in, out + LWS_SEND_BUFFER_PRE_PADDING, len, LWS_WRITE_TEXT);

    printf("write back:%s\n", str);
    free(out);

    return n;
}
static int ws_service_callback(
                         struct lws *wsi,
                         enum lws_callback_reasons reason, void *user,
                         void *in, size_t len)
{

    switch (reason) {

        case LWS_CALLBACK_CLIENT_ESTABLISHED:
            printf("CONN ESTABLISHED\n");
break;
        case LWS_CALLBACK_CLOSED:
            printf("GO");
            break;

        case LWS_CALLBACK_CLIENT_RECEIVE:
            printf("WOW");
            break;
        case LWS_CALLBACK_CLIENT_WRITEABLE :
            printf("HAHA");
            break;

default:
;
    }
    return 0;

}

int main(int argc, char **argv)
{
    struct lws_context *context = NULL;
    struct lws_context_creation_info info;
    struct lws *wsi = NULL;
    struct lws_protocols protocol;


  memset(&info, 0, sizeof info);
    info.port = CONTEXT_PORT_NO_LISTEN; // NOTE - NOT server port
    info.iface = NULL;
    info.protocols = &protocol;
    info.ssl_cert_filepath = NULL;
    info.ssl_private_key_filepath = NULL;
    info.extensions = lws_get_internal_extensions();
    info.options = 0;

    protocol.name  = "my-echo-protocol";
    protocol.callback = &ws_service_callback;
    protocol.per_session_data_size = sizeof(struct session_data);
    protocol.rx_buffer_size = 0;

    context = lws_create_context(&info);
    assert(context != NULL);

    wsi = lws_client_connect(context, "localhost", 9191, 0,
    "/", "localhost:9191", NULL,
    protocol.name, -1);
    assert(wsi != NULL);
    while(1){
lws_service(context, 50);
}
    lws_context_destroy(context);

    return 0;
}
