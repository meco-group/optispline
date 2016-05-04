//
// Created by erik on 11/04/16.
//

#include <vector>
#include "UnivariateBasis.h"

namespace spline {

//     UnivariateBasis::UnivariateBasis (int dimension, const std::vector<int> &size, const Argument &argument, int degree)
//             : Basis(dimension, size, argument), degree(degree) {
//         setDimension(1);
//     }
//  
//     UnivariateBasis::UnivariateBasis (int degree, std::string argument){
//         initUnivariateBasis(degree, argument);
//     }
//  
//  /    UnivariateBasis::UnivariateBasis (int degree) {
//  /        //TODO(ErikLambre) make default code for arg
//  /        std::string arg = "__";
//  /        initUnivariateBasis(degree, arg);
//  /    }
//  
//     void UnivariateBasis::initUnivariateBasis (int degree, std::string argument){
//         UnivariateBasis::degree = degree;
//         setArguments(argument);
//         setDimensions(1);
//     }
//  
//  
//     void UnivariateBasis::increaseDegree (int d) {
//         degree += d;
//     }
//  
//  
//     std::vector< std::vector<double> > UnivariateBasis::evalBasis(const std::vector<double>& x) const{
//         std::vector< std::vector<double> > r(x.size(), std::vector<double>(length()));
//         for (int i = 0; i < x.size(); ++i) {
//             r[i] = evalBasis(x[i]);
//         }
//         return r;
//     }
//  
//  
//  /    TODO(ErikLambre) check order
//     casadi::DM UnivariateBasis::transformation( const Basis &b) const {
//         std::vector<double> grid = evaluationGrid();
//         casadi::DM A(evalBasis(grid));
//         casadi::DM B(b.evalBasis(grid));
//         return casadi::DM::solve(A, B);
//     }
//  
//  
    Basis UnivariateBasis::operator+ (const Basis &other) const  { return other + *this; } 
    Basis UnivariateBasis::operator+ (const MonomialeBasis &other) const  { 
        return Basis();//plusBasis(*this, other);
    } 
    Basis UnivariateBasis::operator+ (const BSplineBasis &other) const  { 
       return Basis(); 
        //return plusBasis(*this, other); 
      }

//     void UnivariateBasis::setArguments (const std::string& argument){
//         std::vector<std::string> argumentList = {argument};
//         Basis::setArguments(argumentList);
//     }
//  
//  
//     void UnivariateBasis::setDegree (int degree) {
//         UnivariateBasis::degree = degree;
//     }
//  
//     int UnivariateBasis::getLenght () const {
//         return size[0];
//     }
//  

}  // namespace spline

