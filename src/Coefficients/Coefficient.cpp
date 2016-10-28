#include "Coefficient.h"

namespace spline {

    Coefficient::Coefficient (const std::vector< double >& v) :
        data(DT(DM(v),std::vector<int>{static_cast<int>(v.size()),1,1})){
    }

    std::vector< int > Coefficient::getShape() const{
        std::vector< int > dims = data.dims();
        std::cout << "dims coef : " << dims << std::endl;
        int d = dims.size();
        int i = dims[d-2];
        int j = dims[d-1];
        return *(new std::vector< int > {i,j});
    }

    int Coefficient::getNumberCoefficents() const{
        std::vector< int > dims = data.dims();
        int d = dims.size();
        int i = dims[d-2];
        int j = dims[d-1];
        return i*j;
    }

    Coefficient Coefficient::operator-() const {
        return Coefficient( -data);
    }

        std::string Coefficient::getRepresentation() const {return "Coefficient";};
    
}  // namespace spline
