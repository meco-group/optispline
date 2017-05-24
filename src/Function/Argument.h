#ifndef CPP_SPLINES_INDEX_H
#define CPP_SPLINES_INDEX_H

#include <string>
#include <vector>

#include "../SharedObject/SharedObject.h"
#include "../SharedObject/SharedObjectNode.h"

namespace spline {
#ifndef SWIG
    class TensorBasis;
    class Argument;
    class Parameter;
    class ArgumentNode;
    class StringArgumentNode;
    class IntArgumentNode;
    class NullArgumentNode;

    class ArgumentNode : public SharedObjectNode {
    public:
        virtual std::string type() const;
        virtual int concrete(const std::vector<std::string> & args) const = 0;
        virtual bool is_all() const ;

        bool operator ==(const ArgumentNode & arg) const;
        bool operator ==(const StringArgumentNode & arg) const;
        bool operator ==(const IntArgumentNode & arg) const;
        bool operator ==(const NullArgumentNode & arg) const;
    };

    class StringArgumentNode : public ArgumentNode {
    public:
        StringArgumentNode(const std::string &name);
        virtual std::string to_string() const override;
        virtual int concrete(const std::vector<std::string> & args) const;

        bool operator ==(const ArgumentNode & arg) const;
        bool operator ==(const StringArgumentNode & arg) const;
        bool operator ==(const IntArgumentNode & arg) const;
        bool operator ==(const NullArgumentNode & arg) const;

        std::string name() const {return name_;}
    private:
        std::string name_;
    };

    class IntArgumentNode : public ArgumentNode {
    public:
        IntArgumentNode(int index);
        virtual std::string to_string() const override;
        virtual int concrete(const std::vector<std::string> & args) const;

        bool operator ==(const ArgumentNode & arg) const;
        bool operator ==(const StringArgumentNode & arg) const;
        bool operator ==(const IntArgumentNode & arg) const;
        bool operator ==(const NullArgumentNode & arg) const;

        int index() const {return index_;}
    private:
        int index_;
    };

    class NullArgumentNode : public ArgumentNode {
    public:
        NullArgumentNode();
        virtual std::string to_string() const override;
        virtual int concrete(const std::vector<std::string> & args) const;
        virtual bool is_all() const override;

        bool operator ==(const ArgumentNode & arg) const;
        bool operator ==(const StringArgumentNode & arg) const;
        bool operator ==(const IntArgumentNode & arg) const;
        bool operator ==(const NullArgumentNode & arg) const;
    };

#endif // SWIG

    class Argument : public  SharedObject {
    public:
#ifndef SWIG
        ArgumentNode* get() const ;
        ArgumentNode* operator->() const ;
#endif // SWIG
        Argument();
        Argument(const std::string &name);
        Argument(int index);
        Argument(const Parameter& parameter);

        int concrete(const std::vector<std::string> & args) const;

        static std::vector<Argument> from_vector(const std::vector<int>& ind);
        static std::vector<Argument> from_vector(const std::vector<std::string>& ind);
        static std::vector<Argument> from_vector(const std::vector<Parameter>& ind);

        std::string type() const;
        bool is_all() const;

        bool operator==(const Argument & arg) const;

        static std::vector< int > concrete(const std::vector< Argument >& args, const std::vector< std::string >& strings);
        static std::vector< int > concrete(const std::vector< Argument >& args, const TensorBasis& tb);
    };
} // namespace spline
#endif //CPP_SPLINES_INDEX_H
