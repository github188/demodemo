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


from xml.etree import cElementTree as ET

from django.http import HttpResponse, HttpResponseRedirect
from django.shortcuts import render_to_response
from django import forms
from django.forms.util import ErrorList

from google.appengine.api import users
from rfdoc.coreapp.models import *
import logging


def upload(request):
    if not users.get_current_user():
        return HttpResponseRedirect(users.create_login_url(request.path))

    libname = None
    if request.method == 'POST':
        form = UploadFileForm(request.POST, request.FILES)
        if form.is_valid():
            libname = _import_keyword_doc(form.cleaned_data['library_group'],
                                request.FILES['file'],
                                form.cleaned_data['override'],
                                _errors=form._errors
                                )
    else:
        form = UploadFileForm()
        
    return render_to_response('upload.html', {'form': form, 'libname': libname})

def _import_keyword_doc(library_group, fileobj, override=True, _errors=[]):
    library_group = library_group or 'official'
    library_group = LibraryGroup.get_or_create(library_group)
    
    logging.info("import keyword doc, library_group:%s" % library_group)
    try:
        libdata = LibraryData(fileobj)
        
        library = Library.get_by_name(library_group, libdata.name)
        if library:
            if not override:
                raise InvalidXmlError("Library %s already exists." % libdata.name)
            else:
                db.delete(Keyword.all().filter('library =', library).ancestor(library_group).fetch(1000))
                db.delete(LibraryInit.all().filter('library =', library).ancestor(library_group).fetch(100))
                library.delete()
                #Library.objects.filter(name=libdata.name).delete()
        
        lib = Library(parent=library_group, name=libdata.name, doc=libdata.doc, version=libdata.version)
        lib.put()
        for init in libdata.inits:
            i = LibraryInit(parent=library_group, library=lib, doc=init.doc, args=init.args)
            i.put()
        
        for kw in libdata.kws:
            k = Keyword(parent=library_group, library=lib, name=kw.name, doc=kw.doc, args=kw.args)
            k.put()

    except InvalidXmlError, err:
        _errors['file'] = ErrorList([str(err)])
        return None
    return lib.name

class UploadFileForm(forms.Form):
    library_group = forms.CharField(label="Library Group", required=False, initial='robot')
    file = forms.FileField()
    file.widget.attrs['size'] = 40
    override = forms.BooleanField(required=False)

class LibraryData(object):
    
    def __init__(self, fileobj):
        root = self._get_root(fileobj)
        try:
            self.name = self._get_name(root)
            self.version = self._get_version(root)
            self.doc = self._get_doc(root)
        except InvalidXmlError:
            raise InvalidXmlError('Given file contains invalid XML.')
        self.inits = [ InitData(data) for data in self._get_inits(root) ]
        self.kws = [ KeywordData(data) for data in self._get_keywords(root) ]

    def _get_root(self, fileobj):
        try:
            root = ET.parse(fileobj).getroot()
        except SyntaxError:
            raise InvalidXmlError('Given file is not XML.')
        if root.tag != 'keywordspec':
            raise InvalidXmlError('Given file contains invalid XML.')
        return root

    def _get_name(self, elem):
        return get_attr(elem, 'name')

    def _get_doc(self, elem):
        return get_child_element(elem, 'doc').text or ''

    def _get_version(self, elem):
        # libdoc.py didn't add version in 2.1 and earlier
        try:
            version_elem = get_child_element(elem, 'version')
        except InvalidXmlError:
            version = None
        else:
            version = version_elem.text
        return version or '<unknown>'

    def _get_inits(self, elem):
        return elem.findall('init')
    
    def _get_keywords(self, elem):
        # 'keywords/kw' is backwards compatibility for libdoc.py 2.1 and earlier
        kws = elem.findall('keywords/kw') + elem.findall('kw')
        if not kws:
            raise InvalidXmlError('Given test library contains no keywords.')
        return kws


class KeywordData(object):

    def __init__(self, elem):
        try:
            self.name = self._get_name(elem)
            self.doc = self._get_doc(elem)
            self.args = ', '.join(arg.text for arg in self._get_args(elem))
        except InvalidXmlError:
            raise InvalidXmlError('Given file contains invalid XML.')

    def _get_name(self, elem):
        return get_attr(elem, 'name')

    def _get_doc(self, elem):
        return get_child_element(elem, 'doc').text or ''

    def _get_args(self, elem):
        return get_child_element(elem, 'arguments').findall('arg')


class InitData(KeywordData):

    def _get_name(self, elem):
        return '<init>'


def get_attr(elem, attr_name):
    attr = elem.get(attr_name)
    if not attr:
        raise InvalidXmlError
    return attr

def get_child_element(elem, child_name):
    child = elem.find(child_name)
    if child is None:
        raise InvalidXmlError
    return child


class InvalidXmlError(TypeError):
    pass

