#ifndef CPP_SPLINE_UNIVARIATEBASIS_H
#define CPP_SPLINE_UNIVARIATEBASIS_H

#include <casadi/casadi.hpp>
#include <string>

#include "Basis.h"
#include "../SharedObject/SharedObject.h"
#include "../SharedObject/SharedObjectNode.h"

namespace spline {
    
class MonomialeBasis;
class BSplineBasis;

#ifndef SWIG

class UnivariateBasisNode : public BasisNode {
public:
//   UnivariateBasis (int dimension, const std::vector<int> &size, const Argument &argument, int degree);
//   UnivariateBasis (int degree);
//   UnivariateBasis (int degree, std::string argument);
//
//   //    evalution Basis
//   std::vector<double> operator()(double x) const;
//   virtual std::vector< std::vector<double> > operator()(const std::vector<double>& x) const = 0;
///    std::vector< double > operator()(const std::vector<double, 1>& x) const;
//   std::vector< std::vector< std::vector<double> > > operator()(const std::vector< std::vector<double> >& x) const;
//
///        virtual std::shared_ptr<Basis> plus(Basis &other) = 0;
//
//   int getDegree () const { return degree;}
//   void setDegree (int degree);
//   void increaseDegree(int d);
//
//   int getLenght() const ;
//
//   casadi::DM transformation( const Basis &b) const;
//
//   virtual std::vector<double> evaluationGrid (void) const = 0;
    UnivariateBasisNode() {};
    virtual Basis operator+ (const Basis& other) const;
    virtual Basis operator+ (const MonomialeBasis& other) const;
    virtual Basis operator+ (const BSplineBasis& other) const;
    virtual std::string getRepresentation() const ;

protected:
    int degree;
};

#endif // SWIG

class UnivariateBasis : public Basis {
public:
#ifndef SWIG
//   UnivariateBasis (int dimension, const std::vector<int> &size, const Argument &argument, int degree);
//   UnivariateBasis (int degree);
//   UnivariateBasis (int degree, std::string argument);
//
//   //    evalution Basis
//   std::vector<double> operator()(double x) const;
//   virtual std::vector< std::vector<double> > operator()(const std::vector<double>& x) const = 0;
///    std::vector< double > operator()(const std::vector<double, 1>& x) const;
//   std::vector< std::vector< std::vector<double> > > operator()(const std::vector< std::vector<double> >& x) const;
//
///        virtual std::shared_ptr<Basis> plus(Basis &other) = 0;
//
//   int getDegree () const { return degree;}
//   void setDegree (int degree);
//   void increaseDegree(int d);
//
//   int getLenght() const ;
//
//   casadi::DM transformation( const Basis &b) const;
//
//   virtual std::vector<double> evaluationGrid (void) const = 0;
    UnivariateBasis() { };
    UnivariateBasisNode* get() const ;
    UnivariateBasisNode* operator->() const ;
#endif // SWIG
    virtual Basis operator+ (const Basis& other) const;
    virtual Basis operator+ (const MonomialeBasis& other) const;
    virtual Basis operator+ (const BSplineBasis& other) const;

    virtual std::string getRepresentation() const ;
    };
} // namespace spline

#endif //CPP_SPLINE_UNIVARIATEBASIS_H
