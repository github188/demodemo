
from wiki import Page
from google.appengine.api import users

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
    
    # User must be logged in to edit
    if not users.get_current_user() and (r.method == 'POST' or mode == 'edit'):
      #The GET version of this URI is just the view/edit mode, which is a
      #reasonable thing to redirect to
      #self.redirect(users.create_login_url(self.request.uri))
      return ("redirect:%s" % users.create_login_url(r.path), )
    
    if r.method == 'POST':
        page.content = r.POST.get('content', "")
        page.save()
        mode = "view"

    return ("wiki_%s.html" % mode, {'page': page,
                                    'user': users.get_current_user(),
                                    'logout_url': users.create_logout_url(r.path),
                                    'login_url': users.create_login_url(r.path)
                                    })
