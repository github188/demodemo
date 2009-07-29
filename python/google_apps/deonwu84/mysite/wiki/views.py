
from wiki import Page

def pre_handler(r, page_name="MainPage"):
    mode = r.REQUEST.get('mode', 'view')
    page = Page.load(page_name or "MainPage")
    # Default to edit for pages that do not yet exist
    if not page.entity:
      mode = 'edit'
    else:
      modes = ['view', 'edit']
      if not mode in modes:
        mode = 'view'
    
    if r.method == 'POST':
        page.content = r.POST.get('content', "")
        page.save()
        mode = "view"

    return ("wiki_%s.html" % mode, {'page': page, })
