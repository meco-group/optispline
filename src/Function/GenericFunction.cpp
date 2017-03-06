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

    Function GenericFunction::operator+(const AnyTensor& t) const {
        if (t.is_scalar() && t.dims()!=shape())
            return operator+(AnyTensor::repeat(t.as_scalar(), shape()));
        return operator+(Constant(t));
    }

    Function GenericFunction::operator*(const AnyTensor& t) const {
        if (t.is_scalar() && t.dims()!=shape())
            return operator*(AnyTensor::repeat(t.as_scalar(), shape()));
        return operator*(Constant(t));
    }

    Function GenericFunction::operator-(const Function& f) const {
        return operator+(-f);
    }

    Function GenericFunction::operator-(const AnyTensor& t) const {
        if (t.is_scalar() && t.dims()!=shape())
            return operator+(AnyTensor::repeat( -t.as_scalar(), shape()));
        return operator+(Constant(-t));
    }

    Function GenericFunction::mtimes(const AnyTensor& t) const {
        if (t.is_scalar() && t.dims()!=shape())
            return operator+(AnyTensor::repeat(t.as_scalar(), shape()));
        return operator+(Constant(t));
    }

    Function GenericFunction::mtimes(const AnyTensor& t) const {
        if (t.is_scalar() && t.dims()!=shape())
            return operator+(AnyTensor::repeat(t.as_scalar(), shape()));
        return operator+(Constant(t));
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

            Function zero12 = Constant(0, shape12);
            Function zero21 = Constant(0, shape21);

            Function upper = GenericFunction::horzcat(std::vector< Function >{b, zero12});
            Function lower = GenericFunction::horzcat(std::vector< Function >{zero21, f[i]});

            b = GenericFunction::vertcat(std::vector< Function >{upper, lower});
        }

        return b;
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

    std::vector< int > GenericFunction::shape() const {
        return coeff_.shape();
    }
}  // namespace spline
