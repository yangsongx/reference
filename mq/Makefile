# show how to use Message Queue
#
DEBUG=true

CC = gcc
CXX = g++

SRC = zero_mq.cc
C_SRC = zeromq_client.c

# Release should use -O2
ifeq ($(DEBUG),false)
CFLAGS += -DDEBUG -O2
else
CFLAGS += -DCHECK_MEM_LEAK -DDEBUG -g -O0
endif

LDFLAGS = -lzmq


OBJS = $(SRC:.cc=.o)
COBJS = $(C_SRC:.c=.o)

all:mqs mqc

mqs:$(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

mqc:$(COBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

.cc.o:
	$(CXX) $(CFLAGS) -c $< -o $@
.c.o:
	$(CXX) $(CFLAGS) -c $< -o $@

.PHONY:clean
clean:
	rm -rf *.o mqs mqc
	find . -name '*.o' -exec rm -rf {} ";"
