# -*- coding: utf-8 -*-

enum_dict = {}

def enum(enum_name, val=None, val_name=None, desc=None):
    
    if not enum_dict.has_key(enum_name):
        enum_dict[enum_name] = Enum(enum_name)
    
    if str(val) and val_name and desc:
        enum_dict[enum_name].add(val, val_name, desc)
    
    return enum_dict[enum_name]

class Enum(object):
    def __init__(self, enum_name):
        self.values = []
        
    def add(self, val, val_name, desc):
        self.values.append(EnumValue(val, val_name, desc))

    def __getattr__(self, name):
        for v in self.values:
            if v.name == name: return v
        return None
    
    def value(self, val):
        for v in self.values:
            if v.value == str(val): return v
        return None        

class EnumValue(object):
    def __init__(self, val, val_name, desc):
        self.value = str(val)
        self.name = val_name
        self.desc = desc
        
    def json_dict(self):
        result = {}
        result['id'] = self.value
        result['name'] = self.name
        result['desc'] = self.desc
        return result

#the coding used in client views
enum('bill_type', "save_money", '充值', "")
enum('bill_type', "fetch", '提取', "")
enum('bill_type', "expense", 'FB消费', "")
enum('bill_type', "transfer", '转账', "")
enum('bill_type', "check", '校对', "")

