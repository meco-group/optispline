#!/usr/bin/env python
import sys
sys.path.insert(0, '../swig')
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

b =  Basis.MonomialeBasis(2)
c =  Basis.MonomialeBasis(1)
c2 =  Basis.MonomialeBasis(3,x)

d = c + b
print(d)
print(list(d.getSubBasis()))
d2 = c2 + b
print(d2)
print(list(d2.getSubBasis()))

d = DM([[1,0,2]])
t = Basis.DTensor(d,[3,1,1])
coef = Basis.Coefficient(t)


g = Basis.Polynomial([0,2])
g2 = Basis.Polynomial([0,2],x)
h = Basis.Polynomial([1,0,1],'y')

h = Basis.Function(b, coef)
#print(x)
#print(g)
#print(h)
# print(g.getArgument())
#print(h([0]).data())
#print(h([1]).data())
#print(h([2]).data())
#print(h.basis)
print(h.getBasis())
print(g.getBasis())
print(h.basis.getArgument())
print(g.basis.getArgument())
#print('--------------')
b_ = g.getBasis() + h.getBasis()
print(b_)
print(list(b_.getSubBasis()))
print('--------------')
s = g + h
print(s)
print(list(s.getBasis().getSubBasis()))
print(g([1]).data())
print(h([2]).data())
print(s)
b = s.getBasis()
print(b)
print(list(b.getSubBasis()))
#print(b([1,2]).data())

# print(s([1,2]).data())
