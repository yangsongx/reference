# -*- coding: utf-8 -*-
import scrapy

class BookItem(scrapy.Item):
    tag = scrapy.Field()
    title = scrapy.Field()
    author = scrapy.Field()
    rating = scrapy.Field()
    cover = scrapy.Field()
    url = scrapy.Field()

