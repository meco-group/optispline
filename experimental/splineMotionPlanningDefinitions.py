#This file contains all extra functions which are needed to run the splineMotionPlanning code
#e.g.: calculating rectangles, drawing rectangles,...


# Imports
# =======

import numpy as np
from spline import *
from matplotlib import pyplot as plt

# calc_rectangle
# ==============

#This function calculates a hyperplane representation Ax<=b of a
#rectangle, based on the width, height, midpoint and orientation. The
#    function also gives the vertices of the rectangle. This data is needed
#    to plot and avoid a rectangular obstacle.
#    
#	Inputs: 
#		w          = width  [m]
#		l          = length [m]
#		mid        = midpoint of the rectangle   [m]
#       orient     = counter clockwise rotation of rectangle w.r.t. x-axis [deg]
#	Outputs:
#		Ao         = A-matrix of the hyperplane-representation
#		bo         = b-matrix of the hyperplane-representation
#       x_vertices = x-coordinates of vertices
#       y_vertices = y-coordinates of vertices
# 
#                      l
#           <-------------------->                      y            
#         ^ #====================#                      ^
#         | #====================#                      |
#        w| #====================#                      |
#         | #====================#                      |
#         = #====================#  orient = 0          |
#                                                       ---------> x
# 
# ----------------------------------------------------------------------    
# ----------------------------------------------------------------------

def calc_rectangle(w,l,mid,orient):
    #Calculate Ao and bo
    #----------------------------------------------------------------------
    Ao    = np.matrix([[-1, 0 ] ,  [0, -1] , [1, 0] , [0, 1]])  #matrix with normal vectors
    bo    = np.zeros((4,1))
    theta = orient * np.pi/180                #from degrees to radians
    #note: all normal vectors are pointing away from the rectangle   
    R     = np.matrix([[np.cos(theta), -np.sin(theta)],
                       [np.sin(theta),  np.cos(theta)]]) #rotation matrix
    Ao    = Ao * R.transpose()                         #rotate normal vectors
    
    #Calculate limits in x and y-direction
    bo[0] =   l/2. #min x
    bo[1] =   w/2. #min y
    bo[2] =   l/2. #max x
    bo[3] =   w/2. #max y
    bo    =   bo + Ao * mid
    
    #Calculate vertices
    #----------------------------------------------------------------------
    #Find vertices, starting with lower left corner, moving counter
    #clockwise
    xo         = mid[0,0] - l/2.            
    yo         = mid[1,0] - w/2.
    x_vertices = np.array([xo , xo+l , xo+l , xo])  
    y_vertices = np.array([yo , yo   , yo+w , yo+w])
    
    #Move rectangle to origin
    #x_vertices = x_vertices - mid[0,0]
    #y_vertices = y_vertices - mid[1,0]

    for i in range(4):
        x_vertices[i] = x_vertices[i] - mid[0,0]
        y_vertices[i] = y_vertices[i] - mid[1,0]
    
    #Rotate around origin
    theta      = orient * np.pi/180          #from degrees to radians
    vertices   = np.array([x_vertices , y_vertices])
    vertices   = R * vertices
    
    #Move rectangle back to midpoint
    for i in range(4):
        x_vertices[i] = vertices[0,i] + mid[0,0]
        y_vertices[i] = vertices[1,i] + mid[1,0]

    return Ao, bo, [x_vertices], [y_vertices]


#draw_rectangle
#==============

#This function draws a rectangle with a certain orientation
#Inputs: 
#       #w   = width  [m]
#       #l   = length [m]
#       #mid = midpoint of the rectangle   [m]
#       #orient = orientation of rectangle [rad]
#        
#                      l
#           <-------------------->                      y            
#         ^ #====================#                      ^
#         | #====================#                      |
#        w| #====================#                      |
#         | #====================#                      |
#         = #====================#  orient = 0          |
#                                                       ---------> x
#  
# 
# ----------------------------------------------------------------------    
# ----------------------------------------------------------------------
    
def draw_rectangle(w,l,mid,orient,color):
   
    #Find vertices, starting with lower left corner, moving counter
    #clockwise
    xo         = mid[0,0] - l/2.
    yo         = mid[1,0] - w/2.
    x_vertices = np.array([xo, xo+l, xo+l, xo, xo]) 
    y_vertices = np.array([yo, yo,   yo+w, yo+w, yo])
    
    #Move rectangle to origin
    x_vertices = x_vertices - mid[0,0]
    y_vertices = y_vertices - mid[1,0]
    
    #Rotate around origin
    theta      = orient
    R          = np.matrix([[np.cos(theta), -np.sin(theta)],
                            [np.sin(theta),  np.cos(theta)]]) #rotation matrix
    vertices   = np.array([x_vertices , y_vertices])
    vertices   = R * vertices
    
    #Move rectangle back to midpoint
    x_vertices = vertices[0,:] + mid[0,0]
    y_vertices = vertices[1,:] + mid[1,0]
    
    #Get vertices out of matrices, put in list for plotting
    x_vert = []
    y_vert = []    
    for i in range(0,4,1):
        x_vert.append(x_vertices[0,i])
        y_vert.append(y_vertices[0,i])
    x_vert.append(x_vertices[0,0])
    y_vert.append(y_vertices[0,0])    
    #Plot rectangle
    obstacle_path = plt.plot(x_vert , y_vert , color+'-')
    return obstacle_path    
        
# calc_rectangle4
# ===============

#This function calculates the representation Ax <= b and the corner points of a
#rectangle, given its width, height, mid and orientation

#sin(theta) and cos(theta) follow from the variable r1 = tan(theta/2)
#as sin(theta) = 2*r1/(1+r1^2) and  cos(theta) = (1-r1^2)/(1+r1^2). This
#is called the 'tangent half angle substitution'
    #Inputs: 
        #w          = width  [m]
        #l          = length [m]
        #mid        = midpoint of the rectangle   [m]
        #r1         = tan(theta/2) approximation 
    #Outputs:
        #Ao         = A-matrix of the hyperplane-representation
        #bo         = b-matrix of the hyperplane-representation
        #x_vertices = x-coordinates of  vertices
        #y_vertices = y-coordinates of  vertices
    
#----------------------------------------------------------------------    
#----------------------------------------------------------------------    

def calc_rectangle4(w,l,mid,r1):

#Calculate Ao and bo
#----------------------------------------------------------------------
    Ao    = np.matrix([[-1, 0 ] ,  [0, -1] , [1, 0] , [0, 1]])  #matrix with normal vectors
    bo    = np.zeros((4,1))
    #note: all normal vectors are pointing away from the rectangle   
    
    #In the next line the denominator is ommited, this means that you get
    #R*(1+r1^2) instead of just R. So also bo has to be bo*(1+r1^2). And
    #the x_vert and y_vert you get are actually (1+r1^2)*[x_vert;y_vert]
    sin_approx = 2*r1#/(1+r1^2)      #tangent half angle substitution
    cos_approx = (1-r1*r1)#/(1+r1^2) #tangent half angle substitution    
    R     = np.matrix([[cos_approx, -sin_approx],
                       [sin_approx,  cos_approx]]) #rotation matrix
    Ao    = Ao * R.transpose()             #rotate normal vectors

    #Calculate limits in x and y-direction
    bo[0] =   l/2. #min x
    bo[1] =   w/2. #min y
    bo[2] =   l/2. #max x
    bo[3] =   w/2. #max y
    
#    if isinstance(mid[0][0].coeffs,list):
#       mid1 = BSpline(mid[0][0].basis, mid[0][0].coeffs[1])
#       mid2 = BSpline(mid[1][0].basis, mid[1][0].coeffs[1])
#       mid = [mid1 , mid2]
        
    bo    = (1+r1*r1) * bo + Ao * mid 
    
    #Calculate vertices
    #----------------------------------------------------------------------
    #Find vertices, starting with lower left corner, moving counter
    #clockwise
    xo         = mid[0,0] - l/2      
    yo         = mid[1,0] - w/2
    x_vertices = np.array([xo , xo+l , xo+l , xo])  
    y_vertices = np.array([yo , yo   , yo+w , yo+w])
    
    #Move rectangle to origin
    for i in range(4):  
        x_vertices[i] = x_vertices[i] - mid[0,0]
        y_vertices[i] = y_vertices[i] - mid[1,0]

    #Rotate around origin
    vertices   = np.array([x_vertices , y_vertices])
    vertices   = R * vertices

    a = vertices[1,0]
    b = mid[1,0] * (1+r1*r1)

    x_vertices = {}
    y_vertices = {}

    #Move rectangle back to midpoint
    for i in range(4):  
        x_vertices[i] = vertices[0,i] + mid[0,0] * (1+r1*r1)
        y_vertices[i] = vertices[1,i] + mid[1,0] * (1+r1*r1)
    
    return Ao, bo, [x_vertices], [y_vertices]


# calc_rectangle_vert
# ===================

#This function calculates the data needed to plot and avoid a
#rectangular obstacle. Given the vertices of the corners it calculates
#the representation Ax <= b, the width, height, mid and orientation

#Inputs: 
        #x_limits   = x-coordinates limits
        #y_limits   = y-coordinates limits    
#Outputs:
        #Ao         = A-matrix of the hyperplane-representation
        #bo         = b-matrix of the hyperplane-representation
        #w          = width of rectangle
        #l          = length of rectangle
        #mid        = midpoint of rectangle
        #orient     = orientation of rectangle
    
#----------------------------------------------------------------------    
#----------------------------------------------------------------------    

def calc_rectangle_vert(x_limits, y_limits):
    #Calculate w, l, mid and orient
    #----------------------------------------------------------------------
    xU     = np.max(x_limits)
    ixmax  = np.argmax(x_limits)
    xL     = np.min(x_limits)
    ixmin  = np.argmin(x_limits)
    yU     = np.max(y_limits)
    yL     = np.min(y_limits)
    mid    = np.array([[(xU-xL)/2+xL] , [(yU-yL)/2+yL]])
    l      = np.abs(xU-xL)
    w      = np.abs(yU-yL)
    orient = np.arctan2((y_limits[ixmax] - y_limits[ixmin]),(xU - xL))#in radians
    theta  = orient

    #Compose vertices of corner points
    #----------------------------------------------------------------------
    
    x_vert = [xL , xL , xU , xU] #np.array([xL , xL , xU , xU])
    y_vert = [yL , yU , yL , yU] #np.array([yL , yU , yL , yU])

    Ao    = np.matrix([[-1, 0 ] ,  [0, -1] , [1, 0] , [0, 1]])  #matrix with normal vectors
    bo    = np.zeros((4,1))
    theta = orient * np.pi/180.                #from degrees to radians
    #note: all normal vectors are pointing away from the rectangle   
    R     = np.matrix([[np.cos(theta), -np.sin(theta)],
                       [np.sin(theta),  np.cos(theta)]]) #rotation matrix
    Ao    = Ao * R.transpose()                         #rotate normal vectors
    
    #Calculate limits in x and y-direction
    bo[0] =   l/2. #min x
    bo[1] =   w/2. #min y
    bo[2] =   l/2. #max x
    bo[3] =   w/2. #max y
    bo    =   bo + Ao * mid
    
    return w , l , mid , orient , Ao , bo , x_vert , y_vert


# draw_wheels_bicycle
# ===================

#This function draws a rectangular vehicle (bicycle model) with a certain
#orientation, including the wheels, with a certain steering angle
    
    #Inputs: 
        #w   = width  [m]
        #l   = length [m]
        #mid = midpoint of the rectangle   [m]
        #orient = orientation of rectangle [rad]


def draw_wheels_bicycle(w,l,mid,orient,steer,color):
    
    #Calculate vertices
    #----------------------------------------------------------------------
    #Find vertices, starting with lower left corner, moving counter
    #clockwise
    xo         = mid[0,0] - l/2.            
    yo         = mid[1,0] - w/2.
    x_vertices = np.array([xo , xo+l , xo+l , xo, xo])  
    y_vertices = np.array([yo , yo   , yo+w , yo+w, yo])
    
    #Move rectangle to origin
    x_vertices = x_vertices - mid[0,0]
    y_vertices = y_vertices - mid[1,0]
    
    #--Calculate vertices of wheel
    x_wheel  = np.array([-w/6. , w/6. , w/6. , -w/6. , -w/6.]).reshape(1,5) #width w/6 length w/12
    y_wheel  = np.array([-w/15. , -w/15. , w/15. ,  w/15. , -w/15.]).reshape(1,5)
       
    #Rotate around origin
    theta = orient #from degrees to radians
    R     = np.matrix([[np.cos(theta), -np.sin(theta)],
                       [np.sin(theta),  np.cos(theta)]]) #rotation matrix
    vertices   = np.array([x_vertices , y_vertices])
    vertices   = R * vertices
    
    
    #--Rotate wheel vertices around origin over steering angle
    theta = steer ;
    R     = np.matrix([[np.cos(theta), -np.sin(theta)],
                       [np.sin(theta),  np.cos(theta)]]) #rotation matrix
    wheel = R * np.r_[x_wheel , y_wheel]  
    
    #--Move wheels to front of the rectangle
    wheel1 = np.r_[wheel[0,:] + l/2.5 , wheel[1,:] - 0] #front wheel (steering)
    wheel2 = np.r_[x_wheel - l/2.5 , y_wheel - 0]       #back wheel  (fixed)
    wheel2 = np.asmatrix(wheel2)                        #convert to matrix
    
    #--Rotate wheel vertices around origin over vehicle orientation
    theta = orient 
    R     = np.matrix([[np.cos(theta), -np.sin(theta)],
                       [np.sin(theta),  np.cos(theta)]]) #rotation matrix
    wheel1 = R * np.r_[wheel1[0,:] , wheel1[1,:]]
    wheel2 = R * np.r_[wheel2[0,:] , wheel2[1,:]]

    #Move rectangle back to midpoint
    x_vertices = vertices[0,:] + mid[0,0]
    y_vertices = vertices[1,:] + mid[1,0]
    
    #Move wheels over same distance
    x_wheel1 = wheel1[0,:] + mid[0,0]
    y_wheel1 = wheel1[1,:] + mid[1,0]
    x_wheel2 = wheel2[0,:] + mid[0,0]
    y_wheel2 = wheel2[1,:] + mid[1,0]
    
    #Plot rectangle
    h = []
    if isinstance(x_vertices, BSpline) == False:
        h.append(plt.plot(x_vertices.tolist()[0], y_vertices.tolist()[0], color))
        h.append(plt.plot(x_wheel1.tolist()[0], y_wheel1.tolist()[0], 'k'))
        h.append(plt.plot(x_wheel2.tolist()[0], y_wheel2.tolist()[0], 'k'))
    return h


#draw_wheels_AGV
#===============

#This function draws a rectangular vehicle (bicycle model) with a certain
#orientation, including the wheels, with a certain steering angle

#Inputs: 
    #w   = width  [m]
    #l   = length [m]
    #mid = midpoint of the rectangle   [m]
    #orient = orientation of rectangle [rad]

def draw_wheels_AGV(w,l,mid,orient,steer,color):

    #Calculate vertices
    #----------------------------------------------------------------------
    #Find vertices, starting with lower left corner, moving counter
    #clockwise
    xo         = mid[0,0] - l/2.            
    yo         = mid[1,0] - w/2.
    x_vertices = np.array([xo , xo+l , xo+l , xo, xo])  
    y_vertices = np.array([yo , yo   , yo+w , yo+w, yo])

    #Move rectangle to origin
    x_vertices = x_vertices - mid[0,0]
    y_vertices = y_vertices - mid[1,0]

    #--Calculate vertices of wheel
    x_wheel  = np.array([-w/6. , w/6. , w/6. , -w/6. , -w/6.]).reshape(1,5) #width w/6 height w/12
    y_wheel  = np.array([-w/15. , -w/15. , w/15. ,  w/15. , -w/15.]).reshape(1,5)
       
    #Rotate around origin
    theta = orient #from degrees to radians
    R     = np.matrix([[np.cos(theta), -np.sin(theta)],
                       [np.sin(theta),  np.cos(theta)]]) #rotation matrix
    vertices   = np.array([x_vertices , y_vertices])
    vertices   = R * vertices


    #--Rotate wheel vertices around origin over steering angle
    theta = steer ;
    R     = np.matrix([[np.cos(theta), -np.sin(theta)],
                       [np.sin(theta),  np.cos(theta)]]) #rotation matrix
    wheel = R * np.r_[x_wheel , y_wheel]  

    #--Move wheels to front of the rectangle
    wheel1 = np.r_[wheel[0,:] - l/2.5 , wheel[1,:] - 0] #back wheel (steering)
    wheel2 = np.r_[x_wheel + l/2.5 , y_wheel - 0]       #front wheel (fixed)
    wheel2 = np.asmatrix(wheel2)                        #convert to matrix

    #--Rotate wheel vertices around origin over vehicle orientation
    theta = orient 
    R     = np.matrix([[np.cos(theta), -np.sin(theta)],
                       [np.sin(theta),  np.cos(theta)]]) #rotation matrix
    wheel1 = R * np.r_[wheel1[0,:] , wheel1[1,:]]
    wheel2 = R * np.r_[wheel2[0,:] , wheel2[1,:]]

    #Move rectangle back to midpoint
    x_vertices = vertices[0,:] + mid[0,0]
    y_vertices = vertices[1,:] + mid[1,0]

    #Move wheels over same distance
    x_wheel1 = wheel1[0,:] + mid[0,0]
    y_wheel1 = wheel1[1,:] + mid[1,0]
    x_wheel2 = wheel2[0,:] + mid[0,0]
    y_wheel2 = wheel2[1,:] + mid[1,0]

    #Plot rectangle
    h = []
    if isinstance(x_vertices, BSpline) == False:
        h.append(plt.plot(x_vertices.tolist()[0], y_vertices.tolist()[0], color))
        h.append(plt.plot(x_wheel1.tolist()[0], y_wheel1.tolist()[0], 'k'))
        h.append(plt.plot(x_wheel2.tolist()[0], y_wheel2.tolist()[0], 'k'))
    return h

#calc_rectangle_joint1
#=====================

#This function calculates the movement in time of the corner points of the rectangle
#which represents the first joint of the robot. 
#This function is based on calc_rectangle4.
#But, remark: the input 'mid' here is actually the real position*(1+r1**2), that is why
#there are some modifications needed. The output is actually the real vertices * (1+r1**2)**2

#Inputs: 
    #w   = width  [m]
    #l   = length [m]
    #mid = midpoint of the rectangle   [m] <!!!>real position *(1+r1**2)<!!!>
    #r1 = orientation of rectangle
#Outputs:
    #Ao         = A-matrix of the hyperplane-representation
    #bo         = b-matrix of the hyperplane-representation
    #x_vertices = x-coordinates of  vertices  <!!!>real values *(1+r1**2)**2<!!!>
    #y_vertices = y-coordinates of  vertices  <!!!>real values *(1+r1**2)**2<!!!>

#<!!!>Remark: since the rotation matrix R is actually R*(1+r1**2) and the mid is actually
#the mid*(1+r1**2) the vertices from the output are actually the real vertices * (1+r1**2)**2 <!!!>

def calc_rectangle_joint1(w,l,mid,r1):

#Calculate Ao and bo
#----------------------------------------------------------------------
    Ao    = np.matrix([[-1, 0 ] ,  [0, -1] , [1, 0] , [0, 1]])  #matrix with normal vectors
    b    = np.zeros((4,1))

    #note: all normal vectors are pointing away from the rectangle   
    
    #In the next line the denominator is ommited, this means that you get
    #R*(1+r1^2) instead of just R. So also bo has to be bo*(1+r1^2). And
    #the x_vert and y_vert you get are actually (1+r1^2)*[x_vert;y_vert]
    sin_approx = 2*r1#/(1+r1^2)      #tangent half angle substitution
    cos_approx = (1-r1*r1)#/(1+r1^2) #tangent half angle substitution    
    R     = np.matrix([[cos_approx, -sin_approx],
                       [sin_approx,  cos_approx]]) #rotation matrix
    Ao    = Ao * R.transpose()             #rotate normal vectors

    #Calculate limits in x and y-direction

    b[0] =   l/2. #min x
    b[1] =   w/2. #min y
    b[2] =   l/2. #max x
    b[3] =   w/2. #max y
    
    #    if isinstance(mid[0][0].coeffs,list):
    #       mid1 = BSpline(mid[0][0].basis, mid[0][0].coeffs[1])
    #       mid2 = BSpline(mid[1][0].basis, mid[1][0].coeffs[1])
    #       mid = [mid1 , mid2]

    bo = {}

    for i in range(4):    
        bo[i] = (1+r1*r1)**2 * b[i] + (Ao[i,0] * mid[0,0] + Ao[i,1] * mid[1,0])

    #Calculate vertices
    #----------------------------------------------------------------------
    #Find vertices, starting with lower left corner, moving counter
    #clockwise

    xo         = mid[0,0] - l/2*(1+r1**2)      
    yo         = mid[1,0] - w/2*(1+r1**2)
    x_vertices = np.array([xo , xo+l*(1+r1**2) , xo+l*(1+r1**2) , xo])  
    y_vertices = np.array([yo , yo   , yo+w*(1+r1**2) , yo+w*(1+r1**2)])
    
    #Move rectangle to origin
    for i in range(4):  
        x_vertices[i] = x_vertices[i] - mid[0,0]
        y_vertices[i] = y_vertices[i] - mid[1,0]
    
    #Rotate around origin
    vertices   = np.array([x_vertices , y_vertices])
    vertices   = R * vertices
    
    #Move rectangle back to midpoint
    for i in range(4):  
        x_vertices[i] = vertices[0,i] + (1+r1*r1) * mid[0,0]
        y_vertices[i] = vertices[1,i] + (1+r1*r1) * mid[1,0]  
    
    return Ao, bo, [x_vertices], [y_vertices]




#draw_quadrotor
#==============

#This function draws a quadrotor with a certain orientation

#Inputs: 
    #mid = midpoint of the quadrotor [x,y]  [m]
    #l   = total length, including propellers [m]
    #orient = orientation of the quadrotor [rad]

    #             ^ F
    #   ##        |         ##
    #   ##        |         ##
    #   ======================
    #   <-------------------->
    #             l

def draw_quadrotor(mid,orient,l,color):

    #Calculate corner points and propeller vertices in world frame (0,0), orient=0
    #-----------------------------------------------------------------------------

    corner_l = np.array([-l/2,0]) #left end of base line
    corner_r = np.array([+l/2,0]) #right end of base line

    # corner_l = np.array([mid[0]-l/2,mid[1]]) #left end of base line
    # corner_r = np.array([mid[0]+l/2,mid[1]]) #right end of base line

    w = l/8 #propeller size, length of a side
    prop_l = np.array([corner_l,[corner_l[0],corner_l[1] + w], [corner_l[0] + w,corner_l[1] + w], [corner_l[0] + w,corner_l[1]]]).reshape(2,4)  #left propeller coordinates
    prop_r = np.array([corner_r,[corner_r[0],corner_r[1] + w], [corner_r[0] - w,corner_r[1] + w], [corner_r[0] - w,corner_r[1]]]).reshape(2,4)  #left propeller coordinates

    #Rotate standard quadrotor around origin
    #-----------------------------------------------------------------------    


    # print "prop_l: " + str(prop_l)
    # print "prop_r: " + str(prop_r)


    theta = orient #from degrees to radians
    R     = np.matrix([[np.cos(theta), -np.sin(theta)],
                       [np.sin(theta),  np.cos(theta)]]) #rotation matrix

    corner_l = R * corner_l.reshape(2,1) 
    corner_r = R * corner_r.reshape(2,1)

    corner_l = corner_l.tolist()
    corner_r = corner_r.tolist()

    prop_l = R * prop_l
    prop_r = R * prop_r  

    prop_l = prop_l.tolist()
    prop_r = prop_r.tolist()

    
    # y_wheel  = np.array([-w/15. , -w/15. , w/15. ,  w/15. , -w/15.]).reshape(1,5)  
    # vertices   = np.array([x_vertices , y_vertices])
    # vertices   = R * vertices

    #Plot quadrotor
    h = []
    if isinstance(all(mid), BSpline) == False:
        h.append(plt.plot([corner_l[0]+mid[0], corner_r[0]+mid[0]], [corner_l[1]+mid[1], corner_r[1]+mid[1]], color))
        # h.append(plt.plot(prop_l[0] , prop_l[1], color))
        # h.append(plt.plot(prop_r[0] , prop_r[1], color))

        # h.append(plt.plot(x_wheel2.tolist()[0], y_wheel2.tolist()[0], 'k'))
    return h

# calc_rectangle3
# ===============

#This function calculates the representation Ax <= b and the corner points of a
#rectangle, given its width, height, mid and orientation

#sin(theta) and cos(theta) follow from the variable r1 = sin(theta) and
#r2 = cos(theta)
    #Inputs: 
        #w          = width  [m]
        #l          = length [m]
        #mid        = midpoint of the rectangle   [m]
        #r1         = sin(theta) approximation 
        #r2         = cos(theta) approximation 
    #Outputs:
        #Ao         = A-matrix of the hyperplane-representation
        #bo         = b-matrix of the hyperplane-representation
        #x_vertices = x-coordinates of  vertices
        #y_vertices = y-coordinates of  vertices
    
#----------------------------------------------------------------------    
#----------------------------------------------------------------------    

def calc_rectangle3(w,l,mid,r1,r2):

#Calculate Ao and bo
#----------------------------------------------------------------------
    Ao    = np.matrix([[-1, 0 ] ,  [0, -1] , [1, 0] , [0, 1]])  #matrix with normal vectors
    bo    = np.zeros((4,1))
    #note: all normal vectors are pointing away from the rectangle   
     
    R     = np.matrix([[r2, -r1],
                       [r1,  r2]]) #rotation matrix
    Ao    = np.dot(Ao , R.transpose())             #rotate normal vectors

    #Calculate limits in x and y-direction
    bo[0] =   l/2. #min x
    bo[1] =   w/2. #min y
    bo[2] =   l/2. #max x
    bo[3] =   w/2. #max y
    
#    if isinstance(mid[0][0].coeffs,list):
#       mid1 = BSpline(mid[0][0].basis, mid[0][0].coeffs[1])
#       mid2 = BSpline(mid[1][0].basis, mid[1][0].coeffs[1])
#       mid = [mid1 , mid2]
        
    bo    = bo + Ao * mid
    
    #Calculate vertices
    #----------------------------------------------------------------------
    #Find vertices, starting with lower left corner, moving counter
    #clockwise
    xo         = mid[0,0] - l/2      
    yo         = mid[1,0] - w/2
    x_vertices = np.array([xo , xo+l , xo+l , xo])  
    y_vertices = np.array([yo , yo   , yo+w , yo+w])
    
    #Move rectangle to origin
    for i in range(4):
        x_vertices[i] = x_vertices[i] - mid[0,0]
        y_vertices[i] = y_vertices[i] - mid[1,0]
    
    #Rotate around origin
    vertices   = np.array([x_vertices , y_vertices])
    vertices   = R * vertices
    
    #Move rectangle back to midpoint
    for i in range(4):
        x_vertices[i] = vertices[0,i] + mid[0,0]
        y_vertices[i] = vertices[1,i] + mid[1,0]
    
    return Ao, bo, [x_vertices], [y_vertices]