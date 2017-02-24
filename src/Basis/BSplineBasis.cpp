#include <sstream>
#include "BSplineBasis.h"

#include "operations/operationsBasis.h"

namespace casadi {


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
        if (i == 0) {
          return Sparsity::dense(n_knots_, 1);
        } else {
          return Sparsity::dense(1, 1);
        }
        return Sparsity();
      }

      Sparsity BSplineEvaluator::get_sparsity_out(int i) {
        if (i == 0) {
          return Sparsity::dense(n_knots_ - degree_ - 1, 1);
        }
        return Sparsity();
      }


      BSplineEvaluator::BSplineEvaluator(const std::string &name, int n_knots, int degree) :
        casadi::FunctionInternal(name), n_knots_(n_knots), degree_(degree) {}

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
}

namespace spline {

    BSplineBasisNode* BSplineBasis::get() const {
      return static_cast<BSplineBasisNode*>(SharedObject::get());
    }

    BSplineBasisNode* BSplineBasis::operator->() const { return get(); }

    std::string BSplineBasisNode::getRepresentation() const {
        std::stringstream s;
        s << "BSplineBasis object";
        return s.str();
    }

    Basis BSplineBasisNode::operator+ (const Basis& other) const {
        return other + shared_from_this<BSplineBasis>();
    }

    Basis BSplineBasisNode::operator+ (const EmptyBasis& other) const {
        return shared_from_this<BSplineBasis>();
    }

    Basis BSplineBasisNode::operator+ (const BSplineBasis& other) const {
        return plus_basis (shared_from_this<BSplineBasis>(), other);
    }

    Basis BSplineBasisNode::operator+ (const MonomialBasis& other) const {
        return plus_basis (shared_from_this<BSplineBasis>(), other);
    }

    Basis BSplineBasisNode::operator* (const Basis& other) const {
        return other * shared_from_this<BSplineBasis>();
    }

    Basis BSplineBasisNode::operator* (const EmptyBasis& other) const {
        return shared_from_this<BSplineBasis>();
    }

    Basis BSplineBasisNode::operator* (const BSplineBasis& other) const {
        return times_basis (shared_from_this<BSplineBasis>(), other);
    }

    Basis BSplineBasisNode::operator* (const MonomialBasis& other) const {
        return times_basis (shared_from_this<BSplineBasis>(), other);
    }

    std::vector<AnyScalar> BSplineBasis::greville() const { return (*this)->greville(); }
    std::vector<AnyScalar> BSplineBasisNode::greville() const {
        int deg = degree();
        if (deg == 0) {
            deg = 1;
        }
        std::vector<AnyScalar> grevillePoints(dimension());
        for (int i = 0; i < dimension(); ++i) {
          grevillePoints[i] = AnyScalar(0.0);
          for (int j = 0; j < deg; j++) {
            grevillePoints[i] += knots_[i+1+j];
          }
          grevillePoints[i] = grevillePoints[i] / deg;
        }
        return grevillePoints;
    }

    int BSplineBasisNode::dimension() const {
        return knots_.size() - degree() - 1;
    }


    BSplineBasis::BSplineBasis(const std::vector<AnyScalar>& knots, int degree) {
        spline_assert_message(knots.size() > degree+1, "Incompatible dimensions." <<
            " Got " << knots.size() << " knots and degree "  << degree << ".");
        assign_node(new BSplineBasisNode(knots, degree));
    }

    BSplineBasisNode::BSplineBasisNode(const std::vector<AnyScalar>& knots, int degree)
        : UnivariateBasisNode(degree, Interval(knots[0], knots[knots.size()-1])),
      bspline_evaluator_(casadi::BSplineEvaluator::create("f", knots.size(), degree)) {
        AnyVector kn(vertcat(knots));
        knots_ = kn.sort().to_scalar_vector();
      }

    BSplineBasis::BSplineBasis(const std::vector<AnyScalar>& bounds, int degree, int n_intervals)  {
        int n_knots = 2*degree + n_intervals;
        std::vector<AnyScalar> knot_vector(n_knots);

        for (int i = 0; i < degree; ++i) {
            knot_vector[i] = bounds[0];
            knot_vector[n_knots - i - 1] = bounds[1];
        }

        for (int i = 0; i < n_intervals; ++i) {
            double f = static_cast<double>(i)/(n_intervals-1);
            knot_vector[degree + i] = bounds[0] + (bounds[1] - bounds[0])*f;
        }
      assign_node(new BSplineBasisNode(knot_vector, degree));
    }

    std::vector<AnyScalar> BSplineBasis::knots() const { return (*this)->knots(); }
    std::vector<AnyScalar> BSplineBasisNode::knots() const {
        return knots_;
    }

    AnyTensor BSplineBasisNode::operator() (const std::vector<AnyScalar> & x) const {
        return basis_evaluation(x);
    }

    std::vector< std::vector<AnyScalar> > BSplineBasisNode::getEvaluationGrid() const {
        std::vector< std::vector<AnyScalar> > ret;
        for (auto const& point : greville()) {
          ret.push_back(std::vector<AnyScalar> {point});
        }
        return ret;
    }

    AnyTensor BSplineBasisNode::const_coeff_tensor(const AnyTensor& t) const {
        std::vector< int > coeff_size = t.dims();
        coeff_size.insert(coeff_size.begin(), 1);

        AnyTensor values = t.shape(coeff_size);

        std::vector< AnyTensor > coeffs = std::vector< AnyTensor >(dimension(), values);

        return AnyTensor::concat(coeffs, 0);
    }

    AnyTensor BSplineBasisNode::basis_evaluation(const std::vector< AnyScalar > & x_) const {
        AnyScalar x = x_[0];
        TensorType t = AnyScalar::merge(AnyScalar::type(knots_), x.type());

        if (t == TENSOR_DOUBLE) {
          casadi::DM ret = bspline_evaluator_(
            std::vector<casadi::DM>{AnyScalar::as_double(knots_), x.as_double()})[0];
          return DT(ret, {ret.numel()});
        } else if (t == TENSOR_MX) {
          casadi::MX ret = bspline_evaluator_(
            std::vector<casadi::MX>{vertcat(AnyScalar::as_MX(knots_)), x.as_MX()})[0];
          return MT(ret, {ret.numel()});
        } else if (t == TENSOR_SX) {
          casadi::SX ret = bspline_evaluator_(
            std::vector<casadi::SX>{vertcat(AnyScalar::as_SX(knots_)), x.as_SX()})[0];
          return ST(ret, {ret.numel()});
        }
        spline_assert(0);
        return AnyTensor();
    }

    Basis BSplineBasisNode::derivative(int order, AnyTensor& T) const {
        /* Computes the BSplineBasis derivative using eq. (16) in [de Boor, Chapter X, 2001].
        * Args:
        *     o (int): order of the derivative (default is 1)
        * Returns:
        *     Derivative of the basis (new_basis) and transformation matrix to transform
        *     the coefficients of the function (T)
        */
        int n_dim = dimension();  // Number of basis functions in the basis
        int n_dim_new = n_dim-1;
        int deg = degree();;

        std::vector<AnyScalar> kn = knots();
        std::vector<AnyScalar> new_knots(kn.begin() + order, kn.end() - order);
        Basis new_basis = BSplineBasis(new_knots, deg - order);  // New basis

        // initialization of data of transformation matrix
        std::vector<AnyScalar> data(n_dim*n_dim, 0);
        for (int i = 0; i < n_dim; i++) {
            data[i*(n_dim+1)] = 1.;  // to make eye matrix
        }
        AnyTensor T_ = vertcat(data).shape({n_dim, n_dim});  // initialize matrix to multiply

        AnyScalar c_j;
        for (int i = 0; i < order; i++) {
            kn.erase(kn.begin()); // remove first element
            kn.pop_back();  // remove last element

            data.resize(n_dim_new*n_dim);
            std::fill(data.begin(), data.end(), 0);

            for (int j = 0; j <= n_dim-2; j++) {
                if (deg <= 0) {
                    //  This is the case if taking the degree+n'th derivative
                    //  In that case the T-matrix is all zero
                    c_j = 0;
                } else {
                    c_j = deg/(kn[j+deg]-kn[j]);
                }
                data[j*(n_dim-1)+j] = -c_j;
                data[(j+1)*(n_dim-1)+j] = c_j;
            }
            // multiply transformation matrices, for higher order
            T_ = mtimes(vertcat(data).shape({n_dim_new, n_dim}), T_);
            n_dim_new--;
            n_dim--;
            deg--;
        }
        T = T_;
        return new_basis;
    }

    Basis BSplineBasisNode::antiderivative(int order, AnyTensor& T) const {
        int n_dim = dimension();
        int n_dim_new = n_dim;
        int deg = degree();
        std::vector<AnyScalar> kn = knots();
        // construct coefficient transformation matrix
        std::vector<AnyScalar> data(n_dim*n_dim, 0);
        for (int i=0; i < n_dim; i++) {
            data[i*(n_dim+1)] = 1.;
        }
        AnyTensor T_ = vertcat(data).shape({n_dim, n_dim});
        AnyScalar val;
        for (int k = 0; k < order; k++) {
            deg++;
            n_dim_new++;
            data.resize(n_dim_new*n_dim);
            std::fill(data.begin(), data.end(), 0);
            for (int i = 0;  i < n_dim; i++) {
              val = (kn[i+deg] - kn[i])/deg;
              for (int j = i*(n_dim+1)+i+1; j <= i*(n_dim+1)+n_dim; j++) {
                data[j] = val;
              }
            }
            T_ = mtimes(vertcat(data).shape({n_dim_new, n_dim}), T_);
            kn.insert(kn.begin(), kn[0]);
            kn.insert(kn.end(), kn[kn.size()-1]);
            n_dim++;
        }
        T = T_;
        // construct new basis
        return BSplineBasis(kn, deg);
    }

    AnyTensor BSplineBasisNode::integral(const Interval& dom) const {
        if (dom == domain()){
            int n_dim = dimension();
            int deg = degree();
            std::vector<AnyScalar> kn = knots();
            std::vector<AnyScalar> data(n_dim, 0);
            for (int i=0; i<n_dim; i++) {
                data[i] = (kn[i+deg+1] - kn[i])/(deg+1);
            }
            return vertcat(data).shape({1, n_dim});
        } else {
            AnyTensor T;
            Basis basis_int = antiderivative(1, T);
            return (basis_int({dom.max()}) - basis_int({dom.min()})).shape({1, dimension()+1}).mtimes(T);
        }
    }

    Basis BSplineBasisNode::insert_knots(const AnyVector & new_knots,
      AnyTensor & T) const {
      // construct coefficient transformation matrix
      int n_dim = dimension();
      int n_dim_new = n_dim;
      int deg = degree();
      std::vector<AnyScalar> data(n_dim*n_dim, 0);
      for (int i = 0; i < n_dim; i++) {
        data[i*(n_dim+1)] = 1.;
      }
      AnyTensor T_ = vertcat(data).shape({n_dim, n_dim});
      AnyScalar val;
      std::vector<AnyScalar> knots;
      BSplineBasis ret = shared_from_this<BSplineBasis>();
      for (int k = 0; k < new_knots.size(); k++) {
        std::vector<AnyScalar> knots = ret.knots();
        n_dim_new++;
        data.resize(n_dim_new*n_dim);
        std::fill(data.begin(), data.end(), 0);
        for (int i = 1; i < n_dim; i++) {
          val = (new_knots[k] - knots[i])/(knots[i+deg] - knots[i]);
          data[i*(n_dim_new+1)] = AnyScalar::max(0., AnyScalar::min(1., val));
          val = (knots[i+deg] - new_knots[k])/(knots[i+deg] - knots[i]);
          data[(i-1)*(n_dim_new+1)+1] = AnyScalar::max(0., AnyScalar::min(1., val));
        }
        data[0] = AnyScalar(1);
        data[n_dim_new*n_dim-1] = AnyScalar(1);
        T_ = mtimes(vertcat(data).shape({n_dim_new, n_dim}), T_);
        n_dim++;
        // construct new basis
        knots.push_back(new_knots[k]);
        ret = BSplineBasis(knots, degree());
      }
      T = T_;
      return ret;
    }

    Basis BSplineBasisNode::midpoint_refinement(int refinement, AnyTensor& T) const {
      // check if numeric knots
      spline_assert_message(AnyScalar::is_double(knots()),
        "Midpoint refinement only possible with numeric knot sequence.");
      std::vector<double> kn = AnyScalar::as_double(knots());
      // build inserted knot vector
      std::vector<AnyScalar> new_knots(0);
      int j;
      for (int i = 0; i < kn.size(); i+=j) {
          j = 1;
          while ((i+j < kn.size()) && (kn[i+j] == kn[i])) {
            j++;
          }
          if (i+j < kn.size()) {
            double den = pow(2.0, refinement);
            for (int num = 1; num < den; num++) {
                new_knots.push_back(((den - 1.*num)/den)*kn[i]+((1.*num)/den)*kn[i+j]);
            }
          }
      }
      // invoke knot insertion
      return insert_knots(vertcat(new_knots), T);
    }

    Basis BSplineBasisNode::degree_elevation(int elevation, AnyTensor& T) const {
      	// check if numeric knots
        spline_assert_message(AnyScalar::is_double(knots()),
            "Degree elevation only possible with numeric knot sequence.");
        std::vector<double> kn = AnyScalar::as_double(knots());
        // construct new basis
        std::vector<AnyScalar> new_knots(0);
        int j;
        for (int i=0; i<kn.size(); i+=j) {
            j = 0;
            while ((i+j < kn.size()) && (kn[i+j] == kn[i])) {
                new_knots.push_back(kn[i+j]);
                j++;
            }
            if (i+j <= kn.size()) {
                for (int k=0; k<elevation; k++) {
                    new_knots.push_back(kn[i]);
                }
            }
        }
        BSplineBasis new_basis = BSplineBasis(new_knots, degree()+elevation);
        // project into new basis
        T = project_to(new_basis);
        return new_basis;
    }

    Basis BSplineBasisNode::kick_boundary(const Interval& boundary, AnyTensor& T) const {
        // check if numeric knots
        spline_assert_message(AnyScalar::is_double(knots()),
            "Kick boundary only possible with numeric knot sequence.");
        std::vector<double> kn = AnyScalar::as_double(knots());
        int n_lb = 1;
        while ((n_lb < kn.size()) && (kn[n_lb] == kn[0])) {
            n_lb++;
        }
        int n_ub = 1;
        while ((n_ub < kn.size()) && (kn[kn.size()-n_ub-1] == kn[kn.size()-1])) {
            n_ub++;
        }
        // construct new basis
        std::vector<AnyScalar> new_knots(knots());
        for (int i=0; i<n_lb; i++) {
            new_knots[i] = boundary.min();
        }
        for (int i=0; i<n_ub; i++) {
            new_knots[kn.size()-1-i] = boundary.max();
        }
        BSplineBasis new_basis = BSplineBasis(new_knots, degree());
        // project into new basis
        // T = project_to(new_basis);
        return new_basis;
    }

} // namespace spline
