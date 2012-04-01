# -*- coding: utf8 -*-
import logging
import re
import json
import os

class LoginUqude(object):
    def __init__(self, ):
        self.logger = logging.getLogger("uqude")
        
    def __call__(self, site, http, next_task, url, local_url, *args):
        self.logger.info("start login url............")
        
        
class FetchGuessTopic(object):
    def __init__(self, ):
        self.logger = logging.getLogger("uqude")
        
        self.parser = TopicParser()
    
    def __call__(self, site, http, next_task, url, local_url, *args):        
        local_abs_path = site.real_path(local_url)
        if os.path.isfile(local_abs_path):
            self.logger.info("the topic is exist in local:%s" % url)
            return
        
        self.logger.info("start fetch topic, url:%s" % url)
        data = http.get(url)
        
        topic = self.parser.process(data, {})
        
        if len(topic['options']) == 0:
            self.logger.info("Not found options, It may is not a guess topic")
            return
        
        if not (topic['coverId'] and topic['coverSubjectId']):
            self.logger.info("Not found 'coverId' and 'coverSubjectId', It may is not a guess topic")
            return
        
        values = topic['options'].values()        
        post_data = {'optionId': values[0]['id'], 
                     'coverId': topic['coverId'],
                     'coverSubjectId': topic['coverSubjectId'],
                     }
        
        headers = {'User-Agent': 'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/535.11 (KHTML, like Gecko) Chrome/17.0.963.83 Safari/535.11',
                   'Content-Type': 'application/x-www-form-urlencoded',
                   'Cookie': 'JSESSIONID=5E452CC6EBCE84F422FA6752E5BBF8C9; cnzz_a1741434=19; sin1741434=; rtime=4; ltime=1333264920274; cnzz_eid=92129616-1332144602-'
                   }
        
        answer = http.post_data("http://www.uqude.com/guess/answer", post_data, headers)
        
        an = self.parser.process_answer(answer, {})
        if not an.get('desc', ''):
            self.logger.info("not found answer...")
        
        ok = False
        for e in values:
            if e['desc'] == an['options']:
                e['right'] = 'y'
                e['answer_desc'] = an.get('desc', '')
                ok = True
                break
        
        if not ok:
            self.logger.info("answer process error, ignore the topic...")
        
        self.logger.info("Fetch guess topic ok, url:%s" % url)
        self.save_topic_data(topic, local_abs_path)
        
        next_task.add_action("%s==>%s" % (url, local_url))
        
        
    def save_topic_data(self, topic, local_path):
        from sailing.common.common import *
        if not exists_path(dir_name(local_path)): make_path(dir_name(local_path))
        
        fd = open(local_path, 'w')
        fd.write(json.dumps(topic))
        fd.close()        
        

ALL_ATTRS = "[^>]+"
EMPTY = "\s+"
        
class TopicParser(object):
    
    def process(self, html, topic):
        t = {}
        t['title'] = self._fetch_topic_title(html)
        t['images'] = self._fetch_topic_images(html)
        t['desc'] = self._fetch_topic_desc(html)
        t['options'] = self._fetch_topic_options(html)
        
        t.update(self._fetch_toipc_id(html))
        
        return t
    
    def process_answer(self, html, topic):
        an = self._fetch_correct_option(html)
        
        return an
    
    def _fetch_topic_title(self, data):
        """
    <div class="icons icons2">

      <h1>

        许仙不在我们这里，真的不在，白素贞你就收手吧！</h1>
      """

        pattern = r'<div class="icons icons2">\s+<h1>([^<]+)</h1>'
        t = re.search(pattern, data, re.S)
        if t:
            return t.group(1).strip()
            
        #if re.
    
    def _fetch_topic_desc(self, data):
        """<div class="description">

            <p class="f14">

              这是什么体？<br></p>"""
              
        pattern = r'<div class="description">\s+<p class="f14">(.*?)</p>\s+<form'

        t = re.search(pattern, data, re.S)
        if t:
            return t.group(1).strip()


    def _fetch_topic_images(self, data):
        """
        <p class="append-img"> <img src="http://static.uqude.com/uploads/v2/0D/20120309/1331277758386_357.300s600s0.jpg" /> </p>
        """   
             
        pattern = r'<p class="append-img">\s+<img\s+src="([^"]+)"\s+/>\s+</p>'

        t = re.search(pattern, data, re.S)
        if t:
            return t.group(1).strip()
        
    def _fetch_topic_options(self, data):
        """
                          <td colspan="2"><div class="model-box model-box-h V174161">

                              <p class="clear"> <span class="left">

                                <input type="radio" value="174161" class="radio" name="optionId"/>

                                淘宝体</span> </p>

                            </div></td>

        """
        
        regex = r'<td\s+colspan="2"><div\s+class="model-box.*?">\s+'
        regex += r'<p class="clear">\s+<span class="left">\s+'
        regex += r'<input type="radio" value="(\d+)" class="radio" name="optionId"/>'
        regex += r'(.*?)</span>\s+</p>\s+'
        regex += r'</div></td>'
        
        #print "xxxxxxxxxxxxxx:" + regex 
        #<input type="hidden" name="coverId" value="37920" id="coverId"/><input type="hidden" name="coverSubjectId" value="234644" id="coverSubjectId"/>
        options = {}
        factor = re.compile(regex, re.S)
        for item in factor.finditer(data):
            #print item.groups()
            options[item.group(1)] = {'id':item.group(1), 'desc':item.group(2).strip(), }
            
        return options
    
    def _fetch_toipc_id(self, data):
        """<input type="hidden" name="coverId" value="37920" id="coverId"/><input type="hidden" name="coverSubjectId" value="234644" id="coverSubjectId"/>
        """

        regex = r'<input type="hidden" name="coverId" value="(\d+)" id="coverId"/><input type="hidden" name="coverSubjectId" value="(\d+)" id="coverSubjectId"/>'
        
        ids = {}
        
        t = re.search(regex, data, re.S)
        if t:
            ids['coverId'] = t.group(1)
            ids['coverSubjectId'] = t.group(2)
        
        return ids
    
    def _fetch_correct_option(self, data):
        """
<div class="model-box model-box-h model-proper">
                            <p class="clear"> <span class="left">
                              <input type="radio" name="" class="radio" value=""  disabled="disabled"/>
                              白素贞体</span> <span class="right"> <span class=" proper-icons"></span> <span class="green f12">正确答案</span> </span> </p>
                          </div>

                          <div class="prompt mt20 clear">
                                              <span class="left resultPro"> 
                                                  <span class="bold">答案说明：</span>
                                                  <span class="guessResult">近日白素贞体在网络流行，南方一直阴雨绵绵，网友很无奈发明白素贞体，白素贞体的句式是“许仙不在这里”，白素贞体的流行让不少网友恶搞不断，纷纷发留言称“许仙不在我们这里，真的不在，白素贞你就收手吧”。<br><br></span>
                                              </span> 
                                              <span class="right">
                                                  <a class="blue" href="javascript:void(0)" onclick="AT.guess.showResult(this)">展开全部»</a>
                                              </span>
                                            </div> 
"""                                                    
        regex = r'<div class="model-box model-box-h model-proper">\s+'
        regex += r'<p class="clear"> <span class="left">\s+'
        regex += r'<input type="radio" name="" class="radio" value=""  disabled="disabled"/>(.*?)</span>\s+'
        
        regex += r'<span.*?<span class="guessResult">(.*?)</span>'
        
        answer = {}
        t = re.search(regex, data, re.S)
        if t:
            answer['options'] = t.group(1).strip()
            answer['desc'] = t.group(2).strip()
        
        return answer
    