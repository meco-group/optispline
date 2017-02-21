#!/usr/bin/env python

from helpers import *

class Test_Domain(BasisTestCase):

    def test_intersection(self):
        dom1 = TensorDomain([[-1., 2.], [-2., 1.5]])
        dom2 = TensorDomain([[0., 1.5], [-3., 1.]])
        dom3 = dom1.intersection(dom2)
        doms = dom3.domains()
        for k in range(2):
            print "interval " + str(k) + ": [" + str(doms[k].min()) + "," + str(doms[k].max()) + "]"


if __name__ == '__main__':
    unittest.main()
