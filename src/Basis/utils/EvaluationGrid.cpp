#include "EvaluationGrid.h"
#include "../Basis.h"
#include "../../common.h"

namespace spline{
    EvaluationGrid::EvaluationGrid(TensorBasis basis) : griddedBasis(basis) {}

    std::vector< AnyTensor > EvaluationGrid::eval() const {
        std::vector< AnyTensor > preStep { AnyTensor::unity() };
        std::vector< AnyTensor > postStep ;

        for(Basis b : griddedBasis.bases()){
            std::vector< std::vector< AnyScalar > > evaluationGrid = b.getEvaluationGrid();
            for(auto const & subPoint : evaluationGrid){
                AnyTensor subEvaluation = b(subPoint);
                for( AnyTensor pre : preStep){
                    postStep.push_back(pre.outer_product( subEvaluation ));
                }
            }
            preStep = postStep;
            postStep.clear();
        }
        return preStep;
    }

    AnyTensor EvaluationGrid::eval(const spline::Function & f) const {
        TensorBasis basis = f.tensor_basis();
        std::vector< int > indexPermutation = basis.get_permutation(griddedBasis);

        std::vector< AnyTensor > preStep { AnyTensor::unity() };
        std::vector< AnyTensor > postStep ;

        for(int i = 0; i < griddedBasis.n_basis(); i++){
            Basis subBasis = griddedBasis.basis(i);
            std::vector< std::vector< AnyScalar > > evaluationGrid = subBasis.getEvaluationGrid();
            for(auto const & subPoint : evaluationGrid){
                if(indexPermutation[i] < 0){
                    for( AnyTensor pre : preStep){
                        postStep.push_back(pre);
                    }
                }else{
                    Basis correspondingSubBasis = basis.basis(indexPermutation[i]);
                    AnyTensor subEvaluation = correspondingSubBasis(subPoint);
                    for( AnyTensor pre : preStep){
                        postStep.push_back(pre.outer_product( subEvaluation ));
                    }
                }
            }
            preStep = postStep;
            postStep.clear();
        }

        int n = f.coeff_tensor().n_dims();
        std::vector<int> b_i = mrange(n);
        std::vector<int> a_i = std::vector<int>(b_i.begin(), b_i.begin()+b_i.size()-2);
        a_i.insert(a_i.begin(), -n-1);
        std::vector<int> c_i = {-n-1, -n+1, -n};

        AnyTensor P = AnyTensor::pack(preStep, 0);

        return P.einstein(f.coeff().data(), a_i, b_i, c_i);
    }

    std::string EvaluationGrid::type() const {
        return "EvaluationGrid";
    }

    std::string EvaluationGrid::to_string() const {
        return type();
    }
}
