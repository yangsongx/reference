# -*- coding: utf-8 -*-
import scrapy


class BookSpider(scrapy.Spider):
    name = "book"
    allowed_domains = ["book.douban.com"]
    start_urls = ['http://book.douban.com/tag/编程']

    def _save_offline(self, fn, data):
        fd = open(fn, "w")
        fd.write(data)
        fd.close()

    def parse(self, response):
        bksel = response.xpath("//li[@class='subject-item']")
        print("Totally %d items found" %(len(bksel)))
        self._save_offline("/tmp/my.html", str(response.body))

        for it in bksel:
            print(type(it.xpath("div/@class")))
            print(type(it.xpath("div/@class").extract()))

            print(it.xpath("div/@class").extract())
            print(it.xpath("div[@class='info']/h2/a/text()").extract()[0].strip())
            if it.xpath("div[@class='pub']/text()").extract() is None:
                print("BAD, empty")
            else:
                print(it.xpath("div[@class='pub']/text()").extract())
#print(it.xpath("div[@class='star_clearfix']/span[@class='rating_nums']/text()").extract())
#print(h2)

        pass
