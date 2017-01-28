# -*- coding: utf-8 -*-
from scrapy import Selector

fd = open("my.html", "r")
alldata = fd.read()
fd.close()

doc = Selector(text=alldata, type="html")
print("the type info:")
print(type(doc.xpath("//li/@class")))
print(type(doc.xpath("//li/@class").extract()))
doc.xpath("//li/@class").extract()
print("the value are:")
print(doc.xpath("//li/@class").extract())
bksel = doc.xpath("//li[@class='subject-item']")
print("Totally %d items found" %(len(bksel)))

for it in bksel:
    title = it.xpath("div[@class='info']/h2/a/text()").extract()[0].strip()
    if it.xpath("div[@class='info']/h2/a/span/text()").extract_first() is None:
        print("No second title");
    else:
        title += it.xpath("div[@class='info']/h2/a/span/text()").extract_first()
    print(title)

    print(it.re(r"\<span class=\"rating_nums\"\>?([\s\S]*?)\<\/span\>"))
    print(it.re(r"\<div class=\"pub\"\>?([\s\S]*?)\<\/div\>")[0].strip())

#print(it.xpath("div[@class='info']/text()").extract())
# Rating..
# first.re(r"\<span class=\"rating_nums\"\>?([\s\S]*?)\<\/span\>")

# Author...

