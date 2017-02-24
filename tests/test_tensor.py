#!/usr/bin/env python

from helpers import *


class Test_Tensor(BasisTestCase):

    def test_operations_cast(self):
        
        scalars = [4, 4.0, np.ones((1,1))*4, DM(4), DTensor([4],[]), SX(4)]
        matrices = [np.ones((2,2))*4, DM.ones(2,2)*4, SX(DM.ones(2,2)*4), DTensor([4]*4,[2,2])]

        for A in [DTensor(3,[]), STensor(3,[])]:
          for s in scalars:
            for lhs, rhs in [(A,s),(s,A)]:
              lhs+rhs
              lhs*rhs
              lhs-rhs
                 
        for A in [Polynomial([0,1],'x')]:
          for s in scalars+matrices:
            for lhs, rhs in [(A,s),(s,A)]:
              lhs+rhs
              lhs*rhs
              lhs-rhs      

        for A in [DTensor(range(4),[2,2]), STensor(range(4),[2,2])]:
          for s in matrices:
            for lhs, rhs in [(A,s),(s,A)]:
              lhs+rhs
              lhs*rhs
              lhs-rhs

        knots1 = [0,0,0.4,1,1]
        degree = 1
        basis1 = splines.BSplineBasis(knots1,degree)

        knots2 = [0.,0.,0.5,1,1]
        degree = 1
        basis2 = splines.BSplineBasis(knots2,degree)

        basis3 = MonomialBasis(3);

        mbasis1 = TensorBasis([basis1,basis2]);
        coeff1 = DTensor(numpy.random.randn(9*4,1),[3,3,2,2])
        
        func1 = Function(mbasis1,coeff1)

        for A in [func1]:
          for s in matrices+scalars:
            for lhs, rhs in [(A,s),(s,A)]:
              lhs+rhs
              lhs*rhs
              lhs-rhs
                        
    def test_concat(self):

        A = DTensor(1,[])
        B = DTensor(2,[])

        with self.assertRaises(Exception):
         DTensor.concat([A,B],0)

        A = DTensor([0,1,2],[3])
        B = DTensor([3,4],[2])

        C = DTensor(DM([0,1,2,3,4]),[5])

        self.assertEqualT( DTensor.concat([A,B],0), C)
        with self.assertRaises(Exception):
          DTensor.concat([A,B],1)

        A = DTensor(DM([[0,1],[2,3],[4,5]]),[3,2])
        B = DTensor(DM([[6,7]]),[1,2])

        C = DTensor(DM([[0,1],[2,3],[4,5],[6,7]]),[4,2])

        self.assertEqualT( DTensor.concat([A,B],0), C)
        with self.assertRaises(Exception):
          DTensor.concat([A,B],1)

        A = DTensor(DM([[0,1,2],[3,4,5]]),[2,3])
        B = DTensor(DM([6,7]),[2,1])

        C = DTensor(DM([[0,1,2,6],[3,4,5,7]]),[2,4])

        self.assertEqualT( DTensor.concat([A,B],1), C)
        with self.assertRaises(Exception):
          DTensor.concat([A,B],0)

        A = DTensor(range(24),[2,3,4])
        B = DTensor(-DM(range(16)),[2,2,4])

        with self.assertRaises(Exception):
          DTensor.concat([A,B],0)
        with self.assertRaises(Exception):
          DTensor.concat([A,B],2)

        A = DTensor(range(8),[2,2,2])
        B = DTensor(-DM(range(8)),[2,2,2])

        def myslice(value,axis):
          ret = [-1]*3
          ret[axis] = value
          return ret

        for k in range(3):
          C = DTensor.concat([A,B],k)

          self.assertEqualT( A.index(myslice(0,k)), C.index(myslice(0,k)))
          self.assertEqualT( A.index(myslice(1,k)), C.index(myslice(1,k)))
          self.assertEqualT( B.index(myslice(0,k)), C.index(myslice(2,k)))
          self.assertEqualT( B.index(myslice(1,k)), C.index(myslice(3,k)))


    def test_cross(self):
        STensor(DTensor(5,[]))
        MTensor(DTensor(5,[]))

    def test_input_typemap(self):
      a = np.random.random((2,3,4))
      b = DTensor(a)

      self.assertEqualT( a[:,:,0], b[:,:,0] )
      self.assertEqualT( a[:,:,1], b[:,:,1] )
      self.assertEqualT( a[:,1,:], b[:,1,:] )
      self.assertEqualT( a[0,:,:], b[0,:,:] )

    def test_output_typemap(self):

      B1 = MonomialBasis(2)
      B2 = MonomialBasis(3)
      B3 = MonomialBasis(4)

      B = TensorBasis([B1,B2,B3])

      a = np.random.random(B.dimension())
      a = DTensor(a)
      print a
      a_ = Coefficient(a)
      F = splines.Function(B, a_)

      c = F.coeff()
      r = c.data()

      # a = DTensor(a ,  B.dimension() + [1, 1])
      print "------"
      a = a.shape(a.dims()+[1,1])


      print a[:,:,0,0,0]
      print r[:,:,0,0,0]

      self.assertEqualT( r, a )

    def test_mtimes(self):
      A = DTensor(range(6),[2,3])
      B = DTensor(range(12),[3,4])

      a = np.array(A.matrix())
      b = np.array(B.matrix())

      print "test"
      print A.mtimes(B)
      print a.dot(b)

      self.assertEqualT(A.mtimes(B), a.dot(b))

      with self.assertRaises(Exception):
        B.mtimes(A)

if __name__ == '__main__':
    unittest.main()

