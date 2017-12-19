function []=plotspline(f)

    b = f.basis;
    c = f.coeff_tensor;

    xf = linspace(b.domain.min,b.domain.max,1000);

    hold on
    plot(xf,f.list_eval(xf),'b','linewidth',4);

    for k=b.knots
      plot([k,k],[min(c) max(c)],'r:','linewidth',3)
    end

    plot(b.greville,c,'ko--','linewidth',2,'MarkerSize',8);

end
