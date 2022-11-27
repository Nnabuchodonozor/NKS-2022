function octave_plot2(data, first, last, rows, cols)

% octave_plot2 is a simple plot utility to plot multiple plots on one page with Octave
%
% DESCRIPTION:
%
% The function octave_plot2(data, first, last, rows, cols)
% plots rows*cols plots on one page.
%
% - data : the data that should be plotted
%
% - first : index to start with (1<=first <= 256)
% - last  : index to end with (1<= first,last <= 256)
% - rows  : number of rows of plots
% - cols  : number of columns of plots
%
% RETURNVALUE:
%
% none
%
% EXAMPLE:
%
% octave_plot2(keys,1,256,2,2);

% Author: Thomas Popp, 16.04.2010
% Last revision: 16.04.2010


clf;

for i = first:rows*cols:last
	for j = 1:rows*cols
        subplot(rows, cols, j);
	 	plot(data(i+j-1, :));
        title(sprintf('Plot: %d' , i+j-1));
	end
    
    pause;
    clf;
end
