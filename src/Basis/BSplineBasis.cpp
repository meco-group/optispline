#include <sstream>
#include "BSplineBasis.h"

#include "operations/operationsBasis.h"

namespace spline {


    casadi::Function  BSplineEvaluator::create(const std::string &name, int n_knots, int degree,
        const Dict& opts) {
      casadi::Function ret;
      ret.assignNode(new BSplineEvaluator(name, n_knots, degree));
      ret->construct(opts);
      return ret;
    }

    size_t BSplineEvaluator::get_n_in() { return 2; }
    size_t BSplineEvaluator::get_n_out() { return 1; }

    Sparsity BSplineEvaluator::get_sparsity_in(int i) {
      if (i==0) {
        return Sparsity::dense(n_knots_, 1);
      } else {
        return Sparsity::dense(1, 1);
      }
      return Sparsity();
    }

    Sparsity BSplineEvaluator::get_sparsity_out(int i) {
      if (i==0) {
        return Sparsity::dense(n_knots_ - degree_ - 1, 1);
      }
      return Sparsity();
    }


    BSplineEvaluator::BSplineEvaluator(const std::string &name, int n_knots, int degree) :
      casadi::FunctionInternal(name), n_knots_(n_knots), degree_(degree) {

    }

    void BSplineEvaluator::print(std::ostream &stream) const {
      stream << "BSpline(" << n_knots_ << "," << degree_ << ")";
    }

    void BSplineEvaluator::init(const Dict& opts) {
      casadi::FunctionInternal::init(opts);
      alloc_w((degree_+1)*(n_knots_-1), true);
    }

    Function BSplineEvaluator::getFullJacobian(const std::string& name, const Dict& opts) {
      int n = n_knots_ - degree_ - 1;
      MX knots = MX::sym("knots", n_knots_);
      MX x = MX::sym("x");

      MX delta_knots = knots(range(1+degree_, n_knots_-1))
                       - knots(range(1, n_knots_-degree_-1));

      Sparsity sp_diag = vertsplit(Sparsity::diag(n), {0, n-1, n})[0];
      Sparsity sp_band = vertsplit(Sparsity::band(n, -1), {0, n-1, n})[0];

      MX delta_knots_inv = 1/delta_knots;
      MX T = MX(sp_diag, -delta_knots_inv) + MX(sp_band, delta_knots_inv);

      Function bspline_evaluator = BSplineEvaluator::create("f", n_knots_-2, degree_-1);

      MX res = bspline_evaluator({knots(range(1, n_knots_-1)), x})[0];

      MX J = mtimes(T.T(), res);
      return Function("J", {knots, x}, { horzcat(MX(n, n_knots_), degree_*J) });
    }

    void BSplineEvaluator::eval_sx(const SXElem** arg, SXElem** res, int* iw, SXElem* w, int mem) {
      eval_generic<SXElem>(0, arg, res, iw, w);
    }

    void BSplineEvaluator::eval(void* mem, const double** arg, double** res,
        int* iw, double* w) const {
      eval_generic<double>(mem, arg, res, iw, w);
    }

    BSplineBasisNode* BSplineBasis::get() const {
      return static_cast<BSplineBasisNode*>(SharedObject::get());
    };

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

    AnyVector BSplineBasis::greville() const { return (*this)->greville(); }
    AnyVector BSplineBasisNode::greville() const {
        int degree = getDegree();
        if (degree == 0) {
            degree = 1;
        }

        AnyVector grevillePoints(getLength());
        for (int i = 0; i < getLength(); ++i) {
            grevillePoints[i] = std::accumulate(
                                  knots_.begin()+i+1, knots_.begin()+i+degree+1, AnyScalar(0.0))
                                / degree;
        }

        return grevillePoints;
    }

    int BSplineBasisNode::getLength() const {
        return knots_.size() - degree - 1;
    }

    BSplineBasis::BSplineBasis(const AnyVector& knots, int degree) {
        assign_node(new BSplineBasisNode(knots, degree));
    }

    BSplineBasisNode::BSplineBasisNode(const AnyVector& knots, int degree)
        : UnivariateBasisNode(degree),
      bspline_evaluator_(BSplineEvaluator::create("f", knots.size(), degree) ),
      knots_(knots) { }

    BSplineBasis::BSplineBasis(const AnyVector& bounds, int degree, int numberOfIntervals)  {
      assign_node(new BSplineBasisNode(bounds, degree, numberOfIntervals));
    };

    BSplineBasisNode::BSplineBasisNode(const AnyVector& bounds, int degree, int numberOfIntervals)
          : UnivariateBasisNode(degree) {
        int numberOfKnots = 2*degree + numberOfIntervals;
        knots_.resize(numberOfKnots, 0);

        for (int i = 0; i < degree; ++i) {
            knots_[i] = bounds[0];
            knots_[numberOfKnots - i - 1] = bounds[1];
        }

        for (int i = 0; i < numberOfIntervals; ++i) {
            double f = static_cast<double>(i)/(numberOfIntervals-1);
            knots_[degree + i] = bounds[0] + (bounds[1] - bounds[0])*f;
        }

        setKnots(knots_);
    }

    AnyVector BSplineBasis::getKnots() const { return (*this)->getKnots(); }
    AnyVector BSplineBasisNode::getKnots() const {
        return knots_;
    }

    void BSplineBasis::setKnots(const AnyVector& knots) { return (*this)->setKnots (knots); }
    void BSplineBasisNode::setKnots(const AnyVector& knots) {
        knots_ = knots;
    }


    AnyTensor BSplineBasisNode::operator() (const AnyVector & x) const {
        assert(x.size()==getNumberOfSubBasis());
        return SubBasisEvalution(x);
    }

    std::vector< std::vector < AnyScalar > > BSplineBasisNode::getEvaluationGrid() const {
        std::vector< std::vector < AnyScalar > >ret;
        for (auto const& point : greville()) {
            ret.push_back(std::vector<AnyScalar> {point});
        }
        return ret;
    }



    AnyTensor BSplineBasisNode::SubBasisEvalution(const std::vector< AnyScalar > & x_) const {
        spline_assert(x_.size()>0);
        AnyScalar x = x_[0];
        TensorType t = AnyScalar::merge(AnyScalar::type(knots_), x.type());

        if (t==TENSOR_DOUBLE) {
          DM ret = bspline_evaluator_(
            std::vector<DM>{AnyScalar::as_double(knots_), x.as_double()})[0];
          return DT(ret, {ret.numel()});
        } else if (t==TENSOR_MX) {
          MX ret = bspline_evaluator_(
            std::vector<MX>{vertcat(AnyScalar::as_MX(knots_)), x.as_MX()})[0];
          return MT(ret, {ret.numel()});
        } else if (t==TENSOR_SX) {
          SX ret = bspline_evaluator_(
            std::vector<SX>{vertcat(AnyScalar::as_SX(knots_)), x.as_SX()})[0];
          return ST(ret, {ret.numel()});
        }
        spline_assert(0);
        return AnyTensor();
    }
} // namespace spline
