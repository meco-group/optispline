import matplotlib
import matplotlib.pyplot as plt
from Basis import *
import casadi as ca
import numpy as np
import random

x = [0.15,0.18,0.45,0.50,0.9]
y = np.zeros(5)
plt.plot(x,y,'ko')
# plt.show()

degree = 3
knotsint = 20
knots = np.hstack(((degree)*[0],np.linspace(0.,1.,knotsint),(degree)*[1]))
# knots = (degree)*[0] + [0.,.1,.2,.3,.4,.5,.6,.7,.8,.9,1.] + (degree)*[1]
m = SubBSplineBasis(knots,degree)
b = Basis([m])

a_ = ca.SX.sym('a',m.getLenght(),1)
a = STensor(a_, [m.getLenght(),1,1])
a = Coefficient(a)
s = Function(b,a)
 
# Objective
obj = 0.
x_ = 0.
dx = 0.02
while (x_ < 1.):
    obj = obj + (s([x_]).data())*dx
    x_ = x_ + dx

# Constraints
con = []
con_lower = []
con_upper = []
for x_ in x:
    con.append(s([x_]).data())
    con_lower.append(0.)
    con_upper.append(ca.inf)

for i in range(0,m.getLenght()):
    con.append(a_[i,0])
    con_lower.append(-1.)
    con_upper.append(ca.inf)

nlp = { 'x':ca.vertcat(a_),'f':obj,'g':ca.vertcat(*con) }
solver = ca.nlpsol("solver","ipopt", nlp)
r = solver(lbg=con_lower,ubg=con_upper)

a = r['x'].full()[0:m.getLenght()]
a = DTensor(a, [m.getLenght(),1,1])
a = Coefficient(a)
s = Function(b,a)
interval = np.linspace(0.,1.,101)

field = []

for i in interval:
    field.append(s([i]).data())

plt.figure()
plt.plot(x,y,'ko')
plt.plot(interval,field,'r')
plt.show()

