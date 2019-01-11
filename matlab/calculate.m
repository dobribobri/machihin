% calc integral
T = Trapezium();
z = 10.;
N = 100;
A = zeros(N, N);
for ksi = 1:N
    for eta = 1:N
        fprintf("ksi = %d, eta = %d\n", ksi, eta);
        fprintf("%d %%\n", ksi/N*100);
        
        A(ksi, eta) = T.Intensity_NInt(ksi/N*2-1, eta/N*2-1, z);
        % A(ksi, eta) = T.Intensity_NInt_quad(ksi/N*2-1, eta/N*2-1, z);
        
        fprintf("I = \n");
        disp(A(ksi, eta));
    end
end