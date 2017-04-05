#!/usr/bin/env python

from helpers import *
rand = numpy.random.randn

class Test_Domain(BasisTestCase):

    def test_trace(self):
        c_ = Function(rand(3, 3))

        b = MonomialBasis(3)
        f_ = Function(b, rand(4, 3, 3))

        t_ = f_

        t = t_.trace()
        for i in range(len(t.data())):
            v  = t.data()[i]
            v_ = t_.data()[i]
            self.assertAlmostEqual(v, v_.trace())

        t_ = c_
        t = t_.trace()
        v  = t.data()
        v_ = t_.data()
        self.assertAlmostEqual(v, v_.trace())

if __name__ == '__main__':
    unittest.main()
