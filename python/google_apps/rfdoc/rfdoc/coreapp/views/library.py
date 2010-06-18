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


import re

from django.shortcuts import render_to_response, get_object_or_404

from rfdoc.coreapp.models import *
from rfdoc.coreapp import utils


def library(request, libname):
    #lib = get_object_or_404(Library, name=libname)
    library_group = LibraryGroup.get_or_create('official')
    
    library = Library.get_by_name(library_group, libname)
    if library:
        libdoc = LibraryDoc(library)
    else:
        return HttpResponseRedirect("/index")
    
    return render_to_response('library.html', {'lib': libdoc })


class _DocHelper:
    # This code is adapted from libdoc.py, see
    # http://code.google.com/p/robotframework/wiki/LibraryDocumentationTool

    _name_regexp = re.compile("`(.+?)`")

    def _get_htmldoc(self):
        doc = utils.html_escape(self._doc, formatting=True)
        try:
            result = self._name_regexp.sub(self._link_keywords, doc)
            return result
        except:
            return doc

    def _link_keywords(self, res):
        name = res.group(1)
        keywords = self.keywords if isinstance(self, LibraryDoc) else self._library.keywords
        for kw in keywords:
            if utils.eq(name, kw.name):
                return '<a href="#%s" class="name">%s</a>' % (kw.name, name)
        if utils.eq_any(name, ['introduction', 'library introduction']):
            return '<a href="#introduction" class="name">%s</a>' % name
        if utils.eq_any(name, ['importing', 'library importing']):
            return '<a href="#importing" class="name">%s</a>' % name
        return '<span class="name">%s</span>' % name

    doc = property(_get_htmldoc)

class LibraryDoc(_DocHelper):

    def __init__(self, libdata):
        self.name = libdata.name
        self._doc = libdata.doc
        self.version = libdata.version
        self.inits = [ KeywordDoc(initdata, self) 
                       for initdata in libdata.init_set ]
        self.keywords = [ KeywordDoc(kwdata, self)
                          for kwdata in libdata.keyword_set ]
 
class KeywordDoc(_DocHelper):

    def __init__(self, kwdata, library):
        self.name = kwdata.name
        self.shortname = kwdata.name.split('@')[0]
        self.args = kwdata.args
        self._doc = kwdata.doc
        self.shortdoc = self._doc.split('\n')[0]
        self._library = library
        self.kwdoc = kwdata

    def __getattr__(self, name):
        return getattr(self.kwdoc, name)
    
