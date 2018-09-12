# -*- coding: utf-8 -*-

from scrapy import Selector
from scrapy.exceptions import DropItem
import bs4

# Define your item pipelines here
#
# Don't forget to add your pipeline to the ITEM_PIPELINES setting
# See: http://doc.scrapy.org/en/latest/topics/item-pipeline.html


def extract_english_test_comments(inputdata):
    sel = Selector(text = inputdata, type='html')
    span= sel.xpath("//div[@id='q_indexkuai22111']/span/text()").extract()

    outputdata = "%s,%s" %(span[1], span[2])
    outputdata = outputdata.replace("：",":") #replace UTF-8 to ASCII
    return outputdata

def extract_english_test_contents(inputdata):
    contents = ''

    sel = Selector(text = inputdata, type='html')

    # after below code , all <BR> HTML would be converted to \n
    txt = sel.xpath("//tbody/tr/td/div/text()").extract()

    for it in txt:
        contents = contents + it + '\n'

    return contents

def extract_english_test_answer(inputdata):
    answer= ''

    sel = Selector(text = inputdata, type='html')

    # after below code , all <BR> HTML would be converted to \n
    txt = sel.xpath("//tbody/tr/td/div/text()").extract()

    for it in txt:
        answer = answer + it + '\n'

    return answer 

def get_existed_tests():
    ret = []
    with open('./finished_tests.data', 'r') as f:
        for line in f:
            ret.append(line)
    return ret

def update_existed_tests(record):
    fd = open('./finished_tests.data', 'a+')
    fd.write(record + '\n')
    fd.close()


###################################################################################

class DoubanPipeline(object):
    def open_spider(self, spider):
        if spider.name == 'english_test':
            print("=== [open_spider()] try get existed record for english test topic ===")
            self.existed_tests = get_existed_tests()

    def process_item(self, item, spider):
        if spider.name == 'english_test':

            if item['url'] in self.existed_tests:
                print("####Attention####, duplicated item! DROP IT!")
                return DropItem("duplicated %s url data" %(item['url']))

            # next try to begin formal processing!

            print("English Test related topic...")
            item['grade'] = item['grade'].strip()

            ## 抽取备注部分数据
            item['comments'] = extract_english_test_comments(item['comments'])

            ## 抽取答案部分数据
            item['answer'] = extract_english_test_answer(item['answer'])

            ## 抽取正文部分数据
            item['content'] = extract_english_test_contents(item['content'])

            self.existed_tests.append(item['url'])
            update_existed_tests(item['url'])

        return item
