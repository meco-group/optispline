#include "Polynomial.h"
#include "../Basis/MonomialeBasis.h"
#include "../Basis/CommonBasis.h"

namespace spline{

    Polynomial::Polynomial (const std::vector< double >& coef) : 
// TODO static_cast to supress the waring : narrowing conversion of ‘(& coef)->std::vector<_Tp, _Alloc>::size<double, std::allocator<double> >()’ from ‘std::vector<double>::size_type {aka long unsigned int}’
        // Function(MonomialeBasis((coef.size())-1),
        Function(MonomialeBasis(coef.size()-1), Coefficient(coef)) {
    }
									
    // Polynomial::Polynomial (const std::vector< double >& coef) { 
    //     basis =  MonomialeBasis(static_cast<int>(coef.size())-1);
    //     coef =  Coefficient(new AnyTensor(coef,{static_cast<int>(coef.size()),1,1}))) ;
    // }
		
} // namespace spline
