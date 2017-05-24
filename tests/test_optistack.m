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

x = opti.var();
sol = opti.solver((1-x)^2,{},'yalmip');
sol.solve();
assert(norm(sol.value(x)-1)<1e-5)

yalmip_options = sdpsettings('solver','quadprog','verbose',2);

x = opti.var();
p = opti.par();
sol = opti.solver((p-x)^2,{},'yalmip',struct('yalmip_options',yalmip_options));
sol.value(p,3)
sol.solve();

sol.value(x)

assert(norm(sol.value(x)-3)<1e-5)
sol.value(p,2)
sol.solve();

assert(norm(sol.value(x)-2)<1e-5)


sol = opti.solver((p-x)^2,{x<=0},'yalmip',struct('yalmip_options',yalmip_options));
sol.value(p,3)
sol.solve();

sol.value(x)

assert(norm(sol.value(x))<1e-5)
sol.value(p,-2)
sol.solve();

assert(norm(sol.value(x)+2)<1e-5)


b = MonomialBasis(4);
F = opti.Function(b,[4,4]);
F = opti.Function(b,[4,4],'full');
F = opti.Function(b,[4,4],'symmetric');

opti = OptiSplineYalmip();



x = opti.var();
y = opti.var(4,4);
z = opti.var(1,3);

123
sol = opti.solver(x+trace(y)+z(1),{y>=3},'yalmip')
sol.solve()


