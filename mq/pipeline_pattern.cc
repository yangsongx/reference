/**
 * This file is related with ZMQ's pipeline pattern
 */

#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <zmq.h>

#define HOSTIP          "127.0.0.1"
// generator shared by both generator and consumer
#define GENERATOR_PORT  5557
// consumer will use this to tell sink the final result
#define SINK_PORT       5558

/**
 * Main Entry of the pipline pattern code.
 *
 * @ctx: ZMQ context, usually passed in by user's main()
 *
 */
//void pipline_pattern(void *ctx)

/**
 *
 * @ctx: ZMQ context, usually passed in by user's main()
 *
 */
void prepare_generator(void *ctx)
{
    int i;
    int count = 10; // still totally 10 reqs
    char buf[56];
    void *gen_sock = zmq_socket(ctx, ZMQ_PUSH);

    snprintf(buf, sizeof(buf), "tcp://%s:%d",
            HOSTIP, GENERATOR_PORT);

    zmq_bind(gen_sock, buf);

    void *sink_sock = zmq_socket(ctx, ZMQ_PUSH); // this case ,no need bind

    snprintf(buf, sizeof(buf), "tcp://%s:%d",
            HOSTIP, SINK_PORT);
    zmq_connect(sink_sock, buf);

    printf("Press any key to try generate all data msgs...\n");
    getchar();
    printf("[OK] Fire!\n");

    printf("first, wake up the sink...\n");
    zmq_send(sink_sock, "0", 2, 0);

    printf("next, try send all reqs to the queue\n");
    for(i = 0; i < count; i++){
        buf[0] = '\0';
        zmq_send(gen_sock, buf, sizeof(buf), 0);
        printf("push out the data, no need for waiting response...\n");
    }

    printf("Finished all the push job. pretty cool\n");
    printf("FIXME, don't free the socket, maybe consumer still busy\n");
    //sleep(900000);
    zmq_close(gen_sock);
    printf("the socket closed!\n");
}

/**
 *
 * @ctx: ZMQ context, usually passed in by user's main()
 *
 */
void prepare_consumer(void *ctx)
{
    char buf[56];
    void *con_sock = zmq_socket(ctx, ZMQ_PULL);

    // also the generator port!
    snprintf(buf, sizeof(buf), "tcp://%s:%d",
            HOSTIP, GENERATOR_PORT);
    if(zmq_connect(con_sock, buf) != 0){
        printf("can't connect to generator(%d)\n",
                errno);
        zmq_close(con_sock);
        return;
    }

    printf("connect to generator [OK]\n");

    void *sink = zmq_socket(ctx, ZMQ_PUSH);
    snprintf(buf, sizeof(buf), "tcp://%s:%d",
            HOSTIP, SINK_PORT);
    zmq_connect(sink, buf);

    while(1){
        buf[0] = '\0';
        zmq_recv(con_sock, buf, sizeof(buf), 0);
        sleep(2); // time-wasting job...

        //now notify sink
        zmq_send(sink, "1", 2, 0);
    }
}

void prepare_sink(void *ctx)
{
    int i;
    int count = 10;
    char buf[56];
    void *sock = zmq_socket(ctx, ZMQ_PULL);

    // also the generator port!
    snprintf(buf, sizeof(buf), "tcp://%s:%d",
            HOSTIP, SINK_PORT);
    zmq_bind(sock, buf);

    // wait for generator trigger me...
    zmq_recv(sock, buf, sizeof(buf), 0);

    printf("I wake up, begin calculate the time....\n");
    struct timeval t1, t2;

    gettimeofday(&t1, NULL);

    for(i = 0; i < count; i ++){
        zmq_recv(sock, buf, sizeof(buf), 0);
        printf("got a finished notify\n");
    }

    printf("Wow, got all result, now check time again...\n");
    gettimeofday(&t2, NULL);

    printf("The delta time = %ld\n", (t2.tv_sec - t1.tv_sec));

    zmq_close(sock);

}
