import meco_binaries
meco_binaries(cpp_splines="")
from splines import *
import numpy as np
import time

pi = np.pi
knots = [0,0,0,1,1,1]

sq05 = np.sqrt(0.5)
weights = DTensor([1, sq05, 1], [3])

b = BSplineBasis(knots, 2)

coef_cos_1 = sq05*DTensor([ 1,np.sqrt(2),1], [3])
coef_sin_1 = sq05*DTensor([ -1, 0, 1 ], [3])

cos1 = Function(TensorBasis(b, "alpha") , coef_cos_1)
sin1 = Function(TensorBasis(b, "alpha") , coef_sin_1)

noemer = Function(TensorBasis(b, "alpha") , weights)

no = noemer**2
s1 = sin1*noemer * np.sqrt(2)
c1 = cos1*noemer * np.sqrt(2) - no
s2 = 2*sin1*cos1
c2 = cos1**2 - sin1**2


n = 100
t = [((1.0*i)/(n)) for i in range(n+1)]

s = Function.horzcat([s1, s2])
c = Function.horzcat([c1, c2])

B = TensorBasis([b*b, BSplineBasis([0,1],1,2)], ['a', 'r'])
s = Function(B, s.coeff_tensor().squeeze())
c = Function(B, c.coeff_tensor().squeeze())

# def f(x,y):
#     return [(c([x,y],['a','r'])/no(x)), (s([x,y],['a','r'])/no(x))]

# import matplotlib.pyplot as plt
# t = [1.0*i/(n) for i in range(n+1)]
# for j in np.linspace(0,1,6):
#     p = [ f(i,j) for i in t]
#     p = np.array(p)
#     plt.plot(p[:,0],p[:,1])
#     p = [ f(j,i) for i in t]
#     p = np.array(p)
#     plt.plot(p[:,0],p[:,1])
# plt.show()


p = (c - 0.5*no)**2 + (s - 0.5*no)**2
print(min(p.coeff_tensor().flatten()))
for i in range(7):
    print(min(p.midpoint_refinement(i).coeff_tensor().flatten()))
