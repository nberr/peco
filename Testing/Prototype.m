%
% stereo ping pong delay prototype
%

clear;
close all;

Fs = 48000;
L = 5;
N = L * Fs;

x = zeros(N, 2);
x(1, :) = 1;

fbL = 0.9;
fbR = 0.9;

wetL = 0.5;
dryL = 1-wetL;

wetR = 0.5;
dryR = 1-wetR;

D = 2;
DDL = zeros(D, 2);
rwInx = 1;

y = zeros(N, 2);

for n = 1:N
    y(n, :) = [(x(n, 1)*dryL) + (DDL(rwInx, 1)*wetL), (x(n, 2)*dryR) + (DDL(rwInx, 2)*dryR)];

    dL = DDL(rwInx, 1);
    dR = DDL(rwInx, 2);
    
    DDL(rwInx, 1) = x(n, 2) + (dR*fbR);
    DDL(rwInx, 2) = x(n, 1) + (dL*fbL);
    
    rwInx = rwInx + 1;
    if (rwInx > D)
        rwInx = 1;
    end
end
