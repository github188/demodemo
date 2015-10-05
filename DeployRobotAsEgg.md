# Introduction #
  * install setuptools
  * update setup.py, add from setuptools import setup such that it replaces the existing import of the setup function.
  * update robot/library/init.py to [1](1.md)
  * python setup.py bdist-egg 打包文件

robot/library/init.py
```

import os

STDLIB_NAMES = [ 'BuiltIn', 'Collections', 'DeprecatedBuiltIn', 'DeprecatedOperatingSystem', 'OperatingSystem',
				 'Reserved', 'Screenshot', 'Telnet']
```

http://peak.telecommunity.com/DevCenter/PythonEggs#accessing-package-resources