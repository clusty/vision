function y=gaussMix(x,p)
    y = 0;
    a=p(1:3:end);
    mu = p(2:3:end);
    si = p(3:3:end);
    for i=1:length(a)
        y = y+ a(i) * exp(-(x-mu(i)).^2/si(i));
    end
end