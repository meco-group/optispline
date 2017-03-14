meco_binaries('cpp_splines','develop')

import splines.*


%% plus

% % scalar spline random coeffs + double
b1 = BSplineBasis([0,0,1/2,1,1],1);
c1 = Coefficient(rand(3,1,1));
s1 = splines.Function(b1, c1);
s2 = s1 + 5; % spline + double % does not work yet
assert(abs(s2.eval(0.6) - s1.eval(0.6) - 5) <= eps);
s2 = 5 + s1; % double + spline % does not work yet
assert(abs(s2.eval(0.6) - s1.eval(0.6) - 5) <= eps);

% sum two scalar splines given coeffs
b1 = splines.BSplineBasis([0,0,1/2,1,1],1);
c1 = splines.Coefficient(ones(3,1,1));
s1 = splines.Function(b1, c1);
b2 = splines.BSplineBasis([0,0,1,1],1);
c2 = splines.Coefficient(2*ones(2,1,1));
s2 = splines.Function(b2, c2);
s3 = s1 + s2;
assert(isequal(s3.coeff.data,3*ones(3,1,1)));
s3 = s2 + s1;
assert(isequal(s3.coeff.data,3*ones(3,1,1)));

% sum two scalar splines random coeffs
b1 = BSplineBasis([0,0,1/2,1,1],1);
c1 = Coefficient(rand(3,1,1));
s1 = splines.Function(b1, c1);
b2 = BSplineBasis([0,0,1,1],1);
c2 = Coefficient(rand(2,1,1));
s2 = splines.Function(b2, c2);
s3 = s1 + s2;
assert(abs(s3.eval(0.6) - s1.eval(0.6) - s2.eval(0.6)) <= eps);
s3 = s2 + s1;
assert(abs(s3.eval(0.6) - s1.eval(0.6) - s2.eval(0.6)) <= eps);

% % bivariate matrix-valued spline + double
m = 4; n = 5;
SB1 = BSplineBasis([0 0 0 0.2 0.5 0.9 1 1 1],2);
SB2 = BSplineBasis([0 0 0.4 1 1],1);
B = splines.TensorBasis({SB1,SB2});
C = Coefficient(rand(6,3,m,n));
S = splines.Function(B,C);
R = S + 12;
assert(norm(R.eval(0.3,0.5) - S.eval(0.3,0.5) - 12) < 1e-12);
R = 12 + S;
assert(norm(R.eval(0.3,0.5) - S.eval(0.3,0.5) - 12) < 1e-12);

% sum two bivariate matrix-valued splines
m = 4; n = 5;
SB11 = BSplineBasis([0 0 0 0.9 1 1 1],2);
SB12 = BSplineBasis([0 0 0.4 1 1],1);
B1 = splines.TensorBasis({SB11,SB12});

C1 = Coefficient(rand(SB11.dimension,SB12.dimension,m,n));


S1 = splines.Function(B1,C1);
SB21 = BSplineBasis([0 0 0 0.1 1 1 1],2);
SB22 = BSplineBasis([0 0 0.7 1 1],1);
B2 = splines.TensorBasis({SB21,SB22});
C2 = Coefficient(rand(SB21.dimension,SB22.dimension,m,n));
S2 = splines.Function(B2,C2);
S3 = S1 + S2;
delta = abs(S3.eval(0.3,0.5) - S2.eval(0.3,0.5) - S1.eval(0.3,0.5));
assert(max(delta(:)) < 1e-14);
S3 = S2 + S1;
delta = abs(S3.eval(0.3,0.5) - S2.eval(0.3,0.5) - S1.eval(0.3,0.5));
assert(max(delta(:)) < 1e-14);


%% uminus

% scalar spline given coeffs
b = BSplineBasis([0,0,1/2,1,1],1);
c = Coefficient(ones(3,1,1));
s = splines.Function(b, c);
m = -s;
assert(isequal(m.coeff.data,-ones(3,1,1)));

% scalar spline random coeffs
b = BSplineBasis([0,0,1/2,1,1],1);
c = Coefficient(rand(3,1,1));
s = splines.Function(b, c);
m = -s;
assert(isequal(s.coeff.data, -m.coeff.data));

% bivariate matrix-valued spline random coeffs
m = 4; n = 5;
SB1 = BSplineBasis([0 0 0 0.9 1 1 1],2);
SB2 = BSplineBasis([0 0 0.4 1 1],1);
B = splines.TensorBasis({SB1,SB2});
C = Coefficient(rand(SB1.dimension,SB2.dimension,m,n));
S = splines.Function(B,C);
M = -S;
assert(isequal(S.coeff.data, -M.coeff.data));


%% minus

% % difference scalar spline random coeffs and double
b1 = BSplineBasis([0,0,1/2,1,1],1);
c1 = Coefficient(rand(3,1,1));
s1 = splines.Function(b1, c1);
s2 = s1 - 5; % spline + double % does not work yet
assert(abs(s2.eval(0.6) - s1.eval(0.6) + 5) <= eps);
s2 = 5 - s1; % double + spline % does not work yet
assert(abs(s2.eval(0.6) + s1.eval(0.6) - 5) <= eps);

% difference two scalar splines given coeffs
b1 = BSplineBasis([0,0,1/2,1,1],1);
c1 = Coefficient(ones(3,1,1));
s1 = splines.Function(b1, c1);
b2 = BSplineBasis([0,0,1,1],1);
c2 = Coefficient(2*ones(2,1,1));
s2 = splines.Function(b2, c2);
s3 = s1 - s2;
assert(isequal(s3.coeff.data,-ones(3,1,1)));
s3 = s2 - s1;
assert(isequal(s3.coeff.data,ones(3,1,1)));

% difference two scalar splines random coeffs
b1 = BSplineBasis([0,0,1/2,1,1],1);
c1 = Coefficient(rand(3,1,1));
s1 = splines.Function(b1, c1);
b2 = BSplineBasis([0,0,1,1],1);
c2 = Coefficient(rand(2,1,1));
s2 = splines.Function(b2, c2);
s3 = s1 - s2;
assert(abs(s3.eval(0.6) - s1.eval(0.6) + s2.eval(0.6)) <= eps);
s3 = s2 - s1;
assert(abs(s3.eval(0.6) + s1.eval(0.6) - s2.eval(0.6)) <= eps);

% difference bivariate matrix-valued spline and double
m = 4; n = 5;
SB1 = BSplineBasis([0 0 0 0.2 0.5 0.9 1 1 1],2);
SB2 = BSplineBasis([0 0 0.4 1 1],1);
B = splines.TensorBasis({SB1,SB2});
C = Coefficient(rand(6,3,m,n));
S = splines.Function(B,C);
R = S - 12;
assert(norm(R.eval(0.3,0.5) - S.eval(0.3,0.5) + 12) < 1e-12);
R = 12 - S;
assert(norm(R.eval(0.3,0.5) + S.eval(0.3,0.5) - 12) < 1e-12);

% difference two bivariate matrix-valued splines
m = 4; n = 5;
SB11 = BSplineBasis([0 0 0 0.9 1 1 1],2);
SB12 = BSplineBasis([0 0 0.4 1 1],1);
B1 = splines.TensorBasis({SB11,SB12});
C1 = Coefficient(rand(SB11.dimension,SB12.dimension,m,n));
S1 = splines.Function(B1,C1);
SB21 = BSplineBasis([0 0 0 0.1 1 1 1],2);
SB22 = BSplineBasis([0 0 0.7 1 1],1);
B2 = splines.TensorBasis({SB21,SB22});
C2 = Coefficient(rand(SB21.dimension,SB22.dimension,m,n));
S2 = splines.Function(B2,C2);
S3 = S1 - S2;
delta = abs(S3.eval(0.3,0.5) - S1.eval(0.3,0.5) + S2.eval(0.3,0.5));
assert(max(delta(:)) < 1e-14);
S3 = S2 - S1;
delta = abs(S3.eval(0.3,0.5) + S1.eval(0.3,0.5) - S2.eval(0.3,0.5));
assert(max(delta(:)) < 1e-14);


%% mtimes

% product scalar spline and double
b = BSplineBasis([0,0,0,1,2,3,3,3],2);
c = Coefficient(rand(b.dimension,1,1));
s = splines.Function(b,c);
p = s*7;
delta = p.coeff.data - 7*s.coeff.data;
assert(max(abs(delta(:))) < 1e-14);
p = 7*s;
delta = p.coeff.data - 7*s.coeff.data;
assert(max(abs(delta(:))) < 1e-14);

% product two scalar splines given coeffs
b1 = BSplineBasis([0,0,0,1,2,3,3,3],2);
c1 = Coefficient(2*ones(b1.dimension,1,1));
s1 = splines.Function(b1,c1);
b2 = BSplineBasis([0,0,3,3],1);
c2 = Coefficient(3*ones(b2.dimension,1,1));
s2 = splines.Function(b2,c2);
p = s1*s2;
delta = p.coeff.data - 6*ones(p.basis.dimension,1,1);
assert(max(abs(delta(:))) < 1e-14);
p = s2*s1;
delta = p.coeff.data - 6*ones(p.basis.dimension,1,1);
assert(max(abs(delta(:))) < 1e-14);

% product two scalar splines random coeffs
b1 = BSplineBasis([0,0,0,1,2,3,3,3],2);
c1 = Coefficient(rand(b1.dimension,1,1));
s1 = splines.Function(b1,c1);
b2 = BSplineBasis([0,0,3,3],1);
c2 = Coefficient(rand(b2.dimension,1,1));
s2 = splines.Function(b2,c2);
p = s1*s2;
assert(abs(p.eval(1.5)-s1.eval(1.5)*s2.eval(1.5)) < 1e-14);
p = s2*s1;
assert(abs(p.eval(1.5)-s1.eval(1.5)*s2.eval(1.5)) < 1e-14);

m = 4; n = 5;
b1 = BSplineBasis([0,0,0,1,2,3,3,3],2);
c1 = Coefficient(rand(b1.dimension,m,n));
s1 = splines.Function(b1,c1);
b2 = BSplineBasis([0,0,3,3],1);
c2 = Coefficient(rand(b2.dimension,n,m));
s2 = splines.Function(b2,c2);
p = s1*s2;
assert(norm(p.eval(1.5)-s1.eval(1.5)*s2.eval(1.5)) < 1e-12);
p = s2*s1;
assert(norm(p.eval(1.5)-s2.eval(1.5)*s1.eval(1.5)) < 1e-12);

% % product bivariate matrix-valued spline and double
m = 4; n = 5;
SB1 = BSplineBasis([0 0 0 0.9 1 1 1],2);
SB2 = BSplineBasis([0 0 0.4 1 1],1);
B = splines.TensorBasis({SB1,SB2});
C = Coefficient(rand(SB1.dimension,SB2.dimension,m,n));
S = splines.Function(B,C);
P = 8*S;
delta = P.coeff.data - 8*S.coeff.data;
assert(norm(delta(:)) < 1e-14);
P = S*8;
delta = P.coeff.data - 8*S.coeff.data;
assert(norm(delta(:)) < 1e-14);

% % product bivariate matrix-valued spline and constant matrix
m = 4; n = 4;
SB11 = BSplineBasis([0 0 0 0.9 1 1 1],2);
SB12 = BSplineBasis([0 0 0.4 1 1],1);
B1 = splines.TensorBasis({SB11,SB12});
C1 = Coefficient(rand(SB11.dimension,SB12.dimension,m,n));
S1 = splines.Function(B1,C1);
A = rand(n,m);
P = S1*A;
delta = abs(P.eval(0.3,0.5) - S1.eval(0.3,0.5)*A);
assert(norm(delta(:)) < 1e-12);
P = A*S1;
delta = abs(P.eval(0.3,0.5) - A*S1.eval(0.3,0.5));
assert(norm(delta(:)) < 1e-12);

% product two bivariate matrix-valued splines
m = 4; n = 4;
SB11 = BSplineBasis([0 0 0 0.9 1 1 1],2);
SB12 = BSplineBasis([0 0 0.4 1 1],1);
B1 = splines.TensorBasis({SB11,SB12});
C1 = Coefficient(rand(SB11.dimension,SB12.dimension,m,n));
S1 = splines.Function(B1,C1);
SB21 = BSplineBasis([0 0 0 0.1 1 1 1],2);
SB22 = BSplineBasis([0 0 0.7 1 1],1);
B2 = splines.TensorBasis({SB21,SB22});
C2 = Coefficient(rand(SB21.dimension,SB22.dimension,n,m));
S2 = splines.Function(B2,C2);
P = S1*S2;
delta = abs(P.eval(0.3,0.5) - S1.eval(0.3,0.5)*S2.eval(0.3,0.5));
assert(norm(delta(:)) < 1e-14);
P = S2*S1;
delta = abs(P.eval(0.3,0.5) - S2.eval(0.3,0.5)*S1.eval(0.3,0.5));
assert(norm(delta(:)) < 1e-14);


%% power

% scalar spline constant coeffs
b1 = BSplineBasis([0,0,1/2,1,1],1);
c1 = Coefficient(3*ones(3,1,1));
s1 = splines.Function(b1, c1);
s0 = s1^0; % to the power 0
assert(isequal(s0.data,ones(1,1)));
s4 = s1^4;
assert(abs(s4.eval(0.2)-81) < 1e-13);

% % scalar spline random coeffs
b2 = BSplineBasis([0,0,1,1],1);
c2 = Coefficient(rand*ones(2,1,1));
s2 = splines.Function(b1, c1);
s0 = s2^0; % to the power 0
assert(isequal(s0.data,ones(1,1)));
s4 = s2^4;
assert(norm(s4.eval(0.7)-s2.eval(0.7)^4) < 1e-12);

% % bivariate matrix-valued spline random coeffs
n = 3;
SB1 = BSplineBasis([0 0 0 0.9 1 1 1],2);
SB2 = BSplineBasis([0 0 0.4 1 1],1);
B = splines.TensorBasis({SB1,SB2});
C = Coefficient(rand(SB1.dimension,SB2.dimension,n,n));
S = splines.Function(B,C);
P = S.^4;
assert(norm(P.eval(0.5,0.3) - S.eval(0.5,0.3).^4) < 1e-12);
