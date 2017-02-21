#!/usr/bin/env python

from helpers import *

class Test_Domain(BasisTestCase):

    def test_intersection(self):
        dom1 = TensorDomain([[-1., 2.], [-2., 1.5]], ['x', 'y'])
        dom1 = TensorDomain([[-1., 2.], [-2., 1.5]], ['x', 'y'])
        dom2 = TensorDomain([[0., 1.5], [-3., 1.]], ['y', 'x'])
        dom3 = TensorDomain([[0., 1.5], [-3., 1.]], ['y', 'z'])
        dom4 = TensorDomain([[0., 1.5], [-3., 1.], [-3., 1.]], ['x', 'y', 'z'])
        dom_int = dom1.intersection(dom2)
        assert(dom_int == TensorDomain([Interval([-1., 1.]), Interval([0., 1.5])]))
        assert(dom_int != TensorDomain([Interval([0., 1.5]), Interval([-2.,1.])]))
        with self.assertRaises(Exception):
            dom1.intersection(dom3)
        with self.assertRaises(Exception):
            dom1.intersection(dom4)

if __name__ == '__main__':
    unittest.main()
