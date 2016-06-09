#ifndef CPP_SPLINES_ARGUMENT_H
#define CPP_SPLINES_ARGUMENT_H

#include <string>
#include <vector>

#include "../SharedObject/SharedObject.h"
#include "../SharedObject/SharedObjectNode.h"

namespace spline{
#ifndef SWIG
    class ArgumentNode : public SharedObjectNode
    {
    public:
        ArgumentNode ();
        ArgumentNode (const std::string &name);

        const std::string  & getName () const;
        void setName (const std::string &name);

        virtual std::string getRepresentation() const ;

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

#endif // SWIG
        Argument ();
        Argument (const std::string &name);

        const std::string  & getName () const;
        void setName (const std::string &name);
     
#ifndef SWIG
    inline friend
        std::ostream& operator<<(std::ostream &stream, const Argument& argument);
#endif
        virtual std::string getRepresentation() const ;
    };
}
#endif //CPP_SPLINES_ARGUMENT_H
