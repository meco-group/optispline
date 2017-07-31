import meco_binaries
meco_binaries(cpp_splines="")
from splines import *
import numpy as np
import time

b = BSplineBasis([0, 1], 1, 2)

x = Function(b, [0, 1])
f = x * ( x-1 )
print(f)

# for i in range(14):
#     f_ = f.degree_elevation(i)
#     print i ,
#     print " : ",
#     print len(f_.coeff_tensor().flatten()) ,
#     print " : ",
#     print min((f_.coeff_tensor().flatten()))

# for i in range(2):
#     f_ = f.insert_knots([(1.0+j)/(i+1) for j in range(i)])
#     print i ,
#     print " : ",
#     print len(f_.coeff_tensor().flatten()) ,
#     print " : ",
#     print min((f_.coeff_tensor().flatten()))

# print "--------------------"

x = Function(b, [0, 1])
f = ( x**2-0.5 )**2
print(f)
print(f.coeff_tensor())

polya = []
bspline = []
r = list(range(0,13))
r = [0,1,3,6,10]
for i in r:
    f_ = f.degree_elevation(i)
    #print(i, end=' ')
    #print(" : ", end=' ')
    #print(len(f_.coeff_tensor().flatten()), end=' ')
    #print(" : ", end=' ')
    #print(min((f_.coeff_tensor().flatten())))
    polya.append(min(f_.coeff_tensor().flatten()))

for i in r:
    f_ = f.insert_knots([(1.0+j)/(i+1) for j in range(i)])
    #print(i, end=' ')
    #print(" : ", end=' ')
    #print(len(f_.coeff_tensor().flatten()), end=' ')
    #print(" : ", end=' ')
    #print(min((f_.coeff_tensor().flatten())))
    bspline.append(min(f_.coeff_tensor().flatten()))

import numpy as np
import matplotlib.pyplot as plt
# Just a figure and one subplot
fig, ax = plt.subplots()
# print r
# print polya
# ax.plot(r, np.log(-np.array(polya)),'--', r, np.log(-np.array(bspline)),"-")
ax.plot(r, polya,'--', r, bspline,"-")
ax.plot(r, [0 for _ in r])
# ax.set_yscale('symlog')
plt.show()



# sq2 = np.sqrt(0.5)
# n=2
# f_ = f.insert_knots([-.05+ sq2 + (- n/2.0 + j )/20 for j in range(n)])
# f_ = f.insert_knots([0.51, 0.73])
# # f_ = f.insert_knots([sq2 - 0.2, sq2 - 0.1, sq2, sq2+0.1, sq2+0.2])
# print len(f_.coeff_tensor().flatten()) ,
# print " : ",
# print min((f_.coeff_tensor().flatten()))
# bspline.append(min(f_.coeff_tensor().flatten()))
# # print f_.basis().greville()

# ax.plot(f_.basis().greville(), f_.coeff_tensor().flatten())

# n = 100
# t = [1.0* i/n for i in range(n+1)]
# ax.plot(t,f.list_eval(t))
# # ax.show()
