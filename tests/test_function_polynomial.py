#!/usr/bin/env python
import sys
sys.path.insert( 0, '/home/erik/Documents/cpp_splines/swig')
sys.path.insert(0, '/home/erik/Documents/casadi3.0')
import os

# Boolean flag to indicate if we run in valgrind
# To speed up valgrind, you may choose to put
# expensive computations within 'if not valgrind'
valgrind = int(os.environ.get("VALGRIND",'0'))

import Basis
from casadi import *

basis_x =  Basis.MonomialeBasis(2)
basis_x.setArgument('x')
basis_y =  Basis.MonomialeBasis(1)
basis_y.setArgument('y')

basis_xy = basis_x + basis_y
# print(basis_xy)
# print(list(basis_xy.getSubBasis())) 
# print(basis_x([0]).data())    
# print(basis_y([2]).data())    
# print(basis_xy([0,2]).data())    
d = DM([[0,0,0],[0,0,1]])
# print(d)
t = Basis.DTensor(d,[3,2,1,1])
# print(t)
c = Basis.Coefficient(t)
# print(c)
x2y = Basis.Function(basis_xy,c)

print(x2y([1,2]).data())
print(x2y([2,1]).data())

y2 =  Basis.Polynomial([0,0,1],'y')

print(y2([2]).data())
print(y2([3]).data())

x = Basis.Polynomial([0,1],'x')
y = Basis.Polynomial([0,1],'y')
x2y_ = x*x*y
print(x2y_([1,2]).data())
print(x2y_([2,1]).data())
