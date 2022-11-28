function [key_trace] = demo_dpa(workspace,first, last, method)

%DEMO_DPA is a demo to illustrate DPA attacks
%
% DESCRIPTION:
%
% demo_dpa(workspace, first, last, method)
% performs a DPA with the specified method on some keys for some data
%
% In the demo, the output of the AES SubBytes function is attacked. This
% demo works for the two sample Matlab workspace "WS1.mat" and "WS2.mat".
% Note that the power model is encoded in the demo. You need to look for
% "bitget" and "byte_Hamming_weight". Further note that the entire
% workspace is loaded to memory. This is no problem for WS1, but it might
% be nasty for machines with low memory, when working with WS2.
%
% - workspace : the data that should be analyzed (traces, inputs)
%
% - first : keybyte to start with (1<=first <= 256)
% - last  : keybyte to end with (1<= first,last <= 256)
% - method: defines the statistical analysis (either 'kocher' or
% 'correlation')
%
% RETURNVALUE:
%
% key_trace: a matrix that consist of the DPA traces for the keybytes.
% Although always a matrix with 256 rows is returned only the rows from
% 'first' to 'last' contain meaningful results.
%
% EXAMPLE:
%
% keys = demo_dpa('WS1.zip',1,256,'correlation');


% Author: Elisabeth Oswald, 2.5.2004
% Last revision: 08.06.2006


% load data

disp('Reading in data ...');

load('-mat',workspace);
b=1;
inputs=inputs(:,b);
% predict the SubBytes output: SubBytes(XOR(key,data))

disp('Predicting intermediate values ...');
[m,n] = size(traces);

key = [0:255];
after_sbox = zeros(m,256);

for i=1:m
   % after_sbox(i,:) = SubBytes(bitxor(inputs(i),key)+1);
    after_sbox(i,:) = bitxor(inputs(i),key)+1;
end


key_trace = zeros(256,n);

if (strcmp(method,'kocher'))

    % predict the power consumption

    disp('Predicting the instantaneous power consumption ...');
  bit = [1,2,3,4]
   power_consumption = bitget(after_sbox,bit);

    % correlate the predicted power consumption with the real power
    % consumption
    disp('Generating the difference traces ...');

    for i=first:last

        key_trace(i,:) = sum(traces(find(power_consumption(:,i)==1),:)) - sum(traces(find(power_consumption(:,i)==0),:));

    end

end

if (strcmp(method,'correlation'))

    % predict the power consumption

    disp('Predicting the instantaneous power consumption ...');
    power_consumption = byte_Hamming_weight(after_sbox+1);

    % correlate the predicted power consumption with the real power
    % consumption
    disp('Generating the correlation traces ...');

    chunksize=50;
    chunks=n/50;
    for i=1:256
      disp(i)
        for j=1:chunks
          disp(j)
            cmatrix= corrcoef([traces(:,1+(j-1)*chunksize:j*chunksize)  power_consumption(:,i)]);
            key_trace(i,1+(j-1)*chunksize:j*chunksize) =cmatrix(chunksize+1,1:chunksize);

        end
    end

end


