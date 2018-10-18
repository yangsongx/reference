#coding=utf-8

import os
import boto3
import commands
import csv
import datetime
import re
import requests
import time
import logging
import pyecharts
import smtplib
import matplotlib.pyplot as plt
import numpy as np

import sys
from email.mime.text import MIMEText
#logging.basicConfig(filename='./log.txt', format='%(asctime)s : %(message)s')

reload(sys) 
sys.setdefaultencoding('utf-8') 

class MyReport:
    m_weekday = []
    m_m4_crash_dev = []
    m_m4_crash_sum = []
    m_m4_crash_distinct = []
    m_m10_crash_dev = []
    m_m10_crash_sum = []
    m_m10_crash_distinct = []


    m_dev_count = []

    m_size = []
    m_file_size = []
    m_quota = []
    m_voice = []
    m_crash = []
    m_unknown = []
    m_unknown_ip = []

    m_cur = 0
    m_imgsrc = ''

    def __init__(self):
        self.read_data_from_redash()
        self.m_cur = int(time.time())

    def read_data_from_redash(self):
        m4_url = 'http://show.prod.qspeaker.com/api/queries/88/results/3111.csv?api_key=KtFCzB82rhS3llayX20cyNucg10Zr6J4uHDVn9ZN'
        m10_url = 'http://show.prod.qspeaker.com/api/queries/80/results/3106.csv?api_key=N5LDn4spO4z9gcBET3XskJW0HK4iTU7HNd20y41x'
        tmp_m4data = '/tmp/m4.csv'
        tmp_m10data = '/tmp/m10.csv'

        # first, let's get it for m4
        if False:
            req = requests.get(m4_url)
            data = req.text
            print("the data:%s", data)
            fd = open(tmp_m4data, 'w')
            fd.write(data)
            fd.close()

            req = requests.get(m10_url)
            data = req.text
            print("the data:%s", data)
            fd = open(tmp_m10data, 'w')
            fd.write(data)
            fd.close()

        index = 0
        m4_crash_csv = []
        with open(tmp_m4data) as f:
            reader = csv.reader(f)
            for row in reader:
                if index > 0:
                    m4_crash_csv.append(row)
                index = index + 1

        final_m4_crash_csv = m4_crash_csv[:7] # last 7 -days...
        print(final_m4_crash_csv)

        final_m4_crash_csv.reverse()
        print(final_m4_crash_csv)
        for it in final_m4_crash_csv:
            self.m_weekday.append(it[0])
            self.m_m4_crash_dev.append(it[1])
            self.m_m4_crash_sum.append(it[2])
            self.m_m4_crash_distinct.append(it[3])

        index = 0
        m10_crash_csv = []
        with open(tmp_m10data) as f:
            reader = csv.reader(f)
            for row in reader:
                if index > 0:
                    m10_crash_csv.append(row)
                index = index + 1

        final_m10_crash_csv = m10_crash_csv[:7] # last 7 -days...

        final_m10_crash_csv.reverse()
        print(final_m10_crash_csv)
        for it in final_m10_crash_csv:
            self.m_m10_crash_dev.append(it[1])
            self.m_m10_crash_sum.append(it[2])
            self.m_m10_crash_distinct.append(it[3])

        return 0

    def extract_data(self, full_list):

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

    def start(self):
        return self.start_with_matplotlib()

    # support sub plot
    def mask_start_with_matplotlib(self):
        print("sub plot matplotlib")

        N = len(self.m_weekday)
        ind = np.arange(N)    # the x locations for the groups

        m10_data = []
        m4_data = []
        stage_data = [] # for sum of bottom
        other_data = []

        idx = 0
        for it in self.m_weekday:
            d = float(self.m_size[idx]) * float(self.m_quota[idx][0])
            m4_data.append(d)

            d = float(self.m_size[idx]) * float(self.m_quota[idx][1])
            m10_data.append(d)
            d = float(self.m_size[idx]) * float(self.m_quota[idx][2])
            other_data.append(d)

            idx = idx + 1

        idx = 0
        for it in self.m_weekday:
            d = m10_data[idx] + m4_data[idx]
            stage_data.append(d)
            idx = idx + 1

        mylabel = ('3', '5', '2', '3', '3')
        width = 0.55       # the width of the bars: can also be len(x) sequence

        #### PLOT #### begin
        cur_fig_size = [12.0, 13.0]
        plt.rcParams["figure.figsize"] = cur_fig_size

        fig_1 = plt.subplot(4, 1, 1)
        fig_1.set_xlim(-0.5, 8.5)
        fig_1.grid(axis='y')
        plt.xticks(ind, self.m_weekday)

        fig_2 = plt.subplot(4, 1, 2)
        fig_2.set_xlim(-0.5, 8.5)
        fig_2.grid(axis='y')
        plt.xticks(ind, self.m_weekday)

        fig_3 = plt.subplot(4, 1, 3)
        fig_3.set_xlim(-0.5, 8.5)
        fig_3.grid(axis='y')

        fig_4 = plt.subplot(4, 1, 4)
        fig_4.set_xlim(-0.5, 8.5)
        fig_4.grid(axis='y')

        plt.subplots_adjust(wspace=0, hspace=0.4)


        plt.xticks(ind, self.m_weekday)
        
        # m4
        p1 = fig_1.bar(ind, m10_data, width, align='center', color = '#f00078')
        # m10
        p2 = fig_1.bar(ind, m4_data, width, align='center',  bottom=m10_data,  color='#00db54')
        # other
        p3 = fig_1.bar(ind, other_data, width, align='center',  bottom=stage_data,  color='#909d90')

        idx = 0
        for it in self.m_weekday:
            val = '%.0f%%\n%s' % (float(self.m_quota[idx][1]) * 100, self.m_dev_count[idx][1])
            print(val)
            fig_1.text(idx, m10_data[idx]/2, val, ha='center', va='center')

            val = '%.0f%%\n%s' % (float(self.m_quota[idx][0]) * 100, self.m_dev_count[idx][0])
            fig_1.text(idx, m4_data[idx]/2 + m10_data[idx], val, ha='center', va='center')

            fig_1.text(idx, m10_data[idx] + m4_data[idx] + other_data[idx],
                    self.m_size[idx], ha='center', va='bottom')

            idx = idx + 1

        fig_1.set_ylabel('Log Size')
#        plt.ylabel('Log Size')
        fig_1.set_title('Last 7-days log(Unit - MB)')
#        plt.xticks(ind, self.m_weekday)

        fig_1.legend((p1[0], p2[0], p3[0]), ('M10', 'M4', 'Blank'))

        ## sec subplot
        # m4
        fig_2.set_title('Last 7-days voice recognize')

        idx = 0
        val = []
        for it in self.m_weekday:
            val.append(self.m_voice[idx][0])
            idx = idx + 1
        fig_2.plot(ind, val, color= '#00db54', linewidth=6, marker = 'o', label = 'M4')
        idx = 0
        for it in self.m_weekday:
            fig_2.text(idx, self.m_voice[idx][0], self.m_voice[idx][0], ha='center', va='bottom')
            idx = idx + 1

        idx = 0
        val = []
        for it in self.m_weekday:
            val.append(self.m_voice[idx][1])
            idx = idx + 1
        fig_2.plot(ind, val, color = '#f00078', linewidth=6, marker = 'o', label = 'M10')

        idx = 0
        for it in self.m_weekday:
            fig_2.text(idx, self.m_voice[idx][1], self.m_voice[idx][1], ha='center', va='bottom')
            idx = idx + 1

        ## third subplot
        fig_3.set_title('Last 7-days crash')

        idx = 0
        val = []
        for it in self.m_weekday:
            val.append(self.m_crash[idx][0])
            idx = idx + 1

        fig_3.plot(ind, val, color= '#00db54', linewidth=6, marker = 'o', label = 'M4')
        idx = 0
        for it in self.m_weekday:
            fig_3.text(idx, self.m_crash[idx][0], self.m_crash[idx][0], ha='center', va='bottom')
            idx = idx + 1

        idx = 0
        val = []
        for it in self.m_weekday:
            val.append(self.m_crash[idx][1])
            idx = idx + 1
        fig_3.plot(ind, val, color = '#f00078', linewidth=6, marker = 'o', label = 'M10')

        idx = 0
        for it in self.m_weekday:
            fig_3.text(idx, self.m_crash[idx][1], self.m_crash[idx][1], ha='center', va='bottom')
            idx = idx + 1

        unknow_dev = []
        idx = 0
        for it in self.m_weekday:
            print(self.m_unknown[idx])
            print(type(self.m_unknown[idx]))
            unknow_dev.append(int(self.m_unknown[idx][0]))
            idx = idx + 1

        fig_4.bar(ind, unknow_dev, width, align='center', color='#CC9933')
        idx = 0
        for it in self.m_weekday:
            fig_4.text(idx, unknow_dev[idx], unknow_dev[idx], ha='center', va='bottom')
            val = 'IP:%d' %(int(self.m_unknown_ip[idx][0]))

            fig_4.text(idx, unknow_dev[idx]/2, val, ha='center', va='center')
            idx = idx + 1

        fig_4.set_title('Last 7-days null SN')

#        plt.tight_layout()
#        plt.show()
        img = './{0}.png'.format(self.m_cur)
        plt.savefig(img)
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

    def start_with_matplotlib(self):
        print("my code for matplotlib")


        N = len(self.m_weekday)
        ind = np.arange(N)    # the x locations for the groups

        # m4
        cur_fig_size = [19.0, 7.0]
        plt.rcParams["figure.figsize"] = cur_fig_size

        plt.subplots_adjust(wspace=0, hspace=0.4)

        fig_1 = plt.subplot(2,1,1)
        fig_1.set_xlim(-0.1, 8.2)
        plt.xticks(ind, self.m_weekday)
        fig_2 = plt.subplot(2,1,2)
        fig_2.set_xlim(-0.1, 8.2)
        fig_1.grid(axis='both')
        fig_2.grid(axis='both')
        plt.xticks(ind, self.m_weekday)

        fig_1.set_title('M4 - Last 7-days crash')
        fig_1.plot(ind, self.m_m4_crash_dev, label = 'Crash Devices')
        fig_1.plot(ind, self.m_m4_crash_distinct, label = 'Crash Types')
        fig_1.plot(ind, self.m_m4_crash_sum, label = 'Crash Sum')
#plt.legend((self.m_m4_crash_dev,self.m_m4_crash_distinct, self.m_m4_crash_sum), ('Crash设备数','Crash类型数','Crash总数'))
        fig_1.legend()

        plt.xticks(ind, self.m_weekday)

        fig_2.set_title('M10 - Last 7-days crash')
        fig_2.plot(ind, self.m_m10_crash_dev, label = 'Crash Devices')
        fig_2.plot(ind, self.m_m10_crash_distinct, label = 'Crash Types')
        fig_2.plot(ind, self.m_m10_crash_sum, label = 'Crash Sum')
#plt.legend((self.m_m4_crash_dev,self.m_m4_crash_distinct, self.m_m4_crash_sum), ('Crash设备数','Crash类型数','Crash总数'))
        fig_2.legend()
#        plt.tight_layout()
#        plt.show()
        img = './{0}.png'.format(self.m_cur)
        plt.savefig(img)
#        (st,output) = commands.getstatusoutput('aws s3 cp {0} s3://qbigdata/status_report/'.format(img))
#        print("Now, the image uploaded [OK]")
#        time.sleep(5)

        # 2 days expiration
#        cmd = 'aws s3 presign s3://qbigdata/status_report/{0}.png --expires-in 172800'.format(self.m_cur)
#        (st,output) = commands.getstatusoutput(cmd)
#        self.m_imgsrc = output
#        print("the data:%s" %(self.m_imgsrc))

#        mail_body = '<img src="{0}" />'.format(self.m_imgsrc)
#        when = time.strftime("%m-%d", time.localtime(int(time.time())))
#        self.send_email(mail_body, 'Data Collector Report({0})'.format(when))

        return 0

    # last week
    def start_with_pychart(self):
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
