% contour plot 2

B = zeros(N, N);
for i = 1:N
    for j = 1:N
        if A(i, j) < 0.1
            B(i, j) = A(i, j);
        end
    end
end

contour(X,Y,B);