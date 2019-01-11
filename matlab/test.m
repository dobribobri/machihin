k = 0;
for i = 1:N
    for j = 1:N
        if A(i, j) > 0.01
            k = k + 1;
        end
    end
end
disp(k)
