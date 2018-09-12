# -*- coding: utf-8 -*-
import scrapy
from douban.spiders.items import EnglishTestItem

##
class EnglishTestSpider(scrapy.Spider):
    name = "english_test"
    max_count = 0
    allowed_domains = ["www.mofangge.com/qlist/yingyu/"]
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
        for it in full_list:


            # 进一步处理
            yield scrapy.Request(url = it['url'], meta = {'mytopic': it['name']}, callback=self.parse_reader)

        pass

    def parse_reader(self, response):
        topic = response.meta['mytopic']
        print("now, the coming response is %s, url is %s" %(topic, response.url))


        test_sec = response.xpath("//div[@class='seoleft']/ul[@class='seoleftul']/li")

        test_list = []
        for it in test_sec:
            grade = it.xpath("span[@class='right']/text()").extract()[0]
            location_url = it.xpath("a/@href").extract()[0]
            yield scrapy.Request(url = location_url,
                    meta = {'grade': grade, 'topic': topic},
                    callback=self.parse_final_test)

        pass


    def parse_final_test(self, response):
        if self.max_count > 3:
            print("safe to quit")
            return

        self.max_count = self.max_count + 1

        topic = response.meta['topic']
        grade = response.meta['grade']

        question_sec = response.xpath("//div[@class='q_mokuai']/div[@id='q_indexkuai22']/div[@id='q_indexkuai221']/table/tbody").extract()[0]
        answer_sec = response.xpath("//div[@class='q_mokuai']/div[@id='q_indexkuai32']/div[@id='q_indexkuai321']/table/tbody").extract()[0]
        comments_sec = response.xpath("//div[@class='q_mokuai']/div[@id='q_indexkuai2211']/div[@id='q_indexkuai2211']").extract()[0]


        ei = EnglishTestItem()
        ei['topic'] = topic
        ei['grade'] = grade
        ei['content'] = question_sec
        ei['comments'] = comments_sec

        yield ei

        pass

