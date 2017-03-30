meco_binaries('cpp_splines','fill_in_the_branch_you_want')

import splines.*

opti = OptiSpline();


x = opti.var();
y = opti.var();
z = opti.var();

sol = opti.solver((x-1)^2+(y-2)^2+(z-3)^2,{},'ipopt')
sol.solve()
assert(norm(sol.value(x)-1)<1e-7)
assert(norm(sol.value(y)-2)<1e-7)
assert(norm(sol.value(z)-3)<1e-7)

sol = opti.solver((x-1)^2+(y-2)^2+(z-3)^2,{z<=y, y<=x},'ipopt')
sol.solve()
assert(norm(sol.value(x)-2)<1e-7)
assert(norm(sol.value(y)-2)<1e-7)
assert(norm(sol.value(z)-2)<1e-7)

sol = opti.solver((x-1)^2+(y-2)^2+(z-3)^2,{z<=y<=x},'ipopt')
sol.solve()
assert(norm(sol.value(x)-2)<1e-7)
assert(norm(sol.value(y)-2)<1e-7)
assert(norm(sol.value(z)-2)<1e-7)

sol = opti.solver((x-1)^2+(y-2)^2+(z-3)^2,{z>=y>=x},'ipopt')
sol.solve()
assert(norm(sol.value(x)-1)<1e-4)
assert(norm(sol.value(y)-2)<1e-4)
assert(norm(sol.value(z)-3)<1e-4)

opti = OptiSplineYalmip();
P = opti.var(2,2,'symmetric');

T = 2*eye(2);

sol = opti.solver(trace(P),{P>=T},'yalmip')
sol.solve()

assert(norm(sol.value(P)-T)<1e-2)

sol = opti.solver(-trace(P),{P<=T},'yalmip')
sol.solve()

assert(norm(sol.value(P)-T)<1e-2)


sol = opti.solver(trace(P),{3*eye(2)>=P>=T},'yalmip')
sol.solve()

assert(norm(sol.value(P)-T)<1e-2)


sol = opti.solver(-trace(P),{3*eye(2)>=P>=T},'yalmip')
sol.solve()

assert(norm(sol.value(P)-3*eye(2))<1e-2)

sol = opti.solver(trace(P),{T<=P<=3*eye(2)},'yalmip')
sol.solve()

assert(norm(sol.value(P)-T)<1e-2)


sol = opti.solver(-trace(P),{T<=P<=3*eye(2)},'yalmip')
sol.solve()

assert(norm(sol.value(P)-3*eye(2))<1e-2)


sol = opti.solver(trace(P),{-P<=-T},'yalmip')
sol.solve()

assert(norm(sol.value(P)-T)<1e-2)

