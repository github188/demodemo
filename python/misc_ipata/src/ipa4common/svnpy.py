import logging, tempfile, subprocess, os, re
import time
import types
import xml.parsers.expat

"""
The svnpy is svn command line wrapper in python. you can use Python function
to call svn command line. the interface is similar with pysvn. 

@_@, Wow~~~ what's the pysvn too? see http://pysvn.tigris.org/
"""


def svn_shell_version():
    """find local tortoiseSVN version. return None if not found.
    
    return string (e.g. 1.5.5.14361)
    """
    import _winreg as win
    try:
        reg = win.ConnectRegistry(None, win.HKEY_CURRENT_USER)
        reg_key = win.OpenKey(reg, r"SOFTWARE\TortoiseSVN") 
        (key_value, t) = win.QueryValueEx(reg_key, "CurrentVersion")
        win.CloseKey(reg_key)
        win.CloseKey(reg)
        return ".".join(key_value.split(", "))
    except:
        return None

class CliClient():
    
    def __init__(self, svn_cli='svn', tmp_dir=None, work_path=""):
        self.logger = logging.getLogger("svn.py")
        
        self.svn_cli = svn_cli
        self.tmp_dir = tmp_dir or tempfile.gettempdir()
        from platform import system
        self.subprocess_shell = system() == 'Windows'
        self.escape = system() == 'Windows' and "^" or ""
        
        version = self.__version()
        if version is None:
            raise RuntimeError, "'%s' is not exists in system or not executable." % self.svn_cli
        self.logger.info("subversion version '%s'" % version)
        self.work_path = work_path
        self.svn_version = version
    
    def add(self, path):
        """run cli: svn add [path]
        
        return None
        
        Exception, if the path ready under version control
        
        """
        
        output = self.__execute_svn_cli(["add", ] + self.__covert_to_list(path))
        if "under version control" in output:
            raise RuntimeError, output
        return None
    
    def revert(self, path):
        """run cli: svn revert [path]
        
        return None
        """
        
        self.__execute_svn_cli(["revert", ] + self.__covert_to_list(path))
        return None
    
    def resolved(self, path):
        """run cli: svn resolved [path]
        
        return None
        """
        
        self.__execute_svn_cli(["resolved", path])
        return None
        
    def cleanup(self, path):
        """run cli: svn cleanup [path]
        
        return None
        """        
        self.__execute_svn_cli(["cleanup", path])
        return None
    
    def checkin(self, path, log_message):
        """run cli: svn ci -m [log_message] [path]
        
        return last version
        """        
        if len(log_message) < 10: raise RuntimeError, "Log messages less than 10 characters not allowed"
              
        log_message = log_message.replace("\n", "\\n")
        message = self.__execute_svn_cli(["ci", "-m", log_message] + self.__covert_to_list(path))
        result = r"(Committed revision)\s+(\d+)"
        
        version = re.search(result, message)
        if version: return version.group(2)
        if message: raise RuntimeError, message
    
    
    def checkout(self, url, path,):
        """run cli: svn co [url] [path]
        
        return None
        """
        
        if not os.path.exists(path): os.makedirs(path)
                
        self.__execute_svn_cli(["checkout", url, path])
        return None
    
    def diff(self, temp_path, url_or_path):
        """run cli: svn di [url_or_path]
        
        return command output
        """        
        return self.__execute_svn_cli(["diff", url_or_path])

    def diff_summarize(self, url_or_path='', old_revision='PREV', new_revision='HEAD'):
        """run cli: svn di --summarize [url_or_path] -r <old_revision>:<new_revision>
        
        return list of dict.
        dict keys:
        'path': --- status item 
        'status': --- item status ('unversioned', 'modified', 'added', 'deleted', 'conflicted')
        """
        if "1.4" in self.svn_version or '1.5' in self.svn_version:
            url_or_path = url_or_path or self.work_path
            xml_output = self.__execute_svn_cli(["di", "--summarize", 
                                                 "-r","%s:%s" % (old_revision, new_revision),
                                                 url_or_path])
            result = []
            for line in xml_output.splitlines():
                status, file = re.split(r"\s+", line.strip(), 1)
                kind = os.path.isfile(file) and 'file' or 'dir'
                status = {"M":"modified", "A":"added", 
                          "D":"deleted", "C":"conflicted"}.get(status, "modified")
                result.append({'status': status, 'path':file, 'kind':kind})            
        else:
            url_or_path = url_or_path or self.work_path
            xml_output = self.__execute_svn_cli(["di", "--summarize", "--xml", 
                                                 "-r","%s:%s" % (old_revision, new_revision),
                                                 url_or_path])
            
            result = []
            class XMLParser():
                def __init__(self):self.cur_kw = None
                def path_start(self, **kw): 
                    result.append(kw)
                    self.cur_kw = kw
                def path_end(self,): self.cur_kw = None
                def content(self, path):
                    if self.cur_kw: self.cur_kw['path'] = path
                    
            self.__parse_xml_output(xml_output, XMLParser())
            for e in result: 
                e['status'] = e['item']
                del e['item']
        
        return result         
        #return self.__execute_svn_cli(["diff", url_or_path])
        
    def info(self, url_or_path=None):
        """run cli: svn info [path]
        
        return last version       
        """
        
        url_or_path = url_or_path or self.work_path
        
        url_or_path = url_or_path.replace('&', "%s&" % self.escape)
        
        xml_output = self.__execute_svn_cli(["info", "--xml", url_or_path])
        
        svn_info = []
        class XMLParser():
            def __init__(self): self.attr_item = None
            def entry_start(self, **kw): svn_info.append(kw)
            def content(self, data):
                if svn_info and self.attr_item: svn_info[0][self.attr_item] = data
            def __getattr__(self, name):
                tag = name.split("_")[0]
                if tag in ['url', 'root', 'uuid', 'author', 'date']:
                    if name.endswith("_start"):
                        def start_tag(**kw): self.attr_item = tag
                        return start_tag
                    else:
                        def end_tag(): self.attr_item = None
                        return end_tag
                raise AttributeError, "'XMLParser' object has no attribute '%s'" % name
        
        self.__parse_xml_output(xml_output, XMLParser())
        return svn_info[0]
            
    def update(self, path=None, revision=""):
        """run cli: svn up [path] -r [revision]
        
        return last version
        """
        path = path or self.work_path
        
        result = r"(At revision|Updated to revision)\s+(\d+)"
        
        revision = str(revision)
        command = ["up", ] + self.__covert_to_list(path)
        if revision.isdigit(): command.extend(["-r", revision])
        
        message = self.__execute_svn_cli(command)
        version = re.search(result, message)
        if version: return version.group(2)
        
        return None 
    
    def remove(self, path):
        """run cli: svn remove [path]
        
        return None
        """        
        
        self.__execute_svn_cli(["remove", ] + self.__covert_to_list(path))
        return None
    
    def status(self, path):
        """run cli: svn st [path]
        
        return list of dict.
        dict keys:
        'path': --- status item 
        'status': --- item status ('unversioned', 'modified', 'added', 'deleted', 'conflicted')
        """
        path = path or self.work_path
        
        xml_output = self.__execute_svn_cli(["st", "--xml"] + self.__covert_to_list(path))
        
        result = []
        class XMLParser():
            def entry_start(self, **kw): result.append(kw)
            def wc_status_start(self, **kw):result[-1].update(kw)
        
        self.__parse_xml_output(xml_output, XMLParser())
        for e in result: 
            e['status'] = e['item']
            del e['item']
        
        return result
    
    def __covert_to_list(self, obj):
        result = None
        if isinstance(obj, types.ListType):
            result = obj
        elif isinstance(obj, types.TupleType):
            result = list(obj)
        else:
            result = [obj, ]
        
        return [ e.replace("&", "%s&" % self.escape) for e in result ]
    
    def __version(self):
        try:
            return self.__execute_svn_cli(["--version", "--quiet"]).strip()
        except:
            return None
    
    def __execute_svn_cli(self, command):
        """execute command line in subprocess!"""
        error_code = 255
        
        svn_output = os.path.join(self.tmp_dir, "%s.svn" % time.time()) 
        
        cli_output = open(svn_output, "w")
        try:
            cli_command = [self.svn_cli, ] + command
            self.logger.debug("%s %s" % (self.svn_cli, " ".join(command)))
            error_code = subprocess.call(cli_command, shell=self.subprocess_shell, 
                                         stdout=cli_output, stderr=cli_output)
            
            cli_output.close()
            f = open(svn_output, 'r'); output = f.read(); f.close()
            self.logger.debug(output)
            if error_code != 0:
                output = output.strip()
                raise RuntimeError, "call subversion cli error, '%s', code=%s" % (output, error_code)
            
            return output
        finally:
            if os.path.exists(svn_output):
                os.remove(svn_output)
                
    def __parse_xml_output(self, xml_output, callback):
        def find_handler(name):
            name = name.replace("-", "_")
            h = hasattr(callback, name) and getattr(callback, name) or None
            return callable(h) and h or None
        
        def start_element(name, attrs):
            h = find_handler("%s_start" % name)
            if h is None: return
            h(**dict([ (str(k), v) for k, v in attrs.iteritems() ]))
        
        def end_element(name):
            h = find_handler("%s_end" % name)
            if h is None: return
            h()
        
        def content(data):
            h = find_handler("content")
            if h is None: return
            import xml.sax.saxutils as a            
            h(a.unescape(data))
        
        parser = xml.parsers.expat.ParserCreate()
        parser.StartElementHandler = start_element
        parser.EndElementHandler = end_element
        parser.CharacterDataHandler = content
        parser.Parse(xml_output, True)

class PYSVNClient(object):
    def __init__(self):
        self.svn = svnClient()
        
    def add(self, path):
        self.svn.add(self.__fix_file_name(path))

    def remove(self, path):
        self.svn.remove(self.__fix_file_name(path))

    def revert(self, path):
        self.svn.revert(self.__fix_file_name(path))
        
    def resolved(self, path):
        self.svn.resolved(path)
        
    def status(self, path):
        file_name = None
        path = self.__fix_file_name(path)
        
        new_result = []
        for p in path:
            result = self.svn.status(p, get_all=False)
            for e in result:
                status = str(e['text_status'])
                new_result.append({"path": e['path'],
                                   "status": status.split(".")[-1]
                                   })
        return new_result
    
    def update(self, path, revision=""):
        path = self.__fix_file_name(path)
        if revision:
            import pysvn
            revision = pysvn.Revision(pysvn.opt_revision_kind.number, 
                                      number=int(revision))
            result = self.svn.update(path, revision=revision)
        else:
            result = self.svn.update(path)

        return result[0].number
    
    def checkin(self, path, log_message):
        if len(log_message) < 10: raise RuntimeError, "Log messages less than 10 characters not allowed"
        
        path = self.__fix_file_name(path)
        result = self.svn.checkin(path, log_message)
        if result: return result.number
        return None
    
    def __fix_file_name(self, path):
        """the pysvn is case sensitive in file name"""
        def local_file(p):
            if os.path.isfile(p):
                p, file_name = os.path.split(p)
                file_name = file_name.lower()      
                for f in os.listdir(p):
                    if f.lower() == file_name.lower(): 
                        file_name = f
                        break
                return os.path.join(p, file_name)
        
        if not (isinstance(path, types.ListType) or \
                isinstance(path, types.TupleType)):
            path = [path, ]
            
        return [ local_file(e) for e in path if os.path.exists(e) ]
    
    def __getattr__(self, name):
        func = getattr(self.svn, name)
        setattr(self, name, func)
        return func

try:
    from pysvn import Client as svnClient
    TeamClient = PYSVNClient
except:
    TeamClient = CliClient


if "__main__" == __name__:
    #print "xxxxxxxxxxxxxxxxxxx"
    resp = "https://svne1.access.nokiasiemensnetworks.com/isource/svnroot/IPA_TestAutomation/trunk/Tools/ipataportal/docs"
    resp = r"/opt/extra_tools/testcase_for_rbtgrep/TestCase/FDT/TT/TRM/"
    #local_path = r"C:\USERS\_work\workspace_pydev\ipataportal\docs2\ss\dd"
    svn = CliClient("svn")
    print svn.diff_summarize(resp, 38256)
    
    
