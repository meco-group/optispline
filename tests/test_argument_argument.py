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

v = [2,0,1]

d = DM(v)
t = Basis.DTensor(d,[d.size1(),1,1])

print(v)
print (d.size1())

a = Basis.Coefficient(t)
print (a)
print (a.data)
print (a.data.data())


b = Basis.Coefficient(v)
print(b)
print (b.data)
print (b.data.data())
