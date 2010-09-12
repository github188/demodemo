from setuptools import setup
from distutils.command.install_data import install_data
from distutils.command.install import INSTALL_SCHEMES
import os
import sys

def fullsplit(path, result=None):
    """
    Split a pathname into components (the opposite of os.path.join) in a
    platform-neutral way.
    """
    if result is None:
        result = []
    head, tail = os.path.split(path)
    if head == '':
        return [tail] + result
    if head == path:
        return result
    return fullsplit(head, [tail] + result)

# Tell distutils to put the data_files in platform-specific installation
# locations. See here for an explanation:
# http://groups.google.com/group/comp.lang.python/browse_thread/thread/35ec7b2fed36eaec/2105ee4d9e8042cb
for scheme in INSTALL_SCHEMES.values():
    scheme['data'] = scheme['purelib']

# Compile the list of packages available, because distutils doesn't have
# an easy way to do this.
packages, data_files = [], []
root_dir = os.path.dirname(__file__)
cur_dir = os.path.abspath(os.getcwd())
#if root_dir != '':
os.chdir("src")

for dirpath, dirnames, filenames in os.walk("."):
    if ".egg" in dirpath: continue
    if 'ipa4rpc' not in dirpath: continue
    # Ignore dirnames that start with '.'
    for i, dirname in enumerate(dirnames):
        if dirname.startswith('.'): del dirnames[i]
    if '__init__.py' in filenames:
        packages.append('.'.join(fullsplit(dirpath)))
    elif filenames:
        data_files.append([dirpath, [os.path.join("src", dirpath, f) for f in filenames]])

os.chdir(cur_dir)
# Small hack for working with bdist_wininst.
# See http://mail.python.org/pipermail/distutils-sig/2004-August/004134.html
if len(sys.argv) > 1 and sys.argv[1] == 'bdist_wininst':
    for file_info in data_files:
        file_info[0] = '\\PURELIB\\%s' % file_info[0]

# Dynamically calculate the version based on django.VERSION.
version = "1 0 0"

setup(
    name = "ipa4rpc",
    version = version.replace(' ', '-'),
    url = 'https://svne1.access.nokiasiemensnetworks.com/isource/svnroot/IPA_TestAutomation/trunk/Tools/ipataport/misc_ipata',
    download_url="xx",
    author = 'IpaTA Team',
    author_email = 'coo-ra-mr-ipatasigmateamdg@mlist.emea.nsn-intra.net',
    description = 'A PRC solution based on HTTP.',
    packages = packages,
    #cmdclass = cmdclasses,
    data_files = data_files,
    package_dir={'': 'src'},
)
