% scaling
N = 100;
n_min = 0; n_max = 1000;
min_a = min(min(A)); max_a = max(max(A));

for ksi = 1:N
    for eta = 1:N
        A(ksi, eta) = n_min + (n_max - n_min) / (max_a - min_a) *...
            (A(ksi, eta) - min_a);
        
    end
end

% contour plot 1
x = linspace(-1,1,N);
y = linspace(-1,1,N);
[X,Y] = meshgrid(x,y);
contour(X,Y,A);

