# -*- coding: utf-8 -*-
import scrapy


class NeteasySpider(scrapy.Spider):
    name = "neteasy"
    allowed_domains = ["163.com"]
    start_urls = ['http://163.com/']

    def parse(self, response):
        pass
