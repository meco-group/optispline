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

b = Basis.MonomialeBasis(2)

## Bad synitax
d = DM([[1,0,2]])
print(d)
t = Basis.DTensor(d,[3,1,1])
print(t)
c = Basis.Coefficient(t)
print(c)
f = Basis.Function(b, c)
print(f)
# print(f.basis)
print(f.coef)
print(f.coef.data)
r = b([2])
print(r.data())
r = f([2])
print(r)
print(r.data())
