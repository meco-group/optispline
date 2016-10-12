#include <sstream>
#include "SubBSplineBasis.h"

#include "operations/operationsBasis.h"

namespace spline{

    SubBSplineBasisNode* SubBSplineBasis::get() const { return static_cast<SubBSplineBasisNode*>(SharedObject::get()); };
    SubBSplineBasisNode* SubBSplineBasis::operator->() const { return get(); }

    std::string SubBSplineBasisNode::getRepresentation() const {
        std::stringstream s;
        s << "BSplineBasis object" << getKnots();
        return s.str();
    };
    std::string SubBSplineBasis::getRepresentation() const { return (*this)->getRepresentation() ;};

    SubBasis SubBSplineBasisNode::operator+ (const SubBasis& other) const {
        return other + shared_from_this<SubBSplineBasis>();
    }

    SubBasis SubBSplineBasisNode::operator+ (const SubBasisDummy& other) const {
        return shared_from_this<SubBSplineBasis>();
    }

    SubBasis SubBSplineBasisNode::operator+ (const SubBSplineBasis& other) const {
        return plusSubBasis (shared_from_this<SubBSplineBasis>(), other);
    }

    SubBasis SubBSplineBasisNode::operator+ (const SubMonomialBasis& other) const {
        return plusSubBasis (shared_from_this<SubBSplineBasis>(), other);
    }

    std::vector<double> SubBSplineBasis::greville () const {return (*this)->greville();}
    std::vector<double> SubBSplineBasisNode::greville () const {
        int degree = getDegree();
        if(degree == 0){
            degree = 1;
        }

        std::vector<double> grevillePoints (getLength());
        for (int i = 0; i < getLength(); ++i) {
            grevillePoints[i] = std::accumulate(knots.begin()+i+1,knots.begin()+i+degree+1, 0.0) / degree;
        }

        return grevillePoints;
    }

    //   std::vector<double> std::make_shared<Basis>::evaluationGrid (void) const {
    //       return greville();
    //   }
    //
    //
    //   std::make_shared<Basis>::addKnots(const std::vector<double> newKnots, bool unique) const {
    //       std::vector<double> kn;
    //       if (unique){
    //           kn = addKnotsUnique(knots, newKnots);
    //       } else {
    //           kn = addKnotsNotUnique(knots, newKnots);
    //       }
    //       return std::make_shared<Basis>(kn, degree);
    //   }

    int SubBSplineBasisNode::getLength () const{
        return knots.size() - degree - 1;
    }

    SubBSplineBasis::SubBSplineBasis (const std::vector<double >& knots, int degree)  {
        assign_node(new SubBSplineBasisNode(knots, degree));
    }

    SubBSplineBasisNode::SubBSplineBasisNode (const std::vector<double >& knots, int degree)
    : SubUnivariateBasisNode(degree), knots(knots){ }

    SubBSplineBasis::SubBSplineBasis (const std::vector<double >& bounds, int degree, int numberOfIntervals)  { assign_node(new SubBSplineBasisNode(bounds, degree, numberOfIntervals)); };
    SubBSplineBasisNode::SubBSplineBasisNode (const std::vector<double >& bounds, int degree, int numberOfIntervals) : SubUnivariateBasisNode(degree) {
        int numberOfKnots = 2*degree + numberOfIntervals;
        knots.resize(numberOfKnots, 0);

        for (int i = 0; i < degree; ++i) {
            knots[i] = bounds[0];
            knots[numberOfKnots - i - 1] = bounds[1];
        }

        for (int i = 0; i < numberOfIntervals; ++i) {
            knots[degree + i] = bounds[0] + (bounds[1] - bounds[0]) * (double)i/(numberOfIntervals-1);
        }

        setKnots(knots);
    }

    std::vector< double >& SubBSplineBasis::getKnots () { return (*this)->getKnots(); }
    std::vector< double >& SubBSplineBasisNode::getKnots () {
        return knots;
    }

    const std::vector< double >& SubBSplineBasis::getKnots () const { return (*this)->getKnots(); }
    const std::vector< double >& SubBSplineBasisNode::getKnots () const {
        return knots;
    }

    void SubBSplineBasis::setKnots (std::vector< double >& knots) { return (*this)->setKnots (knots); }
    void SubBSplineBasisNode::setKnots (std::vector< double >& knots) {
        knots = knots;
    }


    AnyTensor SubBSplineBasisNode::operator() (const std::vector<AnyScalar> & x) const {
        assert(x.size()==getDimension());
        if(AnyScalar::is_double(x)) {
            return SubBasisEvalution<double>(AnyScalar::vector_double(x));
        } else {
            return SubBasisEvalution<AnyScalar>(x);
        }
    }

    std::vector< std::vector < double > > SubBSplineBasisNode::getEvaluationGrid() const {
         std::vector< std::vector < double > > grid = {};
         for(double const& point : greville()){
            grid.push_back(std::vector<double> {point});
         }
         return grid;
    }
} // namespace spline
