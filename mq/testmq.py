#!/usr/bin/env python

# I want to do regression test in script method...
# This is trying simulate a lot of, and different
# clients, sending request to Server...

import commands
import json
import re
import sys
import unittest

import xml.sax
import xml.dom.minidom

global glbhost
glbhost = '59.110.8.34'
global glbport
glbport = 5555


class MqTesting(unittest.TestCase):

    def test_many_reqs(self):
        assert 0 == 1

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

#
    print 'trigger all test cases...'
    unittest.main() # Trigger all test cases...
    print 'exit the test'
