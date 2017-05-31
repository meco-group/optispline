#include "Parameter.h"

namespace spline {

    Parameter::Parameter() : Polynomial({0, 1}) {}

    Parameter::Parameter(const std::string& a) : Polynomial({0, 1}, a) {}

    std::string Parameter::type() const{
    	return "Parameter";
    }

    std::string Parameter::to_string() const{
    	return "Parameter " + tensor_basis().to_string();
    }

    std::string Parameter::name() const{
        return tensor_basis().arguments()[0];
    }
} // namespace spline
