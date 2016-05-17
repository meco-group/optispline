import sys
sys.path.insert(0, '../swig')
sys.path.insert(0, '/home/erik/Documents/casadi3.0')
import os

def assertEqual(v1, v2, message = '', tol = 10**-10):
	assert abs(v1 - v2) < tol, message

# Boolean flag to indicate if we run in valgrind
# To speed up valgrind, you may choose to put
# expensive computations within 'if not valgrind'
valgrind = int(os.environ.get("VALGRIND",'0'))

import Basis

B = Basis.Basis()

BS1 = Basis.BSplineBasis()
BS2 = Basis.BSplineBasis()

MO1 = Basis.MonomialeBasis()
MO2 = Basis.MonomialeBasis()

BS3 = BS1 + BS2

print 'BS1 is a ' +  BS1.getRepresentation()
print 'BS3 is a ',
print BS3

print 'B is a ',
print B
