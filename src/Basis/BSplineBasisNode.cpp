#include <sstream>
#include "BSplineBasis.h"
#include "BSplineBasisNode.h"

#include "operations/operationsBasis.h"

namespace casadi {

      casadi_int BSplineEvaluator::get_n_in() { return 2; }
      casadi_int BSplineEvaluator::get_n_out() { return 1; }

      Sparsity BSplineEvaluator::get_sparsity_in(casadi_int i) {
        if (i == 0) {
          return Sparsity::dense(n_knots_, 1);
        } else {
          return Sparsity::dense(1, 1);
        }
        return Sparsity();
      }

      Sparsity BSplineEvaluator::get_sparsity_out(casadi_int i) {
        if (i == 0) {
          return Sparsity::dense(n_knots_ - degree_ - 1, 1);
        }
        return Sparsity();
      }


      BSplineEvaluator::BSplineEvaluator(casadi_int n_knots, casadi_int degree) :
        n_knots_(n_knots), degree_(degree) {
           construct("BSplineEvaluator");
      }

      void BSplineEvaluator::init() {
        casadi::Callback::init();
        alloc_w((degree_+1)*(n_knots_-1), true);
      }

      Function BSplineEvaluator::get_jacobian(const std::string& name,
                                  const std::vector<std::string>& inames,
                                  const std::vector<std::string>& onames,
                                  const Dict& opts) const {
        casadi_int n = n_knots_ - degree_ - 1;
        MX knots = MX::sym("knots", n_knots_);
        MX x = MX::sym("x");

        MX delta_knots = knots(range(1+degree_, n_knots_-1))
                         - knots(range(1, n_knots_-degree_-1));

        Sparsity sp_diag = vertsplit(Sparsity::diag(n), {0, n-1, n})[0];
        Sparsity sp_band = vertsplit(Sparsity::band(n, -1), {0, n-1, n})[0];

        MX delta_knots_inv = 1/delta_knots;
        MX T = MX(sp_diag, -delta_knots_inv) + MX(sp_band, delta_knots_inv);

        Callback& bspline_evaluator = BSplineEvaluator::construct_cached(n_knots_-2, degree_-1);

        MX res = bspline_evaluator({knots(range(1, n_knots_-1)), x})[0];

        MX J = MX::mtimes(T.T(), res);
        return Function("J", {knots, x, MX::sym("dummy", sparsity_out(0))}, { horzcat(MX(n, n_knots_), degree_*J) });
      }

      int BSplineEvaluator::eval_sx(const SXElem** arg, SXElem** res, casadi_int* iw, SXElem* w, void* mem) const {
        eval_generic<SXElem>(0, arg, res, iw, w);
        return 0;
      }

      int BSplineEvaluator::eval(const double** arg, double** res,
          casadi_int* iw, double* w, void* mem) const {
        eval_generic<double>(mem, arg, res, iw, w);
        return 0;
      }


      BSplineEvaluator& BSplineEvaluator::construct_cached(casadi_int n_knots, casadi_int degree) {
        static std::map<std::pair<casadi_int, casadi_int>, std::unique_ptr<BSplineEvaluator> > cache;
        std::pair<casadi_int, casadi_int> key = {n_knots, degree};
        auto it = cache.find(key);
        if (it==cache.end()) cache[key] = std::unique_ptr<BSplineEvaluator>(new casadi::BSplineEvaluator(key.first, key.second));
        it = cache.find(key);
        return *it->second;
      }
}

namespace spline {

    std::string BSplineBasisNode::to_string() const {
        return "BSplineBasis of degree " + std::to_string(degree()) +
               ", with " + std::to_string(knots().size()) + " knots, on "
               + domain().to_string();
    }

    Basis BSplineBasisNode::operator+ (const BasisNode& other) const {
        return other + *this;
    }

    Basis BSplineBasisNode::operator+ (const EmptyBasisNode& other) const {
        return shared_from_this<BSplineBasis>();
    }

    Basis BSplineBasisNode::operator+ (const BSplineBasisNode& other) const {
        return plus_basis (*this, other);
    }

    Basis BSplineBasisNode::operator+ (const MonomialBasisNode& other) const {
        return plus_basis (*this, other);
    }

    Basis BSplineBasisNode::operator* (const BasisNode& other) const {
        return other * *this;
    }

    Basis BSplineBasisNode::operator* (const EmptyBasisNode& other) const {
        return shared_from_this<BSplineBasis>();
    }

    Basis BSplineBasisNode::operator* (const BSplineBasisNode& other) const {
        return times_basis (*this, other);
    }

    Basis BSplineBasisNode::operator* (const MonomialBasisNode& other) const {
        return times_basis (*this, other);
    }

    bool BSplineBasisNode::operator== (const BasisNode& other) const {
      spline_assert_message(false, type() << " == ");
      return false;
    }

    bool BSplineBasisNode::operator== (const EmptyBasisNode& other) const {
      spline_assert_message(false, type() << " == ");
      return false;
    }

    bool BSplineBasisNode::operator== (const BSplineBasisNode& other) const {
      spline_assert_message(false, type() << " == ");
      return false;
    }

    bool BSplineBasisNode::operator== (const MonomialBasisNode& other) const {
      spline_assert_message(false, type() << " == ");
      return false;
    }

    std::vector<AnyScalar> BSplineBasisNode::greville() const {
        casadi_int deg = degree();
        std::vector<AnyScalar> grevillePoints(dimension());
        for (casadi_int i = 0; i < dimension(); ++i) {
          grevillePoints[i] = AnyScalar(0.0);
          for (casadi_int j = 0; j < deg; j++) {
            grevillePoints[i] += knots_[i+1+j];
          }
          grevillePoints[i] = grevillePoints[i] / deg;
        }
        return grevillePoints;
    }

    casadi_int BSplineBasisNode::dimension() const {
        return knots_.size() - degree() - 1;
    }


    BSplineBasisNode::BSplineBasisNode(const std::vector<AnyScalar>& knots, casadi_int degree)
        : UnivariateBasisNode(degree, Interval(knots[degree], knots[knots.size()-degree-1])),
      bspline_evaluator_(casadi::BSplineEvaluator::construct_cached(knots.size(), degree)) {
        AnyVector kn(vertcat(knots));
        knots_ = kn.sort().to_scalar_vector();
      }

    std::vector<AnyScalar> BSplineBasisNode::knots() const {
        return knots_;
    }

    AnyTensor BSplineBasisNode::operator() (const std::vector<AnyScalar> & x) const {
        return basis_evaluation(x);
    }

    AnyTensor BSplineBasisNode::evaluation_grid() const {
        if (degree() == 0) {
            std::vector<AnyScalar> grevillePoints(dimension());
            for (casadi_int i = 0; i < dimension(); ++i) {
                grevillePoints[i] = AnyScalar(0.0);
                for (casadi_int j = 0; j < 2; j++) {
                    grevillePoints[i] += knots_[i+j];
                }
                grevillePoints[i] = grevillePoints[i] / 2;
            }
            return AnyVector(grevillePoints).shape(std::vector< casadi_int > {dimension(), 1}) ;
        }
        return AnyVector(greville()).uniquify().perturbation().shape(std::vector< casadi_int > {dimension(), 1}) ;
    }

    AnyTensor BSplineBasisNode::const_coeff_tensor(const AnyTensor& t) const {
        std::vector< casadi_int > coeff_size = t.dims();
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

    template<typename T>
    T derivative_transform(const T& knots, casadi_int degree, casadi_int order) {
      tensor_assert(order>0);
      casadi_int n_knots = knots.numel();
      casadi_int n = n_knots - degree - 1;

      T delta_knots = knots(casadi::range(1+degree, n_knots-1))
                       - knots(casadi::range(1, n_knots-degree-1));

      casadi::Sparsity sp_diag = vertsplit(casadi::Sparsity::diag(n), {0, n-1, n})[0];
      casadi::Sparsity sp_band = vertsplit(casadi::Sparsity::band(n, -1), {0, n-1, n})[0];

      T delta_knots_inv = degree>0? degree/delta_knots : 0;

      T res = T(sp_diag, -delta_knots_inv) + T(sp_band, delta_knots_inv);

      if (order>1) {
        T new_knots = knots(casadi::range(1, n_knots-1));
        res = mtimes(derivative_transform(new_knots, degree-1, order-1), res);
      }
      return res;
    }

    Basis BSplineBasisNode::derivative(casadi_int order, AnyTensor& T) const {
        /* Computes the BSplineBasis derivative using eq. (16) in [de Boor, Chapter X, 2001].
        * Args:
        *     o (casadi_int): order of the derivative (default is 1)
        * Returns:
        *     Derivative of the basis (new_basis) and transformation matrix to transform
        *     the coefficients of the function (T)
        */
        casadi_int deg = degree();
        if (order > deg) {
            order = deg;
        }

        std::vector<AnyScalar> kn = knots();
        std::vector<AnyScalar> new_knots(kn.begin() + order, kn.end() - order);
        Basis new_basis = BSplineBasis(new_knots, deg - order);  // New basis

        AnyTensor knots = vertcat(kn);

        if (knots.is_DT()) {
          T = DT(densify(derivative_transform<casadi::DM>(knots.as_DT().matrix(), deg, order)));
        } else if (knots.is_ST()) {
          T = ST(densify(derivative_transform<casadi::SX>(knots.as_ST().matrix(), deg, order)));
        } else if (knots.is_MT()) {
          T = MT(densify(derivative_transform<casadi::MX>(knots.as_MT().matrix(), deg, order)));
        }

        return new_basis;
    }

    Basis BSplineBasisNode::antiderivative(casadi_int order, AnyTensor& T) const {
        casadi_int n_dim = dimension();
        casadi_int n_dim_new = n_dim;
        casadi_int deg = degree();
        std::vector<AnyScalar> kn = knots();
        // construct coefficient transformation matrix
        std::vector<AnyScalar> data(n_dim*n_dim, 0);
        for (casadi_int i=0; i < n_dim; i++) {
            data[i*(n_dim+1)] = 1.;
        }
        AnyTensor T_ = vertcat(data).shape({n_dim, n_dim});
        AnyScalar val;
        for (casadi_int k = 0; k < order; k++) {
            deg++;
            n_dim_new++;
            data.resize(n_dim_new*n_dim);
            std::fill(data.begin(), data.end(), 0);
            for (casadi_int i = 0;  i < n_dim; i++) {
              val = (kn[i+deg] - kn[i])/deg;
              for (casadi_int j = i*(n_dim+1)+i+1; j <= i*(n_dim+1)+n_dim; j++) {
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
        Interval dom_int = domain().intersection(dom);
        if (dom_int == domain()){
            casadi_int n_dim = dimension();
            casadi_int deg = degree();
            std::vector<AnyScalar> kn = knots();
            std::vector<AnyScalar> data(n_dim, 0);
            for (casadi_int i=0; i<n_dim; i++) {
                data[i] = (kn[i+deg+1] - kn[i])/(deg+1);
            }
            return vertcat(data).shape({1, n_dim});
        } else {
            AnyTensor T;
            Basis basis_int = antiderivative(1, T);
            return (basis_int(AnyVector( dom_int.max() )) - basis_int(AnyVector( dom_int.min() ))).shape({1, dimension()+1}).mtimes(T);
        }
    }

    Basis BSplineBasisNode::insert_knots(const AnyVector & new_knots,
      AnyTensor & T) const {
      // construct coefficient transformation matrix
      casadi_int n_dim = dimension();
      casadi_int n_dim_new = n_dim;
      casadi_int deg = degree();
      std::vector<AnyScalar> data(n_dim*n_dim, 0);
      for (casadi_int i = 0; i < n_dim; i++) {
        data[i*(n_dim+1)] = 1.;
      }
      AnyTensor T_ = vertcat(data).shape({n_dim, n_dim});
      AnyScalar val;
      std::vector<AnyScalar> knots;
      BSplineBasis ret = shared_from_this<BSplineBasis>();
      for (casadi_int k = 0; k < new_knots.size(); k++) {
        std::vector<AnyScalar> knots = ret.knots();
        n_dim_new++;
        data.resize(n_dim_new*n_dim);
        std::fill(data.begin(), data.end(), 0);
        for (casadi_int i = 1; i < n_dim; i++) {
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

    Basis BSplineBasisNode::midpoint_refinement(casadi_int refinement, AnyTensor& T) const {
      // check if numeric knots
      spline_assert_message(AnyScalar::is_double(knots()),
        "Midpoint refinement only possible with numeric knot sequence.");
      std::vector<double> kn = AnyScalar::as_double(knots());
      // build inserted knot vector
      std::vector<AnyScalar> new_knots(0);
      casadi_int j;
      for (casadi_int i = 0; i < kn.size(); i+=j) {
          j = 1;
          while ((i+j < kn.size()) && (kn[i+j] == kn[i])) {
            j++;
          }
          if (i+j < kn.size()) {
            double den = pow(2.0, refinement);
            for (casadi_int num = 1; num < den; num++) {
                new_knots.push_back(((den - 1.*num)/den)*kn[i]+((1.*num)/den)*kn[i+j]);
            }
          }
      }
      // invoke knot insertion
      return insert_knots(vertcat(new_knots), T);
    }

    Basis BSplineBasisNode::degree_elevation(casadi_int elevation, AnyTensor& T) const {
        // check if numeric knots
        spline_assert_message(AnyScalar::is_double(knots()),
            "Degree elevation only possible with numeric knot sequence.");
        std::vector<double> kn = AnyScalar::as_double(knots());
        // construct new basis
        std::vector<AnyScalar> new_knots(0);
        casadi_int j;
        for (casadi_int i=0; i<kn.size(); i+=j) {
            j = 0;
            while ((i+j < kn.size()) && (kn[i+j] == kn[i])) {
                new_knots.push_back(kn[i+j]);
                j++;
            }
            if (i+j <= kn.size()) {
                for (casadi_int k=0; k<elevation; k++) {
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
        std::vector<AnyScalar> kn = knots();
        casadi_int deg = degree();
        // check on validity of kicking when numeric
        if (boundary.min().is_double() && kn[deg+1].is_double()) {
            spline_assert_message(boundary.min().as_double() < kn[deg+1].as_double(),
                "New boundary can not pass a knot.");
        }
        if (boundary.max().is_double() && kn[kn.size()-1-deg].is_double()) {
            spline_assert_message(boundary.max().as_double() > kn[kn.size()-2-deg].as_double(),
                "New boundary can not pass a knot.");
        }
        // construct new basis
        std::vector<AnyScalar> new_knots(kn);
        for (casadi_int i=0; i<deg+1; i++) {
            new_knots[i] = boundary.min();
        }
        for (casadi_int i=0; i<deg+1; i++) {
            new_knots[kn.size()-1-i] = boundary.max();
        }
        BSplineBasis new_basis = BSplineBasis(new_knots, degree());
        // project into new basis
        T = project_to(new_basis);
        return new_basis;
    }

} // namespace spline
