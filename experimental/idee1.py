import meco_binaries;
meco_binaries(cpp_splines='develop')

from numpy.linalg import solve
from numpy import ndarray

from splines import *

import time
add = lambda x,y : x + y
times =  lambda x,y : x * y

x = Parameter('x')
y = Parameter('y')

norm2 = [add, [times, x, x], [times, y, y]]

test_call = lambda operation, list_ : operation(*list_)
test_call2 = lambda operation, list_ : sum([i**2 for i in list_])
b= [add , [times, 4, 2] , -3]

def complex_operation(tree):
    call_basis = lambda operation, list_ : operation(*[f.tensor_basis() if isinstance(f, Function) else f for f in list_])
    bases_functions = recursief_(tree, call_basis)
    grid = bases_functions.evaluation_grid()
    GRID = [DTensor(g, [g.size]) for g in grid]
    evaluation_basis = bases_functions.grid_eval(GRID)

    args = bases_functions.arguments()
    call_function = lambda operation, list_ : operation(*[f.grid_eval(GRID, args) if isinstance(f, Function) else f for f in list_])
    evaluation_function = recursief_(tree, call_function)

    dim = evaluation_function.size
    A = evaluation_basis.reshape(dim, dim)
    B = evaluation_function.reshape(dim)

    C = solve(A,B)

    dims_coef = list( bases_functions.dimension() ) + list( evaluation_function.shape[  -2: ])
    return Function(bases_functions, C.reshape(dims_coef))

def brut_force(tree):
    call_basis = lambda operation, list_ : operation(*[f for f in list_])
    return recursief_(tree, call_basis)

def recursief_(tree, call_):
    if(type(tree) == list):
        return call_(tree[0], [recursief_(b, call_) for b in tree[1:]])
    return tree





start_time = time.time()
brut_force(norm2)
print("--- %s seconds ---" % (time.time() - start_time))

start_time = time.time()
complex_operation(norm2)
print("--- %s seconds ---" % (time.time() - start_time))

opti = OptiSpline()

m = MonomialBasis(4)
x_ = Polynomial([2,3,4],'x_')
y_ = Polynomial([1,3,4],'y_')
z_ = Polynomial([1,2,4],'z_')
t_ = Polynomial([1,2,3],'t_')

test_compositie1 = [times,[add, [times, x_, x_], [times, y_, y_]], [add, [times, x_, x_], [times, y_, y_]]]
test_compositie2 = [times,[add, [times, z_, z_], [times, t_, t_]], [add, [times, z_, z_], [times, t_, t_]]]
test_compositie = [add, test_compositie1, test_compositie2]

start_time = time.time()
brut_force(test_compositie)
print("--- %s seconds ---" % (time.time() - start_time))

start_time = time.time()
complex_operation(test_compositie)
print("--- %s seconds ---" % (time.time() - start_time))

