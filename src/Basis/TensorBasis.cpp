#include "TensorBasis.h"
#include "DummyBasis.h"
#include "BSplineBasis.h"
#include "operations/operationsBasis.h"
#include "../common.h"

namespace spline {

    TensorBasisNode* TensorBasis::get() const {
        return static_cast<TensorBasisNode*>(SharedObject::get());
    };

    TensorBasisNode* TensorBasis::operator->() const { return get(); }

    TensorBasis::TensorBasis() {
        assign_node(new TensorBasisNode(std::vector< Basis >{}));
    }

    TensorBasis::TensorBasis(const std::vector< TensorBasis >& allBasis) {
        std::vector< Basis > allSubBasis {};
        for ( auto &basis : allBasis ) {
            for ( auto &subBasis : basis.getSubBasis() ) {
                allSubBasis.push_back(subBasis);
            }
        }
        assign_node(new TensorBasisNode(allSubBasis));
    }

    TensorBasis::TensorBasis(const Basis & allSubBasis) {
        assign_node(new TensorBasisNode(std::vector< Basis > {allSubBasis}));
    }

    TensorBasis::TensorBasis(const std::vector< Basis >& allSubBasis) {
        assign_node(new TensorBasisNode(allSubBasis));
    }

    TensorBasisNode::TensorBasisNode(const std::vector< Basis >& allSubBasis_)
      : allSubBasis(allSubBasis_), allArguments(std::vector<Argument>{})
      {}

    int TensorBasis::n_basis() const { return (*this)->n_basis(); }
    int TensorBasisNode::n_basis() const {
        return allSubBasis.size();
    }

    void TensorBasis::setArguments(const std::vector< spline::Argument >& args) {
        return (*this)->setArguments(args);
    }

    void TensorBasisNode::setArguments(const std::vector< Argument >& args) {
        allArguments = args;
    }

    std::vector< spline::Argument > TensorBasis::getArguments() const {
        return (*this)->getArguments();
    }
    std::vector< Argument > TensorBasisNode::getArguments() const {
        return allArguments;
    }

    spline::Argument TensorBasis::getSubArgument(int index) const {
        return (*this)->getSubArgument(index);
    }
    Argument TensorBasisNode::getSubArgument(int index) const {
        return allArguments[index];
    }

    int TensorBasis::indexArgument(Argument a) const { return (*this)->indexArgument(a); }
    int TensorBasisNode::indexArgument(Argument a) const {
        auto it = std::find(allArguments.begin(), allArguments.end(), a);
        int index;
        if (it == allArguments.end()) {
            index = -1;
        } else {
            index = std::distance(allArguments.begin(), it);
        }
        return  index;
    }

    bool TensorBasis::hasArguments() const { return (*this)->hasArguments();}
    bool TensorBasisNode::hasArguments() const {
        return allArguments.size() > 0;
    }

    std::vector< Basis > TensorBasis::getSubBasis() const { return (*this)->getSubBasis (); }
    std::vector< Basis > TensorBasisNode::getSubBasis() const {
        return allSubBasis;
    }

    Basis TensorBasis::getBasis() const {
        spline_assert(n_basis() == 1);
        return getSubBasis()[0];
    }

    Basis TensorBasis::getBasis(Argument a) const {
        int index = indexArgument(a);
        if (index < 0) {
            return DummyBasis();
        } else {
            return getBasis(index);
        }
    }

    Basis TensorBasis::getBasis(const Index& index) const {
      int ind = index.concrete(getArguments());
      spline_assert(ind < n_basis());
      return getSubBasis()[ind];
    }

    void TensorBasis::addBasis(TensorBasis basis) { (*this)->addBasis(basis);}
    void TensorBasisNode::addBasis(TensorBasis basis) {
        for ( auto &subBasis : basis.getSubBasis() ) {
            allSubBasis.push_back(subBasis);
        }
    }

    void TensorBasis::addBasis(Basis basis) { (*this)->addBasis(basis);}
    void TensorBasisNode::addBasis(Basis basis) {
        this->allSubBasis.push_back(basis);
    }

    std::vector<int> TensorBasis::dimension() const { return (*this)->dimension ();}
    std::vector<int> TensorBasisNode::dimension() const {
        std::vector<int> shape;
        for (auto const& b : getSubBasis()) {
            shape.push_back(b.dimension());
        }
        return shape;
    }

    std::string TensorBasisNode::getRepresentation() const {
        return "TensorBasis"  + std::to_string(allArguments.size()) +
            std::to_string(allSubBasis.size());
    };

    std::string TensorBasis::getRepresentation() const { return (*this)->getRepresentation() ;};
    std::ostream& operator<<(std::ostream &stream, const TensorBasis& base) {
        return stream << base.getRepresentation();
    }

    TensorBasis TensorBasis::operator+ (const TensorBasis& other) const {
        return plusBasis(*this, other);
    }

    TensorBasis TensorBasis::operator* (const TensorBasis& other) const {
        return timesBasis(*this, other);
    }

    AnyTensor TensorBasis::operator() (const std::vector< AnyScalar > &  x) const {
        return (*this)->operator()(x);
    }
    AnyTensor TensorBasisNode::operator() (const std::vector< AnyScalar > &  x) const {
        spline_assert(x.size()==allSubBasis.size());
        AnyTensor ret = AnyTensor::unity();
        for (int i = 0; i < x.size(); ++i) {
            ret = ret.outer_product(allSubBasis[i](std::vector< AnyScalar >{x[i]}));
        }
        return ret;
    }

    int TensorBasis::totalNumberBasisFunctions() const {
        return (*this)->totalNumberBasisFunctions();
    }
    int TensorBasisNode::totalNumberBasisFunctions() const {
        int r = 1;
        for (int i : dimension()) {
            r *= i;
        }
        return r;
    }

    int TensorBasis::n_inputs() const {
        return (*this)->n_inputs();
    }
    int TensorBasisNode::n_inputs() const {
        int r = 0;
        for (auto const& b : getSubBasis()) {
            r += b.n_inputs();
        }
        return r;
    }

    AnyTensor TensorBasis::const_coeff_tensor(const AnyTensor& t) const {
        return (*this)->const_coeff_tensor(t);
    }
    AnyTensor TensorBasisNode::const_coeff_tensor(const AnyTensor& t) const {
        AnyTensor prod = DTensor({1},{});

        for (int i = 0; i < n_basis(); i++) {
            AnyTensor temp = allSubBasis[0].const_coeff_tensor(AnyTensor(AnyScalar(1)));
            temp.shape({temp.numel()});
            prod = outer_product(prod,temp);
        }

        return outer_product(prod, t)
    }

    // Basis TensorBasis::derivative(int order, int direction, AnyTensor& T) const {
    //     // Call univariate_derivative on basis, for each direction
    // }


} // namespace spline
