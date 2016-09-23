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

m1 = Basis.MonomialBasis(2)
m2 = Basis.MonomialBasis(3)
m3 = Basis.MonomialBasis(4)
b1 = Basis.BSplineBasis([0.0 ,1.0 ],5,3)
b2 = Basis.BSplineBasis([0.0 ,1.0 ],3,3)
b3 = Basis.BSplineBasis([0.0 ,1.0 ],4,3)

x = Basis.Argument("x")
y = Basis.Argument("y")

m1.setArgument(x)
m2.setArgument(x)
m3.setArgument(y)
b1.setArgument(x)
b2.setArgument(x)
b3.setArgument(y)

print 'm1 : ', m1
print '     ', m1.getArgument()
print 'm2 : ', m2
print '     ', m2.getArgument()
print 'm3 : ', m3
print '     ', m3.getArgument()
print 'b1 : ', b1
print '     ', b1.getArgument()
print 'b2 : ', b2
print '     ', b2.getArgument()
print 'b3 : ', b3
print '     ', b3.getArgument()
print ''

s1 =   m1 + m2
print 'm1 + m2 : ', s1
print '          ', s1.getArgument()
s2 =   m1 + m3
print 'm1 + m3 : ', s2
print '          ', s2.getArgument()
s3 =   m1 + b1
print 'm1 + b1 : ', s3
print '          ', s3.getArgument()
s3_ =  b1 + m1
print 'b1 + m1 : ', s3_
print '          ', s3_.getArgument()
s4 =   m1 + b3
print 'm1 + b3 : ', s4
print '          ', s4.getArgument()
s5 =   b1 + b2
print 'b1 + b2 : ', s5
print '          ', s5.getArgument()
print ''
print '(m1 + m2)(x)   : ', s1([ 0.1 ]).data()
print '(m1 + m3)(x,y)   : ', s2([ 0.1,0.2 ]).data()
print '(m1 + b1)(x) : ', s3([ 0.1 ]).data()
print '(b1 + m1)(x)   : ', s3_([ 0.1 ]).data()
print '(m1 + b3)(x,y) : ', s4([ 0.1,0.2 ]).data()
print 'b1(x)           : ', b1([ 0.1 ]).data()
print '(b1 + b2)(x)   : ', s5([ 0.1 ]).data()
