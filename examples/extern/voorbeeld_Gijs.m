% Set to where you downloaded the contents of 'SET-PMA-MECO-ME0038/Research-0002/software'
path = 'C:/Users/u0078690/Desktop/software';

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

% define two univariate subbases
knots = linspace(0,1,20);
degree = 3;
B1 = MonomialBasis(degree);
B2 = BSplineBasis(knots,degree);

% test fields
degree = B1.getDegree; % this seems OK
knots  = B2.getKnots;  % why is this a swig object instead of vector?
degree = B2.getDegree; % this seems OK

% add and multiply subbases
B3 = B1 + B2;
B4 = B1*B2;

% create Cartesian product of B1 and B2
sbv = SubBasisVector();
sbv.append(B1);
sbv.append(B2);
B = TensorBasis.TensorBasis(sbv); % what are the fields of this object???

% create multivariate spline
a_ = rand(B1.getLenght(),B2.getLenght());
a = DTensor(a_, [B1.getLenght(),B2.getLenght(),1,1]);
a = Coefficient(a); % how to use the Coefficient class???
mvar_spline = TensorBasis.Function(B,a);

% evaluate multivariate spline and plot
[x,y] = meshgrid(linspace(0,1,30),linspace(0,1,30));
z=zeros(30,30);
for i = 1:30
    for j = 1:30
        eval_spline = mvar_spline({x(i),y(j)});
        eval_spline = DM(eval_spline.data());
        z(i,j) = full(eval_spline);
    end
end
surf(x,y,z); %plot seems incorrect... mistake defining coefficients???

