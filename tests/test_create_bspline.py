import sys
sys.path.insert(0, '../swig')
sys.path.insert(0, '/home/erik/Documents/casadi3.0')
import os

# Boolean flag to indicate if we run in valgrind
# To speed up valgrind, you may choose to put
# expensive computations within 'if not valgrind'
valgrind = int(os.environ.get("VALGRIND",'0'))

import Basis

print 'MonomialeBasis'

b = Basis.MonomialeBasis(4)
print b.evalBasis(0.5)

print 'BSplineBasis'

b = Basis.BSplineBasis([0.0, 1.0], 3, 5)
print b.evalBasis(0.5)

print 'Bernstein'

b = Basis.BSplineBasis([0.0, 1.0], 1, 2)

print 'b.evalBasis(0.1)'
print b.getKnots()
print b.evalBasis(0.1)
