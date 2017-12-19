import splines.*
close all

figure(1)

dmax = 4;
Nmax = 6;
c = 1;
for d=0:dmax
    for N=2:Nmax

        subplot(dmax+1,Nmax-1,c)
        c= c+1;
        b1 = BSplineBasis([0,1],d,N);

        E = eye(b1.dimension);


        hold on
        xf = linspace(0,1,200);
        for i=1:b1.dimension
            f = Function(b1,E(:,i));
            
            if i==2
                plot(xf,f.list_eval(xf),'b','linewidth',6);
            else
                plot(xf,f.list_eval(xf),'k','linewidth',2);
            end
        end
        
        for k=b1.knots
          plot([k,k],[0 1],'r:','linewidth',3)
        end
        i=1;
        for k=b1.greville
          if i==2
            plot(k,1,'bo','MarkerFaceColor','b','MarkerSize',8)
          else
            plot(k,1,'ko','MarkerFaceColor','k','MarkerSize',8)
          end
          i = i+1;
        end


        title(['d=' num2str(d) ',N=' num2str(N)])
    end
end

suptitle('BSplineBasis([0,1],d,N)')

fig = gcf;
fig.PaperUnits = 'inches';
fig.PaperPosition = [0 0 10 7];
print('bases.svg','-dsvg','-r0')
%inkscape bases.svg --export-png=bases.png --export-dpi=100

figure(2);clf

b1 = BSplineBasis([0,1],3,6);
c = zeros(b1.dimension,1);
for i=1:b1.dimension
   subplot(2,4,i)
   hold on
   c(i)=1;
   f = Function(b1,c);
   plot(xf,f.list_eval(xf),'b','linewidth',4);
   j=1;
   for k=b1.greville
      if j<=i
        plot(k,1,'bo','MarkerFaceColor','b','MarkerSize',8)
      else
        plot(k,0,'ko','MarkerFaceColor','k','MarkerSize',8)
      end
      j = j+1;
   end   
   ylim([0,1])
end


fig = gcf;
fig.PaperUnits = 'inches';
fig.PaperPosition = [0 0 10 7];
print('bases_add.svg','-dsvg','-r0')



figure(3);clf

b1 = BSplineBasis([0,1],3,6);
c = zeros(b1.dimension,1);
for i=1:b1.dimension
   subplot(2,4,i)
   hold on
   k = b1.greville;
   c(i)=k(i);
   f = Function(b1,c);
   plot(xf,f.list_eval(xf),'b','linewidth',4);
   j=1;
   for k=b1.greville
      if j<=i
        plot(k,k,'bo','MarkerFaceColor','b','MarkerSize',8)
      else
        plot(k,0,'ko','MarkerFaceColor','k','MarkerSize',8)
      end
      j = j+1;
   end   
   ylim([0,1])
end


fig = gcf;
fig.PaperUnits = 'inches';
fig.PaperPosition = [0 0 10 7];
print('bases_add2.svg','-dsvg','-r0')
