# -*- coding: utf-8 -*-
import scrapy

class BookItem(scrapy.Item):
    tag = scrapy.Field()
    title = scrapy.Field()
    author = scrapy.Field()
    rating = scrapy.Field()
    cover = scrapy.Field()
    url = scrapy.Field()

class EnglishTestItem(scrapy.Item):
    url = scrapy.Field()     #文章的URL标示
    topic = scrapy.Field()   #所属栏目
    level = scrapy.Field()
    content = scrapy.Field()
    updated = scrapy.Field()
    imgs = scrapy.Field()  #注意这个需要是list类型

class ChinaDailyItem(scrapy.Item):
    url = scrapy.Field()     #文章的URL标示
    column = scrapy.Field()  #所属栏目
    title = scrapy.Field()
    content = scrapy.Field()
    updated = scrapy.Field()
    imgs = scrapy.Field()  #注意这个需要是list类型
