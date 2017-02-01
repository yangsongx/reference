# -*- coding: utf-8 -*-
import scrapy

# from local items.py
from douban.spiders.items import BookItem

###
# 豆瓣的书籍分类(tag)
#
# 0 - 编程
# 1 - 儿童文学
#
#######
class BookSpider(scrapy.Spider):
    name = "book"
    allowed_domains = ["book.douban.com"]
    start_urls = ['http://book.douban.com/tag/编程']

    # Just a internal util
    def _save_offline(self, fn, data):
        fd = open(fn, "w")
        fd.write(data)
        fd.close()

    def parse(self, response):
        bksel = response.xpath("//li[@class='subject-item']")
        print("Totally %d items found" %(len(bksel)))

        for it in bksel:
            bitem = BookItem()

            title = it.xpath("div[@class='info']/h2/a/text()").extract()[0].strip()
            if it.xpath("div[@class='info']/h2/a/span/text()").extract_first() != None:
                title += it.xpath("div[@class='info']/h2/a/span/text()").extract_first()

            rating = it.re(r"\<span class=\"rating_nums\"\>?([\s\S]*?)\<\/span\>")[0]
            pubinfo = it.re(r"\<div class=\"pub\"\>?([\s\S]*?)\<\/div\>")[0].strip()
            print("Title:%s  Rating:%s  Pub:%s" %(title, rating, pubinfo))
            bitem["tag"] = 0 #Programing
            bitem["title"] = title
            bitem["author"] = pubinfo 
            bitem["rating"] = rating 

            yield bitem


        pass
