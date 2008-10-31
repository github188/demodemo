#-*- encoding:utf-8 -*-

class User(object):
    """
    @table:sales_user
    @id:user_id, 0, INTEGER
    @field:username, 64, varchar
    @field:password, 64, varchar
    @field:email, 64, Varchar
    
    @field:order_id, 64, Varchar
    @field:order_date, 64, Varchar
    @field:count_score, 4, INTEGER
    
    @field:true_name, 64, Varchar
    @field:start_date, 64, Varchar
    @field:title, 64, Varchar
    @field:salary, 64, Varchar
    @field:new_title, 64, Varchar
    @field:new_salary, 64, Varchar
    
    @field:phone, 64, Varchar
    """
    
    def __init__(self, username='', email='', password=''):
        self.username = username
        self.email = email
        self.password = password
    
class Active(object):
    """
    @table:sales_active
    @id:active_id, 0, INTEGER
    
    @field:category, 64, Varchar
    @field:text, 255, Text
    
    @field:read_times, 4, INTEGER
    @field:rewards, 4, INTEGER

    @field:count_score, 4, INTEGER
    @field:feel_score, 4, INTEGER
        
    @relation:owner,one,User,user_id
    """
    
    def __init__(self, owner='', category='', text=''):
        self.owner = owner
        self.category = category
        self.text = text

