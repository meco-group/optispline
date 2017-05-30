import sys
from spline import *
from splineMotionPlanningDefinitions import calc_rectangle
from splineMotionPlanningDefinitions import draw_rectangle
import numpy as np
import casadi as cas
import matplotlib.pyplot as plt
import time
import pdb
plt.interactive(True)
# plt.ion()


###ALGORITHM
vmax  =  0.5
vmin  = -0.5
amax  =  5
amin  = -5
veh_r =  0.1  #enclosing circle of vehicle

pos_veh0 = [0 , 0]    
goal     = [3 , 3]

Nm_c = 0 #amount of circular obstacles
pos_obj0_c   = {0:np.array([[2],[0]]) , 1:np.array([[4],[-2]]) , 2:np.array([[6],[1.75]])} #midpoint
vel_obj0_c   = {0:np.array([[0],[0.3]]) , 1:np.array([[0],[0.3]])  , 2:np.array([[-0.3],[0]])}     #velocity
r_obj        = {0:0.5 , 1:0.5 , 2:0.25}


Nm_r = 1 #amount of rectangular obstacles
w          = np.array([1 , 1 , 1 , 1 , 1 , 1]) #width 
h          = np.array([1 , 1 , 1 , 1 , 1 , 1])    #height 
orient_obj0    = np.pi/180 * np.array([0 , 0 , 0 , 0 , 0 , 0]) #orientation 
pos_obj0_r   = {0:np.array([[1],[1]]), 1:np.array([[3],[1]]), 2:np.array([[5],[1]]), 3:np.array([[1],[2.5]]), 4:np.array([[3],[2.5]]), 5:np.array([[5],[2.5]])} #midpoint
vel_obj0_r   = {0:np.array([[0],[0]]), 1:np.array([[0],[0]]), 2:np.array([[0],[0]]), 3:np.array([[0],[0]]), 4:np.array([[0],[0]]), 5:np.array([[0],[0]])} #velocity
omega_obj0 = np.array([0 , 0 , 0 , 0 , 0 , 0])#angular velocity
Ao, bo, x_vert, y_vert = [],[],[],[] 
for i in range(Nm_r):
    Ao_i, bo_i, x_vert_i, y_vert_i = calc_rectangle(w[i],h[i],pos_obj0_r[i],orient_obj0[i])  #!!!correct output? 
    Ao.append(Ao_i),bo.append(bo_i),x_vert.append(x_vert_i),y_vert.append(y_vert_i) 

#Borders
xU =  6.5
xL = -1
yU =  4
yL = -0.5

# Parameterization
deg   = 3
n     = 11
knots = np.r_[np.zeros(deg), np.linspace(0, 1, n), np.ones(deg)]
B     = BSplineBasis(knots, deg)       #basis for motion trajectory
Blin  = BSplineBasis([0, 0, 1, 1], 1); #basis for prediction
Bplane = BSplineBasis([0, 0, 0.25, 0.5, 0.75, 1, 1], 1); #basis for hyperplane variables

# The optimization variables
X = cas.MX.sym('x', 2 * len(B) + 3 * Nm_c * len(Bplane) + 3 * Nm_r * len(Bplane) + 1)
cx = X[:len(B)]
cy = X[len(B):2*len(B)]
cax_c, cay_c, cb_c = {}, {}, {}
for i in range(Nm_c):
    cax_c[i]  = X[2 * len(B) + 3 * i *len(Bplane) + 0 * len(Bplane):2 * len(B) + 3 * i * len(Bplane) + 1 * len(Bplane)]
    cay_c[i]  = X[2 * len(B) + 3 * i *len(Bplane) + 1 * len(Bplane):2 * len(B) + 3 * i * len(Bplane) + 2 * len(Bplane)]
    cb_c[i]   = X[2 * len(B) + 3 * i *len(Bplane) + 2 * len(Bplane):2 * len(B) + 3 * i * len(Bplane) + 3 * len(Bplane)]
cax_r, cay_r, cb_r = {}, {}, {}
for i in range(Nm_r):
    cax_r[i]  = X[2 * len(B) + 3*Nm_c*len(Bplane) + 3 * i *len(Bplane) + 0 * len(Bplane):2 * len(B) + 3*Nm_c*len(Bplane) + 3 * i * len(Bplane) + 1 * len(Bplane)]
    cay_r[i]  = X[2 * len(B) + 3*Nm_c*len(Bplane) + 3 * i *len(Bplane) + 1 * len(Bplane):2 * len(B) + 3*Nm_c*len(Bplane) + 3 * i * len(Bplane) + 2 * len(Bplane)]
    cb_r[i]   = X[2 * len(B) + 3*Nm_c*len(Bplane) + 3 * i *len(Bplane) + 2 * len(Bplane):2 * len(B) + 3*Nm_c*len(Bplane) + 3 * i * len(Bplane) + 3 * len(Bplane)]
T = X[-1]

# The parameters
P = cas.MX.sym('p', 4 + 4*Nm_c + 6*Nm_r + 1)
P0_veh = P[0:2]
V0_veh = P[2:4]  # True velocity, remember to divide by T!
P0_obj_c = []
V0_obj_c = []
for i in range(Nm_c):
    P0_obj_c.append(P[4+i*4:6+i*4])
    V0_obj_c.append(P[6+i*4:8+i*4])
P0_obj_r = []
V0_obj_r = []
W0_obj   = []
Th0_obj  = []
for i in range(Nm_r):
    P0_obj_r.append(P[4 + 4*Nm_c + i*6 : 6 + 4*Nm_c + i*6])
    V0_obj_r.append(P[6 + 4*Nm_c + i*6 : 8 + 4*Nm_c + i*6])
    Th0_obj.append(P[8 + 4*Nm_c + i*6])
    W0_obj.append( P[9 + 4*Nm_c + i*6])
Tprev  = P[-1] #previous time, needed to set up intial guess for next iteration

# initial parameter values
vel_veh0 = [0,0] #initially at rest
p0 = np.array([pos_veh0[0], pos_veh0[1], vel_veh0[0], vel_veh0[1]])
for i in range(Nm_c):
    p0 = np.append(p0, pos_obj0_c[i])
    p0 = np.append(p0, vel_obj0_c[i])
for i in range(Nm_r):
    p0 = np.append(p0, pos_obj0_r[i])
    p0 = np.append(p0, vel_obj0_r[i])
    p0 = np.append(p0, orient_obj0[i])
    p0 = np.append(p0, omega_obj0[i])
p0 = np.append(p0, 1) #last element is Tprev

# The relevant splines
x =   BSpline(B, cx)
y =   BSpline(B, cy)
dx =  x.derivative(1)
dy =  y.derivative(1)
ddx = x.derivative(2)
ddy = y.derivative(2)

ax_c, ay_c, b_c = {}, {}, {}
for i in range(Nm_c):
    ax_c[i]  = BSpline(Bplane,cax_c[i])
    ay_c[i]  = BSpline(Bplane,cay_c[i])
    b_c[i]   = BSpline(Bplane,cb_c[i])

ax_r, ay_r, b_r = {}, {}, {}
for i in range(Nm_r):
    ax_r[i]  = BSpline(Bplane,cax_r[i])
    ay_r[i]  = BSpline(Bplane,cay_r[i])
    b_r[i]   = BSpline(Bplane,cb_r[i])

#Constraints
con = [#-(ddx**2 + ddy**2) + (amax * T**2)**2, #this doesn't work good!
       -(dx**2 + dy**2)   + (vmax * T)**2] #acceleration and speed constraints   

#Add rectangle constraints
for i in range(Nm_r):    
    obj_pred = [BSpline(Blin, cas.vertcat(P0_obj_r[i][0], V0_obj_r[i][0] * T + P0_obj_r[i][0])),
                BSpline(Blin, cas.vertcat(P0_obj_r[i][1], V0_obj_r[i][1] * T + P0_obj_r[i][1]))] #make prediction for obstacle i    
    not1,not2,x_vert,y_vert = calc_rectangle(w[i],h[i],np.array([[obj_pred[0]],[obj_pred[1]]]),orient_obj0[i]) #calculate verts and A,b for obstacle i            
    for j in range(4):
        con.extend([ax_r[i]*x_vert[0][j] + ay_r[i]*y_vert[0][j] - b_r[i]])
        
    con.extend([-(ax_r[i]*x + ay_r[i]*y) + b_r[i] - veh_r])
    con.extend([1 - (ax_r[i] * ax_r[i] + ay_r[i] * ay_r[i])])

    ## HERE ARE THE PRINTS
    print "ax_r[i] basis: ",ax_r[i].basis.knots,"\n"
    print "x basis: ",x.basis.knots,"\n"
    print "ax_r[i]*x basis: ",(ax_r[i]*x).basis.knots,"\n"

pdb.set_trace()


    
#Add border constraints  
con.extend([ xU - veh_r - x])
con.extend([-xL - veh_r + x])
con.extend([ yU - veh_r - y])
con.extend([-yL - veh_r + y])
    
#Translate to constraints on the coeffs
con = cas.vertcat(*[c.coeffs for c in con])

#Positive motion time
con = cas.vertcat(con, T)

#Equality constraints
con = cas.vertcat(con, x(0) - P0_veh[0], y(0) - P0_veh[1],
                  dx(0) - V0_veh[0] * T/Tprev, dy(0) - V0_veh[1] * T/Tprev,
                  dx(1), dy(1),
                  x(1), y(1)) #position and velocity constraints

nlp = {'x':X, 'p':P, 'f':T, 'g':con}
options = {}
#options['ipopt.linear_solver'] = 'ma57'
options['ipopt.tol'] = 1e-5
solver = cas.nlpsol('solver','ipopt', nlp, options)

lbg = np.r_[np.zeros(con.size1()-8), 0, 0, 0, 0, 0, 0, goal[0], goal[1]]
ubg = np.r_[np.inf * np.ones(con.size1()-8), 0, 0, 0, 0, 0, 0, goal[0], goal[1]]
lbx = -100 * np.ones(X.size1())
ubx = 100 * np.ones(X.size1())

solver_input = {}
solver_input['lbx'] = lbx
solver_input['ubx'] = ubx
solver_input['lbg'] = lbg
solver_input['ubg'] = ubg

plt.figure()
plt.axis(1.1*np.array([xL, xU, yL, yU]))#('auto')
plt.hold(True)
plt.show()
plt.plot(goal[0], goal[1], 'rx')
plt.plot([xU , xU , xL , xL , xU],[yL , yU , yU , yL , yL],'r-') #plot border
plt.pause(0.05)
s = np.linspace(0, 1, 101) 

Ts = 0.5 #0.025 #sample time
x0 = np.array(np.zeros(X.size1())) #initial guess, not beneficial to adapt
t_solve = []
# Do the loop

solver_input['x0'] = x0
solver_input['p'] = p0
solver_output = solver(**solver_input)
pdb.set_trace()

iterate = 0 
while iterate and np.linalg.norm(p0[:2] - [goal[0], goal[1]]) >= 1e-2: #within 1 cm of goal
    solver_input['x0'] = x0
    solver_input['p'] = p0
    solver_output = solver(**solver_input)

    import pdb; pdb.set_trace()  # breakpoint 78281288 //
    t_solve.append(solver.stats()['t_proc_mainloop'])

    X = solver_output['x']
    cx = X[:len(B)]
    cy = X[len(B):2*len(B)]
    x = BSpline(B, cx)
    y = BSpline(B, cy)
    # T = X[-1]#.toScalar() # -->doesn't work with new swig???
    T = float(X[-1])
    dx = x.derivative(1)
    dy = y.derivative(1)
    Tprev = p0[-1] #get Tprev out of initial parameters
    obstacles_c = []
    for i in range(Nm_c):
        obstacles_c.append(plt.plot(r_obj[i] * np.sin(2 * np.pi * s) + pos_obj0_c[i][0], r_obj[i] * np.cos(2 * np.pi * s) + pos_obj0_c[i][1], 'b'))
    obstacles_r = []
    for i in range(Nm_r):
        obstacles_r.append(draw_rectangle(w[i],h[i],pos_obj0_r[i],orient_obj0[i],'r'))    
    vehicle_circle = plt.plot(veh_r * np.sin(2 * np.pi * s) + p0[0], veh_r * np.cos(2 * np.pi * s) + p0[1], 'g')
    plt.plot(p0[0], p0[1], 'kx')
    vehicle_path = plt.plot(x(s), y(s), 'g')
    plt.pause(0.05)
    #prediction of movement
    obstacles_pred = []
    for i in range(Nm_c):
        s_ox = BSpline(Blin, [pos_obj0_c[i][0,0], vel_obj0_c[i][0,0] * T + pos_obj0_c[i][0,0]])
        s_oy = BSpline(Blin, [pos_obj0_c[i][1,0], vel_obj0_c[i][1,0] * T + pos_obj0_c[i][1,0]])
        obstacles_pred.append(plt.plot(s_ox(s),s_oy(s),'b'))
    
    plt.draw()
    for i in range(Nm_c):
        obstacles_c[i].pop(0).remove()
        obstacles_pred[i].pop(0).remove()
    for i in range(Nm_r):
        obstacles_r[i].pop(0).remove()
    vehicle_path.pop(0).remove()
    vehicle_circle.pop(0).remove()
    time.sleep(0.01)
    
    #Update parameters
    pos_veh0[0] = x(Ts / T)
    pos_veh0[1] = y(Ts / T)
    vel_veh0[0] = dx(Ts / T) #/ T
    vel_veh0[1] = dy(Ts / T) #/ T
    p0 = np.r_[pos_veh0[0], pos_veh0[1], vel_veh0[0], vel_veh0[1]]
    for i in range(Nm_c):
        pos_obj0_c[i] = pos_obj0_c[i] + Ts*vel_obj0_c[i]
        p0 = np.append(p0 , pos_obj0_c[i])
        vel_obj0_c[i] = vel_obj0_c[i]
        p0 = np.append(p0 , vel_obj0_c[i])
    for i in range(Nm_r):
        pos_obj0_r[i] = pos_obj0_r[i] + Ts*vel_obj0_r[i]
        p0 = np.append(p0 , pos_obj0_r[i])
        vel_obj0_r[i] = vel_obj0_r[i]
        p0 = np.append(p0 , vel_obj0_r[i])
        orient_obj0[i] = orient_obj0[i] + Ts*omega_obj0[i]
        p0 = np.append(p0, orient_obj0[i])
        omega_obj0[i] = omega_obj0[i]
        p0 = np.append(p0, omega_obj0[i])
    p0 = np.append(p0,T)
    #Update initial guess
    x0 = X
    #adapt initial guess such that initial conditions are fulfilled
    #x0[0] = pos_veh0[0] 
    #x0[1] =(vel_veh0[0]*T/Tprev*(B.knots[2+deg]-B.knots[2])+deg*pos_veh0[0])/deg    
    #x0[len(B)] = pos_veh0[1]
    #x0[len(B)+1] = (vel_veh0[1]*T/Tprev*(B.knots[2+deg]-B.knots[2])+deg*pos_veh0[1])/deg
    x0[-1] = T  
    print "===========",pos_veh0[0]
    print "===========",T
    print "\n\n\n\n\n"
plt.savefig('movement')    
