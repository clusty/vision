function j = minFuncJac(params,x)
    j = zeros (length(x),length(params));
    delta = zeros(size(params));
    h = 1e-5;
    %for i=1:length(x)
        %compute one row of jac
        for k=1:length(params)
            delta(k) = h;
            j(:,k) = (minFunc(params+delta,x)-minfunc(params-delta,x))/(2*h);
            delta(k) = 0;
        end
        %
    %end
end