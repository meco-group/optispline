#!/usr/bin/env python

from helpers import *

class Test_Domain(BasisTestCase):

    def test_intersection(self):
        dom1 = TensorDomain(Interval([-1., 2.]))


if __name__ == '__main__':
    unittest.main()
