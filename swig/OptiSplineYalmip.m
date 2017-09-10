classdef OptiSplineYalmip < splines.OptiSpline & OptiSplineYalmipInterface
   properties
   
   end
   methods
      function [ self ] = OptiSplineYalmip()
        self@splines.OptiSpline();
        self@OptiSplineYalmipInterface();
      end

      function out = solve(opti)
        out = solve@OptiSplineYalmipInterface(opti);
      end
      function [ opti ] = solver(opti, varargin)
        opti = solver@OptiSplineYalmipInterface(opti, varargin{:})
      end
      
      function out = debug(self)
        out = advanced(self);
      end
      
      function out = advanced(self)
        optispline_advanced = advanced@splines.OptiSpline(self);
        out = OptiSplineYalmipAdvanced(optispline_advanced);
      end
   end
end

