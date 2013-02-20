# -*- coding: utf-8 -*-
import urllib.request, http.cookiejar
req = urllib.request.Request("http://192.168.0.187:8088/callback/logon.php?userId=1234567890&key=012345678901234567890123&channelId=xx&region=0&cpparam=xx")
try:
    content = urllib.request.urlopen(req).read()    
    print(content)
except urllib.request.URLError as e:
    print(e)

