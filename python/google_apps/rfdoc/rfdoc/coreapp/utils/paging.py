# -*- coding: utf-8 -*-

class Paging(object):
    def __init__(self, count, cur_page=0, url="x", limit=10):
        self.count = count
        self.cur_page = cur_page
        self.url = url
        self.limit = limit
        
        self._output_html = None
    
    def output_html(self):
        """
        << 0 1 2 3 4 ... >>
        """
        
        if self._output_html is not None: return self._output_html
        
        page_count = self.count / self.limit
        if self.count % self.limit != 0: page_count += 1
        cur_page_range = max((self.cur_page - 1) / 10, 0)
        end_page = min(page_count, (cur_page_range + 1) * 10) + 1
        
        menu = []
        
        pre_page = max(self.cur_page -1, 1)
        url_link = self.url.replace("PAGE", str(pre_page))
        menu.append(u"<a href='%s'><<</a>" % url_link)
        for page in range(cur_page_range * 10 + 1, end_page):
            cur_style = page == self.cur_page and "class='cur'" or ''
            url_link = self.url.replace("PAGE", str(page))
            menu.append("<a href='%s' %s>%s</a>" % (url_link, cur_style, page))
        
        next_page = min(self.cur_page + 1, page_count)
        url_link = self.url.replace("PAGE", str(next_page))
        menu.append(u"<a href='%s'>>></a>" % url_link)
        
        self._output_html = "".join(menu)
        return self._output_html
        
if "__main__" == __name__:
    p = Paging(190, 10, url="PAGE")
    print p.output_html()        