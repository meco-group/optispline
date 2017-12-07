#ifndef CPP_SPLINE_BSPLINEBASISNODE_H_
#define CPP_SPLINE_BSPLINEBASISNODE_H_

#include <any_tensor.hpp>
#include <vector>

#include <casadi/core/callback.hpp>

#include "Basis.h"
#include "UnivariateBasis.h"

#include "BSplineBasis.h"
#include "BSplineBasisNode.h"


namespace casadi {

    class BSplineEvaluator : public casadi::Callback {
        public:
            BSplineEvaluator(int n_knots, int degree);

            /** \brief  Destructor */
            ~BSplineEvaluator() override {};

            ///@{
            /** \brief Number of function inputs and outputs */
            int get_n_in() override;
            int get_n_out() override;
            ///@}

            /// @{
            /** \brief Sparsities of function inputs and outputs */
            Sparsity get_sparsity_in(int i) override;
            Sparsity get_sparsity_out(int i) override;
            /// @}

            /** \brief  Initialize */
            void init() override;

            /** \brief  Evaluate numerically, work vectors given */
            int eval(const double** arg, double** res, int* iw, double* w, void* mem) const override;

            int eval_sx(const SXElem** arg, SXElem** res, int* iw, SXElem* w, void* mem) const override;

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
            bool has_jacobian() const override { return true;}
            casadi::Function get_jacobian(const std::string& name,
                    const std::vector<std::string>& inames,
                    const std::vector<std::string>& onames,
                    const Dict& opts) const override;

            int n_knots_;
            int degree_;


            static BSplineEvaluator& construct_cached(int n_knots, int degree);
    };

}

namespace spline{
    class MonomialBasis;

    class BSplineBasisNode : public UnivariateBasisNode {

        public:
            BSplineBasisNode(const std::vector<AnyScalar>& knots, int degree);

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

            virtual int dimension() const override;

            AnyTensor basis_evaluation(const std::vector<AnyScalar> & x ) const;

            virtual AnyTensor evaluation_grid() const override;

            virtual Basis derivative(int order, AnyTensor& T) const override;
            virtual Basis antiderivative(int order, AnyTensor& T) const override;
            virtual AnyTensor integral(const Interval& domain) const override;
            virtual Basis insert_knots(const AnyVector & new_knots, AnyTensor & T) const override;
            virtual Basis midpoint_refinement(int refinement, AnyTensor& T) const override;
            virtual Basis degree_elevation(int elevation, AnyTensor& T) const override;
            virtual Basis kick_boundary(const Interval& boundary, AnyTensor& T) const override;

        private:
            std::vector<AnyScalar> knots_;
            casadi::Callback& bspline_evaluator_;
    };

}

#endif //CPP_SPLINE_BSPLINEBASISNODE_H_
