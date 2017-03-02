#include <vector>
#include "GenericFunction.h"
#include "../Basis/utils/EvaluationGrid.h"
#include "../Basis/TensorBasisConstant.h"
#include "../common.h"

namespace spline {

    AnyTensor GenericFunction::operator()(const AnyTensor& x, const std::vector< std::string >& args) const{
        if(x.dims()[0] == n_inputs() && x.dims()[1] == 1){
            std::vector< AnyScalar > x_ = x.unpack_1();
            return basis_(x_).inner(coeff().data());
        }
        spline_assert_message(x.dims()[1] == n_inputs(), "Can evaluate list of " + std::to_string(n_inputs()) + " inputs. Got " + std::to_string(x.dims()[0])+ " by " + std::to_string(x.dims()[1]));
        std::vector< AnyTensor > tensor = {};

        std::vector< std::vector< AnyScalar > > X_ = x.unpack_2();
        for(int i = 0; i < X_.size(); i++){
            tensor.push_back(basis_(X_[i]));
        }
        AnyTensor packed_tensor = AnyTensor::pack(tensor, 0);
        int shared_dim = packed_tensor.n_dims();
        std::vector<int> a_r = mrange(0, shared_dim);
        std::vector<int> b_r = mrange(1, shared_dim + 2);
        std::vector<int> c_r = { a_r[0] };
        c_r.push_back(b_r[b_r.size() - 2]);
        c_r.push_back(b_r[b_r.size() - 1]);
        return packed_tensor.einstein(coeff().data(), a_r, b_r, c_r).squeeze();
    }

    casadi::MX GenericFunction::operator<=(const casadi::MX& x) const {
        return coeff().data().as_MT().data()<=x;
    }
    casadi::MX GenericFunction::operator>=(const casadi::MX& x) const {
        return coeff().data().as_MT().data()>=x;
    }

    std::string GenericFunction::type() const{
        return "Function";
    }

    std::string GenericFunction::to_string() const{
        return "Function, consisting of a " + basis_.to_string() + "and:\n\t" + coeff_.to_string();
    }

    Function GenericFunction::generic_operation(const Function& f,
            const BasisComposition & bc, const TensorComposition & tc) const  {

        TensorBasis sumBasis = bc(tensor_basis(), f.tensor_basis());
        EvaluationGrid evaluationGrid = EvaluationGrid(sumBasis);
        std::vector< AnyTensor > basisEvaluated;
        std::vector< AnyTensor > thisFunctionEvaluated;
        std::vector< AnyTensor > otherFunctionEvaluated;
        std::vector< AnyTensor > sumFunctionEvaluated;

        basisEvaluated = evaluationGrid.eval();
        thisFunctionEvaluated = evaluationGrid.eval(*this);
        otherFunctionEvaluated = evaluationGrid.eval(f);

        for (int i = 0; i < basisEvaluated.size(); i++) {
            AnyTensor lhs = thisFunctionEvaluated[i];
            AnyTensor rhs = otherFunctionEvaluated[i];
            if (lhs.dims() == std::vector< int > {1, 1}) {
                lhs = lhs.shape({});
            }
            if (rhs.dims() == std::vector< int > {1, 1}) {
                rhs = rhs.shape({});
            }
            sumFunctionEvaluated.push_back(tc(lhs, rhs));
        }

        AnyTensor A = AnyTensor::pack(basisEvaluated, 0);
        AnyTensor B = AnyTensor::pack(sumFunctionEvaluated, 0);

        int numberEval = basisEvaluated.size();
        int numberBasis = sumBasis.totalNumberBasisFunctions();
        std::vector< int > elemShape = sumFunctionEvaluated[0].dims();
        int numberCoef = (elemShape.size() == 0)? 1: spline::product(elemShape);

        std::vector< int > shapeA = {numberEval, numberBasis};
        std::vector< int > shapeB = {numberBasis, numberCoef};
        A = A.shape(shapeA);
        B = B.shape(shapeB);
        AnyTensor C = A.solve(B);

        std::vector< int > shapeCoef = elemShape;
        std::vector< int > shapeBasis = sumBasis.dimension();
        shapeBasis.insert(shapeBasis.end(), shapeCoef.begin(), shapeCoef.end());

        C = C.shape(shapeBasis);
        return Function(sumBasis, C);
    }

    Function GenericFunction::operator+(const Function& f) const {
        return generic_operation(f,
                [](const TensorBasis& lhs, const TensorBasis& rhs) { return lhs + rhs; },
                [](const AnyTensor& lhs, const AnyTensor& rhs) { return lhs + rhs; });
    }

    Function GenericFunction::operator+(const AnyTensor& t) const {
        if (t.is_scalar() && t.dims()!=shape())
            return operator+(AnyTensor::repeat(t.as_scalar(), shape()));
        return operator+(GenericFunction::Constant(this->tensor_basis(), t));
    }

    Function GenericFunction::operator*(const Function& f) const {
        return generic_operation(f,
                [](const TensorBasis& lhs, const TensorBasis& rhs) { return lhs * rhs; },
                [](const AnyTensor& lhs, const AnyTensor& rhs) { return lhs * rhs; });
    }

    void GenericFunction::homogenize_args(Function& f, AnyTensor& t) {
        if (t.is_scalar() && t.dims()!=f.shape()) t = AnyTensor::repeat(t.as_scalar(), f.shape());
        if (f.is_scalar() && !t.is_scalar()) {
            AnyTensor ones = AnyTensor::repeat(AnyScalar(1), t.dims());
            Coefficient C = f.coeff();
            f =  Function(f.tensor_basis(),
                    C.data().shape(C.dimension()).outer_product(ones)).operator*(t);
        }
    }

    Function GenericFunction::operator*(const AnyTensor& rhs) const {
        Function f = *this;
        AnyTensor t = rhs;
        homogenize_args(f, t);

        std::vector< int > tdims = f.coeff_tensor().dims();
        tdims.pop_back();
        tdims.pop_back();

        AnyTensor transf = AnyTensor::repeat(AnyScalar(1), tdims);
        transf = transf.outer_product(t);
        AnyTensor data = f.coeff_tensor()*transf;

        return Function(f.tensor_basis(), Coefficient(data));
    }

    Function GenericFunction::mtimes(const Function& f) const {
        return generic_operation(f,
                [](const TensorBasis& lhs, const TensorBasis& rhs) { return lhs * rhs; },
                [](const AnyTensor& lhs, const AnyTensor& rhs) { return lhs.mtimes(rhs);});
    }

    Function GenericFunction::mtimes(const AnyTensor& t) const {
        if (t.is_scalar()) return operator*(t);
        spline_assert(t.n_dims() == 2);
        Coefficient c = coeff();
        int dir = n_inputs() + 1; //0 based, 2nd matrix dimension
        return Function(tensor_basis(), c.transform(t.reorder_dims({1, 0}), dir));
    }

    Function GenericFunction::rmtimes(const AnyTensor& t) const {
        if (t.is_scalar()) return operator*(t);
        return GenericFunction::Constant(this->tensor_basis(), t).mtimes(*this);
    }

    Function GenericFunction::pow(int power) const {
        spline_assert(power >= 0);
        Function f = *this;
        Function fpow = *this;

        if (power == 0) {
            fpow = GenericFunction::Constant(tensor_basis(), 1, shape());
        } else {
            for (int i = 1; i < power; i++) {
                fpow = fpow*f;
            }
        }

        return fpow;
    }

    Function GenericFunction::operator-(const Function& f) const {
        return operator+(-f);
    }

    Function GenericFunction::operator-(const AnyTensor& t) const {
        return operator+(-t);
    }

    Function GenericFunction::operator-() const {
        return Function(basis_, -coeff_);
    }

    Function GenericFunction::transpose() const {
        return Function(tensor_basis(), coeff().transpose());
    }

    Function GenericFunction::trace() const {
        std::vector< int > shape_ = shape();
        spline_assert_message(shape_[0] == shape_[1],
                "Trace only defined for square matrices. Dimensions are " << shape_ << ".");

        AnyTensor t = DT(casadi::DM::densify(casadi::DM::eye(shape_[0])));
        Function fdiag = operator*(t); //keep diagonal entries only

        Coefficient cdiag = fdiag.coeff();
        AnyTensor ones = AnyTensor::repeat(AnyScalar(1), std::vector< int >{1, shape_[0]});
        cdiag = cdiag.transform(ones, cdiag.dimension().size()); //sum over all columns
        cdiag = cdiag.transform(ones, cdiag.dimension().size()+1); //sum over all rows

        return Function(tensor_basis(), cdiag);
    }

    Function GenericFunction::vertcat(const std::vector< spline::Function >& f) {
        return GenericFunction::cat(0, f);
    }

    Function GenericFunction::horzcat(const std::vector< spline::Function >& f) {
        return GenericFunction::cat(1, f);
    }

    Function GenericFunction::blkdiag(const std::vector< spline::Function >& f) {
        Function b = f[0];
        for (int i = 1; i < f.size(); i++) {
            std::vector< int > shape12 = std::vector< int >{b.shape()[0], f[i].shape()[1]};
            std::vector< int > shape21 = std::vector< int >{f[i].shape()[0], b.shape()[1]};

            Function zero12 = GenericFunction::Constant(b.tensor_basis(), 0, shape12);
            Function zero21 = GenericFunction::Constant(b.tensor_basis(), 0, shape21);

            Function upper = GenericFunction::horzcat(std::vector< Function >{b, zero12});
            Function lower = GenericFunction::horzcat(std::vector< Function >{zero21, f[i]});

            b = GenericFunction::vertcat(std::vector< Function >{upper, lower});
        }

        return b;
    }

    Function GenericFunction::transform_to(const Basis& basis) const {
        return transform_to(TensorBasis(basis));
    }

    Function GenericFunction::transform_to(const TensorBasis& basis) const {
        if(basis_.type() == "TensorBasisConstant"){
            AnyTensor T = coeff_.rm_direction( std::vector< int > {0} ).data();
            return Function(basis, basis.const_coeff_tensor(T));
        }

        TensorBasis unionBasis = tensor_basis() + basis;
        EvaluationGrid evaluationGrid = EvaluationGrid(unionBasis);
        std::vector< AnyTensor > basisEvaluated;
        std::vector< AnyTensor > thisFunctionEvaluated;

        basisEvaluated = evaluationGrid.eval();
        thisFunctionEvaluated = evaluationGrid.eval(*this);

        AnyTensor A = AnyTensor::pack(basisEvaluated, 0);
        AnyTensor B = AnyTensor::pack(thisFunctionEvaluated, 0);
        int numberEval = basisEvaluated.size();
        int numberBasis = unionBasis.totalNumberBasisFunctions();
        std::vector< int > elemShape = thisFunctionEvaluated[0].dims();
        int numberCoef = spline::product(elemShape);

        std::vector< int > shapeA = {numberEval, numberBasis};
        std::vector< int > shapeB = {numberBasis, numberCoef};
        A = A.shape(shapeA);
        B = B.shape(shapeB);
        AnyTensor C = A.solve(B);

        std::vector< int > shapeCoef = elemShape;
        std::vector< int > shapeBasis = unionBasis.dimension();
        shapeBasis.insert(shapeBasis.end(), shapeCoef.begin(), shapeCoef.end());

        C = C.shape(shapeBasis);
        return Function(unionBasis, C);
    }

    Function GenericFunction::project_to(const Basis& basis) const {
        return project_to(TensorBasis(basis));
    }

    Function GenericFunction::project_to(const TensorBasis& b) const {
        Function b2 = b.basis_functions();
        Function f = reshape(std::vector< int >{1,spline::product(shape())});

        Function b22 = b2.mtimes(b2.transpose());
        Function b2f = b2.mtimes(f); //f already is a row vector

        AnyTensor B22 = b22.integral();
        AnyTensor B2f = b2f.integral();

        AnyTensor C = B22.solve(B2f);

        std::vector< int > M = b.dimension();
        std::vector< int > N = shape();
        std::vector< int > shapeC = M;
        shapeC.insert(shapeC.end(), N.begin(), N.end());
        C = C.shape(shapeC);

        return Function(b,C);
    }

    Function GenericFunction::cat(NumericIndex index,
            const std::vector< spline::Function >& functions) {
        TensorBasis unionBasis = functions[0].tensor_basis();  // load first basis
        for (int i = 1; i< functions.size(); i++) {
            unionBasis = unionBasis + functions[i].tensor_basis();  // add other bases
        }

        std::vector< Coefficient > coefVec;
        for (int i = 0; i< functions.size(); i++) {
            coefVec.push_back(functions[i].transform_to(unionBasis).coeff());
        }

        return Function(unionBasis, Coefficient::cat(index, coefVec));
    }

    Function GenericFunction::reshape(const std::vector< int >& shape) const {
        return Function(tensor_basis(), coeff().reshape(shape));
    }

    Function GenericFunction::slice(const AnySlice& i, const AnySlice& j) const {
        return Function(tensor_basis(), coeff_tensor().get_slice(i, j));
    }

    Function GenericFunction::slice(const AnySlice& i) const {
        return Function(tensor_basis(), coeff_tensor().get_slice(i));
    }

}  // namespace spline
