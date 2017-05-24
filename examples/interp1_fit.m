meco_binaries('cpp_splines','develop')
import splines.*

rng(0);
x = linspace(0,100,30);
y = rand(30,1);

m_low = min(x);
m_high = max(x);

opti = OptiSpline();

b = BSplineBasis([m_low, m_high],3,60);

F = opti.Function(b);

H = F.derivative(2);

Flin = splines.Function.linear(x, y);

e = F-Flin;

curve_bound = 1;

L = opti.var(dimension(e.coeff));
sol = opti.solver(sum(L),{e<=L, e>=0, H<=curve_bound, H>=-curve_bound},'ipopt',struct('ipopt',struct('tol',1e-12)));
sol.solve();

F = sol.value(F);

Fc = F.to_casadi();
e = sol.value(e);

H = sol.value(H);

xs = linspace(m_low,m_high,1000);

figure()
hold on

plot(F.basis.knots, F.list_eval(F.basis.knots),'x')
plot(xs, e.list_eval(xs))
plot(xs, F.list_eval(xs));
plot(x,y,'o-')
plot(xs, H.list_eval(xs)/curve_bound)

