# -*- coding: utf-8 -*-
import scrapy

#######################################
#中国日报英文版的数据

class ChinadailySpider(scrapy.Spider):
    name = "chinadaily"
    allowed_domains = ["www.chinadaily.com.cn"]
    start_urls = ['http://www.chinadaily.com.cn/']

    def parse(self, response):
        pass
