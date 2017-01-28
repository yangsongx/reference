import scrapy

class BookItem(scrapy.Item):
    tag = scrapy.Field()
    title = scrapy.Field()
    author = scrapy.Field()
    rating = scrapy.Field()

