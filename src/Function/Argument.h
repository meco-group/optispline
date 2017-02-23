#ifndef CPP_SPLINES_INDEX_H
#define CPP_SPLINES_INDEX_H

#include <string>
#include <vector>

#include "../SharedObject/SharedObject.h"
#include "../SharedObject/SharedObjectNode.h"

namespace spline {
#ifndef SWIG
    class Argument;
    class ArgumentNode : public SharedObjectNode {
    public:
        virtual std::string getRepresentation() const = 0;
        virtual int concrete(const std::vector<std::string> & args) const = 0;
    };

    class StringArgumentNode : public ArgumentNode {
    public:
        StringArgumentNode(const std::string &name);
        virtual std::string getRepresentation() const ;
        virtual int concrete(const std::vector<std::string> & args) const;
    private:
        std::string name_;
    };

    class IntArgumentNode : public ArgumentNode {
    public:
        IntArgumentNode(int index);
        virtual std::string getRepresentation() const ;
        virtual int concrete(const std::vector<std::string> & args) const;
    private:
        int index_;
    };

    class NullArgumentNode : public ArgumentNode {
    public:
        NullArgumentNode();
        virtual std::string getRepresentation() const ;
        virtual int concrete(const std::vector<std::string> & args) const;
    };

#endif // SWIG

    class Argument : public  SharedObject {
    public:
#ifndef SWIG
        ArgumentNode* get() const ;
        ArgumentNode* operator->() const ;

        inline friend
            std::ostream& operator<<(std::ostream &stream, const Argument& argument);
#endif // SWIG
        Argument();
        Argument(const std::string &name);
        Argument(int index);

        int concrete(const std::vector<std::string> & args) const;

        static std::vector<Argument> from_vector(const std::vector<int>& ind);

        virtual std::string getRepresentation() const ;
    };
} // namespace spline
#endif //CPP_SPLINES_INDEX_H
