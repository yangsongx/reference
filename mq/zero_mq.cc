/**
 * Show how to use Zero Message Queue
 */
#include <stdio.h>
#include <zmq.h>

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
        printf("=== Cool, port bind [OK]\n");
    }

    char buf[10];

    while(1) {
        zmq_recv(mq_s, buf, sizeof(buf), 0);
        printf("[Server] Got req from a client\n");

        zmq_send(mq_s, "I got it", 9, 0);
    }

    zmq_ctx_destroy(ctx);

    return 0;
}
