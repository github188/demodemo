
def main(path):
    print "working:%s" % path

if __name__ == '__main__':
    import sys
    if len(sys.argv) != 2:
        print 'python -m spider <path>'
    else:    
        main(sys.argv[1])