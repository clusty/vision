clear all;
x=-5:0.1:5;
ngauss = 10;
rand('seed',123);
params = rand(3*ngauss,1)-0.5;
params(1:3:end) = abs(params(1:3:end) * 3);%coefs
params(2:3:end) = params(2:3:end) * 5 ;%center
params(3:3:end) = 5*abs(params(3:3:end));  %sigma

data = gaussMix(x,params);
plot(x,data);

jac = @(p)minFuncJac(p,x);
func = @(p)minFunc(p,x);
p0 = zeros(size(params));
opts=zeros(1,2);
opts(1) = 1e-5;
opts(2) = 100;
opts(3) = 1e-5;
dogLeg(func,jac,data,p0,opts)