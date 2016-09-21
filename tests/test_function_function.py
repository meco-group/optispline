#!/usr/bin/env python
import sys
sys.path.insert( 0, '/home/erik/Documents/cpp_splines/swig')
sys.path.insert(0, '/home/erik/Documents/casadi3.0')
import os
import unittest

# Boolean flag to indicate if we run in valgrind
# To speed up valgrind, you may choose to put
# expensive computations within 'if not valgrind'
valgrind = int(os.environ.get("VALGRIND",'0'))

import Basis
from casadi import *

b1 = Basis.MonomialeBasis(1)
b2 = Basis.MonomialeBasis(1)

## Bad synitax
b = Basis.Basis()
b.addBasis(b1)
b.addBasis(b2)

d = DM([[1,2],[3,4]])
print(d)
t = Basis.DTensor(DM([[1,2],[3,4]]),[2,2,1,1])
print(t)
c = Basis.Coefficient(t)
print(c)
f = Basis.Function(b, c)
print(f)
# print(f.basis)
print(f.coef)
print(f.coef.data)
r = b([0.5,2])
print(r.data())
r = f([0.5,2])
print(r)
print(r.data())
