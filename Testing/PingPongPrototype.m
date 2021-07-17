%
% stereo ping pong delay prototype
%

clear;
close all;

Fs = 48000;
L = 1;
N = L * Fs;

% user parameters
% input signal
N = 20;
x = zeros(N, 2);
x(1, 1) = 1;

% distortion control
dLevelL = 4;
dLevelR = 4;

% feedback values
fbL = 1;
fbR = 0;

% wet and dry
wetL = 0.5;
dryL = 1-wetL;

wetR = 0.5;
dryR = 1-wetR;

% delay storage
D = 15;
DDL = zeros(D, 2);
writeIndex = 1;

delayIndex1 = 5;
delayIndex2 = 7;
delayIndex3 = 10;

% output signal
y = zeros(N, 2);

for n = 1:N
    % get the original input signal
    inputL = x(n, 1);
    inputR = x(n, 2);
    
    % summed delay
    summedDelayL = (DDL(delayIndex1, 1) + DDL(delayIndex2, 1) + DDL(delayIndex3, 1)) / 3;
    summedDelayR = (DDL(delayIndex1, 2) + DDL(delayIndex2, 2) + DDL(delayIndex3, 2)) / 3;
    
    % apply distortion
    distL = hard_limit(summedDelayL * dLevelL);
    distR = hard_limit(summedDelayR * dLevelR);
    
    % write the wet + dry signal to the output
    y(n, 1) = (inputL*dryL) + (distL * wetL);
    y(n, 2) = (inputR*dryR) + (distR * wetR);
  
    % update the DDL
    dL = summedDelayL;
    dR = summedDelayR;
    
    % add the fed back DDL and the original input
    % be sure to scale them so the value will be between -1 and 1
    DDL(writeIndex, 1) = (inputR + (dR*fbR)) / 2;
    DDL(writeIndex, 2) = (inputL + (dL*fbL)) / 2;
    
    % increment the indices
    writeIndex = writeIndex + 1;
    if (writeIndex > D)
        writeIndex = 1;
    end
    
    delayIndex1 = delayIndex1 + 1;
    if (delayIndex1 > D)
        delayIndex1 = 1;
    end
    
    delayIndex2 = delayIndex2 + 1;
    if (delayIndex2 > D)
        delayIndex2 = 1;
    end
    
    delayIndex3 = delayIndex3 + 1;
    if (delayIndex3 > D)
        delayIndex3 = 1;
    end
    
end

function output = hard_limit(input)
    output = min(max(input, -1), 1);
end
