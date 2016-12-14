/**
 * Show how to use Zero Message Queue(Client)
 */
#include <stdio.h>
#include <string.h>
#include <zmq.h>

int main(int argc, char **argv)
{
    void *ctx = zmq_ctx_new();
    if(!ctx) {
        printf("*** failed get the ZMQ resource!\n");
        return -1;
    }

    void *mq_c = zmq_socket(ctx, ZMQ_REQ);
    if(!mq_c){
        printf("failed create the client MQ\n");
        goto quitcode;
    }

    printf("try connect to MQ server...\n");
    if(zmq_connect(mq_c, "tcp://59.110.8.34:5555") == 0){
        printf("connect the MQ server [OK]\n");
    } else {
        printf("connect MQ [failed]\n");
        goto quitcode;
    }

    char buf[10];
    strcpy(buf, "hello");

    zmq_send(mq_c, buf, strlen(buf) + 1, 0);
    printf("req sent, wait for response....\n");
    zmq_recv(mq_c, buf, sizeof(buf), 0);
    printf("<---- Server tell me with %s\n", buf);

    zmq_close(mq_c);

quitcode:
    zmq_ctx_destroy(ctx);

    return 0;
}
