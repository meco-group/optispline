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

    TensorBasis::TensorBasis(const Basis& allSubBasis, const Argument& args) {
        assign_node(new TensorBasisNode(std::vector< Basis > {allSubBasis}, std::vector< Argument > {args}));
    }

    TensorBasis::TensorBasis(const TensorBasis& tensor, const std::vector< Argument >& args) {
        assign_node(new TensorBasisNode(tensor.getSubBasis(), args));
    }

    TensorBasis::TensorBasis(const std::vector< Basis >& allSubBasis, const std::vector< Argument >& args) {
        assign_node(new TensorBasisNode(allSubBasis, args));
    }


    TensorBasisNode::TensorBasisNode(const std::vector< Basis >& allSubBasis_)
      : allSubBasis(allSubBasis_), allArguments(std::vector<Argument>{})
      {}

    TensorBasisNode::TensorBasisNode(const std::vector< Basis >& allSubBasis_, const std::vector< Argument >& args) :
        allSubBasis(allSubBasis_), allArguments(args)
    {}

    int TensorBasis::n_basis() const { return (*this)->n_basis(); }
    int TensorBasisNode::n_basis() const {
        return allSubBasis.size();
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

    TensorBasis TensorBasis::addBasis(TensorBasis basis) const {return (*this)->addBasis(basis);}
    TensorBasis TensorBasisNode::addBasis(TensorBasis basis) const {
        std::vector< Basis > all_basis = getSubBasis();
        for ( auto &subBasis : basis.getSubBasis() ) {
            all_basis.push_back(subBasis);
        }
        return TensorBasis(all_basis, getArguments());
    }

    TensorBasis TensorBasis::addBasis(Basis basis) const {return (*this)->addBasis(basis);}
    TensorBasis TensorBasisNode::addBasis(Basis basis) const {
        std::vector< Basis > all_basis = getSubBasis();
        all_basis.push_back(basis);
        return TensorBasis(all_basis, getArguments());
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

    // Basis TensorBasis::derivative(int order, int direction, AnyTensor& T) const {
    //     // Call univariate_derivative on basis, for each direction
    // }


} // namespace spline
