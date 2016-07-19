#!/usr/bin/env python
import sys
sys.path.insert(0, '../swig')
sys.path.insert(0, '/home/erik/Documents/casadi3.0')
import os
import unittest

# Boolean flag to indicate if we run in valgrind
# To speed up valgrind, you may choose to put
# expensive computations within 'if not valgrind'
valgrind = int(os.environ.get("VALGRIND",'0'))

import Basis
from casadi import *

f = Basis.Polynomial([1,0,2])
# print(f)
# print(f.basis)
# print(f.coef)
# print(f.coef.data)
# b = f.getBasis()
b = f.basis

# print(b)
# print(list(b.getSubBasis()))
# print(b)
r = b([2])
# print(r)
# print(r.data())
r = f([2])
# print(r)
assert(r.data()==9)

x = Basis.Argument('x')
g = Basis.Polynomial([0,2],x)
h = Basis.Polynomial([1,0,1],'y')
print(x)
print(g)
print(h)
# print(g.getArgument())
print(h([0]).data())
print(h([1]).data())
print(h([2]).data())
print(h.basis)
print(h.basis.getArgument())
print(g.basis.getArgument())
s = g + h
print(g([1]).data())
print(h([2]).data())
print(s)
print('--------------')
b = s.basis
print(b)
print(list(b.getSubBasis()))
b_ = g.basis + h.basis
print(b_)
print(list(b_.getSubBasis()))
print(b([1,2]).data())

# print(s([1,2]).data())
