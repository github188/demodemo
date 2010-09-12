#!/usr/bin/env python

"""Helper script to run all Robot Framework's unit tests.

usage: run_utest.py [options]

options: 
    -q, --quiet     Minimal output
    -v, --verbose   Verbose output
    -d, --doc       Show test's doc string instead of name and class
                    (implies verbosity)
    -h, --help      Show help
"""

import unittest
import os
import sys
import re
import getopt


base = os.path.abspath(os.path.normpath(os.path.split(sys.argv[0])[0]))
for path in [ "../src", ]:
    path = os.path.join(base, path.replace('/', os.sep))
    if path not in sys.path:
        sys.path.insert(0, path)
        
testfile = re.compile("^test_.*\.py$", re.IGNORECASE)          


def get_tests(directory=None):
    if directory is None:
        directory = base
    sys.path.append(directory)
    tests = []
    modules = []
    for name in os.listdir(directory):
        if name.startswith("."): continue
        fullname = os.path.join(directory, name)
        if os.path.isdir(fullname):
            tests.extend(get_tests(fullname))
        elif testfile.match(name):
            modname = os.path.splitext(name)[0]
            modules.append(__import__(modname))
    tests.extend([ unittest.defaultTestLoader.loadTestsFromModule(module)
                   for module in modules ])
    return tests 

def get_tests_from_file(name):
    base_dir = os.path.dirname(name)
    sys.path.append(os.path.abspath(base_dir))
    modname = os.path.splitext(os.path.basename(name))[0]
    return [ unittest.defaultTestLoader.loadTestsFromModule(__import__(modname)) ]


def parse_args(argv):
    docs = 0
    verbosity = 1
    test_file = None
    try:
        options, args = getopt.getopt(argv, 'hH?vqd', 
                                      ['help','verbose','quiet','doc'])
        if len(args) != 0:
            test_file = args[0]

    except getopt.error, err:
        usage_exit(err)
    for opt, value in options:
        if opt in ('-h','-H','-?','--help'):
            usage_exit()
        if opt in ('-q','--quit'):
            verbosity = 0
        if opt in ('-v', '--verbose'):
            verbosity = 2
        if opt in ('-d', '--doc'):
            docs = 1
            verbosity = 2
    return docs, verbosity, test_file


def usage_exit(msg=None):
    print __doc__
    if msg is None:
        rc = 251
    else:
        print '\nError:', msg
        rc = 252
    sys.exit(rc)


if __name__ == '__main__':
    docs, vrbst, test_file = parse_args(sys.argv[1:])
    if(test_file):
        tests = get_tests_from_file(test_file)
    else:
        tests = get_tests(None)
        
    suite = unittest.TestSuite(tests)
    runner = unittest.TextTestRunner(descriptions=docs, verbosity=vrbst)
    result = runner.run(suite)
    rc = len(result.failures) + len(result.errors)
    if rc > 250: rc = 250
    sys.exit(rc)
