#coding=utf-8

import os
import boto3
import commands
import datetime
import re
import time
import logging
import pyecharts
import smtplib
import sys
from email.mime.text import MIMEText
#logging.basicConfig(filename='./log.txt', format='%(asctime)s : %(message)s')

class MyReport:
    m_weekday = []
    m_size = []
    m_file_size = []
    m_cur = 0
    m_imgsrc = ''

    def __init__(self):
        print("good")
        self.read_filesize()
        self.m_cur = int(time.time())

    def read_filesize(self):
        with open('./fileinfo.txt') as f:
            for line in f:
                self.m_file_size.append(line)
        full_list = [] 
        # TODO ,below should be Last-7 data..
        for it in self.m_file_size[-7:]:
            full_list.append(it)

        self.extract_data(full_list)

        return 0

    def extract_data(self, full_list):
        for it in full_list:
            section = it.split("    ")
            ret = re.search(r'\d{4}-(.*)\(Day (.*)\)', section[0])
            print ret.group(1)
            print ret.group(2)
            if ret.group(2) == '0':
                self.m_weekday.append('{0}(W7)'.format(ret.group(1)))
            else:
                self.m_weekday.append('{0}(W{1})'.format(ret.group(1), ret.group(2)))

            for it in self.m_weekday:
                print it
            size = float(section[1])/ 1000000.00
            self.m_size.append('{0:.2f}'.format(size))
            for it in self.m_size:
                print it
        return 0

    def send_email(self, text, title):
        mail_host = 'smtp.qiye.163.com'
        mail_user = 'mserver@shuzijiayuan.com'
        mail_pass = 'Adv^Tech%'
        mail_postfix = "shuzijiayuan.com"

        mail_senderTo = 'chenyang@shuzijiayuan.com, yangsongxiang@shuzijiayuan.com, wangzhipeng@shuzijiayuan.com, yuanfanjie@shuzijiayuan.com'
#        mail_senderTo = 'yangsongxiang@shuzijiayuan.com, 13614278@qq.com'

        me="mserver"+"<"+mail_user+">"
        msg = MIMEText(text,_subtype='html',_charset='utf-8')
        msg['Subject'] = title
        msg['From'] = me
        msg['To'] = mail_senderTo
        try:
            server = smtplib.SMTP_SSL()
            server.connect(mail_host,465)
            server.login(mail_user,mail_pass)
            server.sendmail(me, mail_senderTo.split(','), msg.as_string())
            server.close()
        except Exception, e:
            print("Exception found:")
            print str(e)
            return 0

    # last week
    def start(self):
        bar=pyecharts.Bar("Last 7 days")
        bar.add("Unit(MB)",
                self.m_weekday,
                self.m_size,
                is_label_show = True,
                label_text_color ='#3367FF')
        img = './{0}.png'.format(self.m_cur)
        bar.render(path=img)
        (st,output) = commands.getstatusoutput('aws s3 cp {0} s3://qbigdata/status_report/'.format(img))
        print("Now, the image uploaded [OK]")
        time.sleep(5)

        # 2 days expiration
        cmd = 'aws s3 presign s3://qbigdata/status_report/{0}.png --expires-in 172800'.format(self.m_cur)
        (st,output) = commands.getstatusoutput(cmd)
        self.m_imgsrc = output
        print("the data:%s" %(self.m_imgsrc))

        mail_body = '<img src="{0}" />'.format(self.m_imgsrc)
        when = time.strftime("%m-%d", time.localtime(int(time.time())))
        self.send_email(mail_body, 'Data Collector Report({0})'.format(when))
        return 0


####Main
print("report begins...")
r = MyReport()
r.start()
#up.run_test()
