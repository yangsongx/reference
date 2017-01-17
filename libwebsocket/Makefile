# show how to use WEBSOCKET
#
DEBUG=true

CC = gcc
CXX = g++

SRC = server.c

C_SRC = zeromq_client.cc gen_random.cc datamsg.pb.cc

# Release should use -O2
ifeq ($(DEBUG),false)
CFLAGS += -DDEBUG -O2
else
CFLAGS += -DCHECK_MEM_LEAK -DDEBUG -g -O0
endif

LDFLAGS = -lwebsockets


OBJS = $(SRC:.c=.o)
COBJS = $(C_SRC:.cc=.o)

all:webs

webs:$(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY:clean
clean:
	rm -rf *.o webs
	find . -name '*.o' -exec rm -rf {} ";"
