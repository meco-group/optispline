meco_binaries('cpp_splines','fill_in_the_branch_you_want')

import splines.*



opti = OptiSpline();


x = opti.variable();
y = opti.variable();

p = opti.parameter();

opti.solver('ipopt');
opti.minimize((x-1)^4+(y-p)^4)


A = [];

opti.set_value(p, 3);
sol = opti.solve();

opti.debug.value(p)

d = opti.debug
d.value(p)
sol.value(p)

opti.callback(@() evalin('base',['A=' num2str(opti.debug.value(p)) ';']));
opti.set_value(p, 3);
sol = opti.solve();
disp(A)
assert(A==3);
opti.set_value(p, 2);
sol = opti.solve();
assert(A==2);
opti.set_value(p, 3);
sol = opti.solve();
assert(A==3);

A = [];
opti.callback();
sol = opti.solve();
assert(isempty(A));

B = [];

opti.callback(@() evalin('base',['B=' num2str(opti.debug.value(p)) ';']));
sol = opti.solve();
assert(B==3);

opti = OptiSpline();




x = opti.variable();
y = opti.variable();
z = opti.variable();

opti.minimize((x-1)^2+(y-2)^2+(z-3)^2)
opti.solver('ipopt')
sol = opti.solve();
assert(norm(sol.value(x)-1)<1e-7)
assert(norm(sol.value(y)-2)<1e-7)
assert(norm(sol.value(z)-3)<1e-7)

opti.subject_to()
opti.subject_to({z<=y, y<=x})
sol = opti.solve();

assert(norm(sol.value(x)-2)<1e-7)
assert(norm(sol.value(y)-2)<1e-7)
assert(norm(sol.value(z)-2)<1e-7)

opti.subject_to()
opti.subject_to({z<=y<=x})
sol = opti.solve();

assert(norm(sol.value(x)-2)<1e-7)
assert(norm(sol.value(y)-2)<1e-7)
assert(norm(sol.value(z)-2)<1e-7)

opti.subject_to()
opti.subject_to({z>=y>=x})
sol = opti.solve();

assert(norm(sol.value(x)-1)<1e-4)
assert(norm(sol.value(y)-2)<1e-4)
assert(norm(sol.value(z)-3)<1e-4)

opti = OptiSplineYalmip();
P = opti.variable(2,2,'symmetric');

T = 2*eye(2);


opti.minimize(trace(P));
opti.subject_to(P>=T);
sol = opti.solve();

assert(norm(sol.value(P)-T)<1e-2);

opti.subject_to();
opti.minimize(-trace(P));
opti.subject_to(P<=T);
sol = opti.solve();

assert(norm(sol.value(P)-T)<1e-2);

opti.subject_to();
opti.minimize(trace(P));
opti.subject_to(3*eye(2)>=P>=T);
sol = opti.solve();

assert(norm(sol.value(P)-T)<1e-2);

opti.subject_to();
opti.minimize(-trace(P));
opti.subject_to(3*eye(2)>=P>=T);
sol = opti.solve();

assert(norm(sol.value(P)-3*eye(2))<1e-2);

opti.subject_to();
opti.minimize(trace(P));
opti.subject_to(T<=P<=3*eye(2));
sol = opti.solve();

assert(norm(sol.value(P)-T)<1e-2);

opti.subject_to();
opti.minimize(-trace(P));
opti.subject_to(T<=P<=3*eye(2));
sol = opti.solve();

assert(norm(sol.value(P)-3*eye(2))<1e-2)

opti.subject_to();
opti.minimize(trace(P));
opti.subject_to(-P<=-T);
sol = opti.solve();

assert(norm(sol.value(P)-T)<1e-2)

x = opti.variable();

opti.subject_to()
opti.minimize((1-x)^2)
sol = opti.solve()

sol.value(x)-1
assert(norm(sol.value(x)-1)<1e-4)

yalmip_options = sdpsettings('solver','quadprog','verbose',2);

x = opti.variable();
p = opti.parameter();

opti.subject_to();
opti.minimize((p-x)^2);
opti.solver('yalmip',struct('yalmip_options',yalmip_options));
opti.set_value(p, 3)
sol = opti.solve()

sol.value(x)

assert(norm(sol.value(x)-3)<1e-5)
opti.set_value(p,2)
sol = opti.solve();

assert(norm(sol.value(x)-2)<1e-5)


opti.subject_to();
opti.minimize((p-x)^2);
opti.subject_to(x<=0);
opti.solver('yalmip',struct('yalmip_options',yalmip_options));
opti.set_value(p, 3);

sol = opti.solve();

sol.value(x)

assert(norm(sol.value(x))<1e-5)
opti.set_value(p,-2);
sol = opti.solve();

norm(sol.value(x)+2)
sol.value(x)
assert(norm(sol.value(x)+2)<1e-4)


b = MonomialBasis(4);
F = opti.Function(b,[4,4]);
F = opti.Function(b,[4,4],'full');
F = opti.Function(b,[4,4],'symmetric');

opti = OptiSplineYalmip();



x = opti.variable();
y = opti.variable(4,4);
z = opti.variable(1,3);

opti.minimize(x+trace(y)+z(1))
opti.subject_to(y>=3)
sol = opti.solve()

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

x = opti.variable();
y = opti.variable();
f = (x-1)^2+(y-2)^2;
opti.minimize(f);

opti.solver('ipopt')

tests_yalmip = fun(x_yalmip, y_yalmip);
tests = fun(x, y);


for i = 1:size(tests,1)
  con = tests_yalmip{i};
  
  sol = optimize(con,f_yalmip);
  dual_yalmip = dual(con);
  xs_yalmip = value(x_yalmip);
  ys_yalmip = value(y_yalmip);
  
  con = tests{i};
  
  opti.subject_to();
  opti.subject_to(con);
  sol = opti.solve();
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

x = opti.variable(3,1);
y = opti.variable(3,1);

f = sum((x-1).^2+((y-2).*s).^2);
opti.minimize(f);
opti.solver('ipopt');

tests_yalmip = fun(x_yalmip, y_yalmip);
tests = fun(x, y);

for i = 1:size(tests,1)
  con = tests_yalmip{i};
  
  sol = optimize(con,f_yalmip);
  dual_yalmip = dual(con);
  xs_yalmip = value(x_yalmip);
  ys_yalmip = value(y_yalmip);
  
  con = tests{i};
  
  opti.subject_to();
  opti.subject_to(con);
  sol = opti.solve();
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

x = opti.variable(1,3);
y = opti.variable(1,3);

opti.solver('ipopt');
f = sum((x-1).^2+((y-2).*s).^2);
opti.minimize(f);

tests_yalmip = fun(x_yalmip, y_yalmip);
tests = fun(x, y);

for i = 1:size(tests,1)
  con = tests_yalmip{i};
  
  sol = optimize(con,f_yalmip);
  dual_yalmip = dual(con);
  xs_yalmip = value(x_yalmip);
  ys_yalmip = value(y_yalmip);
  
  con = tests{i};
  
  opti.subject_to();
  opti.subject_to(con);
  sol = opti.solve();
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

x = opti.variable(3,2);
y = opti.variable(3,2);
opti.solver('ipopt')
f = sum(sum((x-1).^2+((y-2).*s).^2));
opti.minimize(f);

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
  
  opti.subject_to();
  opti.subject_to(con);
  sol = opti.solve();
  assert(norm(sol.value(x)-xs_yalmip)<1e-3);
  assert(norm(sol.value(y)-ys_yalmip)<1e-3);
  dual_1 = full(sol.dual(con)');
  assert(norm(dual_1(:)-dual_yalmip(:))<1e-3);  

end


disp('the end')
