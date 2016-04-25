import sys
sys.path.insert(0, '../swig')
sys.path.insert(0, '/home/erik/Documents/casadi3.0')
import os

# Boolean flag to indicate if we run in valgrind
# To speed up valgrind, you may choose to put
# expensive computations within 'if not valgrind'
valgrind = int(os.environ.get("VALGRIND",'0'))

import Basis


print 'BSplineBasis'

a = Basis.BSplineBasis([0.0, 10.0],2, 5)
b = Basis.BSplineBasis([0.0, 10.0],2, 5, 'x')

print a.getArguments()
print b.getArguments()