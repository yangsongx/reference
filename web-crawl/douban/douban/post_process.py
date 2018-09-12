# -*- coding: utf-8 -*-
#
# Post Process the static HTML page, take it into TeX

import sys
import json
import nltk
import bs4

class EnglishTest:
    json_fn = ''
    existed_fn = "finished_list.json"
    existed_record = []
    existed_fd = None

    def __init__(self, fn):
        print("coming with init")
        self.json_fn = fn

    def load_json_file(self):
        with open(self.existed_fn, 'r') as f:
            for line in f:
                if line != None or line != '':
                    self.existed_record.append(line)

        print("now, re-open for appending")
        self.existed_fd = open(self.existed_fn, 'a+')

        with open(self.json_fn, 'r') as f:
            data = f.read()
            jsdata = json.loads(data)
            return jsdata

    def push_finished_status(self, keyname):
        if keyname not in self.existed_record:
            self.existed_record.append(keyname)
            self.existed_fd.write(keyname + '\n')

        return 0

    def run(self):
        jsdata = self.load_json_file()
        print("there are totally %d lines of test" %(len(jsdata)))
        for it in jsdata:
            self.push_finished_status(it['url'])
            bs = bs4.BeautifulSoup(it['content'])
            print(bs.get_text())

        return 0

if __name__ == "__main__":
    jsfile = sys.argv[1]
    print("coming with %s" %(jsfile))

    et = EnglishTest(jsfile)
    et.run()
