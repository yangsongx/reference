# -*- coding: utf-8 -*-
import scrapy
from douban.spiders.items import EnglishTestItem

##
class EnglishTestSpider(scrapy.Spider):
    name = "english_test"
    max_count = 0
    allowed_domains = ["www.mofangge.com"]
    start_urls = ['http://www.mofangge.com/qlist/yingyu/']

    def parse(self, response):
        bksel = response.xpath("//li[@class='lastulli']/span/a")
        full_list = []
        print("totally %d found" %(len(bksel)))
        for it in bksel:
            group_url = it.xpath("@href").extract()[0]
            group_name = it.xpath("text()").extract()[0]
            print("%s  --> %s" %(group_name, group_url))
            if "阅读" in group_name:
                item = {
                    'url': group_url,
                    'name': group_name
                }
                full_list.append(item)

        print("after traverse, totally %d read materials" %(len(full_list)))

        print("let's handle them one-by-one")
#        for it in full_list:
#            print("trigger to %s" %(it['url']))
            # 进一步处理
#            yield scrapy.Request(url = it['url'], meta = {'mytopic': it['name']}, callback=self.parse_reader)
#
        print("just debug, for the first one")
        yield scrapy.Request(url = full_list[0]['url'], meta = {'mytopic': full_list[0]['name']}, callback=self.parse_reader)

        pass

    def parse_reader(self, response):
        if self.max_count > 0:
            print("safe to quit")
            return

        self.max_count = self.max_count + 1

        print("coming this parse function")
        topic = response.meta['mytopic']
        print("now, the coming response is %s, url is %s" %(topic, response.url))


        test_sec = response.xpath("//div[@class='seoleft']/ul[@class='seoleftul']/li")

        debug = True
        test_list = []
        for it in test_sec:
            grade = it.xpath("span[@class='right']/text()").extract()[0]
            location_url = it.xpath("a/@href").extract()[0]
            if debug == True:
                debug = False
                yield scrapy.Request(url = location_url,
                    meta = {'grade': grade, 'topic': topic},
                    callback=self.parse_final_test)

        pass


    def parse_final_test(self, response):

        if self.max_count > 1:
            print("try safe quit")
            self.max_count = self.max_count + 1

        topic = response.meta['topic']
        grade = response.meta['grade']
        print("== This is final test, for %s of %s" %(grade, topic))
        print("== URL is %s" %(response.url))

        question_sec = response.xpath("//div[@class='q_mokuai']/div[@id='q_indexkuai22']/div[@id='q_indexkuai221']/table/tbody").extract()[0]
        answer_sec = response.xpath("//div[@class='q_mokuai']/div[@id='q_indexkuai32']/div[@id='q_indexkuai321']/table/tbody").extract()[0]
        comments_sec = response.xpath("//div[@class='q_mokuai']/div[@id='q_indexkuai22']/div[@id='q_indexkuai221']/div[@id='q_indexkuai2211']").extract()[0]


        ei = EnglishTestItem()
        ei['topic'] = topic
        ei['grade'] = grade
        ei['content'] = question_sec
        ei['answer'] = answer_sec
        ei['comments'] = comments_sec
        ei['url'] = response.url

        yield ei

        pass

