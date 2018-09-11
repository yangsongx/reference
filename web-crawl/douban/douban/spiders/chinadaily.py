# -*- coding: utf-8 -*-
import scrapy

from douban.spiders.items import ChinaDailyItem

#######################################
#中国日报英文版的数据

class ChinadailySpider(scrapy.Spider):
    name = "chinadaily"
    max_count = 10 # 暂时定成每个栏目爬个10个左右...
    allowed_domains = ["www.chinadaily.com.cn"]
    start_urls = ['http://www.chinadaily.com.cn/']

    def parse(self, response):

        #首先寻找到所有的专栏
        column_sel = response.xpath("//ul[@class='xiao-ul']/li[@shape='rect']/@href")
        print("Totally %d columns found" %(len(column_sel)))

        pass
