classdef OptiSplineYalmip < splines.OptiSpline

  properties
      yalmip_variables
  end
  methods
      
      function [ self ] = OptiSplineYalmip()
        self@splines.OptiSpline();
        self.yalmip_variables = {};
      end
      function [out] = yalmip_expr_primitive( opti, vars, expr, args )
      
          name = 'yalmip_helper';
          helper = casadi.Function(name, vars, expr);
          helper = helper.expand();
          
          clear(name);
          opti.matlab_dump(helper, [pwd filesep name '.m']);
          rehash

          [out{1:length(expr)}] = feval(name, args);
          
          for i=1:helper.n_out()
              sp = helper.sparsity_out(i-1);
              if (sp.is_dense())
                  out{i} = reshape(out{i},size(sp));
              else
                  [sp_i,sp_j] = sp.get_triplet();
                  [m,n] = size(sp);
                  out{i} = sparse(sp_i+1,sp_j+1,out{i},m,n);
              end
          end
          
          delete([name '.m'])
      end
      function [ out ] = yalmip_expr( opti, expr )

          if (~iscell(expr))
             out = yalmip_expr( opti, {expr} );
             out = out{1};
             return;
          end

          if length(expr)==0
            out = {};
            return
          end
          vars = opti.symvar(veccat(expr{:}));
          args = yalmip_var(opti, vars);

          expr_canon = cell(1,length(expr));
          expr_lb = cell(1,length(expr));
          expr_ub = cell(1,length(expr));
          expr_types = cell(1,length(expr));
          for i=1:length(expr)
              e = opti.canon_expr(expr{i});
              expr_canon{i} = e.canon;
              expr_lb{i} = e.lb;
              expr_ub{i} = e.ub;
              expr_types{i} = e.type;
          end

          out = yalmip_expr_primitive(opti, vars, expr_canon, args);
          
          lb = yalmip_expr_primitive(opti, vars, expr_lb, args);
          ub = yalmip_expr_primitive(opti, vars, expr_ub, args);

          for i=1:length(out)
              if (expr_types{i}==opti.OPTI_DOUBLE_INEQUALITY || expr_types{i}==opti.OPTI_INEQUALITY || expr_types{i}==opti.OPTI_GENERIC_INEQUALITY)
                if isinf(lb{i})
                  if isinf(ub{i})
                    out{i} = {};
                  else
                    out{i} = out{i}<=ub{i};
                  end
                else
                  if isinf(ub{i})
                    out{i} = lb{i}<=out{i};
                  else
                    out{i} = lb{i}<=out{i}<=ub{i};
                  end
                end
              elseif (expr_types{i}==opti.OPTI_EQUALITY || expr_types{i}==opti.OPTI_GENERIC_EQUALITY)
                  out{i} = out{i}==ub{i};
              elseif (expr_types{i}==opti.OPTI_PSD)
                  out{i} = 0.5*(out{i}+out{i}')>=0;
              end
          end


      end

      function [ ret ] = yalmip_var( opti, vars )

        if (~iscell(vars))
           ret = opti.yalmip_var(opti.symvar(vars));
           if length(ret)==1
             ret = ret{1};
           end
           return;
        end

        N = length(vars);

        counts = zeros(1,N);
        for i=1:length(vars)
          m = opti.get_meta(vars{i});
          c = m.count;
          counts(i) = c+1;
        end


        all_vars = opti.symvar();

        for i=length(opti.yalmip_variables)+1:max(counts)
          m = opti.get_meta(all_vars{i});
          if strcmp(m.attribute,'symmetric')
              ind = find(tril(ones(m.m, m.n)));
              arg = sdpvar(m.m, m.n, 'symmetric');
              arg = arg(ind);
          elseif strcmp(m.attribute,'full');
              arg = sdpvar(m.m, m.n, 'full');
          end

          opti.yalmip_variables{i} = arg;
        end

        ret = opti.yalmip_variables(counts);

      end

      function [ sol ] = solver(self, f, g, solver, varargin)
        if length(varargin)==0
          options = struct();
        elseif length(varargin)==1
          options = varargin{1};
        else
          error('Invalid arguments');
        end

        if strcmp(solver,'yalmip')
          [sol] = OptiSplineSolverYalmip(self, f, g, solver, options);
        else
          [sol] = self.solver@splines.OptiSpline(f, g, solver, options);
        end
      end
   end
end

