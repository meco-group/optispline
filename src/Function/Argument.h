#ifndef CPP_SPLINES_ARGUMENT_H
#define CPP_SPLINES_ARGUMENT_H

#include <string>
#include <vector>

#include "../SharedObject/SharedObject.h"
#include "../SharedObject/SharedObjectNode.h"

namespace spline{
#ifndef SWIG
    class Argument;
    class ArgumentNode : public SharedObjectNode
    {
    public:
        ArgumentNode ();
        ArgumentNode (const std::string &name);

        const std::string& getName () const;
        void setName (const std::string &name);

        virtual std::string getRepresentation() const ;

        bool operator==(const Argument& other_arg) const;
        bool operator==(const std::string& other_arg_name) const;
    protected:
        std::string name;
    };

#endif // SWIG

    class Argument : public  SharedObject 
    {
    public:
#ifndef SWIG
        ArgumentNode* get() const ;
        ArgumentNode* operator->() const ;

        inline friend
            std::ostream& operator<<(std::ostream &stream, const Argument& argument);
#endif // SWIG
        Argument ();
        Argument (const std::string &name);

        virtual std::string getRepresentation() const ;

        const std::string  & getName () const;
        void setName (const std::string &name);

        bool operator==(const Argument& other_arg) const;
        bool operator==(const std::string& other_arg_name) const;
    };
}
#endif //CPP_SPLINES_ARGUMENT_H
