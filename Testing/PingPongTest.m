function y = PingPongTest(x, fbL, fbR, wetL, wetR, D, dLevelL, dLevelR, distortion)

    N = length(x);

    dryL = 1 - wetL;
    dryR = 1 - wetR;

    DDL = zeros(D, 2);
    rwInx = 1;

    y = zeros(N, 2);
    
    for n = 1:N
        % get the original input signal
        inputL = x(n, 1);
        inputR = x(n, 2);
        
        % apply distortion
        distL = hard_limit(DDL(rwInx, 1) * dLevelL);
        distR = hard_limit(DDL(rwInx, 2) * dLevelR);
    
        % write the wet + dry signal to the output
        y(n, 1) = (inputL*dryL) + (distL * wetL);
        y(n, 2) = (inputR*dryR) + (distR * wetR);
  
        % update the DDL
        % get the output of the DDL
        dL = DDL(rwInx, 1);
        dR = DDL(rwInx, 2);
    
        % add the fed back DDL and the original input
        % be sure to scale them so the value will be between -1 and 1
        DDL(rwInx, 1) = (inputR + (dR*fbR)) / 2;
        DDL(rwInx, 2) = (inputL + (dL*fbL)) / 2;
    
        % increment the read/write index
        rwInx = rwInx + 1;
        if (rwInx > D)
            rwInx = 1;
        end
    end
end

function output = hard_limit(input)
    output = min(max(input, -1), 1);
end
