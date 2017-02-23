#ifndef CPP_SPLINE_SUBBSPLINEBASIS_H
#define CPP_SPLINE_SUBBSPLINEBASIS_H

#include <any_tensor.hpp>
#include <vector>

#include <casadi/core/function/function_internal.hpp>

#include "Basis.h"
#include "UnivariateBasis.h"


#ifndef SWIG
namespace casadi {

  class BSplineEvaluator : public casadi::FunctionInternal {
  public:
    static casadi::Function create(const std::string &name, int n_knots, int degree, const Dict& opts=Dict());

    BSplineEvaluator(const std::string &name, int n_knots, int degree);

    /** \brief  Destructor */
    virtual ~BSplineEvaluator() {};

    ///@{
    /** \brief Number of function inputs and outputs */
    virtual size_t get_n_in() override;
    virtual size_t get_n_out() override;
    ///@}

    /// @{
    /** \brief Sparsities of function inputs and outputs */
    virtual Sparsity get_sparsity_in(int i) override;
    virtual Sparsity get_sparsity_out(int i) override;
    /// @}

    /** \brief  Initialize */
    virtual void init(const Dict& opts) override;

    /** \brief  Evaluate numerically, work vectors given */
    virtual void eval(void* mem, const double** arg, double** res, int* iw, double* w) const override;

    virtual void eval_sx(const SXElem** arg, SXElem** res, int* iw, SXElem* w, int mem) override;

    template<class T>
    void eval_generic(void* mem, const T** arg, T** res, int* iw, T* w) const {
      T x = arg[1][0];
      const T* knots= arg[0];

      int length = n_knots_ - degree_ - 1;

      T *temp = w;

      int n1 = n_knots_-1;

      for (int i=0; i<(n_knots_-1); i++) {
          if ((i < degree_+1) && casadi_limits<T>::is_zero(knots[0]-knots[i])) {
              temp[i] = ((x >= knots[i]) && (x <= knots[i+1]));
          } else {
              temp[i] = ((x > knots[i]) && (x <= knots[i+1]));
          }
      }

      for (int d=1; d<(degree_+1); d++) {
          int offset = (d-1)*n1;
          for (int i=0; i < length; i++) {
              T b = 0;
              T bottom = knots[i+d] - knots[i];
              if (!casadi_limits<T>::is_zero(bottom)) {
                  b = (x - knots[i])*temp[offset+i]/bottom;
              }
              bottom = knots[i+d+1] - knots[i+1];
              if (!casadi_limits<T>::is_zero(bottom)) {
                  b += (knots[i+d+1] - x)*temp[offset+i+1]/bottom;
              }
              temp[d*n1+i] = b;
          }
      }

      std::copy(temp+degree_*n1, temp+degree_*n1+length, res[0]);
    }
    virtual bool hasFullJacobian() const override { return true;}
    virtual casadi::Function getFullJacobian(const std::string& name, const Dict& opts) override;

    /** \brief  Print description */
    virtual void print(std::ostream &stream) const override;

    int n_knots_;
    int degree_;

  };

}
#endif //SWIG

namespace spline{
    class MonomialBasis;

#ifndef SWIG

    class BSplineBasisNode : public UnivariateBasisNode {

    public:
        BSplineBasisNode(const std::vector<AnyScalar>& knots, int degree);

        virtual std::string type() const override {return "BSplineBasis";}

        virtual Basis operator+(const MonomialBasis& other) const override;
        virtual Basis operator+(const BSplineBasis& other) const override;
        virtual Basis operator+(const Basis& other) const override;
        virtual Basis operator+(const EmptyBasis& other) const override;

        virtual Basis operator*(const MonomialBasis& other) const override;
        virtual Basis operator*(const BSplineBasis& other) const override;
        virtual Basis operator*(const Basis& other) const override;
        virtual Basis operator*(const EmptyBasis& other) const override;

        std::vector<AnyScalar> knots() const;
        std::vector<AnyScalar> greville() const;

        virtual std::string getRepresentation() const override;

        virtual AnyTensor operator()(const std::vector< AnyScalar >& x) const override;

        virtual AnyTensor const_coeff_tensor(const AnyTensor& t) const override ;

        virtual int dimension() const override;


        AnyTensor basis_evaluation(const std::vector<AnyScalar> & x ) const;

        virtual std::vector< std::vector < AnyScalar > > getEvaluationGrid() const override;

        virtual Basis derivative(int order, AnyTensor& T) const override;
        virtual Basis antiderivative(int order, AnyTensor& T) const override;
        virtual AnyTensor integral(const Interval& domain) const override;
        virtual Basis insert_knots(const AnyVector & new_knots, AnyTensor & T) const override;
        virtual Basis midpoint_refinement(int refinement, AnyTensor& T) const override;
        virtual Basis degree_elevation(int elevation, AnyTensor& T) const override;

    private:
        std::vector<AnyScalar> knots_;
        mutable casadi::Function bspline_evaluator_;
    };

#endif // SWIG

    class BSplineBasis : public UnivariateBasis {

    public:

#ifndef SWIG
        BSplineBasis(){};
        BSplineBasisNode* get() const ;
        BSplineBasisNode* operator->() const ;
        BSplineBasis(const std::vector<AnyScalar>& knots, int degree);
        BSplineBasis(const std::vector<AnyScalar>& bounds, int degree, int numberOfIntervals);
#endif // SWIG

        BSplineBasis(const AnyVector& knots, int degree) :
          BSplineBasis(knots.to_scalar_vector(), degree) {};
        BSplineBasis(const AnyVector& bounds, int degree, int numberOfIntervals) :
          BSplineBasis(bounds.to_scalar_vector(), degree, numberOfIntervals) {};

        std::vector<AnyScalar> knots() const;
        std::vector<AnyScalar> greville() const;

    };

}

#endif //CPP_SPLINE_SUBBSPLINEBASIS_H
