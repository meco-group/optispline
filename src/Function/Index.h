#ifndef CPP_SPLINES_INDEX_H
#define CPP_SPLINES_INDEX_H

#include <string>
#include <vector>

#include "../SharedObject/SharedObject.h"
#include "../SharedObject/SharedObjectNode.h"
#include "Argument.h"

namespace spline {
#ifndef SWIG
    class Index;
    class IndexNode : public SharedObjectNode {
    public:
        virtual std::string getRepresentation() const = 0;
        virtual int concrete(const std::vector<Argument> & args) const = 0;
    };

    class StringIndexNode : public IndexNode {
    public:
        StringIndexNode(const std::string &name);
        virtual std::string getRepresentation() const ;
        virtual int concrete(const std::vector<Argument> & args) const;
    private:
        std::string name_;
    };

    class IntIndexNode : public IndexNode {
    public:
        IntIndexNode(int index);
        virtual std::string getRepresentation() const ;
        virtual int concrete(const std::vector<Argument> & args) const;
    private:
        int index_;
    };

    class NullIndexNode : public IndexNode {
    public:
        NullIndexNode();
        virtual std::string getRepresentation() const ;
        virtual int concrete(const std::vector<Argument> & args) const;
    };

#endif // SWIG

    class Index : public  SharedObject {
    public:
#ifndef SWIG
        IndexNode* get() const ;
        IndexNode* operator->() const ;

        inline friend
            std::ostream& operator<<(std::ostream &stream, const Index& argument);
#endif // SWIG
        Index();
        Index(const std::string &name);
        Index(int index);

        int concrete(const std::vector<Argument> & args) const;

        virtual std::string getRepresentation() const ;
    };
} // namespace spline
#endif //CPP_SPLINES_INDEX_H
