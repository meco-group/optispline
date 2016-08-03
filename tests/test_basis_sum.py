#!/usr/bin/env python
import sys
sys.path.insert(0, '../swig')
sys.path.insert(0, '/home/erik/Documents/casadi3.0')
import os

# Boolean flag to indicate if we run in valgrind
# To speed up valgrind, you may choose to put
# expensive computations within 'if not valgrind'
valgrind = int(os.environ.get("VALGRIND",'0'))

import Basis
from casadi import *

basis_x =  Basis.MonomialeBasis(2)
basis_x.setArgument('x')
basis_y =  Basis.MonomialeBasis(1)
basis_y.setArgument('y')
basis_y2 =  Basis.MonomialeBasis(3)
basis_y2.setArgument('y')
basis_z =  Basis.MonomialeBasis(2)
basis_z.setArgument('z')

basis_xy = basis_x + basis_y
basis_yz = basis_y2 + basis_z

basis_xy + basis_y
basis_xy + basis_yz

