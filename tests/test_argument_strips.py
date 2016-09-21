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

#     def test_upper(self):
#         self.assertEqual('foo'.upper(), 'FOO')
#
# if __name__ == '__main__':
#     suite = unittest.TestLoader().loadTestsFromTestCase(TestStringMethods)
#     unittest.TextTestRunner(verbosity=2).run(suite)
#

B = Basis.Basis()
print B.getRepresentation()
assert B.getRepresentation()== 'Basis'

M = Basis.Basis()

BS1 = Basis.BSplineBasis([0.0 ,1.0 ],3,4)
print list( BS1.getKnots() )
BS2 = Basis.BSplineBasis([0.0 ,1.0 ],3,4)

MO1 = Basis.MonomialeBasis(3)
MO2 = Basis.MonomialeBasis(7)

BS3 = BS1 + BS2

a = Basis.Argument()
b = Basis.Argument("x")
assert  (a=="_") == True
assert  (b==a) == False
assert  (b=='x') == True
a.setName('Erik')
assert  (a=="Erik")
assert  B.getArgument() == "_"
B.setArgument('x')
assert  B.getArgument() == "x"
B.setArgument(a)
assert  B.getArgument() == "Erik"
a.setName('z')
assert  B.getArgument() == "z"

# assert BS3.getRepresentation() == 'BSplineBasis object'

print 'B subBasis',
print list(B.getSubBasis())


print 'BS3 subBasis',
print list(BS3.getSubBasis())

MO1.setDegree(2)
MO2.setDegree(3)

B.addBasis(BS2)
B.addBasis(MO2)
print 'B subBasis',
print list(B.getSubBasis())
print list(BS1.getSubBasis())
print BS2.getLenght()
print list(BS2.getKnots())

print BS2([0.1])
print B([1,2])

x = SX.sym("x")

print [x]
print BS2( [x] )

BS1.setDegree(3)
print BS1.getDegree()

print B([0.5,0.8]).data()

print MO1
print MO1.getSubBasis()
print MO1([0.5]).data()
print MO2([0.5]).data()

M.addBasis(MO1)
M.addBasis(MO2)
print M([2,3]).data()
