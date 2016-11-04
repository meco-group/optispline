% Set to where you downloaded the contents of 'SET-PMA-MECO-ME0038/Research-0002/software'
path = '/mnt/Shared/SET-PMA-MECO-ME0038/Research-0002/software';

if ispc
  os = 'WINDOWS';
else
  os = 'LINUX';
end

addpath([path filesep 'optistack/MATLAB/src'],'-begin');
addpath([path filesep 'casadi/3.0.0/' os '-MATLAB'],'-begin');
import casadi.*
SX.sym('x');

addpath([path filesep 'cpp_splines/develop/' os '-MATLAB'],'-begin');
%%
import Basis.*

n = 4;

rng(1);
[x,y] = meshgrid(1:n,1:n);
z = 0.2*rand(n);

scatter3(x(:),y(:),z(:),'ko');


degree = 3;
m = SubMonomialBasis(degree);
sbv = SubBasisVector();
sbv.append(m);
sbv.append(m);
b = Basis.Basis(sbv);
%b = Basis({m,m});  % syntax not supported yet
a_ = optivar(m.getLenght(),m.getLenght());
bound = optivar();

a = MTensor(a_, [m.getLenght(),m.getLenght(),1,1]);
a = Coefficient(a);
est_function = Basis.Function(b,a);

con = {};

for i=1:length(x(:))
   z_expr = est_function({x(i),y(i)});
   z_expr= MX(z_expr.data());
   con = {con{:} z_expr+bound>=z(i)};
   con = {con{:} z_expr-bound<=z(i)};
end

obj = bound;

optisolve(obj,con);
%%
a = DTensor(optival(a_), [m.getLenght(),m.getLenght(),1,1]);
a = Coefficient(a);
est_function = Basis.Function(b,a);

ns = 20;

[xs,ys] = meshgrid(linspace(1,n,ns),linspace(1,n,ns));
zs=zeros(ns,ns);
for i=1:ns
    for j=1:ns
        z_val = est_function({xs(i,j),ys(i,j)});
        z_val = DM(z_val.data());
        zs(i,j) = full(z_val);
    end
end

figure
hold on
scatter3(x(:),y(:),z(:),'ko');
surf(xs,ys,zs);

