#include <vector>
#include "GenericFunction.h"
#include "../Basis/utils/EvaluationGrid.h"
#include "../Basis/TensorBasisConstant.h"
#include "../common.h"

namespace spline {

    casadi::MX GenericFunction::operator<=(const casadi::MX& x) const {
        return coeff().data().as_MT().data()<=x;
    }
    casadi::MX GenericFunction::operator>=(const casadi::MX& x) const {
        return coeff().data().as_MT().data()>=x;
    }

    std::string GenericFunction::type() const{
        return "GenericFunction";
    }

    std::string GenericFunction::to_string() const{
        return "Function, consisting of a " + basis_.to_string() + "and:\n\t" + coeff_.to_string();
    }

    GenericFunction GenericFunction::operator+(const Function& f) const {
        return generic_operation(f,
                [](const TensorBasis& lhs, const TensorBasis& rhs) { return lhs + rhs; },
                [](const AnyTensor& lhs, const AnyTensor& rhs) { return lhs + rhs; });
    }

    GenericFunction GenericFunction::operator*(const Function& f) const {
        return generic_operation(f,
                [](const TensorBasis& lhs, const TensorBasis& rhs) { return lhs * rhs; },
                [](const AnyTensor& lhs, const AnyTensor& rhs) { return lhs * rhs; });
    }

    GenericFunction GenericFunction::mtimes(const Function& f) const {
        return generic_operation(f,
                [](const TensorBasis& lhs, const TensorBasis& rhs) { return lhs * rhs; },
                [](const AnyTensor& lhs, const AnyTensor& rhs) { return lhs.mtimes(rhs);});
    }

    GenericFunction GenericFunction::operator-(const Function& f) const {
        return operator+(-f);
    }

    GenericFunction GenericFunction::operator-() const {
        return Function(basis_, -coeff_);
    }

    GenericFunction GenericFunction::transpose() const {
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

    void GenericFunction::homogenize_args(Function& f, AnyTensor& t) {
        if (t.is_scalar() && t.dims()!=f.shape()) t = AnyTensor::repeat(t.as_scalar(), f.shape());
        if (f.is_scalar() && !t.is_scalar()) {
            AnyTensor ones = AnyTensor::repeat(AnyScalar(1), t.dims());
            Coefficient C = f.coeff();
            f =  Function(f.tensor_basis(),
                    C.data().shape(C.dimension()).outer_product(ones)).operator*(t);
        }
    }

    GenericFunction GenericFunction::vertcat(const std::vector< spline::Function >& f) {
        return GenericFunction::cat(0, f);
    }

    GenericFunction GenericFunction::horzcat(const std::vector< spline::Function >& f) {
        return GenericFunction::cat(1, f);
    }

    GenericFunction GenericFunction::blkdiag(const std::vector< spline::Function >& f) {
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

    GenericFunction GenericFunction::transform_to(const Basis& basis) const {
        return transform_to(TensorBasis(basis));
    }

    GenericFunction GenericFunction::transform_to(const TensorBasis& basis) const {
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

    GenericFunction GenericFunction::project_to(const Basis& basis) const {
        return project_to(TensorBasis(basis));
    }

    GenericFunction GenericFunction::project_to(const TensorBasis& b) const {
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

    GenericFunction GenericFunction::cat(NumericIndex index,
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

    std::vector< int > GenericFunction::shape() const {
        return coeff_.shape();
    }
}  // namespace spline
