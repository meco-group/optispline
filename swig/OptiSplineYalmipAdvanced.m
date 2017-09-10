classdef OptiSplineYalmipAdvanced < splines.OptiSplineAdvanced & OptiSplineYalmipInterface
   properties
   
   end
   methods
      function [ self ] = OptiSplineYalmipAdvanced(varargin)
        self@splines.OptiSplineAdvanced(varargin{:});
        self@OptiSplineYalmipInterface();
      end
      
      function out = solve(opti)
        out = solve@OptiSplineYalmipInterface(opti);
      end
      function [ opti ] = solver(opti, varargin)
        opti = solver@OptiSplineYalmipInterface(opti, varargin{:})
      end
   end
   
end

