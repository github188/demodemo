# Copyright 2009 Nokia Siemens Networks Oyj
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#     http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


from django.shortcuts import render_to_response
from django import forms
from django.db.models import Q

from rfdoc.coreapp.models import *
from rfdoc.coreapp.utils.paging import Paging

def search(request):
    kws = []
    term = ''
    form = SearchForm(request.REQUEST)
    term = request.REQUEST.get('search_term', '').strip()
    if not term:
        from index import index 
        return index(request)
    
    library_group = LibraryGroup.get_or_create('official')
    
    kws = Keyword.all().ancestor(library_group).search(term,
                                                       properties=['name', 'doc'])
    
    offset = int(request.REQUEST.get('page', '1').strip())
    limit = int(request.REQUEST.get('limit', '50').strip())
    keyword_count = kws.count()
    if keyword_count > limit:
        page_url = "?search_term=%s&limit=%s&page=PAGE" % (term, limit)
        page_nav = Paging(keyword_count, offset, page_url, limit)
    else:
        page_nav = None
    
    from library import KeywordDoc
    start_index, end_index = (offset -1 ) * limit, offset * limit
    kws = [ KeywordDoc(e, e.library) for e in kws.fetch(limit, start_index) ]   
    result_page = kws and "search.html" or "not_found_keyword.html"
    
    return render_to_response(result_page, {'form': form, 'kws': kws,
                                            'term':term,  
                                            'keyword_count': keyword_count,
                                            'page_nav':page_nav})


class SearchForm(forms.Form):
    search_term = forms.CharField() 
    include_doc = forms.BooleanField(required=False)

