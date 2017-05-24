classdef OptiSplineYalmip < splines.OptiSpline

  properties
      yalmip_variables
  end
  methods
      
      function [ self ] = OptiSplineYalmip()
        self@splines.OptiSpline();
        self.yalmip_variables = {};
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

          expr_canon = {};
          expr_types = {};
          for i=1:length(expr)
              [e,t] = opti.canon_expr(expr{i});
              expr_canon = {expr_canon{:} e};
              expr_types = {expr_types{:} t};
          end

          helper = casadi.Function('helper', vars, expr_canon);
          helper = helper.expand();

          name = 'yalmip_helper';
          clear(name);

          main = fopen([name '.m'],'w');

          fprintf(main,['function [varargout] = ' name '(args)\n']);

          algorithm = strsplit(helper.getDescription(),'\n');

          for i=1:helper.n_in()
           fprintf(main,'argin_%d = args{%d};\n',i-1,i);
          end
          for i=1:helper.n_out()
           fprintf(main,'argout_%d = cell(%d,1);\n',i-1,helper.nnz_out(i-1));
          end
          for i=1:numel(algorithm)
          l = algorithm{i};
          if strfind(l, '@')
            break
          end
          end
          algorithm = strjoin(algorithm(i:end),'\n');

          algorithm = regexprep(algorithm,'@','at');
          algorithm = regexprep(algorithm,'input\[(\d+)\]\[(\d+)\]','argin_$1(${num2str(str2num($2)+1)})');
          algorithm = regexprep(algorithm,'output\[(\d+)\]\[(\d+)\]','argout_$1{${num2str(str2num($2)+1)}}');
          algorithm = regexprep(algorithm,'sq\((.*?)\)','($1)^2');

          fprintf(main,algorithm);
          for i=1:helper.n_out()
          fprintf(main,'varargout{%d} = [argout_%d{:}];\n',i,i-1);
          end

          fprintf(main,'end\n');

          fclose(main);
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

              if (expr_types{i}==opti.OPTISTACK_INEQUALITY)
                  out{i} = out{i}<=0;
              elseif (expr_types{i}==opti.OPTISTACK_EQUALITY)
                  out{i} = out{i}==0;
              elseif (expr_types{i}==opti.OPTISTACK_PSD)
                  out{i} = 0.5*(out{i}+out{i}')>=0;
              end
          end

          delete([name '.m'])

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
          m = opti.meta(vars{i});
          c = m.count;
          counts(i) = c+1;
        end


        all_vars = opti.symvar();

        for i=length(opti.yalmip_variables)+1:max(counts)
          m = opti.meta(all_vars{i});
          if strcmp(m.variable_type,'symmetric')
              ind = find(tril(ones(m.m, m.n)));
              arg = sdpvar(m.m, m.n, 'symmetric');
              arg = arg(ind);
          elseif strcmp(m.variable_type,'full');
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

