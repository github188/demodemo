from django.db import models
from db.full_index_field import FullIndexTextField

class IndexFile(models.Model):
    branch = models.CharField(max_length=10, default="trunk")
    uuid = models.CharField(max_length=32, unique=True)
    path = models.CharField(max_length=1024)
    file_type = models.CharField(max_length=1, 
                                 choices=(('D', "Directory"),
                                          ('F', "File"),
                                          ))
    status = models.CharField(max_length=1, 
                                  choices=(('D', "Removed"),
                                           ('U', "Updated"), 
                                           #('R', "Resource")
                                           ))
    parent = models.CharField(max_length=32, ) #parent uuid.
    svn_version = models.CharField(max_length=8)
    svn_author = models.CharField(max_length=32)
    svn_lastupdated = models.CharField(max_length=32)
    lastupdated = models.DateTimeField('last update', auto_now=True)


class RobotIndexItem(models.Model):
    branch = models.CharField(max_length=10, default="trunk")
    uuid = models.CharField(max_length=32, unique=True)
    file = models.ForeignKey(IndexFile)
    item_type = models.CharField(max_length=1, 
                                  choices=(('S', "Suite"),
                                           ('T', "TestCase"), 
                                           #('R', "Resource")
                                           ))
    status = models.CharField(max_length=1, 
                                  choices=(('D', "Removed"),
                                           ('U', "Updated"), 
                                           #('R', "Resource")
                                           ))
    execution_type = models.CharField(max_length=1, 
                                        choices=(('A', "Auto"),
                                           ('M', "Manual"), 
                                           ))
    
    parent = models.CharField(max_length=32, ) #parent uuid.
    test_case_code = models.CharField(max_length=32) #used exchange with reporting. 
    name = models.CharField(max_length=1024)
    #force_tags = models.TextField()
    #search_force_tags = FullIndexTextField(copy_from="force_tags")
    tags = models.TextField()
    search_tags = FullIndexTextField(copy_from="tags")
    
    lastupdated = models.DateTimeField('last update', auto_now=True)
    
    @property
    def file_path(self): return self.file.path
    