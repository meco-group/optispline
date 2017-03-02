#ifndef EVALUATIONGRID_H
#define EVALUATIONGRID_H

#include <vector>
#include <any_tensor.hpp>


#include "../Function/Function.h"
#include "../TensorBasis.h"
#include "../TensorBasisConstant.h"
namespace spline{


    class EvaluationGrid : public SharedObject {
    public:

        std::string type() const;
        std::string to_string() const ;

        EvaluationGrid(TensorBasis basis);
        std::vector< AnyTensor > eval() const;
        std::vector< AnyTensor > eval(const spline::Function & f) const;
    private:
        TensorBasis griddedBasis;
    };

} // namespace spline

#endif
