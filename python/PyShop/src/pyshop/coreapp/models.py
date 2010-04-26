from django.db import models

# Create your models here.
class Classifier(models.Model):
    name = models.CharField(max_length=50, unique=True)

    def __unicode__(self):
        return self.name
    
    def projects(self):
        prj_list = self.project_set.filter(~models.Q(status='expired'))\
                                .order_by('name')
        return prj_list

class Project(models.Model):
    name = models.CharField(max_length=50, unique=True)
    summary = models.TextField(blank=True)
    description = models.TextField(blank=True)
    classifiers = models.ForeignKey(Classifier, null=True)
    last_version = models.CharField(max_length=50, null=True)
    dev_type = models.CharField(max_length=30, default='new',
                                choices=(('dev', 'developed by TA team'),
                                        ('ext', 'external project'),))
    
    dev_status = models.CharField(max_length=30, default='new',
                                  choices=(('1', '1 - Launched'),
                                          ('2', '2 - Developing'),
                                          ('3', '3 - Alpha'),
                                          ('4', '4 - Release candidate'),
                                          ('5', '5 - Product'),
                                          ))
    
    language = models.CharField(max_length=30, default='python' ,)
    main_entry = models.CharField(max_length=255, default='', )
    user_guide = models.CharField(max_length=500, default='', )
    status = models.CharField(max_length=10, default='new',
                              choices=(('new', 'new'),
                                      ('updated', 'updated'),
                                      ('updating', 'updating'),
                                      ('expired', 'expired'),
                                      #('error', 'error'),
                                      ))
    
    #active_release = models.OneToOneField('Release', null=True, related_name="")
    
    def __unicode__(self):
        return self.name
    
    def active_release(self):
        return self.releases.all().order_by("-release_order")[0]
    
    def online_install(self):
        if self.main_entry.startswith("py:"):
            return None
        
        return "PyShop.jnlp?name=%s&action=install" % self.name

    def online_start(self):
        if not self.main_entry or self.main_entry.startswith("http:"):
            return None
        
        return "online/%s.jnlp" % self.name

    def online_try_use(self):
        if not self.main_entry.startswith("http:"):
            return None
        
        return self.main_entry
        
class Release(models.Model):
    version = models.CharField(max_length=128)
    dist_md5sum = models.CharField(max_length=255, blank=True)
    signature = models.CharField(max_length=128, blank=True)
    project = models.ForeignKey(Project, related_name="releases")
    path = models.CharField(max_length=255)
    release_order = models.IntegerField()

    def __unicode__(self):
        return u"%s %s (%s)" % (self.project.name, self.version, self.platform)
