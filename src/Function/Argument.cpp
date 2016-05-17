//
// Created by erik on 29/04/16.
//

#include "Argument.h"
namespace spline {

    Argument::Argument (const std::vector< std::string > &name) : name(name) { }

    Argument::Argument () {
        std::vector< std::string > n{std::string("_")};
        setName(n);
    }

    Argument::Argument (const std::string &name) {
        std::vector< std::string > n{name};
        setName(n);
    }

    const std::vector< std::string > &Argument::getName () const {
        return name;
    }

    void Argument::setName (const std::string &name) {
        std::vector< std::string > n{name};
        setName(n);
    }

    void Argument::setName (const std::vector< std::string > &name) {
        name = name;

    }


} // namespace spline