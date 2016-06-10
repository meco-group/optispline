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


#     def test_upper(self):
#         self.assertEqual('foo'.upper(), 'FOO')
#
# if __name__ == '__main__':
#     suite = unittest.TestLoader().loadTestsFromTestCase(TestStringMethods)
#     unittest.TextTestRunner(verbosity=2).run(suite)
#

B = Basis.Basis()

BS1 = Basis.BSplineBasis()
BS2 = Basis.BSplineBasis()

# MO1 = Basis.MonomialeBasis()
# MO2 = Basis.MonomialeBasis()

BS3 = BS1 + BS2

print 'BS1 is a ' +  BS1.getRepresentation()
print 'BS3 is a ',
print BS3

print 'B is a ',
print B

a = Basis.Argument()
print 'a is ',
print a
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


