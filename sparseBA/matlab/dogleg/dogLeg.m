function p = dogLeg(f,jac,x,p0,opts)
% minimizes |x-f(p)|^2
%opts(1) stopping |g|_inf < opts(0)
%opts(2) iterma
    k = 0;
    delta = 1e-3;
    p = p0;
    J = feval(jac,p);
    size(J)
    A = J'*J;
    size(A)
    eps_p = (x - feval(f,p))';
    size(eps_p)
    g = J'* eps_p;
    stop = max(g) > opts(1); %L_inf
    
   % while (~stop) & k < opts(2)
        k = k + 1;
        delta_sd = norm(g)^2 / norm(J*g)^2 * g;
        gn_computed = 0;
        rho = 0;
        while ((rho <= 0) & (~stop) )
            if norm(delta_sd) > delta
                delta_dl = delta / norm (delta_sd) * delta_sd;
            else
                if (~gn_computed)
                    delta_gn = A\g;
                    gn_computed = 1;
                end
                if norm(delta_gn)<= delta
                    delta_dl = delta_gn;
                else
                    betta = 1;%FIXME
                    delta_dl = delta_sd + betta * (delta_gn-delta_sd)
                    %betta st |delta_dl| = delta
                end
        end
   % end
end