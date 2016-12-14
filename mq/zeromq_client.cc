/**
 * Show how to use Zero Message Queue(Client)
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <zmq.h>

#include <pthread.h>

// defined in another c source file
extern int random_int(int from, int to);

struct testmqthread{
    int th_count;   // how many reqs in current thread
    void *th_zmq;   // zmq socket connection
};

struct testmqthread thd;

void *sending_thread(void *param)
{
    struct testmqthread *p = (struct testmqthread *)param;

    int num = (p->th_count/2);
    int i;

    for(i = 0; i < num; i++){
        // do a lot of reqs...
        printf("gen random=%d\n", random_int(1, 12));
    }

    return NULL;
}


/**
 * Try send totally @reqcount request to server
 *
 * @s : the ZMQ socket
 */
void simulate_many_req(int reqcount, void *s)
{
    void *thdresult[2];

    thd.th_count = reqcount;
    thd.th_zmq = s;

    /* create totally 2 threads... */
    pthread_t tid[2];
    int i;

    // need random...
    srand(time(NULL));

    for(i = 0; i < 2; i++){
        if(pthread_create(&tid[i],
                    NULL,
                    sending_thread,
                    &thd) != 0){
            printf("The Thread-%d creation failed!\n", i);
        } else {
            printf("==> [%d] thread created [OK]\n", i);
        }
    }

    for(i = 0; i < 2; i++) {
        pthread_join(tid[i], &thdresult[i]);
    }

    printf("\n\n<-- all child thread finished, main program quit as well\n");
    printf("Thread-1 got %d [OK case]\n", *((int*)thdresult[0]));
    printf("Thread-2 got %d [OK case]\n", *((int*)thdresult[1]));
}

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

    /* ./a.out 25000 -t  (-t here means sleep for a while before sending req) */
    if(argc > 1) {
        int count = atoi(argv[1]);
        printf("We will send totally %d req packages to server...\n", count);

        if(argc == 3) {
            printf("this is a tricky sleep for simulation, for nearly simultaneous in multiple machine...\n");
            sleep(2);
        }

        // drop into a testing world...
        simulate_many_req(count, mq_c);

        goto quitcode;
    }


    // normal one-step client test

    char buf[10];
    strcpy(buf, "hello");

    zmq_send(mq_c, buf, strlen(buf) + 1, 0);
    printf("req sent, wait for response....\n");
    zmq_recv(mq_c, buf, sizeof(buf), 0);
    printf("<---- Server tell me with %s\n", buf);


quitcode:
    printf("try clean the code...\n");

    if(mq_c != NULL)
        zmq_close(mq_c);
    zmq_ctx_destroy(ctx);

    return 0;
}
