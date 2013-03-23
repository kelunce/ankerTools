# -*- coding: utf-8 -*-
import urllib.request, http.cookiejar, re

postdata = urllib.parse.urlencode({
    'email': '4545@qq.com',
    'pwd': 'safafa',
    'save_login': 1
    })


headers = {
    'User-Agent': 'Mozilla/5.0 (Windows NT 5.1; rv:14.0) Gecko/20100101 Firefox/14.0.1'
    }

postdata = postdata.encode('gbk')
req = urllib.request.Request(
    url = 'http://192.168.0.163:8081',
    data = postdata,
    headers = headers
    )
try:
    content = urllib.request.urlopen(req).read()    
    print(content)
except urllib.request.URLError as e:
    print(e)

