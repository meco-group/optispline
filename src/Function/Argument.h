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
        virtual casadi_int concrete(const std::vector<std::string> & args) const = 0;
        virtual bool is_all() const ;
    };

    class StringArgumentNode : public ArgumentNode {
    public:
        StringArgumentNode(const std::string &name);
        virtual std::string to_string() const override;
        virtual casadi_int concrete(const std::vector<std::string> & args) const;

        std::string name() const {return name_;}
    private:
        std::string name_;
    };

    class IntArgumentNode : public ArgumentNode {
    public:
        IntArgumentNode(casadi_int index);
        virtual std::string to_string() const override;
        virtual casadi_int concrete(const std::vector<std::string> & args) const;

        casadi_int index() const {return index_;}
    private:
        casadi_int index_;
    };

    class NullArgumentNode : public ArgumentNode {
    public:
        NullArgumentNode();
        virtual std::string to_string() const override;
        virtual casadi_int concrete(const std::vector<std::string> & args) const;
        virtual bool is_all() const override;
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
        Argument(casadi_int index);
        Argument(const Parameter& parameter);

        casadi_int concrete(const std::vector<std::string> & args) const;

        static std::vector<Argument> from_vector(const std::vector<casadi_int>& ind);
        static std::vector<Argument> from_vector(const std::vector<std::string>& ind);
        static std::vector<Argument> from_vector(const std::vector<Parameter>& ind);

        std::string type() const;
        bool is_all() const;

        static std::vector< casadi_int > concrete(const std::vector< Argument >& args, const std::vector< std::string >& strings);
        static std::vector< casadi_int > concrete(const std::vector< Argument >& args, const TensorBasis& tb);
        static casadi_int concrete(const Argument& arg, const TensorBasis& tb);
    };
} // namespace spline
#endif //CPP_SPLINES_INDEX_H
