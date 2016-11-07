#include <sstream>
#include "BSplineBasis.h"

#include "operations/operationsBasis.h"

namespace spline{

    BSplineBasisNode* BSplineBasis::get() const { return static_cast<BSplineBasisNode*>(SharedObject::get()); };
    BSplineBasisNode* BSplineBasis::operator->() const { return get(); }

    std::string BSplineBasisNode::getRepresentation() const {
        std::stringstream s;
        s << "BSplineBasis object";
        return s.str();
    };

    Basis BSplineBasisNode::operator+ (const Basis& other) const {
        return other + shared_from_this<BSplineBasis>();
    }

    Basis BSplineBasisNode::operator+ (const DummyBasis& other) const {
        return shared_from_this<BSplineBasis>();
    }

    Basis BSplineBasisNode::operator+ (const BSplineBasis& other) const {
        return plusSubBasis (shared_from_this<BSplineBasis>(), other);
    }

    Basis BSplineBasisNode::operator+ (const MonomialBasis& other) const {
        return plusSubBasis (shared_from_this<BSplineBasis>(), other);
    }

    Basis BSplineBasisNode::operator* (const Basis& other) const {
        return other * shared_from_this<BSplineBasis>();
    }

    Basis BSplineBasisNode::operator* (const DummyBasis& other) const {
        return shared_from_this<BSplineBasis>();
    }

    Basis BSplineBasisNode::operator* (const BSplineBasis& other) const {
        return timesSubBasis (shared_from_this<BSplineBasis>(), other);
    }

    Basis BSplineBasisNode::operator* (const MonomialBasis& other) const {
        return timesSubBasis (shared_from_this<BSplineBasis>(), other);
    }

    std::vector<double> BSplineBasis::greville () const {return (*this)->greville();}
    std::vector<double> BSplineBasisNode::greville () const {
        int degree = getDegree();
        if(degree == 0){
            degree = 1;
        }

        std::vector<double> grevillePoints (getLength());
        for (int i = 0; i < getLength(); ++i) {
            grevillePoints[i] = std::accumulate(knots_.begin()+i+1,knots_.begin()+i+degree+1, 0.0) / degree;
        }

        return grevillePoints;
    }

    int BSplineBasisNode::getLength () const{
        return knots_.size() - degree - 1;
    }

    BSplineBasis::BSplineBasis (const std::vector<double >& knots, int degree)  {
        assign_node(new BSplineBasisNode(knots, degree));
    }

    BSplineBasisNode::BSplineBasisNode (const std::vector<double >& knots, int degree)
    : UnivariateBasisNode(degree), knots_(knots){ }

    BSplineBasis::BSplineBasis (const std::vector<double >& bounds, int degree, int numberOfIntervals)  { assign_node(new BSplineBasisNode(bounds, degree, numberOfIntervals)); };
    BSplineBasisNode::BSplineBasisNode (const std::vector<double >& bounds, int degree, int numberOfIntervals) : UnivariateBasisNode(degree) {
        int numberOfKnots = 2*degree + numberOfIntervals;
        knots_.resize(numberOfKnots, 0);

        for (int i = 0; i < degree; ++i) {
            knots_[i] = bounds[0];
            knots_[numberOfKnots - i - 1] = bounds[1];
        }

        for (int i = 0; i < numberOfIntervals; ++i) {
            knots_[degree + i] = bounds[0] + (bounds[1] - bounds[0]) * (double)i/(numberOfIntervals-1);
        }

        setKnots(knots_);
    }

    std::vector< double > BSplineBasis::getKnots () const { return (*this)->getKnots(); }
    std::vector< double > BSplineBasisNode::getKnots () const {
        return knots_;
    }

    void BSplineBasis::setKnots (const std::vector< double >& knots) { return (*this)->setKnots (knots); }
    void BSplineBasisNode::setKnots (const std::vector< double >& knots) {
        knots_ = knots;
    }


    AnyTensor BSplineBasisNode::operator() (const std::vector<AnyScalar> & x) const {
        assert(x.size()==getNumberOfSubBasis());
        if(AnyScalar::is_double(x)) {
            return SubBasisEvalution<double>(AnyScalar::as_double(x));
        } else {
            return SubBasisEvalution<AnyScalar>(x);
        }
    }

    void BSplineBasisNode::getEvaluationGrid(std::vector< std::vector < AnyScalar > > * grid) const{
        for(auto const& point : greville()){
            grid->push_back(std::vector<AnyScalar> {point});
        }
    }
} // namespace spline
