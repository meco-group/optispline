meco_binaries('cpp_splines','develop')
import splines.*

rng(0)
% load data
data = rand(700, 3);
data(:,3) = abs(data(:,3));
data(:,3) = log(data(:,3));

bounds  = [min(data(:,1:2))' max(data(:,1:2))'];

b1 = BSplineBasis(bounds(1,:), 3, 30);
b2 = BSplineBasis(bounds(2,:), 3, 30);

b = TensorBasis({b1, b2});
opti = OptiSpline();

F = opti.Function(b);

% Option 1. Use an lsqr solver
J = sparse(F.fast_jac(data(:,1:2)'));
c_opt = lsqr(J,data(:,3),1e-8,10000);

F = Function(b, DTensor(c_opt, dims(F.coeff_tensor)));

[x,y]= ndgrid(linspace(bounds(1,1),bounds(1,2),100),linspace(bounds(2,1),bounds(2,2),100));
xy = [x(:)';y(:)'];
z = full(F.fast_eval(xy));
z = reshape(z,size(x));

figure()
mesh(x,y,z)
hold on
scatter3(data(:,1),data(:,2),data(:,3))

norm(F.fast_eval(data(:,1:2)') - data(:,3),'inf')

% Option 2: use optistack

F = opti.Function(b);

diff = F.fast_eval(data(:,1:2)') - data(:,3);

obj = diff'*diff;

opti.minimize(obj)
opti.solver('ipopt');
sol = opti.solve();

F = sol.value(F);

norm(F.fast_eval(data(:,1:2)') - data(:,3),'inf')


% Option 2: use optistack (using fast_jac)


F = opti.Function(b);

diff = F.fast_jac(data(:,1:2)')*F.coeff_tensor.data - data(:,3);

obj = diff'*diff;

opti.minimize(obj)
sol = opti.solve();

F = sol.value(F);

norm(F.fast_eval(data(:,1:2)') - data(:,3),'inf')


