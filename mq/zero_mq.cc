/**
 * Show how to use Zero Message Queue (Server Side)
 */
#include <stdio.h>
#include <zmq.h>

//#include <google/protobuf/io/coded_stream.h>
//#include <google/protobuf/io/zero_copy_stream_impl_lite.h>

#include "datamsg.pb.h"

/**
 *@s: the ZMQ socket representation
 */
void try_send_back(void *s, mqdemo::ReqAction *req)
{
    mqdemo::ReqResult robj;
    printf("the req type = %d\n", req->type());
    robj.set_code(2*(req->type()));
    std::string prepend = "C++" + req->name();
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

    zmq_send(s, "world", 5, 0);

    free(buf);
}
// NOTE - DO NOT USE below function
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
#if 1
    mqdemo::ReqAction obj;
    try_get_reqmsg(s, &obj);
#else
    zmq_msg_t msg;
    zmq_msg_init(&msg);

    int size = zmq_msg_recv(&msg, s, 0);
    if(size == -1) {
        printf("the data len is in-valid!");
        return;
    }
    size_t  len2 = zmq_msg_size(&msg);
    printf("---> %d byte(%lu) ---> server\n", size, len2);
    char *buf = (char *) malloc(size);
    memcpy(buf, zmq_msg_data(&msg), size);

    // buf should be the object
    google::protobuf::io::ArrayInputStream in(buf,size);
    google::protobuf::io::CodedInputStream is(&in);

    mqdemo::ReqAction obj;
    if(obj.ParseFromCodedStream(&is) == true){
        printf("Good, protobuf obj parsed OK\n");
        printf("\tThe type:%d, the name:%s\n", obj.type(), obj.name().c_str());
    } else {
        printf("**failed parse the protobuf obj\n");
    }

    zmq_msg_close(&msg);
    free(buf);

#endif
    // next try reponse sth to caller...
    try_send_back(s, &obj);

    return;
}

int main(int argc, char **argv)
{
    void *ctx = zmq_ctx_new();
    if(!ctx) {
        printf("*** failed get the ZMQ resource!\n");
        return -1;
    }

    void *mq_s = zmq_socket(ctx, ZMQ_REP);
    if(zmq_bind(mq_s, "tcp://*:5555") != 0) {
        printf("*** failed bind the port on Server\n");
        printf("***    error number:%d\n", errno);
    } else {
        printf("=== Cool, 5555 port bind [OK]\n");
    }

    char buf[10];

    while(1) {
        handle_client_request(mq_s);
    }

    zmq_ctx_destroy(ctx);

    return 0;
}
