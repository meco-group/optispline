classdef OptiSplineSolverYalmip < splines.OptiSplineSolver

  properties
    sol
    constraints
    variables
    parameters

  end
  methods


      function [ self ] = OptiSplineSolverYalmip(opti, f, g, solver, varargin )
        self@splines.OptiSplineSolver(opti, f, g);

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
        e = veccat(f,g{:});
        vars_x = opti.symvar(e, opti.OPTISTACK_VAR);
        vars_p = opti.symvar(e, opti.OPTISTACK_PAR);

        vars_x_yalmip = opti.yalmip_var(vars_x);
        vars_p_yalmip = opti.yalmip_var(vars_p);

        vars_x_yalmip = cellfun(@(e) e(:), vars_x_yalmip, 'uni', false);
        vars_p_yalmip = cellfun(@(e) e(:), vars_p_yalmip, 'uni', false);
                
        constr = opti.yalmip_expr(g);
        c = [constr{:}];
        if isempty(c)
          c = zeros(0,1);
        end

        sol = optimizer(c, opti.yalmip_expr(f), yalmip_options, vertcat(vars_p_yalmip{:}), vertcat(vars_x_yalmip{:}));
        self.sol = sol;
        self.constraints = c;
        self.variables = vertcat(vars_x_yalmip{:});
        self.parameters = vertcat(vars_p_yalmip{:});
      end


      function [ ] = solve(self)
          self.solve_prepare();
          a = self.arg();
          r = self.sol{full(a.p)};
          %if ~isempty(self.constraints)
          %  checkset(self.constraints);
          %end
          assign(self.variables,r);
          if ~isempty(self.parameters)
            assign(self.parameters,full(a.p));
          end
          %if ~isempty(self.constraints)
          %  checkset(self.constraints)
          %end
          self.res(struct('x',r));
     end
  end
end
