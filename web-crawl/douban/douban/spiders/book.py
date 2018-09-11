# -*- coding: utf-8 -*-
import scrapy

# from local items.py
from douban.spiders.items import BookItem

###
# 豆瓣的书籍分类(tag)
#
# 0 - 编程
# 1 - 儿童文学
# 2 - 英语
#
#######
class BookSpider(scrapy.Spider):
    name = "book"
    allowed_domains = ["book.douban.com"]

    global_count = 0 # protection
    leading_url = 'http://book.douban.com'

    start_urls = ['http://book.douban.com/tag/英语?start=0&type=T'] # only one starting...

#    start_urls = ['http://book.douban.com/tag/编程?start=940&type=T',
#                  'http://book.douban.com/tag/编程?start=960&type=T',
#                  'http://book.douban.com/tag/编程?start=980&type=T',
#                  'http://book.douban.com/tag/编程?start=1000&type=T',
#                  'http://book.douban.com/tag/编程?start=1020&type=T',
#                  'http://book.douban.com/tag/编程?start=1040&type=T',]


    # Just a internal util
    def _save_offline(self, fn, data):
        fd = open(fn, "w")
        fd.write(data)
        fd.close()

#    def start_requests(self):
#

    def parse(self, response):
        print(type(response))
        print(response.url)
#        print(response.text)


        bksel = response.xpath("//li[@class='subject-item']")
        print(type(bksel))
        print("Totally %d items found" %(len(bksel)))

        if len(bksel) == 0:
            print("~~~Attention, probably this is the last section!!!")
            return

        for it in bksel:
            title = it.xpath("div[@class='info']/h2/a/text()").extract()[0].strip()
            if it.xpath("div[@class='info']/h2/a/span/text()").extract_first() != None:
                title += it.xpath("div[@class='info']/h2/a/span/text()").extract_first()


            rating_match  = it.re(r"\<span class=\"rating_nums\"\>?([\s\S]*?)\<\/span\>")
            if rating_match != None and len(rating_match) > 0:
                if rating_match[0] != ''  and rating_match[0] != ' ':
                    rating = rating_match[0]
                else:
                    rating = 0.0
            else:
                print("=========@@@@@@ not YET @@@@=====")
                rating = 0.0 # not be rated yet

            pubinfo = it.re(r"\<div class=\"pub\"\>?([\s\S]*?)\<\/div\>")[0].strip()
            # 2018-09-07 : try extract the cover images...
            cover_data = it.xpath("div[@class='pic']/a[@class='nbg']/img/@src").extract()[0]
            url = it.xpath("div[@class='info']/h2/a/@href").extract()[0]

            print("Title:%s  Rating:%s  Pub:%s" %(title, rating, pubinfo))
            bitem = BookItem()
            bitem["tag"] = 2
            bitem["title"] = title
            bitem["author"] = pubinfo 
            bitem["rating"] = float(rating)
            bitem["cover"] = cover_data
            bitem["url"] = url

            yield bitem

        print('keep moving...')

        pgsel = response.xpath("//div[@class='paginator']/span[@class='next']/link/@href")
        print(type(pgsel))
        print(pgsel)
        if len(pgsel.extract()) > 0:

            next_url = response.urljoin(pgsel.extract()[0])
            print("next url is : %s" %(next_url))
            next_url2 = response.urljoin(next_url)
            self.global_count = self.global_count + 1
            yield scrapy.Request(url = next_url)

        pass
