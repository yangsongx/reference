/**
 * This file is related with ZMQ's req/rep pattern
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <zmq.h>

#ifndef HOSTIP
#define HOSTIP          "127.0.0.1"
#endif

#define REQREP_PORT  5555

/**
 * Main Entry of the req/rep pattern code, and I also
 * call it as nomral pattern, as it is the simplest.
 *
 * @ctx: ZMQ context, usually passed in by user's main()
 *
 */
//void normal_pattern(void *ctx)

/**
 *
 * @ctx: ZMQ context, usually passed in by user's main()
 *
 */
void *prepare_server(void *ctx)
{
    char buf[56];
    void *sock = zmq_socket(ctx, ZMQ_REP);

    snprintf(buf, sizeof(buf), "tcp://%s:%d",
            HOSTIP, REQREP_PORT);

    if(zmq_bind(sock, buf) != 0){
        printf("failed bind the server side socket(%d)\n",
                errno);
        zmq_close(sock);
        return NULL;
    }

    printf("Server binded on port %d [OK]\n", REQREP_PORT);

    buf[0] = '\0';
    while(1) {
        zmq_recv(sock, buf, sizeof(buf), 0);

        sleep(2); // fake time-wasting work

        strcpy(buf, "finished");
        zmq_send(sock, buf, sizeof(buf), 0);
    }
}

/**
 * @sock: the connected socket object
 */
int send_simple_req(void *sock)
{
    char data[32];
    int count = 10; // I aim to try sending 10
    int i;

    for(i = 0; i < count; i ++){
        zmq_send(sock, data, sizeof(data), 0);
        printf("sent data, wait for response...\n");
        zmq_recv(sock, data, sizeof(data), 0);
        printf("got this, cool\n");
    }

    printf("finished all %d time testing\n", count);

    return 0;
}

/**
 *
 * @ctx: ZMQ context, usually passed in by user's main()
 *
 * return the created client socket, DO NOT forget to
 * close it if it became useless.
 */
void *prepare_client(void *ctx)
{
    char buf[56];
    void *sock = zmq_socket(ctx, ZMQ_REQ);

    snprintf(buf, sizeof(buf), "tcp://%s:%d",
            HOSTIP, REQREP_PORT);
    if(zmq_connect(sock, buf) != 0){
        printf("can't connect to server(%d)\n",
                errno);
        zmq_close(sock);
        return NULL;
    }
    printf("-->connect to server OK\n");

    send_simple_req(sock);

    zmq_close(sock);
    sock = NULL;

    return sock;
}

