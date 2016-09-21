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


d = DM([[1,2],[3,4]])
print(d)
t = Basis.DTensor(DM([[1,2],[3,4]]),[2,2])
print(t)
c = Basis.Coefficient(t)

print(c)
