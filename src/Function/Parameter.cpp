#include "Parameter.h"

namespace spline {

    Parameter::Parameter(const std::string& a) : Polynomial({0, 1}, a) {}

    std::string Parameter::type() const{
    	return "Parameter";
    }

    std::string Parameter::to_string() const{
    	return "Parameter " + tensor_basis().to_string();
    }

} // namespace spline
