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

    std::vector<AnyScalar> BSplineBasis::greville() const { return (*this)->greville(); }
    std::vector<AnyScalar> BSplineBasisNode::greville() const {
        int degree = getDegree();
        if (degree == 0) {
            degree = 1;
        }
        std::vector<AnyScalar> grevillePoints(getLength());
        for (int i = 0; i < getLength(); ++i) {
          grevillePoints[i] = AnyScalar(0.0);
          for (int j = 0; j < degree; j++){
            grevillePoints[i] += knots_[i+1+j];
          }
          grevillePoints[i] = grevillePoints[i] / degree;
        }
        return grevillePoints;
    }

    int BSplineBasisNode::getLength() const {
        return knots_.size() - degree - 1;
    }

    BSplineBasis::BSplineBasis(const std::vector<AnyScalar>& knots, int degree) {
        assign_node(new BSplineBasisNode(knots, degree));
    }

    BSplineBasisNode::BSplineBasisNode(const std::vector<AnyScalar>& knots, int degree)
        : UnivariateBasisNode(degree),
      bspline_evaluator_(BSplineEvaluator::create("f", knots.size(), degree) ){
        setKnots(knots);
    }

    BSplineBasis::BSplineBasis(const std::vector<AnyScalar>& bounds, int degree, int numberOfIntervals)  {
      assign_node(new BSplineBasisNode(bounds, degree, numberOfIntervals));
    };

    BSplineBasisNode::BSplineBasisNode(const std::vector<AnyScalar>& bounds, int degree, int numberOfIntervals)
          : UnivariateBasisNode(degree) {
        int numberOfKnots = 2*degree + numberOfIntervals;
        std::vector<AnyScalar> knot_vector(numberOfKnots);

        for (int i = 0; i < degree; ++i) {
            knot_vector[i] = bounds[0];
            knot_vector[numberOfKnots - i - 1] = bounds[1];
        }

        for (int i = 0; i < numberOfIntervals; ++i) {
            double f = static_cast<double>(i)/(numberOfIntervals-1);
            knot_vector[degree + i] = bounds[0] + (bounds[1] - bounds[0])*f;
        }
        setKnots(knot_vector);
    }

    std::vector<AnyScalar> BSplineBasis::getKnots() const { return (*this)->getKnots(); }
    std::vector<AnyScalar> BSplineBasisNode::getKnots() const {
        return knots_;
    }

    void BSplineBasis::setKnots(const std::vector<AnyScalar>& knots) { return (*this)->setKnots (knots); }
    void BSplineBasisNode::setKnots(const std::vector<AnyScalar>& knots) {
        AnyVector kn(vertcat(knots));
        knots_ = kn.sort().to_scalar_vector();
    }


    AnyTensor BSplineBasisNode::operator() (const std::vector<AnyScalar> & x) const {
        return SubBasisEvalution(x);
    }

    std::vector< std::vector<AnyScalar> > BSplineBasisNode::getEvaluationGrid() const {
        std::vector< std::vector<AnyScalar> > ret;
        for (auto const& point : greville()) {
          ret.push_back(std::vector<AnyScalar> {point});
        }
        return ret;
    }

    AnyTensor BSplineBasisNode::SubBasisEvalution(const std::vector< AnyScalar > & x_) const {
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

    Basis BSplineBasisNode::univariate_derivative(int order, AnyTensor& SWIG_OUTPUT(T)) const {


        // Computes the BSplineBasis derivative using eq. (16) in [de Boor, Chapter X, 2001].
        // Args:
            // o (int): order of the derivative (default is 1)
        // Returns:
            // Derivative of the basis (new_basis) and transformation matrix to transform the coefficients of the function (T)
        
        // int curr_degree = this->getDegree();;
        // std::vector<double> curr_knots = this->getKnots();
        // std::vector<double> new_knots(curr_knots.begin() + order, curr_knots.end() - order);
        // Basis new_basis = BSplineBasis(new_knots, curr_degree - order);  // New basis
        // int basis_length = (*this)->dimension()
        // // T.reshape(eye(basis_length)) // Reshape transformation matrix

        // std::vector<int> delta_knots;
        // AnyTensor R;
        // std::vector<int> j;
        // for (int i=0; i<=order; i++){
        //     curr_knots.erase(curr_knots.begin()); // remove first element
        //     curr_knots.pop_back();  // remove last element
        //     for (int l=0; l<=curr_knots.size()-curr_degree-i; l++){
        //         delta_knots[l] = curr_knots[curr_degree - i + l] - curr_knots[-curr_degree + i -l];
        //     }
        //     // Todo: how? 
        //     R = AnyTensor(DTensor([0*len],[basis_length - 1 - i, basis_length - i]))
        //     for (int l=0; l<=basis_length-1-i; l++){
        //         j.push_back(l);
        //     }
        //     for (int l=0; j.size(); l++){
        //         R[(j[l], j[l])] = -1. / delta_knots[l]
        //         R[(j[l], j[l] + 1)] = 1. / delta_knots[l]
        //     }
        //     // write in for loop
        //     T = (curr_degree - i) * np.dot(R, T)
        // }
        // T = AnyTensor(DTensor(coeffs, new_degree));  // Transformation tensor to apply on coefficients of function
        // return new_basis;
        return shared_from_this<BSplineBasis>();

    }

} // namespace spline
