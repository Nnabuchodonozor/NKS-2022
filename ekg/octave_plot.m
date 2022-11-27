function octave_plot(data, first, last, vert, hor)

%OCTAVE_PLOT is a simple plot utility to plot multiple plots on one page with Octave
%
% DESCRIPTION:
%
% octave_plot(data, first, last, vert, hor) 
% plots vert*hor plots on one page 
%    
% 
% - data : the data that should be plotted
%		       
% - first : index to start with (1<=first <= 256)
% - last  : index to end with (1<= first,last <= 256)
% - vert  : some integer
% - hor   : some integer such that vert*hor nicely fits on your screen
%		
% RETURNVALUE:
%
% none
%
% EXAMPLE:
%                                            
% octave_plot(keys,1,256,2,2);


% Author: Elisabeth Oswald, 06.06.2006
% Last revision: 08.06.2006

oneplot;
clearplot;


for i=first:vert*hor:last
  multiplot(vert,hor);
	str=sprintf('Plots: %d - %d',i,i+vert*hor-1);
	title(str);
  
	for j=1:vert*hor
	 	mplot(data(i-1+j,:));
	 end
  pause;
  oneplot;
  clearplot;
end