import meco_binaries
meco_binaries(cpp_splines="")
from splines import *
import numpy as np
from pylab import *

np.random.seed(1)

b = BSplineBasis([0,1], 2, 3)
c = np.random.rand(b.dimension())

f = Function(b,c)

def splineplot(f):
  b = f.basis()
  knots = b.knots()
  xe = np.linspace(knots[0],knots[-1],1000)
  d=f.basis().degree();

  figure()
  subplot(2+d,1,1)
  plot(xe,f.list_eval(xe))
  plot(knots,f.list_eval(knots),'o')
  subplot(2+d,1,2)

  pos = []
  count = 0
  prev = np.nan
  for k in knots:
    if np.linalg.norm(k-prev)<1e-9:
      count+=1
    else:
      count = 0
    pos.append(count)
    prev = k
  title('knot locations')
  plot(b.knots(),pos,'o')

  for i in range(d):
    f = f.derivative(1)
    subplot(2+d,1,3+i)
    title('derivative(%d)' % (i+1))
    plot(xe,f.list_eval(xe))
    plot(knots,f.list_eval(knots),'o')    

splineplot(f)

splineplot(f.degree_elevation(1))

splineplot(f.insert_knots([0.2,0.8]))

show()
