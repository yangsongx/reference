# -*- coding: utf-8 -*-
import scrapy


class BookSpider(scrapy.Spider):
    name = "book"
    allowed_domains = ["book.douban.com"]
    start_urls = ['http://book.douban.com/tag/编程']

    def parse(self, response):
        bksel = response.xpath("//li[@class='subject-item']")
        print("Totally %d items found" %(len(bksel)))
        for it in bksel:
            print(it.xpath("div/@class").extract())
#print(h2)

        pass
