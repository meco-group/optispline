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

# x = Basis.Argument('x')

b =  Basis.MonomialBasis(2)
c =  Basis.MonomialBasis(1)
c.setArgument('x')
# d = c + b
# print(d)
# print(list(d.getSubBasis()))

d = DM([[1,0,2]])
t = Basis.DTensor(d,[3,1,1])
coef1 = Basis.Coefficient(t)

d = DM([[1,2]])
t = Basis.DTensor(d,[2,1,1])
coef2 = Basis.Coefficient(t)


f = Basis.Function(b, coef1)
g = Basis.Function(c, coef2)

# print(f)
# print(g)
# print(b)
# print(c)
# print(f.getBasis())
# print(g.getBasis())
# print('f ' + str(list(f.getBasis().getSubBasis())))
# print('g ' + str(list(g.getBasis().getSubBasis())))
# print('--------------')
b_ = b+c
print(b_)
print(list(b_.getSubBasis()))
b_ = b+g.basis
print(b_)
print(list(b_.getSubBasis()))
b_ = g.getBasis() + f.getBasis()
print(b_)
print(list(b_.getSubBasis()))
print('--------------')
print('b : ' + str(b))
print('  : ' + str(f.getBasis()))
print('c : ' + str(c))
print('  : ' + str(g.getBasis()))
print(b == b)
print(b == f.getBasis())
print(list(b.getSubBasis()))
print(list(f.getBasis().getSubBasis()))
# print('b : ' + str(b.getDegree()))
# print('  : ' + str(f.getBasis().getDegree()))
# s = g + f
# print(s)
# print(list(s.getBasis().getSubBasis()))
# print(g([1]).data())
# print(h([2]).data())
# print(s)
# b = s.getBasis()
# print(b)
# print(list(b.getSubBasis()))
# #print(b([1,2]).data())
#
# # print(s([1,2]).data())
