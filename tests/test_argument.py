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

v = Basis.BasisPtrVector()
v.append(a)
v.append(b)
c = Basis.TensorBasis(v)

print c.getArguments()
print c.getDimensions()

print a(1.0)
print b(8.0)

print b([2.0,  6.8])

print c([1.0,8.0])


