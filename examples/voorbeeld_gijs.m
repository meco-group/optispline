meco_binaries('cpp_splines','develop')

import Basis.*

% define two univariate subbases

knots = linspace(0,1,20);

degree = 3;

B1 = MonomialBasis(degree);

B2 = BSplineBasis(knots,degree);



% test fields

degree = B1.degree; % this seems OK

knots  = B2.knots;  % why is this a swig object instead of vector?

degree = B2.degree; % this seems OK



% add and multiply subbases

B3 = B1 + B2;

B4 = B1*B2;



% create Cartesian product of B1 and B2

B = Basis.TensorBasis({B1,B2});



% create multivariate spline

a = rand(B.dimension());

mvar_spline = Basis.Function(B, a);



% evaluate multivariate spline and plot

[x,y] = meshgrid(linspace(0,1,30),linspace(0,1,30));

z=zeros(30,30);

for i = 1:30

    for j = 1:30

        z(i,j) = mvar_spline.eval(x(i,j),y(i,j));

    end

end

z(1)

