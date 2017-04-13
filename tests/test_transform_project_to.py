#!/usr/bin/env python

from helpers import *

class Test_Project_To(BasisTestCase):
    def test_univariate_basis(self):
        #np.random.seed(0)
        degree = 3
        knots1 = [0,0,0,0,0.2,0.5,0.8,1,1,1,1]
        b1 = BSplineBasis(knots1,degree)
        c1 = Coefficient(5*np.random.random(b1.dimension()))
        s1 = Function(b1,c1)

        #knots2 = [0.1,0.1,0.1,0.1,0.1,0.2,0.5,0.8,1,1,1,1,1]
        #knots2 = [0,0,0,0,0,0.2,0.2,0.5,0.5,0.6,0.8,1,1,1,1,1]
        knots2 = [0,0,0,0,0,0.2,0.2,0.5,0.5,0.7,0.8,0.8,1,1,1,1,1]
        b2 = BSplineBasis(knots2,4)

        T = b1.project_to(b2)
        c2 = c1.transform(T)
        s2 = Function(b2,c2)

        g2 = numpy.random.random(10)
        for i in g2:
            #print 2*(s1(i)-s2(i))/(s1(i)+s2(i))
            self.assertEqualT(s1(i), s2(i), 1e-6)

    def test_multivariate_basis(self):
        #np.random.seed(0)
        degree = 3
        knots1 = [0,0,0,0,0.2,0.5,0.8,1,1,1,1]
        b11 = BSplineBasis(knots1,degree)
        b12 = MonomialBasis(1)
        b1 = TensorBasis([b11, b12])
        c1 = Coefficient(np.random.rand(b11.dimension(), b12.dimension()))
        s1 = Function(b1,c1)

        knots2 = [0,0,0,0,0,0.2,0.2,0.5,0.5,0.7,0.8,0.8,1,1,1,1,1]
        #knots2 = [0,0,0,0,0,0.2,0.4,0.5,0.6,0.8,1,1,1,1,1]
        b21 = BSplineBasis(knots2,4)
        b22 = BSplineBasis([0,0,0.5,1,1], 1)
        b2 = TensorBasis([b21, b22])
        T = b1.project_to(b2)
        c2 = c1.transform(T)
        s2 = Function(b2,c2)

        N = 10;
        x = numpy.random.random(N)
        y = numpy.random.random(N)
        for i in range(0,N):
            # print s1(x[i],y[i]) - s2(x[i],y[i])
            self.assertEqualT(s1(x[i],y[i]), s2(x[i],y[i]), 1e-6)

    def test_univariate_function(self):
        #np.random.seed(0)
        degree = 3
        knots1 = [0,0,0,0,0.1,0.3,0.7,1,1,1,1]
        b1 = BSplineBasis(knots1,degree)
        c1 = Coefficient(5*np.random.random(b1.dimension()))
        s1 = Function(b1,c1)

        knots2 = [0,0,0,0,0,0.1,0.1,0.3,0.3,0.7,0.7,0.8,1,1,1,1,1]
        b2 = BSplineBasis(knots2,4)

        T = b1.project_to(b2)
        c2 = c1.transform(T)
        s2 = s1.project_to(b2)

        self.assertEqualT(c2,s2.coeff_tensor(),1e-6)

        g2 = numpy.random.random(10)
        for i in g2:
            self.assertEqualT(s1(i), s2(i), 1e-6)


    def test_multivariate_function(self):
        #np.random.seed(0)
        degree = 3
        knots1 = [0,0,0,0,0.2,0.5,0.8,1,1,1,1]
        b11 = BSplineBasis(knots1,degree)
        b12 = MonomialBasis(1)
        b1 = TensorBasis([b11, b12])
        c1 = Coefficient(np.random.rand(b11.dimension(), b12.dimension(),2,3))
        s1 = Function(b1,c1)

        knots2 = [0,0,0,0,0,0.2,0.2,0.5,0.5,0.7,0.8,0.8,1,1,1,1,1]
        b21 = BSplineBasis(knots2,4)
        b22 = BSplineBasis([0,0,0.5,1,1], 1)
        b2 = TensorBasis([b21, b22])
        T = b1.project_to(b2)
        c2 = c1.transform(T)
        s2 = s1.project_to(b2)

        self.assertEqualT(c2,s2.coeff_tensor(),1e-6)

        N = 100;
        x = numpy.random.random(N)
        y = numpy.random.random(N)
        for i in range(0,N):
            # print s1(x[i],y[i]) - s2(x[i],y[i])
            self.assertEqualT(s1(x[i],y[i]), s2(x[i],y[i]), 1e-6)


    def test_transform(self):
        x = Polynomial([0, 1],'x');
        y = Polynomial([0, 1],'y');
        # x = vertcat(x,y);
        # x_ = vertcat(x,y);
        # print x([1,1.2],[ 'y','x' ])
        # print x([1,1.2],[ 'x','y' ])
        # tb = TensorBasis([ BSplineBasis([-1.5,1.5],1,2),BSplineBasis([-1.5,1.5],1,2) ],[ 'x','y' ])
        tb =  BSplineBasis([-1.5,1.5],1,2)
        tx = x.tensor_basis().transform_to(tb);
        x = x.transform_to(tb);
        print x_
        print x
        print tx
        print x([0.2],[ 'x' ])
        # print x([1,1.2],[ 'x','y' ])


if __name__ == '__main__':
    unittest.main()

