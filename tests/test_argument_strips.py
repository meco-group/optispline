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

#     def test_upper(self):
#         self.assertEqual('foo'.upper(), 'FOO')
#
# if __name__ == '__main__':
#     suite = unittest.TestLoader().loadTestsFromTestCase(TestStringMethods)
#     unittest.TextTestRunner(verbosity=2).run(suite)
#

B = Basis.Basis()
M = Basis.Basis()

BS1 = Basis.BSplineBasis()
BS2 = Basis.BSplineBasis([0.0 ,1.0 ],3,4)

MO1 = Basis.MonomialeBasis()
MO2 = Basis.MonomialeBasis()

BS3 = BS1 + BS2

assert B.getRepresentation()== 'Basis'
assert BS3.getRepresentation()== 'BSplineBasis object'

a = Basis.Argument()
b = Basis.Argument("x")
print 'a is ',
print a
print 'b is ',
print b
print 'a == a', str(a=="_")
print 'b == a', str(b==a)
print "b == 'x'", str(b=='x')
a = Basis.Argument('erik')
print 'a is ',
print a
a.setName('Erik')
print 'a changed to ' + str( a )
print  B.getArgument()
B.setArgument('x')
print  B.getArgument()
B.setArgument(a)
print  B.getArgument()
a.setName('z')
print  B.getArgument()

print 'B subBasis',
print list(B.getSubBasis())


print 'BS3 subBasis',
print list(BS3.getSubBasis())

B.addBasis(BS1)
B.addBasis(BS2)
print 'B subBasis',
print list(B.getSubBasis())

print BS1([1])
print B([1,2])

x = SX.sym("x")

print [x]
print BS1( [x] )

BS1.setDegree(3)
print BS1.getDegree()

MO1.setDegree(2)
MO2.setDegree(3)

print MO1
print MO1.getSubBasis()
print MO1([0.5]).data()

M.addBasis(MO1)
M.addBasis(MO2)
print M([2,3]).data()
