#include "Function.h"
#include "FunctionNode.h"

#include "../Basis/BSplineBasis.h"
#include "../Basis/BSplineBasisNode.h"

namespace spline{

    Function::Function(){}
    Function::Function(const TensorBasis& basis, const Coefficient& coeff) {
        std::vector< casadi_int > dim_basis = basis.dimension();
        std::vector< casadi_int > dim_coef = coeff.data().dims();

        casadi_int total_size_coef = dim_coef.size();
        spline_assert_message(dim_basis.size() <= total_size_coef,
                "Dimensions of basis " << dim_basis << " and coefficient " <<
                dim_coef << " can not be connected.");
        spline_assert_message(dim_basis.size() + 2 >= total_size_coef,
                "Dimensions of basis " << dim_basis << " and coefficient " <<
                dim_coef << " can not be connected.");
        for (casadi_int i = 0; i < dim_basis.size(); i++) {
            spline_assert_message(dim_basis[i] == dim_coef[i],
                    "Mismatch of dimention " + std::to_string(i) + " between basis and coefficient: "
                    << "Got basis " << dim_basis << " and coefficient" << dim_coef <<".");
        }

        assign_node(new FunctionNode(basis, coeff.add_trival_dimension(2 + dim_basis.size() - dim_coef.size())));
    }

    Function::Function(const AnyTensor& tensor) {
        spline_assert_message(tensor.dims().size() <= 2, "Tensor constant not supported ");
        assign_node(new FunctionNode(tensor));
    }

    Function::Function(const AnyScalar& value, const std::vector< casadi_int >& size) {
        assign_node(new FunctionNode(AnyTensor::repeat(AnyTensor(value), size)));
    }

    FunctionNode* Function::get() const { return static_cast<FunctionNode*>(SharedObject::get()); };
    FunctionNode* Function::operator->() const { return get(); }

    std::string Function::type() const { return (*this)->type() ;}

    casadi::MX Function::operator<=(const casadi::MX& x) const {
        return operator-().operator>=(-x);
    }
    casadi::MX Function::operator>=(const casadi::MX& x) const {
        if (shape()[0]>1 && shape()[1]>1) {
          spline_assert(shape()[0]==shape()[1]);
          MT a = coeff_tensor().as_MT();
          casadi_int n = coeff().dimension().size();
          std::vector<casadi_int> reorder = casadi::range(n);
          reorder.insert(reorder.begin(), n);
          reorder.insert(reorder.begin(), n+1);
          casadi::MX b = a.reorder_dims(reorder).shape({shape()[0], casadi::product(coeff().dimension())*shape()[1]}).matrix();
          std::vector<casadi::MX> components = horzsplit(b, shape()[1]);
          return diagcat(components)>x;
        } else {
          return coeff_tensor().as_MT().data()>=x;
        }
    }

    casadi::MX Function::operator==(const casadi::MX& x) const {
      return coeff_tensor().as_MT().data()==x;
    }

    AnyTensor Function::operator()(const AnyTensor& x, const std::vector< Argument >& args ) const {
        return (*this)->operator()(x, args);
    }

    AnyTensor Function::list_eval(const AnyTensor& x, const std::vector< Argument >& args ) const {
        return (*this)->list_eval(x, args);
    }

    AnyTensor Function::grid_eval(const std::vector< AnyTensor >& x, const std::vector< Argument >& args,  bool squeeze_return) const {
        return (*this)->grid_eval(x, args, squeeze_return);
    }

    Function Function::partial_eval(const AnyTensor& x, const Argument& args ) const {
        return (*this)->partial_eval(x, args);
    }

    Function Function::operator+(const Function& f) const { return (*this)->operator+(f) ;}
    Function Function::operator+(const AnyTensor& t) const {
        if (t.is_scalar() && t.dims()!=shape())
            return operator+(AnyTensor::repeat(t.as_scalar(), shape()));
        return operator+(Function(t));
    }

    Function Function::operator*(const Function& f) const { return (*this)->operator*(f) ;}
    Function Function::operator*(const AnyTensor& t) const {
        if (t.is_scalar() && t.dims()!=shape())
            return operator*(AnyTensor::repeat(t.as_scalar(), shape()));
        return operator*(Function(t));
    }

    Function Function::operator-(const Function& f) const { return (*this)->operator+(-f) ;}
    Function Function::operator-(const AnyTensor& t) const { return operator+(-t) ;}
    Function Function::operator-() const { return (*this)->operator-() ;}

    Function Function::mtimes(const Function& f) const {
        if (f.is_scalar() || is_scalar())
            return operator*(f);
        return (*this)->mtimes(f);
    }

    Function Function::mtimes(const AnyTensor& t) const { return mtimes(Function(t)); }

    Function Function::rmtimes(const AnyTensor& t) const {
        if (t.is_scalar() && t.dims()!=shape())
            return operator*(t);
        return (transpose().mtimes(Function(t).transpose())).transpose();
    }

    Function Function::pow(casadi_int power) const {
        spline_assert_message(power >= 0, "No negative power");
        Function r = Function(1, shape());
        for(casadi_int i = 0; i < power; i++) r = operator*(r);
        return r;
    }

    Function Function::mpow(casadi_int power) const {
        spline_assert_message(shape()[0] == shape()[1], "matrix power only for square matrices");
        spline_assert_message(power >= 0, "No negative power");
        Function r = Function(1, shape());
        for(casadi_int i = 0; i < power; i++) r = mtimes(r);
        return r;
    }

    Function Function::slice(const AnySlice& i, const AnySlice& j) const { return (*this)->slice( i, j) ;}
    Function Function::slice(const AnySlice& i) const { return (*this)->slice( i) ;}

    Function Function::transpose() const { return (*this)->transpose() ;}
    Function Function::trace() const { return (*this)->trace() ;}
    Function Function::sum(casadi_int axis) const { return (*this)->sum(axis) ;}
    Function Function::sum() const { return (*this)->sum() ;}

    Coefficient Function::coeff() const {return (*this)->coeff() ;}
    AnyTensor Function::coeff_tensor() const {return (*this)->coeff_tensor() ;}
    AnyTensor Function::data() const {return (*this)->data() ;}


    Basis Function::basis() const { return (*this)->basis(); }
    Basis Function::basis(const Argument& i) const { return (*this)->basis( i); }
    TensorBasis Function::tensor_basis() const { return (*this)->tensor_basis(); }
    TensorDomain Function::domain() const { return (*this)->domain(); }

    bool Function::is_scalar() const { return (*this)->is_scalar() ;}
    bool Function::is_vector() const { return (*this)->is_vector() ;}
    bool Function::is_column() const { return (*this)->is_column() ;}


    std::vector< casadi_int > Function::shape() const { return (*this)->shape() ;}

    Function Function::reshape(const std::vector< casadi_int >& shape) const { return (*this)->reshape(shape) ;}

    Function Function::transform_to(const Basis& basis) const { return (*this)->transform_to(TensorBasis( basis )) ;}
    Function Function::transform_to(const TensorBasis& basis) const { return (*this)->transform_to( basis ) ;}
    Function Function::project_to(const Basis& basis) const { return (*this)->project_to(TensorBasis( basis )) ;}
    Function Function::project_to(const TensorBasis& basis) const { return (*this)->project_to(basis) ;}

    casadi_int Function::n_inputs() const { return (*this)->n_inputs() ;}
    Function Function::insert_knots(const AnyVector & new_knots, const Argument& arg) const { return (*this)->insert_knots(vectorize(arg, new_knots), vectorize(arg)) ;}
    Function Function::insert_knots(const std::vector<AnyVector> & new_knots, const std::vector<Argument> & arg) const { return (*this)->insert_knots(new_knots, Argument::concrete(arg, tensor_basis().arguments()));}

    Function Function::midpoint_refinement(casadi_int refinement, const Argument& arg) const {
        return (*this)->midpoint_refinement(vectorize(arg, refinement), vectorize(arg));
    }
    Function Function::midpoint_refinement(const std::vector<casadi_int>& refinement, const std::vector< Argument >& arg_ind) const {
        return (*this)->midpoint_refinement( refinement, Argument::concrete(arg_ind, tensor_basis().arguments()));
    }

    Function Function::degree_elevation(casadi_int order, const Argument& arg) const {
        return (*this)->degree_elevation(vectorize(arg, order), vectorize(arg));
    }
    Function Function::degree_elevation(const std::vector<casadi_int>& orders, const std::vector< Argument >& arg_ind) const {
        return (*this)->degree_elevation( orders, Argument::concrete(arg_ind, tensor_basis().arguments()));
    }

    Function Function::kick_boundary(const TensorDomain& boundary) const { return (*this)->kick_boundary( boundary) ;}
    Function Function::kick_boundary(const TensorDomain& boundary, const NumericIndex& arg_ind) const { return (*this)->kick_boundary(boundary, arg_ind);}
    Function Function::kick_boundary(const TensorDomain& boundary, const std::vector<std::string> & args) const { return (*this)->kick_boundary( boundary, args);}
    Function Function::kick_boundary(const TensorDomain& boundary, const NumericIndexVector & arg_ind) const { return (*this)->kick_boundary( boundary, arg_ind) ;}

    Function Function::derivative(casadi_int order, const Argument& arg) const {
        return (*this)->derivative(vectorize(arg, order), vectorize(arg));
    }
    Function Function::derivative(const std::vector<casadi_int>& orders, const std::vector< Argument >& arg_ind) const {
        return (*this)->derivative( orders, Argument::concrete(arg_ind, tensor_basis().arguments()));
    }

    Function Function::antiderivative(casadi_int order, const Argument& arg) const {
        return (*this)->antiderivative(vectorize(arg, order), vectorize(arg));
    }
    Function Function::antiderivative(const std::vector<casadi_int>& orders, const std::vector< Argument >& arg_ind) const {
        return (*this)->antiderivative( orders, Argument::concrete(arg_ind, tensor_basis().arguments()));
    }

    std::vector<spline::Function> Function::jacobian() const { return (*this)->jacobian() ;}
    std::vector<std::vector<spline::Function> > Function::hessian() const { return (*this)->hessian(); }
    spline::Function Function::jacobian_matrix() const { return (*this)->jacobian_matrix() ;}
    spline::Function Function::hessian_matrix() const { return (*this)->hessian_matrix() ;}


    AnyTensor Function::integral() const { return (*this)->integral() ;}
    AnyTensor Function::integral(const TensorDomain& domain) const { return (*this)->integral( domain) ;}
    Function Function::partial_integral(const TensorDomain& domain, const std::vector<std::string>& args) const { return (*this)->partial_integral(domain, args);}
    Function Function::partial_integral(const TensorDomain& domain, const NumericIndexVector& arg_ind) const { return (*this)->partial_integral(domain, arg_ind);}

    Function Function::vertcat(const std::vector< spline::Function >& f) {
        return Function::cat(0, f);
    }

    Function Function::blockcat(const std::vector< std::vector<spline::Function> >& f) {
        std::vector< spline::Function > flat;
        casadi_int n = -1;
        for (auto e : f) {
            if (n==-1) n = e.size();
            spline_assert(e.size()==n);
            flat.insert(flat.end(), e.begin(), e.end());
        }
        Function c = vertcat(flat);
        casadi_int m = f.size();
        return c.reshape(std::vector<casadi_int>{n, m});
    }

    Function Function::horzcat(const std::vector< spline::Function >& f) {
        return Function::cat(1, f);
    }

    Function Function::blkdiag(const std::vector< spline::Function >& f) {
        Function b = f[0];
        for (casadi_int i = 1; i < f.size(); i++) {
            std::vector< casadi_int > shape12 = std::vector< casadi_int >{b.shape()[0], f[i].shape()[1]};
            std::vector< casadi_int > shape21 = std::vector< casadi_int >{f[i].shape()[0], b.shape()[1]};

            Function zero12 = Function(0, shape12);
            Function zero21 = Function(0, shape21);

            Function upper = Function::horzcat(std::vector< Function >{b, zero12});
            Function lower = Function::horzcat(std::vector< Function >{zero21, f[i]});

            b = Function::vertcat(std::vector< Function >{upper, lower});
        }

        return b;
    }

    Function Function::cat(NumericIndex index,
            const std::vector< spline::Function >& functions) {
        TensorBasis unionBasis = functions[0].tensor_basis();  // load first basis
        for (casadi_int i = 1; i< functions.size(); i++) {
            unionBasis = unionBasis + functions[i].tensor_basis();  // add other bases
        }

        std::vector< Coefficient > coefVec;
        for (casadi_int i = 0; i< functions.size(); i++) {
            coefVec.push_back(functions[i].transform_to(unionBasis).coeff());
        }

        return Function(unionBasis, Coefficient::cat(index, coefVec));
    }

    std::vector<casadi_int> Function::vectorize(const Argument& arg) const{
        if(arg.is_all()){
            return casadi::range(tensor_basis().n_basis());
        } else {
            return {arg.concrete(tensor_basis().arguments())};
        }
    }

    Function Function::linear(const AnyVector & x, const AnyVector & y) {
      return Function(BSplineBasis::from_single(x, 1), y);
    }

    casadi::Function Function::to_casadi() const {
      std::vector<Basis> bases = tensor_basis().bases();
      std::vector< std::vector<double> > knots;
      std::vector<casadi_int> degrees;
      std::vector<double> coeff;
      for (casadi_int i=0;i<bases.size();++i) {
        Basis b = bases[i];
        spline_assert(b.type()=="BSplineBasis");

        BSplineBasis bb = b.get()->shared_from_this<BSplineBasis>();

        spline_assert(AnyScalar::is_double(bb.knots()));
        knots.push_back(AnyScalar::as_double(bb.knots()));
        degrees.push_back(bb.degree());
      }

      spline_assert(coeff_tensor().is_DT());
      return casadi::Function::bspline("bspline", knots,
        coeff_tensor().as_DT().data().nonzeros(), degrees);
    }

    AnyTensor Function::fast_eval(const AnyTensor& xy) const {
      std::vector<Basis> bases = tensor_basis().bases();
      std::vector< std::vector<double> > knots;
      std::vector<casadi_int> degrees;

      for (casadi_int i=0;i<bases.size();++i) {
        Basis b = bases[i];
        spline_assert(b.type()=="BSplineBasis");
        BSplineBasis bb = b.get()->shared_from_this<BSplineBasis>();
        spline_assert(AnyScalar::is_double(bb.knots()));
        knots.push_back(AnyScalar::as_double(bb.knots()));
        degrees.push_back(bb.degree());

      }

      spline_assert(xy.n_dims()==2);
      spline_assert(xy.dims()[0]==n_inputs());
      std::vector<casadi_int> dims = shape();
      dims.insert(dims.begin(), xy.dims()[1]);

      if (xy.is_DT()) {
        std::vector<double> points = xy.as_DT().data().nonzeros();
        casadi::DM jac = casadi::MX::bspline_dual(points, knots, degrees);
        AnyTensor c = coeff_tensor();
        if (c.is_DT()) {
          casadi::DM r = casadi::DM::mtimes(jac, casadi::DM::reshape(c.as_DT().data(), jac.size2(), -1));
          return DT(r, dims).squeeze_tailing();
        } else {
          casadi::MX r = casadi::MX::mtimes(jac, casadi::MX::reshape(c.as_MT().data(), jac.size2(), -1));
          return MT(r, dims).squeeze_tailing();
        }
      }
      if (coeff_tensor().is_DT()) {
        std::vector<double> coeffs = coeff_tensor().as_DT().data().nonzeros();
        casadi::Function nominal = casadi::Function::bspline("nominal", knots, coeffs, degrees);
        nominal = nominal.map(xy.dims()[1]);

        if (xy.is_DT()) {
          casadi::DM r = nominal(std::vector<casadi::DM>{xy.as_DT().matrix()})[0];
          return DT(r, dims).squeeze_tailing();
        } else {
          casadi::MX r = nominal(std::vector<casadi::MX>{xy.as_MT().matrix()})[0];
          return MT(r, dims).squeeze_tailing();
        }
      }
      spline_assert(false);
      return DT();
    }

    casadi::DM Function::fast_jac(const AnyTensor& xy) const {
      casadi::MX c = coeff_tensor().as_MT().data();
      AnyTensor yt = fast_eval(xy);
      casadi::MX y = yt.as_MT().data();
      casadi::MX J = casadi::MX::jacobian(y, c);
      casadi::Function f = casadi::Function("f", {c}, {J});
      return f(std::vector<casadi::DM>{0})[0];
    }
} // namespace spline
