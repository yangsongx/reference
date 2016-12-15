/**
 * Show how to use Zero Message Queue(Client)
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <zmq.h>

#include <pthread.h>

// in order to test via protobuf obj
#include "datamsg.pb.h"

// defined in another c source file
extern int random_int(int from, int to);

struct testmqthread{
    int th_count;   // how many reqs in current thread
    void *th_zmq;   // zmq context, not socket
};

struct testmqthread thd;

int useless = 100;
int *p_useless = &useless;


int send_mqdata(void *s, void *data, int len)
{
    int ret = -1;
    zmq_msg_t msg;

    zmq_msg_init_size(&msg, len);
    memcpy(zmq_msg_data(&msg),
            data,
            len);

    ret = zmq_msg_send(&msg, s, 0);

    zmq_msg_close(&msg);

    return ret == len ? 0 : -1;
}

/**
 * return 0 means req-response are correct.
 */
int verify_response(void *s, int match_num)
{
    int ret = -1;
    zmq_msg_t msg;
    zmq_msg_init(&msg);
    mqdemo::ReqResult obj;

    int size = zmq_msg_recv(&msg, s, 0);
    if(size == -1) {
        printf("the data len is in-valid!");
        return -1;
    }
    char *buf = (char *) malloc(size);
    memcpy(buf, zmq_msg_data(&msg), size);

    // buf should be the object
    if(obj.ParseFromArray(buf, size) == true){
        printf("Good, protobuf obj parsed OK\n");
        printf("\tThe code:%d, the msg:%s\n", obj.code(), obj.msg().c_str());
        printf("[%d] [2*%d=%d] <---> [%d]\n", match_num, match_num, 2*match_num, obj.code());
        if((2*match_num) == obj.code()) {
            ret = 0;
        }
    } else {
        printf("**failed parse the protobuf obj\n");
    }

    zmq_msg_close(&msg);
    free(buf);

    return ret;
}

void try_send_req(void *s, int type)
{
    char buf[128];

    mqdemo::ReqAction obj;
    obj.set_type(type);
    snprintf(buf, sizeof(buf),
            "the msg for %d type\n", type);
    obj.set_name(buf);

    // Now try load them...
    int objlen = obj.ByteSize();
    char *packet = (char *)malloc(objlen);
    if(obj.SerializeToArray(packet, objlen) == true){
        printf("=== Serialize obj [OK] ===\n");
        if(send_mqdata(s, packet, objlen) != 0){
            printf("warning, send MQ probably incorrect\n");
        }

        // next, try keep get response from server....
        if(verify_response(s, type) != 0){
            printf("CASE Error**, req-response not match!\n");
        } else {
            printf("CASE Pass== Congratulations!\n");
        }

    } else {
        printf("****Failed serialize the obj\n");
    }

    free(packet);
}

void *sending_thread(void *param)
{
    struct testmqthread *p = (struct testmqthread *)param;

    void *cs = zmq_socket(p->th_zmq, ZMQ_REQ);
    if(!cs){
        printf("**failed create the socket!\n");
        return &p_useless;
    }

    if(zmq_connect(cs, "tcp://59.110.8.34:5555") != 0) {
        printf("*** failed connect to server\n");
        zmq_close(cs);
        return &p_useless;
    }

    printf("connecting to server .... [OK]\n");

    int num = (p->th_count/2);
    int i;

    for(i = 0; i < num; i++){
        // do a lot of reqs...
        try_send_req(cs, random_int(1, 200));
    }

    zmq_close(cs);

    return &p_useless;
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

void subscriber_mode(void *ctx)
{
}

int main(int argc, char **argv)
{
    int c;
    int sub_mode = 0; // subscriber

    void *ctx = zmq_ctx_new();
    if(!ctx) {
        printf("*** failed get the ZMQ resource!\n");
        return -1;
    }

    while((c = getopt(argc, argv, "s")) != -1) {
        switch(c){
            case 's':
                sub_mode = 1;
                break;

            default:
                break;
        }
    }

#if 1
    if(sub_mode){
        subscriber_mode(ctx);
    } else {
    }
#else

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

    /* stress test case like this:
     * ===============================================
     * ./a.out 25000 -t  (-t here means sleep for a while before sending req)
     * ===============================================
     */
    if(argc > 1) {
        int count = atoi(argv[1]);
        printf("We will send totally %d req packages to server...\n", count);

        if(argc == 3) {
            printf("this is a tricky sleep for simulation, for nearly simultaneous in multiple machine...\n");
            sleep(2);
        }

        // drop into a testing world...
        simulate_many_req(count, ctx);

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
#endif
    printf("destroy the zmq context...\n");
    zmq_ctx_destroy(ctx);

    return 0;
}
