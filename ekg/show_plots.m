function show_plots(variable, firstplot, lastplot, step, yscale)

%SHOW_PLOTS plots rows of a matrix in separate windows
%
% DESCRIPTION:
%
% show_plots(variable, firstplot, lastplot, step, yscale) 
% plots each row a matrix in a separate window 
% 
% - variable : the matrix that shall be plotted
%		       
% - firstplot (optional): the number of the first row to be plotted;
%             default value is 1 
% - lastplot  (optional): the number of the last row to be plotted; default
%             value is the total number of rows
% - step      (optional): the number of plots that are shown simultaneously;
%             default value is 6 (use 6 for a big screen, 4 for a small screen)
% - yscale    (optional): sets the range of values that are shown on the y
%             axis; values from -yscale to +yscale are shown; the default 
%             value for yscale is 1
%		
% RETURNVALUE:
%
% none
%
% EXAMPLE:
%                                            
% show_plots(unidrnd(10,8,10),1,8,4,10)

% AUTHOR: Stefan Mangard
%
% DATE: 18.04.2005

if (~(exist('yscale','var') == 1))
    yscale = 1; 
end

if (~(exist('firstplot','var') == 1))
    firstplot = 1; 
end

if (~(exist('lastplot','var') == 1))
    lastplot = size(variable,1); 
end

if (~(exist('step','var') == 1))
    step = 6;
end

for key=firstplot:step:lastplot

    for i=key:(key+step - 1)

       figure(i - key +1);

       if (i<=lastplot)
            keydata = variable(i,:);
         
            n = length(keydata);

            plot(keydata);
            axis([0 n -yscale yscale])
            xlabel('time')
            ylabel('Voltage / Corrlelation')

            titletext = sprintf('Plot Number: %d, Max: %2.4f, Min: %2.4f',i, max(keydata), min(keydata));
            titletext = strrep(titletext,'\','\\');
            titletext = strrep(titletext,'_','\_');
            title(titletext);
       else
%          close 
       end
   end
   
   disp('Press any key for next file')
   pause;
end
