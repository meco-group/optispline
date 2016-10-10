
# coding: utf-8

# We want to make some estamestions given some points in $2$D 

# Start by loading in al the libarys.

# In[1]:

from Basis import *

import casadi as ca

import numpy as np

import matplotlib
matplotlib.use('TKAgg')
import matplotlib.pyplot as plt

import random


# Generating the points
# 
#  

# In[2]:

n = 10

x = [1.0*i/(n-1) for i in range(n) ]
random.seed(4)
y = [random.random() for _ in range(n)]

plt.figure()
plt.plot(x,y,'ko')


# Construct the casadi problem

# In[3]:

est   = ca.SX.sym('est',1)
bound   = ca.SX.sym('bound',1)

obj = bound

con = []
con_lower = []
con_upper = []

for x_,y_ in zip(x,y):
     con.append(est - bound)
     con_lower.append(-ca.inf)
     con_upper.append(y_)
    
     con.append(est + bound)
     con_lower.append(y_)
     con_upper.append(ca.inf)


# In[4]:

nlp = { 'x':ca.vertcat(est, bound ),'f':obj,'g':ca.vertcat(*con) }
solver = ca.nlpsol("solver","ipopt", nlp)
r = solver(lbg = con_lower, ubg = con_upper)


# In[5]:

est_,bound_ = r['x'].full()
est_ = est_[0]
bound_ = bound_[0]
print "center value is ", str(est_)
print "distance to the bound is ", str(bound_)


# Plotting the result

# In[6]:

upper_bound = 2 * [est_ + bound_]
lower_bound = 2 * [est_ - bound_]
middel = 2 * [est_]
interval = [0, 1]

plt.figure()
plt.plot(x,y,'ko')
plt.plot(interval,upper_bound,'b')
plt.plot(interval,lower_bound,'b')
plt.plot(interval,middel,'r')


# Now we will try something simular but our estimation will be linear depending on $x$. To do this we will construct a Monomial and optimise over its coefficients.

# In[7]:

degree = 1
m = SubMonomialBasis(degree)


# In[8]:

b = Basis([m])
b = MonomialBasis(degree)


# In[9]:

print b([0.1]).data()
print b([ca.MX.sym('x',1,1)]).data()


# In[10]:

a_ = ca.SX.sym('a',degree + 1,1)
a = STensor(a_, [degree + 1,1,1])
#a = STensor('a',[degree + 1,1,1])
a = Coefficient(a)
#a


# In[11]:

est_function = Function(b,a)


#Function( Basis& basis, const Coefficient& coef) : basis(basis), coef(coef) {} 

#    
#Basis b;
#ST s;#

##explicit Coefficient(const ST& a);

#
#Function(b, s);
#c = b.ST(1,1)
#Function(b, c)
#Function(b, Coefficient(b.ST(1,1)))



    


# In[12]:

est_function([0.1]).data()


# In[13]:

con = []
con_lower = []
con_upper = []

for x_,y_ in zip(x,y):
     con.append(est_function([x_]).data() - bound)
     con_lower.append(-ca.inf)
     con_upper.append(y_)
    
     con.append(est_function([x_]).data() + bound)
     con_lower.append(y_)
     con_upper.append(ca.inf)


# In[14]:

nlp = { 'x':ca.vertcat(a_, bound ),'f':obj,'g':ca.vertcat(*con) }
solver = ca.nlpsol("solver","ipopt", nlp)
r = solver(lbg = con_lower, ubg = con_upper)


# In[15]:

print r['x'].full()[0:degree+1]


# In[16]:

est_ = r['x'].full()[0:degree+1]
bound_ = r['x'].full()[degree+1]
bound_ = bound_[0]
print "center value is ", str(est_)
print "distance to the bound is ", str(bound_)


# In[17]:

a = est_
a = DTensor(a, [degree + 1,1,1])
a = Coefficient(a)


# In[18]:

est_function = Function(b,a)


# In[ ]:

interval = np.linspace(0.,1.,101)
upper_bound = []
lower_bound = []
middel = []

for i in interval:
    upper_bound.append(est_function([i]).data() + bound_)
    lower_bound.append(est_function([i]).data() - bound_)
    middel.append(est_function([i]).data())

plt.figure()
plt.plot(x,y,'ko')
plt.plot(interval,upper_bound,'b')
plt.plot(interval,lower_bound,'b')
plt.plot(interval,middel,'r')


# In[ ]:

degree = 2
m = SubBSplineBasis([0,0,0,0.5,1,1,1],degree)
b = Basis([m])
a_ = ca.SX.sym('a',m.getLength(),1)
a = STensor(a_, [m.getLength(),1,1])
a = Coefficient(a)
est_function = Function(b,a)
con = []
con_lower = []
con_upper = []

for x_,y_ in zip(x,y):
     con.append(est_function([x_]).data() - bound)
     con_lower.append(-ca.inf)
     con_upper.append(y_)
    
     con.append(est_function([x_]).data() + bound)
     con_lower.append(y_)
     con_upper.append(ca.inf)
        
nlp = { 'x':ca.vertcat(a_, bound ),'f':obj,'g':ca.vertcat(*con) }
solver = ca.nlpsol("solver","ipopt", nlp)
r = solver(lbg = con_lower, ubg = con_upper)
est_ = r['x'].full()[0:degree+1]
bound_ = r['x'].full()[degree+1]
bound_ = bound_[0]
print "center value is ", str(est_)
print "distance to the bound is ", str(bound_)
a = est_
a = DTensor(a, [degree + 1,1,1])
a = Coefficient(a)
est_function = Function(b,a)
interval = np.linspace(0.,1.,101)
upper_bound = []
lower_bound = []
middel = []

for i in interval:
    upper_bound.append(est_function([i]).data() + bound_)
    lower_bound.append(est_function([i]).data() - bound_)
    middel.append(est_function([i]).data())

plt.figure()
plt.plot(x,y,'ko')
plt.plot(interval,upper_bound,'b')
plt.plot(interval,lower_bound,'b')
plt.plot(interval,middel,'r')

