//
// Created by erik on 29/04/16.
//

#ifndef CPP_SPLINES_ARGUMENT_H
#define CPP_SPLINES_ARGUMENT_H

#include <string>
#include <vector>

namespace spline{
    class Argument {

    public:
        Argument ();
        Argument (const std::string &name);
        Argument (const std::vector< std::string > &name);

        const std::vector< std::string > & getName () const;
        void setName (const std::string &name);
        void setName (const std::vector< std::string > &name);

    private:
        std::vector< std::string > name;
    };
}
#endif //CPP_SPLINES_ARGUMENT_H
