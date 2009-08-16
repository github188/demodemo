from mysite.fb_book.models import *
from google.appengine.ext import db
from datetime import datetime

class BookDetailView(object):
    def __init__(self, book, user_list, record_list, min_row=30, min_col=12):
        self.book = book
        self.user_list = user_list
        self.fb_record_list = record_list
        self.min_row = min_row
        self.min_col = min_col
        
        #the book is cached, to refresh account balance from database.
        self.book_account = db.get(self.book.account.key())
        self.dummy_fb = BookAccountRecord.all()\
                        .filter("action =", "dummy").get()
        self.dummy_user = BookUser.all()\
                        .filter("name =", "dummy").get()
        self.__build_user_view(self.dummy_user)
        self.__fb_record_column_view = None
        self.__user_list_column_view = None
        
        self.fb_record_details = {}
        
        #self.user_list.insert(0, account)
    
    def user_active_in_fb(self, user, fb_record):        
        if user.name == 'dummy' or fb_record.action == 'dummy':
            return BookUserRecord(parent=self.book, 
                                  bookrecord=fb_record, 
                                  user=user,
                                  action='dummy')
            
        cached_key = "%s_%s" % (fb_record.__class__.__name__, 
                                fb_record.id,)
        record_details = self.fb_record_details.get(cached_key, None)
        if record_details is None:
            from google.appengine.api import memcache
            record_details = memcache.get(cached_key)
            if record_details is None:
                record_details = BookUserRecord.all().ancestor(self.book)\
                                .filter("bookrecord =", fb_record)
                record_details = dict(((e.user.name, e) for e in record_details))
                memcache.set(key=cached_key, value=record_details, time=3600, 
                                         namespace='fb_book')
            
            self.fb_record_details[cached_key] = record_details
            
        user_details = record_details.get(user.name, None)
        if user_details is None:
            user_details = BookUserRecord(parent=self.book, 
                                          bookrecord=fb_record, 
                                          user=user,
                                          action='dummy')
            record_details[user.name] = user_details
        user_details = self.__build_details_view(user_details)
        
        return user_details

    def fb_record_row_view(self):        
        def build_row_view(record):          
            user_details = []
            for u in self.user_list_column_view():
                user_details.append(self.user_active_in_fb(u, record))
            
            record = self.__build_fb_record_view(record)
            
            return {"record_view": record,
                    "user_details":user_details}
            
        for e in self.fb_record_list:
            yield build_row_view(e)
        for i in range(self.min_row - len(self.fb_record_list)):
            yield build_row_view(self.dummy_fb)
    
    def user_list_row_view(self):
        def build_row_view(user):          
            user_details = []
            for record in self.fb_record_column_view():
                user_details.append(self.user_active_in_fb(user, record))
            user = self.__build_user_view(user)
            
            return {"user_view": user,
                    "fb_records":user_details}
            
        yield build_row_view(self.book_account)
        for user in self.user_list:
            yield build_row_view(user)
        for i in range(self.min_row - len(self.user_list) - 1):
            yield build_row_view(self.dummy_user)

    def fb_record_column_view(self):
        #logging.info("create column_view:%s" % len(self.__fb_record_column_view))
        if self.__fb_record_column_view is None:
            self.__fb_record_column_view = list(self.fb_record_list)
            for e in self.__fb_record_column_view:
                self.__build_fb_record_view(e)
                
            for i in range(self.min_col - len(self.__fb_record_column_view)):
                self.__fb_record_column_view.append(self.dummy_fb)
        
        return self.__fb_record_column_view
        
    
    def user_list_column_view(self):
        if self.__user_list_column_view is None:
            self.__user_list_column_view = list(self.user_list)
            self.__user_list_column_view.insert(0, self.book_account)
            for e in self.__user_list_column_view:
                self.__build_user_view(e)
            for i in range(self.min_col - len(self.__user_list_column_view)):
                self.__user_list_column_view.append(self.dummy_user)
        
        #logging.info("__user_list_column_view:%s" % len(self.__user_list_column_view))
        return self.__user_list_column_view
    
    def __build_details_view(self, details):
        if details.user.name == details.bookrecord.host:
            details.expense_view = "<span class='master'>%0.2f</span>" % details.expense            
        else:
            details.expense_view = "%0.2f" % details.expense
        if details.action == 'dummy':
            details.expense_view = '0'
        if details.user.name == 'dummy' or details.bookrecord.action == 'dummy':
            details.expense_view = ''
                
        return details
    
    def __build_fb_record_view(self, record):
        if record.action != "dummy":
            record.book_date_view = record.book_date.strftime("%Y-%m-%d")
            record.expense_view = "%0.2f" % record.expense
        else:
            record.book_date_view = "&nbsp;"
            record.expense_view = "&nbsp;"
        
        return record

    def __build_user_view(self, user):
        if user.name == "dummy":
            user.balance_view = "&nbsp;"
            user.name_view = "&nbsp;&nbsp;&nbsp;&nbsp;"
        else:
            user.name_view = user.name            
            if user.balance >= 0:
                user.balance_view = "%0.2f" % user.balance
            else:
                user.balance_view = "<span class='master'>%0.2f</span>" % user.balance
        
        return user

