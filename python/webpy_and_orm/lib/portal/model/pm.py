"""
    A persistent manager, the concept is learning JPA. 
    
    To use webpy.db module as persistent engine implement O/R mapping.
"""
from __future__ import with_statement
import web
import logging
#logging.basicConfig(level=logging.DEBUG)

class PersistentManager(object):
    #only for debug...
    _global_class_loader_ = None
    cache = {}
    
    def __init__(self, name, module, dbn, auto_schema, **keywords):
        self.name = name
        web.db.connect(dbn, **keywords)
        self.syslog = logging.getLogger('portal.pm')
        
        if self._global_class_loader_ == None:
            self._global_class_loader_ = PersistentClassLoader(module)
            
        self.class_loader = self._global_class_loader_
        self._check_mapped_data_model(self.class_loader.pm_class, auto_schema)
        #self.cache = {}

    def load_obj(self, cls, ids, raise_unqiue=True):
        cls = self.get_cls_type(cls)
        obj_id = cls.__name__ + str(ids)
        if not self.cache.has_key(obj_id):
            if isinstance(ids, dict):
                where = ' and '.join([ '%s=$%s' % (e, e) for e in ids.keys() ])
            else:
                where = "%s = $id" % cls._pm_id_column_
                ids = {'id' : ids}
            
            enties = self.select(cls, where, ids)
            if len(enties) > 1 and raise_unqiue:
                raise RuntimeError, "Not a unqiue key '%s' for '%s'." % (repr(ids),
                        cls._pm_db_table_)
            self.cache[obj_id] = len(enties) > 0 and enties[0] or None
                    
        return self.cache[obj_id]
    
    def persist(self, obj):
        
        if obj._pm_state_ in [ 'persisted', 'proxy' ]: return
        self._persist_one_relation(obj)
        
        _pm_var_ = {}
        #with web.transaction():
        if obj._pm_state_ == 'New':
            self._run_fixture(obj, 'save')
            for f in obj._pm_fields_:
                _pm_var_[f[0]] = getattr(obj, f[0])            
            del _pm_var_[obj._pm_id_column_]
            self.syslog.debug("insert:" + web.insert(obj._pm_db_table_, _test=True, **_pm_var_))
            new_id = web.insert(obj._pm_db_table_, **_pm_var_)
            setattr(obj, obj._pm_id_column_, new_id)
            obj._pm_updated_field_ = set()
            #??
            obj._pm_state_ = 'persisted'
            self._persist_list_relation(obj)
            
        elif obj._pm_updated_field_:
            self._run_fixture(obj, 'update')
            for f in obj._pm_updated_field_:
                _pm_var_[f] = getattr(obj, f)
                
            sql = web.update(obj._pm_db_table_,
                       where="%s = $id" % obj._pm_id_column_,
                       vars={'id':getattr(obj, obj._pm_id_column_)},
                       _test=True,
                       **_pm_var_)
            self.syslog.debug("update:" + sql)                      
            web.update(obj._pm_db_table_,
                       where="%s = $id" % obj._pm_id_column_,
                       vars={'id':getattr(obj, obj._pm_id_column_)},
                       **_pm_var_)
            
        obj._pm_state_ = 'persisted'

    def _persist_one_relation(self, obj):
        try:
            relations = obj._relations_
        except AttributeError:
            return
        
        for name, map_type, model_type, col in relations.values():
            if map_type == 'one':
                value = getattr(obj, name)
                if value != None and getattr(obj, col) is None:
                    if not getattr(value, value.__class__._pm_id_column_):
                        self.persist(value)
                    setattr(obj, col, getattr(value, value.__class__._pm_id_column_))
                            
    def _persist_list_relation(self, obj):
        try:
            relations = obj._relations_
        except AttributeError:
            return
        
        for name, map_type, model_type, col in relations.values():
            if map_type == 'list':
                value = getattr(obj, name)
                if value != None:
                    for item in value:
                        setattr(item, col, getattr(obj, obj._pm_id_column_))
                        self.persist(item)
    
    def delete(self, obj):
        #with web.transaction():
        self.evil(obj)
        self._run_fixture(obj, 'delete')
        web.delete(obj._pm_db_table_, where="%s = $id" % obj._pm_id_column_,
                 vars={'id':getattr(obj, obj._pm_id_column_)})
    
    def select(self, cls, where=None, vars=None, order=None, group=None, 
               limit=None, offset=None):
        
        cls = self.get_cls_type(cls)
        if cls._pm_where_ is not None:
            where += " and " + cls._pm_where_
        
        self.syslog.debug("Select SQL:" + web.select(cls._pm_db_table_, 
                            vars=vars, 
                            where=where, 
                            limit=limit,
                            order=order,
                            offset=offset,
                            group=group, _test=True))
        
        results = web.select(cls._pm_db_table_, 
                            vars=vars, 
                            where=where, 
                            limit=limit,
                            order=order,
                            offset=offset,
                            group=group)
        #print results
        
        return self._mapping_to_model(cls, results)
    
    def sqlquery(self, cls, sql_query, vars=None, order=None, limit=None, offset=None):
        
        cls = cls and self.get_cls_type(cls) or cls
        
        if vars == None: vars = {}
        
        #replace ${ClassName} as table name
        import re
        for table in re.finditer("\\$\\{(\w+)\\}", sql_query):
            table = table.group(1)
            table_cls = self.get_cls_type(table)
            sql_query = sql_query.replace('${%s}' % table, table_cls._pm_db_table_)
            
        for (sql, val) in (('ORDER BY', order),
                          ('LIMIT', limit),
                          ('OFFSET', offset)
                           ):
            if val:
                sql_query += ' %s %s' % (sql, val)
        
        self.syslog.debug("Select SQL for sqlquery:%s" % sql_query)        
        results = web.query(sql_query, vars)
        
        if cls:
            return self._mapping_to_model(cls, results)
        else:
            return results
    
    def evil(self, obj):
        """
            Remove the object from cache, and disattach the obj from PM. 
            the object stauts will not be persistent after the object is evil.
        """
        #with web.transaction():
        obj_id = obj.__class__.__name__ + str(getattr(obj, obj._pm_id_column_))
        if self.cache.has_key(obj_id):
            del self.cache[obj_id]
            return True
        return False
    
    def evil_all(self):
        self.cache.clear()
        return True
    
    def cache_obj(self, obj):
        obj_id = obj.__class__.__name__ + str(getattr(obj, obj._pm_id_column_))
        self.cache[obj_id] = obj
    
    def commit(self):
        web.db.commit()
    
    def rollback(self):
        web.db.rollback()
        
    def drop_model(self, cls):
        cls = self.get_cls_type(cls)
        cursor = web.ctx.db_cursor()
        try:
            drop_sql = self.class_loader.export_drop_table(cls)
            self.syslog.debug("Drop model:" + drop_sql)
            cursor.execute(drop_sql)
        except:
            import sys
            type, value, my_traceback = sys.exc_info()
            if 'no such table' in str(value):
                pass         
        finally:
            cursor.close()
        
    def get_cls_type(self, cls):
        if type(cls) == type(''):
            if not self.class_loader.named_class.has_key(cls):
                raise RuntimeError, "Not found class mapping '%s'." % cls
            return self.class_loader.named_class[cls]
        else:
            return cls
        
        
    def _check_mapped_data_model(self, cls_list, auto_schema):
        for cls in cls_list:
            cursor = web.ctx.db_cursor()
            try:
                cursor.execute('select 1 from %s' % cls._pm_db_table_)
            except:
                import sys
                type, value, my_traceback = sys.exc_info()
                if 'no such table' in str(value):
                    self.syslog.info("Not found table:" + cls._pm_db_table_)
                    if 'create' in auto_schema:
                        create_sql = self.class_loader.export_sql_schema(cls)
                        self.syslog.info("Create:" + create_sql)
                        cursor.execute(create_sql)
                        self.syslog.info("Create new table:" + cls._pm_db_table_)
                else:
                    raise
            cursor.close()
    
    def _mapping_to_model(self, cls, datas):
        enties = []
        for res in datas:
            obj_id = cls.__name__ + str(res[cls._pm_id_column_])
            if not self.cache.has_key(obj_id):
                entity = cls()
                entity._pm_fill_object_(res)                
                self.cache[obj_id] = entity
                self._init_lazy_loading_relations(entity)
                self._run_fixture(entity, 'load')
                
            enties.append(self.cache[obj_id])
        
        return enties
    
    def _run_fixture(self,obj, name):
        try:
            f = getattr(obj, name)
        except AttributeError:
            return
        f(self)
    
    class lazy_loading_proxy(object):
        def __init__(self, pm, obj, relation):
            def loader():
                (name, map_type, model_type, col) = relation
                data = None
                id_value = None
                if map_type == 'list':
                    id_value = getattr(obj, obj._pm_id_column_)
                    data = pm.select(model_type, '%s=$id' % col, {'id':id_value})
                elif map_type == 'one':
                    id_value = getattr(obj, col)
                    data = pm.load_obj(model_type, id_value)
                else:
                    raise RuntimeError, "not supported relation type '%s'" % map_type                
                setattr(self, 'proxy', data)
                setattr(obj, name, data)
                if data is None:
                    syslog = logging.getLogger('portal.pm')
                    syslog.warn("not entity:%s id:%s in %s" % (str(relation), id_value, str(obj)))
                return data
                
            self.loader = loader
            self.proxy = None
            self._pm_state_ = 'proxy'
        
            (name, map_type, model_type, col) = relation
            self._str_ = '{proxy:%s<%s> in %s}' % (name, model_type, obj.__class__.__name__)
            
            
        def __getattribute__(self, name):
            proxy = object.__getattribute__(self, "proxy")
            if name == '_pm_state_' and proxy == None:
                return 'proxy'
            
            if proxy == None:
                proxy = object.__getattribute__(self, "loader")()
                                
            if proxy != None:
                return proxy.__getattribute__(name)
            else:
                raise RuntimeError, "not found entity..."
            
        def __iter__(self):
            proxy = object.__getattribute__(self, "proxy")
            if proxy == None:
                proxy = object.__getattribute__(self, "loader")()
            return proxy and iter(proxy) or iter([])
        
        def __getattr__(self, name):
            proxy = object.__getattribute__(self, "proxy")
            if proxy == None:
                proxy = object.__getattribute__(self, "loader")()
            return getattr(proxy, name)
        
        def __getitem__(self, key):
            proxy = object.__getattribute__(self, "proxy")
            if proxy == None:
                proxy = object.__getattribute__(self, "loader")()
                
            return proxy[key]
        
        def __str__(self):
            return object.__getattribute__(self, "_str_")
        
        def __cmp__(self, other):
            proxy = object.__getattribute__(self, "proxy")
            if proxy == None:
                proxy = object.__getattribute__(self, "loader")()
            
            return proxy.__cmp__(other)
             
    
    def _init_lazy_loading_relations(self, obj):
        try:
            relations = obj._relations_
        except AttributeError:
            relations = {}
        for relation in relations.values():
            (name, map_type, model_type, col) = relation
            setattr(obj, relation[0], self.lazy_loading_proxy(self, obj, relation))        
            
class PersistentClassLoader(object):
    """ A class loader for searching data object.
    """
    
    def __init__(self, obj=None):
        self.pm_class = []
        self.named_class = {}
        if obj is None: return
        import types
        if type(obj) in [ types.ClassType, types.TypeType ]:
            self.add_class(obj)
        else:
            self.add_module(obj)
    
    def add_module(self, mod):
        
        """
        try:
            getattr(mod, "_pm_loaded_")
        except AttributeError:
            setattr(mod, '_pm_loaded_', True)
        """ 
        import types
        for key in dir(mod):
            if type(getattr(mod, key)) in [ types.ClassType, types.TypeType ]:
                cls = getattr(mod, key)
                self.add_class(cls)                        
        try:
            import os
            sub_mod = []
            for mod in os.listdir(mod.__path__[0]):
                if mod[0] in ['_', '.'] or mod[-3:] is not '.py':
                    continue
                sub_mod.append(os.path.splitext(e)[0])
        except AttributeError:
            sub_mod = []
        
        for name in sub_mod:
            new_mod = __import__(mod.__name__ + '.' + name, globals(), locals(), [ name ])
            self.add_module(new_mod)
            
    def add_class(self, cls):
        if cls not in self.pm_class:
            self.pm_class.append(cls)
            self.named_class[cls.__name__] = cls
                                
            self._installed_pm_hooks(cls)
                    
    def _installed_pm_hooks(self, cls):
        cls._pm_fill_object_ = _fill_object_
        cls._pm_state_ = 'New'
        cls._pm_where_ = None
        self._parse_comments(cls, cls.__doc__)
        cls.__setattr__ = _set_object_attr_
        
    def _parse_comments(self, cls, docs):
        comments_header = {'@table:':self._commnet_table_,
                           '@id:':self._commnet_id_,
                           '@field:':self._commnet_field_,
                           '@where:':self._commnet_where_,
                           '@relation:':self._commnet_relation_,
                           '@extend:':self._commnet_extend_
                           }
        for doc in docs.split("\n"):
            doc = doc.strip()
            for comment in comments_header.keys():
                if doc.startswith(comment):
                     doc = doc.replace(comment, '').strip()
                     comments_header[comment](cls, doc)
                     break
            
    def _commnet_table_(self, cls, str):
        cls._pm_db_table_ = str

    def _commnet_id_(self, cls, str):
        (f_name, f_len, f_type) = self._parse_filed_commnet(str)
        cls._pm_id_column_ = f_name
        self._commnet_field_(cls, str)
        
    def _commnet_field_(self, cls, str):
        try:
            _pm_fields_ = cls._pm_fields_
        except AttributeError:
            cls._pm_fields_ = _pm_fields_ = []
        
        f = self._parse_filed_commnet(str)
        setattr(cls, f[0], None)
        for fd in _pm_fields_:
            if fd[0] == f[0]: return
            
        _pm_fields_.append(f)
        
    def _commnet_where_(self, cls, str):
        cls._pm_where_ = str
        
    def _commnet_relation_(self, cls, str):
        
        try:
            _relations_ = cls._relations_
        except AttributeError:
            cls._relations_ = _relations_ = {}
        
        if str.count(',') == 3:
            name, map_type, model_type, col = str.split(',')
        _relations_[name] = (name, map_type, model_type, col)
        if map_type == 'one':
            self._commnet_field_(cls, '%s,0,INTEGER' % col)
        
        setattr(cls, name, None)
    
    def _commnet_extend_(self, cls, str):
        self._parse_comments(cls, self.named_class[str].__doc__)
    
    def _parse_filed_commnet(self, str):
        name, length, type = str.split(",")
        length = int(length)
        
        return (name, length, type)
    
    def export_sql_schema(self, cls=None):
        if cls != None:
            schema = "CREATE TABLE %s (\n" % cls._pm_db_table_
            primary_field = cls._pm_id_column_
            def _generate_field_(f):
                f_type = (int(f[1]) > 0) and "%s(%s)" % (f[2], f[1]) or f[2]
                return ((primary_field == f[0]) and "%s %s PRIMARY KEY" or "%s %s") \
                        % (f[0], f_type)
            schema += ",\n".join([ _generate_field_(e) for e in cls._pm_fields_])
            schema += "\n)"
            return schema
        return ""
    
    def export_drop_table(self, cls=None):
        if cls != None:
            return "DROP TABLE %s" % cls._pm_db_table_
        return ""      
#---start persistent object hooks--------------

def _fill_object_(self, data):
    self._pm_updated_field_ = set()
    for f in self._pm_fields_:
        setattr(self, f[0], data[f[0]])
    self._pm_state_ = 'loaded'
        
def _set_object_attr_(self, name, value):
    if self._pm_state_ != 'New':
        for f in self._pm_fields_:
            if name == f[0]:
                if value != getattr(self, name):
                    self._pm_updated_field_.add(name)
                    self._pm_state_ = 'updated'
                break
    super(self.__class__, self).__setattr__(name, value)
    #super.__setattr__(self, name, value)
    #object.__setattr__(self, name, value)
    
        
#---end persistent object hooks----------------

def cur_pm(name='default'):
    """
    Get current thread's PM, the name can distinguish multiple PM. 
    """
    pm_name = "_db_" + name
    pm = None
    from portal.config import db_config as db_param
    try:
        pm = getattr(web.ctx, pm_name)
    except AttributeError:
        pass
    finally:
        if pm == None:
            pm = PersistentManager(name, **db_param[name])
            setattr(web.ctx, pm_name, pm)
            logging.getLogger('cur_pm').info('create new persistent manager.')
        
    
    return pm
