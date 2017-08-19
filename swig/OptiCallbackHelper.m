classdef OptiCallbackHelper < splines.OptiCallback

  properties
    callback
  end  
        
  methods
    function self = OptiCallbackHelper(callback)
      self@splines.OptiCallback();
      self.callback = callback;
    end
    function [] = call(self)
      self.callback();
    end    
  end
end
