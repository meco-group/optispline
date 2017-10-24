classdef OptiSplineYalmipInterface < handle

  properties
      yalmip_variables
      yalmip_optimizer
      yalmip_constraints
      yalmip_parameters
      yalmip_objective
      yalmip_options
      variables
  end
  methods
      
      function [ self ] = OptiSplineYalmipInterface()
        self.yalmip_variables = {};
      end
      function [out] = yalmip_expr_primitive( opti, vars, expr, args )
      
          name = 'yalmip_helper';
          disp('  Create SX graph')
          helper = casadi.Function(name, vars, expr);
          helper = helper.expand();
          
          clear(name);
          disp('  Dump SX graph to matlab file with yalmip instructions')
          opti.matlab_dump(helper, [pwd filesep name '.m']);
          rehash

          disp('  Run matlab file')
          tic
          [out{1:length(expr)}] = feval(name, args);
          toc
          disp('  Post-processing')
          
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
            return;
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
              if (expr_types{i}==casadi.OPTI_DOUBLE_INEQUALITY || expr_types{i}==casadi.OPTI_INEQUALITY || expr_types{i}==casadi.OPTI_GENERIC_INEQUALITY)
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
              elseif (expr_types{i}==casadi.OPTI_EQUALITY || expr_types{i}==casadi.OPTI_GENERIC_EQUALITY)
                  out{i} = out{i}==ub{i};
              elseif (expr_types{i}==casadi.OPTI_PSD)
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
      
      function [ opti ] = solver(opti, solver, varargin)
        
        if length(varargin)==0
          options = struct();
        elseif length(varargin)==1
          options = varargin{1};
        else
          error('Invalid arguments');
        end
 
        if isempty(options) || ~isfield(options, 'yalmip_options')

          yalmip_options = sdpsettings('solver','lmilab','verbose',2);
        else
          yalmip_options = options.yalmip_options;
        end
   
        opti.yalmip_options = yalmip_options;
 
      end

      function out = solve(self)
      
          opti = self.advanced()
          opti.yalmip_options = self.yalmip_options;

          if opti.problem_dirty
            opti.bake();
            g = opti.constraints;
            f = opti.objective;
            e = veccat(f,g{:});
            vars_x = opti.symvar(e, casadi.OPTI_VAR);
            vars_p = opti.symvar(e, casadi.OPTI_PAR);

            disp('Converting CasADi graph (variables) to Yalmip')
            vars_x_yalmip = opti.yalmip_var(vars_x);
            vars_p_yalmip = opti.yalmip_var(vars_p);
            disp('Done')

            vars_x_yalmip = cellfun(@(e) e(:), vars_x_yalmip, 'uni', false);
            vars_p_yalmip = cellfun(@(e) e(:), vars_p_yalmip, 'uni', false);
          
            disp('Converting CasADi graph (constraints) to Yalmip')
            constr = opti.yalmip_expr(g);
            c = [constr{:}];
            if isempty(c)
              c = zeros(0,1);
            end
            disp('Done')
            disp('Converting CasADi graph (objective) to Yalmip')
            self.yalmip_objective = opti.yalmip_expr(f);
            disp('Done')
            
            self.variables = vertcat(vars_x_yalmip{:});
            self.yalmip_parameters = vertcat(vars_p_yalmip{:});
            
            disp('Call yalmip ''optimizer''')
            sol = optimizer(c, self.yalmip_objective, self.yalmip_options, self.yalmip_parameters, self.variables);
            disp('Done')
            self.yalmip_optimizer = sol;
            self.yalmip_constraints = c;
          end

          opti.solve_prepare();
          a = opti.arg();
          r = self.yalmip_optimizer{full(a.p)};
          %if ~isempty(self.constraints)
          %  checkset(self.constraints);
          %end
          assign(self.variables,r);
          if ~isempty(self.yalmip_parameters)
            assign(self.yalmip_parameters,full(a.p));
          end
          %if ~isempty(self.yalmip_constraints)
          %  checkset(self.yalmip_constraints)
          %end
          opti.res(struct('x',r));
          out = splines.OptiSplineSol(splines.OptiSpline(opti));
     end
   end
end

