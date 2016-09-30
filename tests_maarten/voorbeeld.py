#!/usr/bin/env python

from Basis import *
from casadi import *
from casadi.tools import *

import numpy as np
import matplotlib
matplotlib.use('TKAgg')
import matplotlib.pyplot as plt

# T       = 1.
# degree  = 2
# n_knots = 5
#
# vmax    = 10
#
# DT_knots= T/(n_knots-1)
# DT      = DT_knots/2
#
# knots   = np.r_[np.zeros(degree), np.linspace(0, T, n_knots), T*np.ones(degree)]
# Bl      = BSplineBasis(knots, degree)
#
# #var     = MX.sym('x',len(Bl)+1)
# #cx      = SX.sym('t',len(Bl))
gamma   = SX.sym('y',1)
#
#
# V = struct_symMX([
#     entry('cx',shape=len(Bl)),
#     entry('gamma')
# ])
#
# cx = V["cx"]
# gamma = V["gamma"]
#
#
# x   = BSpline(Bl,cx)
# v   = x.derivative()
# a   = x.derivative(2)
#
obj = gamma*(1-gamma)
#
con = []
# con.append(v-vmax)
# con.append(a-gamma)
# con.append(-a-gamma)
#
#
#
# con = list([c.coeffs for c in con])     # Express constraints on coefficients of spline
# con.append(x(0.)-0.)
# con.append(x(T)-1.)
# con.append(v(0.))
# con.append(v(T))
con = vertcat(con)
#
# gmin = np.r_[-inf*np.ones(con.size1()-4), np.zeros(4)]
# gmax = np.zeros(con.size1())
#

# nlpOut(f=obj, g = con)


nlp = { 'x':vertcat( gamma ),'f':-obj,'g':con }
solver = nlpsol("solver","ipopt", nlp)

# solver.setInput(gmin, "lbg")
# solver.setInput(gmax, "ubg")

r = solver()
print r
# sol = V(var)

# cx    = sol["cx"]

# x   = BSpline(Bl,cx)
# v   = x.derivative()
# a   = x.derivative(2)
# t   = np.linspace(0.,T,1001)
# t_knots = np.linspace(0, T, n_knots)

# plt.figure()
# plt.subplot(3,1,1)
# plt.plot(t, x(t), t_knots, x(t_knots), 'x')
# plt.xlabel('t')
# plt.ylabel('x')
# plt.subplot(3,1,2)
# plt.plot(t, v(t), t_knots, v(t_knots), 'x')
# plt.xlabel('t')
# plt.ylabel('v')
# plt.subplot(3,1,3)
# plt.plot(t, a(t), t_knots, a(t_knots), 'x')
# plt.xlabel('t')
# plt.ylabel('a')

# plt.show()

