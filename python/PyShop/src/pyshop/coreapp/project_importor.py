from __future__ import with_statement
from contextlib import closing
from models import Project, Classifier, Release
import os, logging, codecs
from pkg_resources import parse_version

class ProjectImportor(object):
    
    def __init__(self, ):
        self.logger = logging.getLogger("import")
    
    def update_project(self, path):
        
        if not os.path.isdir(path):
            self.logger.info("Not found project %s..." % path)
            
        project = os.path.basename(path)
        
        status = 'new'
        prj, c = Project.objects.get_or_create(name=project)
        if c:
            self.logger.info("importing new project '%s'..." % project)
        else:
            self.logger.info("updating project '%s'..." % project)
            
        prj_info = self._parse_py_info(path)
        
        prj.dev_type = prj_info.get("dev_type", "ext")
        self.logger.info("dev_type '%s'..." % prj.dev_type)
        
        prj.dev_status = prj_info.get("dev_status", "1")
        self.logger.info("dev_status '%s'..." % prj.dev_status)
        
        prj.language = prj_info.get("language", "python")
        self.logger.info("language '%s'..." % prj.language)

        prj.main_entry = prj_info.get("main_entry", "")
        self.logger.info("main_entry '%s'..." % prj.main_entry)
        
        category = prj_info.get("category", "Other")
        classifier, c = Classifier.objects.get_or_create(name = category)
        prj.classifiers = classifier
        self.logger.info("category '%s'..." % prj.classifiers.name)
        
        prj.summary = self._parse_summary(path)
        self.logger.info("summary '%s'..." % prj.summary)
        
        prj.user_guide = prj_info.get("user_guide", "")
        self.logger.info("user_guide '%s'..." % prj.user_guide)
        
        self._update_release(prj, path)
        self.logger.info("last_version:%s" %  prj.last_version)
        if prj.last_version is None:
            prj.status = 'expired'
        else:
            prj.status = 'updated'
        
        prj.save()
        self.logger.info("updated project.")
        
        return prj
        
    
    def _parse_py_info(self, path):
        info_path = os.path.join(path, 'py-info')
        if not os.path.isfile(info_path):
            self.logger.warn("Not found 'py-info' file in project.")
            return {}
        
        info = {}
        with closing(open(info_path, 'r')) as records:
            for l in records:
                l = l.strip()
                if l.startswith("#") or ":" not in l: continue
                k, v = l.split(":", 1)
                info[k.strip()] = v.strip()
                       
        return info
    
    def _parse_summary(self, path):
        summary = os.path.join(path, 'project_summary.txt')
        if not os.path.isfile(summary):
            self.logger.info("Not found 'project_summary.txt' file in project.")
            return ""
        
        return codecs.open(summary, "r", "utf-8").read()
    
    def _update_release(self, prj, path):
        prj.releases.all().delete()
        
        self.logger.info("update project release....")
        release_list = []
        
        project_name = os.path.basename(path)        
        for name in os.listdir(path):
            if not name.startswith(project_name): continue
            file, ext = os.path.splitext(name)
            if ext not in ['.zip', '.gz', ]: continue
            if "-" not in file: continue
            xx, version = file.split("-", 1)
            #version = x.replace("-%s" % project_name, "")
            
            #self._update_release(prj, path)
            self.logger.info("version:%s-->%s" % (version, name))
            release_list.append(Release(version=version,
                                        path=name))
            
        from pkg_resources import parse_version as pv
        release_list.sort(lambda x, y: cmp(pv(x.version), pv(y.version)), )
        
        for i in range(len(release_list)):
            release_list[i].release_order = i
            release_list[i].project = prj
            release_list[i].save()
        
        if len(release_list) > 0:
            prj.last_version = release_list[-1].version
        else:
            prj.last_version = None 
        
    def delete_project(self, name):
        pass
    
    def update_dist(self, dist_path):
        self.logger.info("updating dist %s..." % dist_path)
        
        self._set_updating_flag()
        
        for project in os.listdir(dist_path):
            path = os.path.join(dist_path, project)
            if not os.path.isdir(path):
                self.logger.info("ignore non-directory path '%s'..." % project)
                continue
            
            self.logger.info("=" * 70)
            
            self.update_project(path)
            
        self._set_expired_flag()
            
        self.logger.info("done to update dist.")         
    
    def remove_expire_project(self, ):
        pass
    
    def _parse_last_version(self, path):
        pass
    
    def _set_updating_flag(self):
        for e in Project.objects.all():
            e.status = 'updating'
            e.save()
            
    def _set_expired_flag(self):
        for e in Project.objects.filter(status='updating'):
            e.status = 'expired'
            e.save()
