#!/usr/bin/env python

# publisher client test script

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



def test_sub_begin():
    ctx = zmq.Context()
    print 'Connecting to server...'
    cs = ctx.socket(zmq.SUB)
    cs.connect("tcp://59.110.8.34:5556")

    print 'connecting to Publisher Server OK'

    # FIXME  as the manual said, empty means sub all
    cs.setsockopt_string(zmq.SUBSCRIBE, u'')
    while True:
        obj = datamsg_pb2.ReqAction()
        obj.ParseFromString(cs.recv())
        print 'the msg I got is: %s' %(obj.name)

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
    test_sub_begin()
    print 'exit the test'
