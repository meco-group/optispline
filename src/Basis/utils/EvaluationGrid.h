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
        std::vector< AnyTensor > eval() const;
        std::vector< AnyTensor > eval(const spline::Function & f) const;

        std::vector< int > getPermutation(TensorBasis basis) const;
    private:
        TensorBasis griddedBasis;
    };

} // namespace spline

#endif
