import meco_binaries
meco_binaries(cpp_splines="")
from splines import *
import numpy as np
import time

b = BSplineBasis([0, 1], 1, 2)

x = Function(b, [0, 1])
f = x * ( x-1 )
print f
print f.coeff_tensor()

for i in range(15):
    f_ = f.degree_elevation(i)
    print i ,
    print " : ",
    print len(f_.coeff_tensor().flatten()) ,
    print " : ",
    print min((f_.coeff_tensor().flatten()))

for i in range(2):
    f_ = f.insert_knots([(1.0+j)/(i+1) for j in range(i)])
    print i ,
    print " : ",
    print len(f_.coeff_tensor().flatten()) ,
    print " : ",
    print min((f_.coeff_tensor().flatten()))
