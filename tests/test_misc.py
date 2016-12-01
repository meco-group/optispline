#!/usr/bin/env python

from helpers import *


class Test_Misc(BasisTestCase):

    def test_concat(self):
     e = ""
     try:
       Basis.Function(3)
     except Exception as e:
       e = str(e)
     self.assertTrue("You have: '(int)'" in e)
     
if __name__ == '__main__':        
    unittest.main()

