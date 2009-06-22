import sys

from pytel.interpreter import PyTEL
    
def run(filename):
    tel = PyTEL()
    tel.load(filename)
    
    tel.run()
    print "done!"

if __name__ == "__main__":
    if len(sys.argv) > 1:
        filename  = sys.argv[1]
        run(filename)
    else:
        print "pyhit memmgr.c"
        sys.exit()
        

