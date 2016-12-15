#!/usr/bin/env python

# I want to do regression test in script method...
# This is trying simulate a lot of, and different
# clients, sending request to Server...

import commands
import json
import re
import sys
import unittest
import random

import xml.sax
import xml.dom.minidom

import zmq
import datamsg_pb2

global glbhost
glbhost = '59.110.8.34'
global glbport
glbport = 5555


class MqTesting(unittest.TestCase):

    def test_many_reqs(self):
        assert 0 == 1

# ---- protobuf via ZMQ ----
def sendreq(cs):

    i = random.randint(400,5000)
    req = datamsg_pb2.ReqAction()
    req.type = i
    req.name = 'python msg of %d' %(req.type)

    cs.send(req.SerializeToString())

    print 'sent to Server, waiting for response...'
    obj = datamsg_pb2.ReqResult()
    obj.ParseFromString(cs.recv())

    print "Got response, code:%d, msg:%s" % (obj.code, obj.msg)
    doublenum = 2 * i
    if doublenum == obj.code:
        print 'CASE Pass, good'
    else:
        print 'CASE Error, bad'
    return 0

def testzmq_begin():
    ctx = zmq.Context()
    print 'Connecting to server...'
    cs = ctx.socket(zmq.REQ)
    cs.connect("tcp://59.110.8.34:5555")

    print 'connecting to Server OK'

    for i in range(1, 2000):
        sendreq(cs)
    print 'finished the test'

    return 0

### TEST MAIN ###
if __name__ == "__main__":
    if len(sys.argv) == 2:
        global glbhost
        glbhost = sys.argv[1]
    elif len(sys.argv) == 3:
        global glbhost
        global glbport
        glbhost = sys.argv[1]
        glbport = sys.argv[2]

    print 'trigger all test cases...'
    testzmq_begin()
    print 'exit the test'
