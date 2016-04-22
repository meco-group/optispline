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

b = Basis.BSplineBasis([0.0, 10.0],2, 5)
print b.evalBasis(0.5)
print b.evalBasis([0.5,0.6])
print b.length()
print b.getKnots()
print b.greville()

print 'Bernstein'

c = Basis.BSplineBasis([0.0, 1.0], 1, 2)
print c

print 'c.evalBasis(0.1)'
print c.getKnots()
print c.evalBasis(0.1)

print '\nb.transformation(c)'
print b.length()
print c.length()
print b.transformation(c)
print '\n\n'
print Basis.addKnotsUnique([0.,0.1,0.2], [0.,0.3,0.2])
print '\n\n'
print Basis.addKnotsNotUnique([0.,0.1,0.2], [0.,0.3,0.2])


print '\n\n'
d = b.addKnots([1])
print b.getKnots()
print d.getKnots()
T =  d.transformation(b)
print T

import numpy as np
T = np.matrix(T)
T[abs(T.real) < 10**-3] = 0.0

print T
