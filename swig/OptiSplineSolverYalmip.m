classdef OptiSplineSolverYalmip < splines.OptiSplineSolver

  properties
    sol

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

        constr = opti.yalmip_expr(g);
        c = [constr{:}];
        if isempty(c)
          c = zeros(0,1);
        end
        sol = optimizer(c, opti.yalmip_expr(f), yalmip_options, [vars_p_yalmip{:}], vertcat(vars_x_yalmip{:}));
        self.sol = sol;

      end


      function [ ] = solve(self)
          self.solve_prepare();
          a = self.arg();
          r = self.sol{full(a.p)};
          self.res(struct('x',r));
     end
  end
end
