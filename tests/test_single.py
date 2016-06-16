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

B = Basis.BSplineBasis([0.0 ,1.0 ],3,4)
B1 = Basis.BSplineBasis([0.0 ,1.0 ],2,4)
b = B + B1
print B
print B1
print b
# print b.getKnots()
