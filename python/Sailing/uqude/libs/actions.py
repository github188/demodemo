# -*- coding: utf8 -*-
import logging
import re

class LoginUqude(object):
    def __init__(self, ):
        self.logger = logging.getLogger("uqude")
        
    def __call__(self, site, http, next_task, url, local_url, *args):
        self.logger.info("start login url............")
        
        
class FetchGuessTopic(object):
    def __init__(self, ):
        self.logger = logging.getLogger("uqude")
    
    def __call__(self, site, http, next_task, url, local_url, *args):
        
        self.logger.info("start fetch topic, url:%s" % url)
        data = http.get(url)
        

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


    