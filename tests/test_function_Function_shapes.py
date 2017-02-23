
#!/usr/bin/env python

from helpers import *

from Basis import vertcat, horzcat, blkdiag
class Test_Function_Function_shape(BasisTestCase):

    def test_transpose(self):
        knots1 = [0,0,0.4,0.6,1,1]
        degree = 1
        basis1 = BSplineBasis(knots1,degree)

        knots2 = [0.,0.,0.5,0.6,1,1]
        degree = 1
        basis2 = BSplineBasis(knots2,degree)

        basis3 = MonomialBasis(3);

        mbasis1 = TensorBasis([basis1,basis2]);
        mbasis2 = TensorBasis([basis2,basis3]);

        shape1 = [4,4,2,4]
        shape2 = [4,4,3,3]
        coeff1 = DTensor(numpy.random.randn(numpy.prod(shape1),1),shape1)
        coeff2 = DTensor(numpy.random.randn(numpy.prod(shape2),1),shape2)
        func1 = Function(mbasis1,coeff1)
        func2 = Function(mbasis2,coeff2)

        func1t = func1.transpose();
        func2t = func2.transpose();

        self.assertEqualT(func1t.shape,[4,2])
        self.assertEqualT(func2t.shape,[3,3])
        self.assertEqualT(func2t.transpose().shape,func2.shape)

    def test_concatenation(self):
        knots1 = [0,0,0.4,1,1]
        degree = 1
        basis1 = BSplineBasis(knots1,degree)

        knots2 = [0.,0.,0.5,1,1]
        degree = 1
        basis2 = BSplineBasis(knots2,degree)

        basis3 = MonomialBasis(3);

        mbasis1 = TensorBasis([basis1,basis2]);
        mbasis2 = TensorBasis([basis2,basis3]);

        shape1 = [3,3,1,1]
        shape2 = [3,4,1,1]
        coeff1 = DTensor(numpy.random.randn(numpy.prod(shape1),1),shape1)
        coeff2 = DTensor(numpy.random.randn(numpy.prod(shape2),1),shape2)
        func1 = Function(mbasis1,coeff1)
        func2 = Function(mbasis2,coeff2)
        
        func3 = vertcat(func1, func2)
        self.assertEqual(func3.shape,[2,1])
        self.assertEqualT(func3(0.5,0.2),DTensor([func1(0.5,0.2),func2(0.5,0.2)],[2,1]))

        constant1 = Function.Constant(mbasis1, DTensor([2,2],[2,1]))
        func4 = horzcat(func3, constant1)
        self.assertEqual(func4.shape,[2,2])

        self.assertEqualT(func4(0.5,0.2),DTensor([func1(0.5,0.2),func2(0.5,0.2),2,2],[2,2]))
        constant2 = Function.Constant(mbasis1,DTensor([],[0,1]));
        constant3 = Function.Constant(mbasis2,DTensor([],[1,0]));
        func5 = vertcat(func1, constant2)
        func5 = horzcat(func5, constant3)
        self.assertEqual(func5.shape,[1,1])

    def test_blockdiagonal(self):
        knots1 = [0,0,0.4,1,1]
        degree = 1
        basis1 = BSplineBasis(knots1,degree)

        knots2 = [0.,0.,0.5,1,1]
        degree = 1
        basis2 = BSplineBasis(knots2,degree)

        basis3 = MonomialBasis(3);

        mbasis1 = TensorBasis([basis1,basis2]);
        mbasis2 = TensorBasis([basis2,basis3]);

        shape1 = [3,3,1,1]
        shape2 = [3,4,1,1]
        coeff1 = DTensor(numpy.random.randn(numpy.product(shape1),1),shape1)
        coeff2 = DTensor(numpy.random.randn(numpy.product(shape2),1),shape2)
        func1 = Function(mbasis1,coeff1)
        func2 = Function(mbasis2,coeff2)

        func3 = blkdiag(func1, func2)
        self.assertEqual(func3.shape,[2,2])
        self.assertEqualT(func3(0.5,0.2),DTensor([func1(0.5,0.2),0,0,func2(0.5,0.2)],[2,2]))

        constant1 = Function.Constant(mbasis2, DTensor([],[2,0]))
        func4 = blkdiag(func1, constant1)
        self.assertEqual(func4.shape,[3,1])
        self.assertEqualT(func4(0.5,0.2),DTensor([func1(0.5,0.2),0,0],[3,1]))

        constant2 = Function.Constant(mbasis1, DTensor([2,3],[1,2]))
        func5 = blkdiag(func1, constant2)
        self.assertEqual(func5.shape,[2,3])
        self.assertEqualT(func5(0.5,0.2),DTensor([func1(0.5,0.2),0,0,2,0,3],[2,3]))

    def test_reshape(self):
        knots = [0,0,0.5,1,1]
        degree = 1
        b1 = BSplineBasis(knots, degree)
        b2 = MonomialBasis(3)
        b = TensorBasis([b1,b2])

        shape = [3,4,2,4]
        c = numpy.random.random(shape)
        f = Function(b,c)
        new_f = f.reshape([1,8])

        N = 100
        _x = numpy.random.random(N)
        _y = numpy.random.random(N)
        for i in range(0,N):
            x = _x[i]
            y = _y[i]

            test_value = numpy.reshape((f.transpose())(x,y), [1,8])
            value = new_f(x,y)
            self.assertEqualT(value, test_value)


if __name__ == '__main__':
    unittest.main()
