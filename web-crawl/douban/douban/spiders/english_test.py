# -*- coding: utf-8 -*-
import scrapy

##
class EnglishTestSpider(scrapy.Spider):
    name = "english_test"
    allowed_domains = ["www.mofangge.com/qlist/yingyu/"]
    start_urls = ['http://www.mofangge.com/qlist/yingyu//']

    def parse(self, response):
        pass
