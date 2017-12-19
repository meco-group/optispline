import splines.*

%% 1D spline basis
b1 = BSplineBasis([0,1],3,4)

%% Knots of spline
knots(b1)

%% Plot with random coefficients
rng(2);
dimension(b1)
c = rand(6,1);

f = Function(b1,c)

f.eval(0)
f.eval(0.1)


%%

xf = linspace(0,1,200);

figure(1);clf;
hold on
plot(xf,f.list_eval(xf));

f.integral
%% spline derivatives

figure(2);clf;
subplot(4,1,1)
plot(xf,f.list_eval(xf));

fd = f.derivative();
subplot(4,1,2)
plot(xf,fd.list_eval(xf));

fdd = fd.derivative();
subplot(4,1,3)
plot(xf,fdd.list_eval(xf));

fddd = fdd.derivative();
subplot(4,1,4)
plot(xf,fddd.list_eval(xf));
%%

figure(1);
plot(b1.greville,c,'o--','linewidth',2);
for k=b1.knots
   plot([k k],[0,0.6],'k--')
end


%% 2D spline

b2 = BSplineBasis([0,2],2,3);


t1 = TensorBasis({b1,b2})

rng(0);
t1.dimension
c = rand(t1.dimension);
f = Function({b1,b2},c)
%%
gx = linspace(0,1,20);
gy = linspace(0,2,20);
[X,Y] = ndgrid(gx,gy);

Z = f.grid_eval({gx gy});

figure(3);clf;
surf(X,Y,Z)


%% Symbolic coefficients
f = Function(b1,rand(6,1));

y1 = f.eval(0)
y2 = f.eval(0.1)

jacobian(y2,c)

%% Evaluation at symbolic location

x = casadi.SX.sym('x');
y3 = f.eval(x)

%% Symbolic knots

u = casadi.MX.sym('u');
c = casadi.MX.sym('c',6,1);
x = casadi.MX.sym('x');
b3 = BSplineBasis([0,u],3,4);
f = Function(b3,c);

f.eval(x)
f.derivative()
f.integral()

%% Intermezzo: CasADi's opti (1)

opti = casadi.Opti();

a = 1;
b = 5;
x = opti.variable();
y = opti.variable();

opti.minimize((a-x)^2+b*(y-x^2)^2);
opti.solver('ipopt');

sol = opti.solve();

sol.value(x),sol.value(y)


%% Fitting 1D

N = 10;
rng(0);
x = linspace(0,100,N);
y = rand(N,1);

figure(4);clf;
plot(x,y,'o')

%%

b = BSplineBasis([0,100],4,N);

opti = OptiSpline();
F = opti.Function(b);

e = F.list_eval(x)-y;

opti.minimize(e'*e);
opti.solver('ipopt');

sol = opti.solve();

Fsol = sol.value(F);
%%

figure(5);clf;
hold on
plot(x,y,'o')

xf = linspace(0,100,1000);
plot(xf,Fsol.list_eval(xf))


%% Alternative fit: Hessian

opti = OptiSpline();
F = opti.Function(b);

opti.subject_to(F.list_eval(x)==y);

H = F.hessian_matrix;
opti.minimize(integral(H^2));
opti.solver('ipopt');

sol = opti.solve();

Fsol = sol.value(F);

% plotting
figure(6);clf;
hold on
plot(x,y,'o')

xf = linspace(0,100,1000);
plot(xf,Fsol.list_eval(xf))

%% Alternative fit: above linear, 2-norm


opti = OptiSpline();
F = opti.Function(b);
Flin = Function.linear(x, y);
e = F-Flin;

opti.subject_to(e>=0);
opti.minimize(integral(e^2));

opti.set_initial(coeff_tensor(F),rand(F.basis.dimension,1));

opti.solver('ipopt');

sol = opti.solve();

Fsol = sol.value(F);

% plotting
figure(7);clf
hold on
plot(x,y,'o-')

xf = linspace(0,100,1000);
plot(xf,Fsol.list_eval(xf))


%% Alternative fit: above linear, inf-norm


opti = OptiSpline();
F = opti.Function(b);
Flin = Function.linear(x, y);
e = F-Flin;

L = opti.variable();
opti.subject_to(0<=e<=L);
opti.minimize(L);

opti.set_initial(coeff_tensor(F),rand(F.basis.dimension,1));

opti.solver('ipopt');

sol = opti.solve();

Fsol = sol.value(F);

% plotting
figure(7)
plot(xf,Fsol.list_eval(xf))

legend('data','2-norm','inf-norm')

%% Steering a point mass around an obstacle

opti = OptiSpline();

N = 4;
N = 10;

basis  = BSplineBasis([0,1], 0, N);
U      = opti.Function(basis,[2,1]);

acc = U;
vel = acc.antiderivative();
pos = vel.antiderivative();

opti.minimize(integral(U'*U));

opti.subject_to(pos.eval(1)==[2;1]);
opti.subject_to(vel.eval(1)==0);

c = [1;0.6];
r = 0.4;
opti.subject_to((pos-c)'*(pos-c)>=r^2);

opti.solver('ipopt');

x = Function.linear([0,1],[0,1]);
y = -Function.linear([0,1],[0,1]);
opti.set_initial(U, [x;y]);
sol = opti.solve();

ts = linspace(0,1,1000);

vel_sol = sol.value(vel);
pos_sol = sol.value(pos);
U_sol = sol.value(U);

% plotting
figure(8);clf;
hold on
ptraj = pos_sol.list_eval(ts);
plot(ptraj(:,1),ptraj(:,2));

t=linspace(0,2*pi,100);
plot(c(1)+r*cos(t),c(2)+r*sin(t),'r');
axis equal

figure(9);clf;
subplot(2,1,1)
hold on
plot(ts,vel_sol.list_eval(ts))
plot(ts,pos_sol.list_eval(ts))
subplot(2,1,2)
plot(ts,U_sol.list_eval(ts),'k')



%% parametric problem (Opti)

opti = casadi.Opti();

a = opti.parameter();
b = 5;

x = opti.variable();
y = opti.variable();

obj = (a-x)^2+b*(y-x^2)^2;

opti.minimize(obj);
opts = struct;
opts.print_time = false;
opts.ipopt.print_level = 0;
opti.solver('ipopt',opts);

figure(10);clf
hold on
as = linspace(0,2,10);
for av=as
    opti.set_value(a, av)
    sol = opti.solve();
    plot(sol.value(x),sol.value(y),'o')
end

%% parametric problem (OptiSpline)


af = linspace(0,2,1000);
ba = BSplineBasis([0,2],2,3);

opti = OptiSpline();

a = splines.Function.linear([0,2],[0,2]);

x = opti.Function(ba);
y = opti.Function(ba);

obj = (a-x)^2+b*(y-x^2)^2;

opti.minimize(obj.integral());

opti.solver('ipopt');

sol = opti.solve();

xsol = sol.value(x);
ysol = sol.value(y);

figure(10);

plot(xsol.list_eval(af),ysol.list_eval(af))

%% Yalmip example

opti = OptiSplineYalmip();
% LPD model
aL = 1;
aU = 2;

a = Function.linear([aL,aU],[aL,aU]);

A =  [0 1;-a*a -2*a];
B = [0; 1];
C = [0, 1];
D = [0];
nx = 2;

% Hinf norms by gridding
a_ = linspace(aL, aU, 1e2)';

gamma_grid = zeros(size(a_));
for i = 1:length(a_)
    sys = ss(A.eval(a_(i)), B, C, D);
    gamma_grid(i) = norm(sys, inf);
end

BP = BSplineBasis([aL,aU],2,3);
Bg = BSplineBasis([aL,aU],2,3);

P = opti.Function(BP, [nx,nx],'symmetric');
gamma = opti.Function(Bg);

Z = [A'*P+P*A, P*B   , C'    ;
     B'*P    , -gamma, D'    ;
     C       , D     , -gamma];

opti.minimize(gamma.integral);
opti.subject_to(Z<=0);
opti.subject_to(P>=0);

opti.solver('yalmip')

sol = opti.solve();

gamma_prim = sol.value(gamma);

% Overview figure
figure(11);clf
plot(a_, [gamma_grid, gamma_prim.list_eval(a_)])
xlabel('a'), ylabel('gamma')
legend('gamma_grid','primal')





%% Robust NLP (Opti)

opti = casadi.Opti();

x = opti.variable();
y = opti.variable();

opti.minimize((x-4)^2+y^2);

for a=linspace(-1,1,1000)
  opti.subject_to((a^2-x)^2+(y-a-0.5)^2<=3);
end
opti.solver('ipopt');

sol = opti.solve();

xy_ref = sol.value([x;y])

%% Robust NLP (OptiSpline)

opti = OptiSpline();

af = linspace(0,2,1000);
a = splines.Function.linear([-1,1],[-1,1]);

a = a.degree_elevation(2);
a = a.midpoint_refinement(4);

x = opti.variable();
y = opti.variable();

opti.minimize((x-4)^2+y^2);

opti.subject_to((a^2-x)^2+(y-a-0.5)^2<=3);

opti.solver('ipopt');

sol = opti.solve();

sol.value([x;y])
xy_ref

%% Global optimization






%%

% min_x max_w f(x,w)
%  st g(x,w)<= 0

opti = OptiSpline();

x = opti.variable();
w = splines.Function.linear([1,2],[1,2]);
f = (x*w-w)^4;
g = 2*x^2-w;

e = opti.variable();
opti.minimize(e);
opti.subject_to(f<=e);
opti.subject_to(g<=0);
opti.solver('ipopt');

sol = opti.solve();

%% Van der pol

t0 = 0;
tf = 1;
f = @(x) [(1-x(2))*x(1)-x(2);x(1)];

x0 = [1;0];

options = odeset('RelTol',1e-13);
[~,y] = ode45(@(t,x) f(x),[t0,tf],x0,options);


Ns = [2,4,8,16,32];
res_N = zeros(length(Ns),1);
d = 3;
for Ni=1:length(Ns)
    N = Ns(Ni);
    b = BSplineBasis([t0,tf],d,N);
    opti = OptiSpline();
    c= opti.variable(2,b.dimension-1);

    x = Function(b,[x0 c]');

    e = x.derivative-f(x);

    for i=2:b.dimension
        ci = zeros(b.dimension,1);
        ci(i) = 1;
        bi = Function(b,ci);
        % Galerkin discretization
        opti.subject_to(integral(e*bi)==0);
    end
    opti.solver('ipopt');

    sol = opti.solve();

    xsol = sol.value(x);

    re_Ns(Ni) = norm(xsol.eval(1)-y(end,:)');

end
%%
ds = [2,4,8,16];
res_d = zeros(length(ds),1);
N = 4;
for di=1:length(ds)
    d = ds(di);
    b = BSplineBasis([t0,tf],d,N);
    opti = OptiSpline();
    c= opti.variable(2,b.dimension-1);

    x = Function(b,[x0 c]');

    e = x.derivative-f(x);

    for i=2:b.dimension
        ci = zeros(b.dimension,1);
        ci(i) = 1;
        bi = Function(b,ci);
        % Galerkin discretization
        opti.subject_to(integral(e*bi)==0);
    end
    opti.solver('ipopt');

    sol = opti.solve();

    xsol = sol.value(x);

    res_d(di) = norm(xsol.eval(1)-y(end,:)');

end

% alternative? e.list_eval(iets.greville)==0

