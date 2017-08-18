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

x_yalmip = sdpvar(2,2,'full')
y_yalmip = sdpvar(2,2,'full')

con = 3<=y_yalmip<=4;
sol = optimize([con],trace((x_yalmip-1).^2+(y_yalmip-2).^2));
dual_yalmip = dual(con)


x_yalmip = sdpvar();
y_yalmip = sdpvar();

f_yalmip = (x_yalmip-1)^2+(y_yalmip-2)^2;

eps = 1e-5;
fun = @(x,y) { 
          y>=2.5;
          y>=1.5;
          2.5>=y;
          1.5>=y;
          y<=2.5;
          y<=1.5;
          2.5<=y;
          1.5<=y;
          y>=x;
          y<=x;
          y<=0;
          y==1.5;
          y==x;
          3<= y <=4;
          0<= y <=1;
          4>= y >=3;
          1>= y >=0;
          3<= x<= y<= 4;
          3<= y<= x<= 4;
          4>= y>= x>= 3;
          4>= x>= y>= 3;};

opti = OptiSpline();

x = opti.var();
y = opti.var();
f = (x-1)^2+(y-2)^2;

tests_yalmip = fun(x_yalmip, y_yalmip);
tests = fun(x, y);

for i = 1:size(tests,1)
  con = tests_yalmip{i};
  
  sol = optimize(con,f_yalmip);
  dual_yalmip = dual(con);
  xs_yalmip = value(x_yalmip);
  ys_yalmip = value(y_yalmip);
  
  con = tests{i};
  
  sol = opti.solver(f,{con},'ipopt');
  sol.solve();
  assert(norm(sol.value(x)-xs_yalmip)<1e-3);
  assert(norm(sol.value(y)-ys_yalmip)<1e-3);
  dual_1 = full(sol.dual(con)');
  assert(norm(dual_1(:)-dual_yalmip)<1e-3);  
end

x_yalmip = sdpvar(3,1,'full');
y_yalmip = sdpvar(3,1,'full');

s = [0.9;1.1;1];

f_yalmip = sum((x_yalmip-1).^2+((y_yalmip-2).*s).^2);

eps = 1e-5;

opti = OptiSpline();

x = opti.var(3,1);
y = opti.var(3,1);

f = sum((x-1).^2+((y-2).*s).^2);

tests_yalmip = fun(x_yalmip, y_yalmip);
tests = fun(x, y);

for i = 1:size(tests,1)
  con = tests_yalmip{i};
  
  sol = optimize(con,f_yalmip);
  dual_yalmip = dual(con);
  xs_yalmip = value(x_yalmip);
  ys_yalmip = value(y_yalmip);
  
  con = tests{i};
  
  sol = opti.solver(f,{con},'ipopt');
  sol.solve();
  assert(norm(sol.value(x)-xs_yalmip)<1e-3);
  assert(norm(sol.value(y)-ys_yalmip)<1e-3);
  dual_1 = full(sol.dual(con)');
  assert(norm(dual_1(:)-dual_yalmip(:))<1e-3);  

end

x_yalmip = sdpvar(1,3,'full');
y_yalmip = sdpvar(1,3,'full');

s = [0.9;1.1;1]';

f_yalmip = sum((x_yalmip-1).^2+((y_yalmip-2).*s).^2);

eps = 1e-5;

opti = OptiSpline();

x = opti.var(1,3);
y = opti.var(1,3);

f = sum((x-1).^2+((y-2).*s).^2);

tests_yalmip = fun(x_yalmip, y_yalmip);
tests = fun(x, y);

for i = 1:size(tests,1)
  con = tests_yalmip{i};
  
  sol = optimize(con,f_yalmip);
  dual_yalmip = dual(con);
  xs_yalmip = value(x_yalmip);
  ys_yalmip = value(y_yalmip);
  
  con = tests{i};
  
  sol = opti.solver(f,{con},'ipopt');
  sol.solve();
  assert(norm(sol.value(x)-xs_yalmip)<1e-3);
  assert(norm(sol.value(y)-ys_yalmip)<1e-3);
  dual_1 = full(sol.dual(con)');
  assert(norm(dual_1(:)-dual_yalmip(:))<1e-3);  

end

x_yalmip = sdpvar(3,2,'full');
y_yalmip = sdpvar(3,2,'full');

s = [0.9 0.7;1.1 0.9;1 1];

f_yalmip = sum(sum((x_yalmip-1).^2+((y_yalmip-2).*s).^2));

eps = 1e-5;

opti = OptiSpline();

x = opti.var(3,2);
y = opti.var(3,2);

f = sum(sum((x-1).^2+((y-2).*s).^2));

fun = @(x,y) { 
          y(:)>=2.5;
          y(:)>=1.5;
          2.5>=y(:);
          1.5>=y(:);
          y(:)<=2.5;
          y(:)<=1.5;
          2.5<=y(:);
          1.5<=y(:);
          y(:)>=x(:);
          y(:)<=x(:);
          y(:)<=0;
          y==1.5;
          y==x;
          3<= y(:) <=4;
          0<= y(:) <=1;
          4>= y(:) >=3;
          1>= y(:) >=0;
          3<= x(:)<= y(:)<= 4;
          3<= y(:)<= x(:)<= 4;
          4>= y(:)>= x(:)>= 3;
          4>= x(:)>= y(:)>= 3;};

tests_yalmip = fun(x_yalmip, y_yalmip);
tests = fun(x, y);

for i = 1:size(tests,1)
  con = tests_yalmip{i};
  
  sol = optimize(con,f_yalmip);
  dual_yalmip = dual(con);
  xs_yalmip = value(x_yalmip);
  ys_yalmip = value(y_yalmip);
  
  con = tests{i};
  
  sol = opti.solver(f,{con},'ipopt');
  sol.solve();
  assert(norm(sol.value(x)-xs_yalmip)<1e-3);
  assert(norm(sol.value(y)-ys_yalmip)<1e-3);
  dual_1 = full(sol.dual(con)');
  assert(norm(dual_1(:)-dual_yalmip(:))<1e-3);  

end
