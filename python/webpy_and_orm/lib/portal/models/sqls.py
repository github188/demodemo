
"""this model define the common sql
"""


list_cables_by_device = """select c.* from ${Connection} c, ${Interface} i, ${Device} d 
                            where c.src_int = i.int_id and i.root_resource_id = d.device_id and
                                  d.name like $name and d.status != 'removed'
                        """
                        
list_cables_by_device_id = """select c.* from ${Connection} c, ${Interface} i, ${Interface} di  
                            where (c.src_int = i.int_id and i.root_resource_id = $device_id) or 
                             (c.des_int = di.int_id and di.root_resource_id = $device_id ) 
                        """                        
                        
list_cables_by_int_id = """select c.* from ${Connection} c 
                            where c.src_int = $int_id or c.des_int = $int_id
                        """