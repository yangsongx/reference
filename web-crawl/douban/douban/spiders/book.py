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
    start_urls = ['http://book.douban.com/tag/编程?start=0&type=T',
                  'http://book.douban.com/tag/编程?start=20&type=T',
                  'http://book.douban.com/tag/编程?start=40&type=T']

    # Just a internal util
    def _save_offline(self, fn, data):
        fd = open(fn, "w")
        fd.write(data)
        fd.close()

    def start_requests(self):

        for i in range(0, 96):
            idx = i * 20
            url_str = 'http://book.douban.com/tag/编程?start=%d&type=T' %(idx)
            yield scrapy.Request(url = url_str,
                    callback = self.parse)

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
            bitem["rating"] = float(rating)

            yield bitem


        pass
