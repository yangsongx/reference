/**
 * Show how to use Zero Message Queue (Server Side)
 */
#include <stdio.h>
#include <getopt.h>
#include <signal.h>
#include <unistd.h>
#include <zmq.h>

//#include <google/protobuf/io/coded_stream.h>
//#include <google/protobuf/io/zero_copy_stream_impl_lite.h>

#include "datamsg.pb.h"

/* defined by gen_random.cc */
extern int random_int(int from, int to);

void *glb_zmq_ctx = NULL;
void *glb_pub_s = NULL; // publisher socket


/**
 *@s: the ZMQ socket representation
 */
void try_send_back(void *s, mqdemo::ReqAction *req)
{
    mqdemo::ReqResult robj;
    printf("the req type = %d\n", req->type());
    robj.set_code(2*(req->type()));
    std::string prepend;
    if(req->name().find("python") != std::string::npos){
        prepend = "I know you are Python! " + req->name();
    } else {
        prepend = "C++" + req->name();
    }
    robj.set_msg(prepend);
    int i = 0;
    int rlen = robj.ByteSize();
    printf("the response object len=%d\n", rlen);

    char *buf = (char *)malloc(rlen);
    zmq_msg_t msg;

    if(robj.SerializeToArray(buf, rlen) == true){
        printf("=== Serialize obj [OK] ===\n");
        zmq_msg_init_size(&msg, rlen);
        memcpy(zmq_msg_data(&msg),
           buf, rlen);

        i = zmq_msg_send(&msg, s, 0);
        printf("Totally %d bytes wroten\n", i);

        zmq_msg_close(&msg);
    } else {
        printf("****Failed serialize the obj\n");
    }

//    zmq_send(s, "world", 5, 0);

    free(buf);
}

void try_get_reqmsg(void *s, mqdemo::ReqAction *obj)
{
    zmq_msg_t msg;
    zmq_msg_init(&msg);

    int size = zmq_msg_recv(&msg, s, 0);
    if(size == -1) {
        printf("the data len is in-valid!");
        return;
    }
    char *buf = (char *) malloc(size);
    memcpy(buf, zmq_msg_data(&msg), size);

    // buf should be the object
    if(obj->ParseFromArray(buf, size) == true){
        printf("Good, protobuf obj parsed OK\n");
        printf("\tThe type:%d, the name:%s\n", obj->type(), obj->name().c_str());
    } else {
        printf("**failed parse the protobuf obj\n");
    }

    zmq_msg_close(&msg);
    free(buf);
}
/**
 *@s: the ZMQ socket representation
 */
void handle_client_request(void *s)
{
    mqdemo::ReqAction obj;
    try_get_reqmsg(s, &obj);
    // next try reponse sth to caller...
    try_send_back(s, &obj);

    return;
}

void sig_hdl(int signo)
{
    printf("==SIG HANDLER==(for %d)\n", signo);

    if(glb_pub_s)
        zmq_close(glb_pub_s);
    if(glb_zmq_ctx)
        zmq_ctx_destroy(glb_zmq_ctx);

    printf("==SIG HANDLER== finished res cleanup\n");
    exit(0);
}

void normal_mode(void *ctx)
{
    void *mq_s = zmq_socket(ctx, ZMQ_REP);
    if(zmq_bind(mq_s, "tcp://*:5555") != 0) {
        printf("*** failed bind the port on Server\n");
        printf("***    error number:%d\n", errno);
    } else {
        printf("=== Cool, 5555 port bind [OK]\n");
    }

    while(1) {
        handle_client_request(mq_s);
    }
}

/**
 * Publish a msg out.
 *
 *@sock : the publisher socket
 */
void trigger_a_publish_msg(void *sock, int typecode)
{
    mqdemo::ReqAction pub_data;
    char buffer[64];
    time_t t;
    char  *buf;
    int    len;
    int    i;
    zmq_msg_t msg;

    time(&t);
    snprintf(buffer, sizeof(buffer),
            "msg out at %s", ctime(&t));
    pub_data.set_type(typecode);
    pub_data.set_name(buffer);

    len = pub_data.ByteSize();
    buf = (char *)malloc(len);
    if(pub_data.SerializeToArray(buf, len) == true){
        printf("--- serialization OK, prepare publish out...\n");
        zmq_msg_init(&msg);
        zmq_msg_init_size(&msg, len);
        memcpy(zmq_msg_data(&msg),
                buf, len);
        i = zmq_msg_send(&msg, sock, 0);
        if(i != len) {
            printf("***Warning, sent size (%d), expected size(%d)\n",
                    i, len);
        }

        zmq_msg_close(&msg);
    } else {
        printf("*** seems serialization failed, ignore this publish\n");
    }

    free(buf);
}

void publish_mode(void *ctx)
{
    int interval = 2;
    glb_pub_s = zmq_socket(ctx, ZMQ_PUB);
    if(!glb_pub_s){
        printf("**failed create PUB socket(%d)\n", errno);
        return;
    }

    printf("--->publish socket creation[OK]\n");
    if(zmq_bind(glb_pub_s, "tcp://*:5556") != 0){
        printf("failed bind pub socket(%d)\n", errno);
        zmq_close(glb_pub_s);
        return;
    }

    // Now, try publish any msg at will
    while(1){
        interval = random_int(2, 10);
        sleep(interval);

        trigger_a_publish_msg(glb_pub_s, interval);
    }

    printf("--->publish socket bind[OK]\n");

}

int main(int argc, char **argv)
{
    int c;
    int pub_mode = 0;

    srand(time(NULL));

    signal(SIGINT, sig_hdl);
    signal(SIGTERM, sig_hdl);

    glb_zmq_ctx = zmq_ctx_new();
    if(!glb_zmq_ctx) {
        printf("*** failed get the ZMQ resource!(%d)\n", errno);
        return -1;
    }

    while((c = getopt(argc, argv, "p")) != -1) {
        switch(c){
            case 'p':
                pub_mode = 1;
                break;

            default:
                break;
        }
    }

    if(pub_mode) {
        printf("Server is trying run as Publish-Subscriber mode...\n");
        publish_mode(glb_zmq_ctx);
    } else {
        normal_mode(glb_zmq_ctx);
    }

    zmq_ctx_destroy(glb_zmq_ctx);

    return 0;
}
