#ifndef CPP_SPLINE_BSPLINEBASISNODE_H_
#define CPP_SPLINE_BSPLINEBASISNODE_H_

#include <any_tensor.hpp>
#include <vector>

#include <casadi/core/function_internal.hpp>

#include "Basis.h"
#include "UnivariateBasisNode.h"

namespace casadi {

    class BSplineEvaluator : public casadi::FunctionInternal {
        public:
            static casadi::Function create(casadi_int n_knots, casadi_int degree, const Dict& opts=Dict());
            BSplineEvaluator(casadi_int n_knots, casadi_int degree);

            /** \brief  Destructor */
            ~BSplineEvaluator() override {};

            ///@{
            /** \brief Number of function inputs and outputs */
            size_t get_n_in() override;
            size_t get_n_out() override;
            ///@}

            /// @{
            /** \brief Sparsities of function inputs and outputs */
            Sparsity get_sparsity_in(casadi_int i) override;
            Sparsity get_sparsity_out(casadi_int i) override;
            /// @}

            /** \brief  Initialize */
            void init(const Dict& opts) override;

            /** \brief  Evaluate numerically, work vectors given */
            int eval(const double** arg, double** res, casadi_int* iw, double* w, void* mem) const override;

            int eval_sx(const SXElem** arg, SXElem** res, casadi_int* iw, SXElem* w, void* mem) const override;

            template<class T>
                void eval_generic(void* mem, const T** arg, T** res, casadi_int* iw, T* w) const {
                    T x = arg[1][0];
                    const T* knots= arg[0];

                    casadi_int length = n_knots_ - degree_ - 1;

                    T *temp = w;

                    casadi_int n1 = n_knots_-1;

                    for (casadi_int i=0; i<(n_knots_-1); i++) {
                        if ((i < degree_+1) && casadi_limits<T>::is_zero(knots[0]-knots[i])) {
                            temp[i] = ((x >= knots[i]) && (x <= knots[i+1]));
                        } else {
                            temp[i] = ((x > knots[i]) && (x <= knots[i+1]));
                        }
                    }

                    for (casadi_int d=1; d<(degree_+1); d++) {
                        casadi_int offset = (d-1)*n1;
                        for (casadi_int i=0; i < length; i++) {
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
            bool has_jacobian() const override { return true;}
            casadi::Function get_jacobian(const std::string& name,
                    const std::vector<std::string>& inames,
                    const std::vector<std::string>& onames,
                    const Dict& opts) const override;

            /** \brief  Print description */
            void disp_more(std::ostream &stream) const override;
        
            std::string class_name() const override { return "BSplineEvaluator"; }
      private:
            casadi_int n_knots_;
            casadi_int degree_;

    };

}

namespace spline{

    class MonomialBasis;
    class BSplineBasis;

    class BSplineBasisNode : public UnivariateBasisNode {

        public:
            BSplineBasisNode(const std::vector<AnyScalar>& knots, casadi_int degree);

            virtual std::string type() const override {return "BSplineBasis";}
            virtual std::string to_string() const override;

            virtual Basis operator+(const BasisNode& other) const override;
            virtual Basis operator+(const EmptyBasisNode& other) const override;
            virtual Basis operator+(const MonomialBasisNode& other) const override;
            virtual Basis operator+(const BSplineBasisNode& other) const override;

            virtual Basis operator*(const MonomialBasisNode& other) const override;
            virtual Basis operator*(const BSplineBasisNode& other) const override;
            virtual Basis operator*(const BasisNode& other) const override;
            virtual Basis operator*(const EmptyBasisNode& other) const override;

            virtual bool operator==(const MonomialBasisNode& other) const override;
            virtual bool operator==(const BSplineBasisNode& other) const override;
            virtual bool operator==(const BasisNode& other) const override;
            virtual bool operator==(const EmptyBasisNode& other) const override;

            std::vector<AnyScalar> knots() const;
            std::vector<AnyScalar> greville() const;

            virtual AnyTensor operator()(const std::vector< AnyScalar >& x) const override;

            virtual AnyTensor const_coeff_tensor(const AnyTensor& t) const override ;

            virtual casadi_int dimension() const override;

            AnyTensor basis_evaluation(const std::vector<AnyScalar> & x ) const;

            virtual AnyTensor evaluation_grid() const override;

            virtual Basis derivative(casadi_int order, AnyTensor& T) const override;
            virtual Basis antiderivative(casadi_int order, AnyTensor& T) const override;
            virtual AnyTensor integral(const Interval& domain) const override;
            virtual Basis insert_knots(const AnyVector & new_knots, AnyTensor & T) const override;
            virtual Basis midpoint_refinement(casadi_int refinement, AnyTensor& T) const override;
            virtual Basis degree_elevation(casadi_int elevation, AnyTensor& T) const override;
            virtual Basis kick_boundary(const Interval& boundary, AnyTensor& T) const override;

        private:
            std::vector<AnyScalar> knots_;
            casadi::Function bspline_evaluator_;
    };

}

#endif //CPP_SPLINE_BSPLINEBASISNODE_H_
