#include "optistack.h"
#include "../common.h"

using namespace casadi;

spline::Function OptiSplineAdvanced::value(const spline::Function& f) const {
  return spline::Function(f.tensor_basis(), value(f.coeff()));
}

spline::Coefficient OptiSplineAdvanced::value(const spline::Coefficient& c) const {
  return spline::Coefficient(value(c.data()));
}

AnyTensor OptiSplineAdvanced::value(const AnyTensor& t) const {
  if (t.is_DT()) return t.as_DT();
  if (t.is_MT()) return value(t.as_MT());
  spline_assert_message(false, "Value only supported for MX");
  return DT();
}

Tensor<DM> OptiSplineAdvanced::value(const Tensor<MX>& t, const std::vector< Tensor<MX> >& values) const {
  std::vector<MX> values_data;
  for (const auto& e: values) values_data.push_back(e.data());
  return Tensor<DM>(OptiAdvanced::value(t.data(), values_data), t.dims());
}



  void export_code_body(const casadi::Function& f, const std::string& lang,
      std::ostream &ss, const Dict& options) {

    // Default values for options
    int indent_level = 0;

    // Read options
    for (auto&& op : options) {
      if (op.first=="indent_level") {
        indent_level = op.second;
      } else {
        casadi_error("Unknown option '" + op.first + "'.");
      }
    }

    // Construct indent string
    std::string indent = "";
    for (int i=0;i<indent_level;++i) {
      indent += "  ";
    }

    // Loop over algorithm
    for (int k=0;k<f.n_instructions();++k) {
      // Get operation
      int op = f.instruction_id(k);
      // Get MX node
      MX x = f.instruction_MX(k);
      // Get input positions into workvector
      std::vector<int> o = f.instruction_output(k);
      // Get output positions into workvector
      std::vector<int> i = f.instruction_input(k);

      switch (op) {
        case OP_INPUT:
          ss << indent << "w" << o[0] << " = varargin{" << i[0]+1 << "};" << std::endl;
          break;
        case OP_OUTPUT:
          {
            Dict info = x.info();
            int segment = info["segment"];
            x.dep(0).sparsity().export_code("matlab", ss,
              {{"name", "sp_in"}, {"indent_level", indent_level}, {"as_matrix", true}});
            ss << indent << "argout_" << o[0] << "{" << (1+segment) << "} = ";
            ss << "w" << i[0] << "(sp_in==1);" << std::endl;
          }
          break;
        case OP_CONST:
          {
            DM v = static_cast<DM>(x);
            Dict opts;
            opts["name"] = "m";
            opts["indent_level"] = indent_level;
            v.export_code("matlab", ss, opts);
            ss << indent << "w" << o[0] << " = m;" << std::endl;
          }
          break;
        case OP_SQ:
          ss << indent << "w" << o[0] << " = " << "w" << i[0] << ".^2;" << std::endl;
          break;
        case OP_MTIMES:
          ss << indent << "w" << o[0] << " = ";
          ss << "w" << i[1] << "*w" << i[2] << "+w" << i[0] << ";" << std::endl;
          break;
        case OP_MUL:
          {
            std::string prefix = (x.dep(0).is_scalar() || x.dep(1).is_scalar()) ? "" : ".";
            ss << indent << "w" << o[0] << " = " << "w" << i[0] << prefix << "*w" << i[1] << ";";
            ss << std::endl;
          }
          break;
        case OP_TWICE:
          ss << indent << "w" << o[0] << " = 2*w" << i[0] << ";" << std::endl;
          break;
        case OP_INV:
          ss << indent << "w" << o[0] << " = 1./w" << i[0] << ";" << std::endl;
          break;
        case OP_DOT:
          ss << indent << "w" << o[0] << " = dot(w" << i[0] << ",w" << i[1]<< ");" << std::endl;
          break;
        case OP_BILIN:
          ss << indent << "w" << o[0] << " = w" << i[1] << ".'*w" << i[0]<< "*w" << i[2] << ";";
          ss << std::endl;
          break;
        case OP_RANK1:
          ss << indent << "w" << o[0] << " = w" << i[0] << "+";
          ss << "w" << i[1] << "*w" << i[2] << "*w" << i[3] << ".';";
          ss << std::endl;
          break;
        case OP_FABS:
          ss << indent << "w" << o[0] << " = abs(w" << i[0] << ");" << std::endl;
          break;
        case OP_DETERMINANT:
          ss << indent << "w" << o[0] << " = det(w" << i[0] << ");" << std::endl;
          break;
        case OP_INVERSE:
          ss << indent << "w" << o[0] << " = inv(w" << i[0] << ");";
          ss << "w" << o[0] << "(w" << o[0] << "==0) = 1e-200;" << std::endl;
          break;
        case OP_SOLVE:
          {
            bool tr = x.info()["tr"];
            if (tr) {
              ss << indent << "w" << o[0] << " = ((w" << i[1] << ".')\\w" << i[0] << ").';";
              ss << std::endl;
            } else {
              ss << indent << "w" << o[0] << " = w" << i[1] << "\\w" << i[0] << ";" << std::endl;
            }
            ss << "w" << o[0] << "(w" << o[0] << "==0) = 1e-200;" << std::endl;
          }
          break;
        case OP_DIV:
          {
            std::string prefix = (x.dep(0).is_scalar() || x.dep(1).is_scalar()) ? "" : ".";
            ss << indent << "w" << o[0] << " = " << "w" << i[0] << prefix << "/w" << i[1] << ";";
            ss << std::endl;
          }
          break;
        case OP_POW:
        case OP_CONSTPOW:
          ss << indent << "w" << o[0] << " = " << "w" << i[0] << ".^w" << i[1] << ";" << std::endl;
          break;
        case OP_TRANSPOSE:
          ss << indent << "w" << o[0] << " = " << "w" << i[0] << ".';" << std::endl;
          break;
        case OP_HORZCAT:
        case OP_VERTCAT:
          {
            ss << indent << "w" << o[0] << " = [";
            for (int e : i) {
              ss << "w" << e << (op==OP_HORZCAT ? " " : ";");
            }
            ss << "];" << std::endl;
          }
          break;
        case OP_DIAGCAT:
          {
            for (int k=0;k<i.size();++k) {
              x.dep(k).sparsity().export_code("matlab", ss,
                {{"name", "sp_in" + str(k)}, {"indent_level", indent_level}, {"as_matrix", true}});
            }
            ss << indent << "w" << o[0] << " = [";
            for (int k=0;k<i.size();++k) {
              ss << "w" << i[k] << "(sp_in" << k << "==1);";
            }
            ss << "];" << std::endl;
            Dict opts;
            opts["name"] = "sp";
            opts["indent_level"] = indent_level;
            opts["as_matrix"] = false;
            x.sparsity().export_code("matlab", ss, opts);
            ss << indent << "w" << o[0] << " = ";
            ss << "sparse(sp_i, sp_j, w" << o[0] << ", sp_m, sp_n);" << std::endl;
          }
          break;
        case OP_HORZSPLIT:
        case OP_VERTSPLIT:
          {
            Dict info = x.info();
            std::vector<int> offset = info["offset"];
            casadi::Function output = info["output"];
            std::vector<Sparsity> sp;
            for (int i=0;i<output.n_out();i++)
              sp.push_back(output.sparsity_out(i));
              for (int k=0;k<o.size();++k) {
                if (o[k]==-1) continue;
                x.dep(0).sparsity().export_code("matlab", ss,
                  {{"name", "sp_in"}, {"indent_level", indent_level}, {"as_matrix", true}});
                ss << indent << "tmp = w" << i[0]<< "(sp_in==1);" << std::endl;
                Dict opts;
                opts["name"] = "sp";
                opts["indent_level"] = indent_level;
                opts["as_matrix"] = false;
                sp[k].export_code("matlab", ss, opts);
                ss << indent << "w" << o[k] << " = sparse(sp_i, sp_j, ";
                ss << "tmp(" << offset[k]+1 << ":" << offset[k+1] << "), sp_m, sp_n);" << std::endl;
             }
           }
           break;
         case OP_GETNONZEROS:
         case OP_SETNONZEROS:
            {
            Dict info = x.info();

            std::string nonzeros;
            if (info.find("nz")!=info.end()) {
              nonzeros = "1+" + str(info["nz"]);
            } else if (info.find("slice")!=info.end()) {
              Dict s = info["slice"];
              int start = s["start"];
              int step = s["step"];
              int stop = s["stop"];
              nonzeros = str(start+1) + ":" + str(step) + ":" + str(stop);
              nonzeros = "nonzeros(" + nonzeros + ")";
            } else {
              Dict inner = info["inner"];
              Dict outer = info["outer"];
              int inner_start = inner["start"];
              int inner_step  = inner["step"];
              int inner_stop  = inner["stop"];
              int outer_start = outer["start"];
              int outer_step  = outer["step"];
              int outer_stop  = outer["stop"];
              std::string inner_slice = "(" + str(inner_start) + ":" +
                str(inner_step) + ":" + str(inner_stop-1)+")";
              std::string outer_slice = "(" + str(outer_start+1) + ":" +
                str(outer_step) + ":" + str(outer_stop)+")";
              int N = range(outer_start, outer_stop, outer_step).size();
              int M = range(inner_start, inner_stop, inner_step).size();
              nonzeros = "repmat("+ inner_slice  +"', 1, " + str(N) + ")+" +
                         "repmat("+ outer_slice  +", " + str(M) + ", 1)";
              nonzeros = "nonzeros(" + nonzeros + ")";
            }

            Dict opts;
            opts["name"] = "sp";
            opts["indent_level"] = indent_level;
            opts["as_matrix"] = false;
            x.sparsity().export_code("matlab", ss, opts);

            if (op==OP_GETNONZEROS) {
              x.dep(0).sparsity().export_code("matlab", ss,
                {{"name", "sp_in"}, {"indent_level", indent_level}, {"as_matrix", true}});
              //ss << indent << "w" << i[0] << "" << std::endl;
              //ss << indent << "size(w" << i[0] << ")" << std::endl;
              ss << indent << "in_flat = w" << i[0] << "(sp_in==1);" << std::endl;
              //ss << indent << "in_flat" << std::endl;
              //ss << indent << "size(in_flat)" << std::endl;
              ss << indent << "w" << o[0] << " = in_flat(" << nonzeros << ");" << std::endl;
            } else {
              x.dep(0).sparsity().export_code("matlab", ss,
                {{"name", "sp_in0"}, {"indent_level", indent_level}, {"as_matrix", true}});
              x.dep(1).sparsity().export_code("matlab", ss,
                {{"name", "sp_in1"}, {"indent_level", indent_level}, {"as_matrix", true}});
              ss << indent << "in_flat = w" << i[1] << "(sp_in1==1);" << std::endl;
              ss << indent << "w" << o[0] << " = w" << i[0] << "(sp_in0==1);" << std::endl;
              ss << indent << "w" << o[0] << "(" << nonzeros << ")  = ";
              if (info["add"]) ss << "w" << o[0] << "(" << nonzeros << ") + ";
              ss << "in_flat;";
            }
            ss << indent << "w" << o[0] << " = ";
            ss << "sparse(sp_i, sp_j, w" << o[0] << ", sp_m, sp_n);" << std::endl;
          }
          break;
        case OP_PROJECT:
          {
            Dict opts;
            opts["name"] = "sp";
            opts["indent_level"] = indent_level;
            x.sparsity().export_code("matlab", ss, opts);
            ss << indent << "w" << o[0] << " = ";
            ss << "sparse(sp_i, sp_j, w" << i[0] << "(sp==1), sp_m, sp_n);" << std::endl;
          }
          break;
        case OP_NORM1:
          ss << indent << "w" << o[0] << " = norm(w" << i[0] << ", 1);" << std::endl;
          break;
        case OP_NORM2:
          ss << indent << "w" << o[0] << " = norm(w" << i[0] << ", 2);" << std::endl;
          break;
        case OP_NORMF:
          ss << indent << "w" << o[0] << " = norm(w" << i[0] << ", 'fro');" << std::endl;
          break;
        case OP_NORMINF:
          ss << indent << "w" << o[0] << " = norm(w" << i[0] << ", inf);" << std::endl;
          break;
        case OP_MMIN:
          ss << indent << "w" << o[0] << " = min(w" << i[0] << ");" << std::endl;
          break;
        case OP_MMAX:
          ss << indent << "w" << o[0] << " = max(w" << i[0] << ");" << std::endl;
          break;
        case OP_NOT:
          ss << indent << "w" << o[0] << " = ~" << "w" << i[0] << ";" << std::endl;
          break;
        case OP_OR:
          ss << indent << "w" << o[0] << " = w" << i[0] << " | w" << i[1] << ";" << std::endl;
          break;
        case OP_AND:
          ss << indent << "w" << o[0] << " = w" << i[0] << " & w" << i[1] << ";" << std::endl;
          break;
        case OP_NE:
          ss << indent << "w" << o[0] << " = w" << i[0] << " ~= w" << i[1] << ";" << std::endl;
          break;
        case OP_IF_ELSE_ZERO:
          ss << indent << "w" << o[0] << " = ";
          ss << "if_else_zero_gen(w" << i[0] << ", w" << i[1] << ");" << std::endl;
          break;
        case OP_RESHAPE:
          {
            x.dep(0).sparsity().export_code("matlab", ss,
              {{"name", "sp_in"}, {"indent_level", indent_level}, {"as_matrix", true}});
            x.sparsity().export_code("matlab", ss,
              {{"name", "sp_out"}, {"indent_level", indent_level}, {"as_matrix", false}});
            ss << indent << "w" << o[0] << " = sparse(sp_out_i, sp_out_j, ";
            ss << "w" << i[0] << "(sp_in==1), sp_out_m, sp_out_n);" << std::endl;
          }
          break;
        default:
          if (x.is_binary()) {
            ss << indent << "w" << o[0] << " = " << casadi::casadi_math<double>::print(op,
              "w"+std::to_string(i[0]), "w"+std::to_string(i[1])) << ";" << std::endl;
          } else if (x.is_unary()) {
            ss << indent << "w" << o[0] << " = " << casadi::casadi_math<double>::print(op,
              "w"+std::to_string(i[0])) << ";" << std::endl;
          } else {
            ss << "unknown" + x.class_name() << std::endl;
          }
      }
    }
  }


  void export_code(const casadi::Function& f, const std::string& lang, std::ostream &ss, const Dict& options) {

    casadi_assert(lang=="matlab", "Only matlab language supported for now.");

    // start function
    ss << "function [varargout] = " << f.name() << "(varargin)" << std::endl;

    // Allocate space for output argument (segments)
    for (int i=0;i<f.n_out();++i) {
      ss << "  argout_" << i <<  " = cell(" << f.nnz_out(i) << ",1);" << std::endl;
    }

    Dict opts;
    opts["indent_level"] = 1;
    export_code_body(f, lang, ss, opts);

    // Process the outputs
    for (int i=0;i<f.n_out();++i) {
      const Sparsity& out = f.sparsity_out(i);
      if (out.is_dense()) {
        // Special case if dense
        ss << "  varargout{" << i+1 <<  "} = reshape(vertcat(argout_" << i << "{:}), ";
        ss << out.size1() << ", " << out.size2() << ");" << std::endl;
      } else {
        // For sparse outputs, export sparsity and call 'sparse'
        Dict opts;
        opts["name"] = "sp";
        opts["indent_level"] = 1;
        opts["as_matrix"] = false;
        out.export_code("matlab", ss, opts);
        ss << "  varargout{" << i+1 <<  "} = ";
        ss << "sparse(sp_i, sp_j, vertcat(argout_" << i << "{:}), sp_m, sp_n);" << std::endl;
      }
    }

    // end function
    ss << "end" << std::endl;
    ss << "function y=nonzeros_gen(x)" << std::endl;
    ss << "  if isa(x,'casadi.SX') || isa(x,'casadi.MX') || isa(x,'casadi.DM')" << std::endl;
    ss << "    y = x{:};" << std::endl;
    ss << "  elseif isa(x,'sdpvar')" << std::endl;
    ss << "    b = getbase(x);" << std::endl;
    ss << "    f = find(sum(b~=0,2));" << std::endl;
    ss << "    y = sdpvar(length(f),1,[],getvariables(x),b(f,:));" << std::endl;
    ss << "  else" << std::endl;
    ss << "    y = nonzeros(x);" << std::endl;
    ss << "  end" << std::endl;
    ss << "end" << std::endl;
    ss << "function y=if_else_zero_gen(c,e)" << std::endl;
    ss << "  if isa(c+e,'casadi.SX') || isa(c+e,'casadi.MX') || isa(c+e,'casadi.DM')" << std::endl;
    ss << "    y = if_else(c, e, 0);" << std::endl;
    ss << "  else" << std::endl;
    ss << "    if c" << std::endl;
    ss << "        y = x;" << std::endl;
    ss << "    else" << std::endl;
    ss << "        y = 0;" << std::endl;
    ss << "    end" << std::endl;
    ss << "  end" << std::endl;
    ss << "end" << std::endl;


  }

void OptiSplineAdvanced::matlab_dump(const casadi::Function& f, const std::string &fname) const {
  if (f.is_a("MXFunction")) {
    std::ofstream ss(fname);
    export_code(f, "matlab", ss, Dict());
  } else {
    f.export_code("matlab", fname);
  }
}

spline::Function OptiSplineSol::value(const spline::Function& f) const {
    return static_cast<const OptiSplineAdvanced&>(optistack_).value(f);
}
spline::Coefficient OptiSplineSol::value(const spline::Coefficient& c) const {
    return static_cast<const OptiSplineAdvanced&>(optistack_).value(c);
}

AnyTensor OptiSplineSol::value(const AnyTensor& t) const {
  return static_cast<const OptiSplineAdvanced&>(optistack_).value(t);
}

OptiSplineAdvanced OptiSpline::debug() const {
  OptiSplineAdvanced ret;
  ret.own(get());
  return ret;
}
OptiSplineAdvanced OptiSpline::advanced() const {
  return debug();
}

OptiSplineSol OptiSpline::solve() {
  return Opti::solve();
}

OptiSplineSol OptiSplineAdvanced::solve() {
  return OptiAdvanced::solve();
}

OptiSplineAdvanced::OptiSplineAdvanced(const OptiSpline& rhs) {
  own(rhs.get());
}

OptiSpline OptiSplineSol::opti() const {
  Opti s = OptiSol::opti();
  OptiSpline ret;
  ret.own(s.get());
  return ret;
}

OptiSpline::OptiSpline(const OptiSplineAdvanced& rhs) {
  own(rhs.get());
}
