classdef OptiSplineYalmipInterface < handle

  properties
      yalmip_variables
      yalmip_optimizer
      yalmip_constraints
      yalmip_parameters
      yalmip_objective
      yalmip_options
      yalmip_diagnostics
      expand
      variables
      verbose
      use_optimize
  end
  methods
      
      function [ self ] = OptiSplineYalmipInterface()
        self.yalmip_variables = {};
        self.use_optimize = false;
      end
      function [out] = yalmip_expr_primitive( opti, vars, expr, args )
          [~,name,~] = fileparts(tempname);
          name = ['yalmip_helper_' name];
          if opti.verbose
            disp('  Create MX graph')
          end
          if isempty(vars)
            vars = {};
          end
          helper = casadi.Function(name, vars, expr);
          if opti.expand
            if opti.verbose
               disp('  Expanding to SX')
            end
            helper = helper.expand();
          end
          clear(name);
          if opti.verbose
            disp('  Dump graph to matlab file with yalmip instructions')
          end
          opti.matlab_dump(helper, [pwd filesep name '.m']);
          rehash
          
          if opti.verbose
            disp('  Run matlab file with sdpvar inputs')
          end
          if opti.verbose
            tic
          end
          [out{1:length(expr)}] = feval(name, args{:});
          if opti.verbose
            toc
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
          u = opti.user_dict(all_vars{i});
          
          if isfield(u,'binvar')
            var = @binvar;
          elseif isfield(u,'intvar')
            var = @intvar;
          else
            var = @sdpvar;
          end
          
          if strcmp(m.attribute,'symmetric')
              ind = find(tril(ones(m.n, m.m)));
              arg = var(m.n, m.m, 'symmetric');
              arg = arg(ind);
          elseif strcmp(m.attribute,'full');
              arg = var(m.n, m.m, 'full');
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

          yalmip_options = sdpsettings('solver','mosek','verbose',2);
        else
          yalmip_options = options.yalmip_options;
        end
        
        opti.expand = false;
        if isfield(options, 'expand')
          opti.expand = options.expand
        end
        opti.verbose = false;
        if isfield(options, 'verbose')
          opti.verbose = options.verbose
        end
        opti.use_optimize = false;
        if isfield(options, 'use_optimize')
          opti.use_optimize = options.use_optimize;
        end
        
        opti.yalmip_options = yalmip_options;
 
      end

      function out = solve(self)
      
          opti = self.advanced();
          opti.yalmip_options = self.yalmip_options;
          opti.expand = self.expand;
          opti.verbose = self.verbose;
          opti.use_optimize = self.use_optimize;
          
          if opti.problem_dirty
            opti.bake();
            g = opti.constraints;
            f = opti.objective;
            e = veccat(f,g{:});
            vars_x = opti.symvar(e, casadi.OPTI_VAR);
            vars_p = opti.symvar(e, casadi.OPTI_PAR);

            if opti.verbose
              disp('Converting CasADi graph (variables) to Yalmip')
            end
            vars_x_yalmip = opti.yalmip_var(vars_x);
            vars_p_yalmip = opti.yalmip_var(vars_p);
            if opti.verbose
              disp('Done')
            end
            vars_x_yalmip = cellfun(@(e) e(:), vars_x_yalmip, 'uni', false);
            vars_p_yalmip = cellfun(@(e) e(:), vars_p_yalmip, 'uni', false);
          
            if opti.verbose
              disp('Converting CasADi graph (constraints) to Yalmip')
            end
            constr = opti.yalmip_expr(g);
            c = [constr{:}];
            if isempty(c)
              c = zeros(0,1);
            end
            if opti.verbose
              disp('Done')
              disp('Converting CasADi graph (objective) to Yalmip')
            end
            self.yalmip_objective = opti.yalmip_expr(f);
            if opti.verbose
              disp('Done')
            end
            self.variables = vertcat(vars_x_yalmip{:});
            self.yalmip_parameters = vertcat(vars_p_yalmip{:});
            
            if ~opti.use_optimize
              if opti.verbose
                disp('Call yalmip ''optimizer''')
              end
              sol = optimizer(c, self.yalmip_objective, self.yalmip_options, self.yalmip_parameters, self.variables);
              if opti.verbose
                disp('Done')
              end
              self.yalmip_optimizer = sol;
            end
            self.yalmip_constraints = c;
          end

          opti.solve_prepare();
          a = opti.arg();
          
          if ~isempty(self.yalmip_parameters)
            assign(self.yalmip_parameters,full(a.p));
          end
          
          if opti.use_optimize
            f = self.yalmip_objective;
            if ~isempty(self.yalmip_parameters)
              c = replace(c,self.yalmip_parameters,full(a.p));
              f = replace(f,self.yalmip_parameters,full(a.p));
            end
            self.yalmip_diagnostics = optimize(c,f, self.yalmip_options);
            r = value(self.variables);
          else
            r = self.yalmip_optimizer{full(a.p)};
            assign(self.variables,r);
          end

          opti.res(struct('x',r));
          out = splines.OptiSplineSol(splines.OptiSpline(opti));
     end
   end
end

