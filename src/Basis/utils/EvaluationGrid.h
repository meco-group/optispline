#ifndef EVALUATIONGRID_H
#define EVALUATIONGRID_H

#include <vector>
#include <any_tensor.hpp>


#include "../Function/Argument.h"
#include "../Function/Function.h"
#include "../TensorBasis.h"
namespace spline{


    class EvaluationGrid : public SharedObject {
    public:
        std::string getRepresentation() const ;

        EvaluationGrid(TensorBasis basis);
        std::vector< AnyTensor > evaluateEvaluationGrid() const;
        std::vector< AnyTensor > evaluateEvaluationGrid(const spline::Function & f) const;

        std::vector< int > getPermutation(TensorBasis basis) const;
    private:
        TensorBasis griddedBasis;
    };

} // namespace spline

#endif
