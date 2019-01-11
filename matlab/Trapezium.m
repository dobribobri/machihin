classdef Trapezium
    properties
        lambda = 5*power(10.,-7.);
        a = 5*power(10.,-3.);
        E0 = 1
        % E0 = power(10, 8);
        % E0 = power(10, 18);
    end
    
    methods (Access = public)
        function [r] = Energy_NInt(obj, ksi, eta, z)
            ifunc = @(k, e, d)...
                (@(x, y) exp(-1j*(2*pi/obj.lambda)*((x*k+y*e)/d)));
            e1 = exp(1j*(2*pi/obj.lambda)*z) / (1j*(2*pi/obj.lambda)*z);
            e2 = exp(1j*(2*pi/obj.lambda) * (ksi*ksi + eta*eta)/(2*z));
            
            %e3 = integral2(ifunc(ksi,eta,z),...
            %    obj.a/2, obj.a, 0, @(x) -4*x+4*obj.a,...
            %    'Method', 'iterated', 'AbsTol',1e-8,'RelTol',1e-4);
            %e4 = integral2(ifunc(ksi,eta,z),...
            %    -obj.a/2, obj.a/2, 0, 2*obj.a,...
            %    'Method', 'iterated', 'AbsTol',1e-8,'RelTol',1e-4);
            %e5 = integral2(ifunc(ksi,eta,z),...
            %    -obj.a, -obj.a/2, 0, @(x) 4*x+4*obj.a,...
            %    'Method', 'iterated', 'AbsTol',1e-8,'RelTol',1e-4);
            %
            %r = obj.E0*e1*e2*(e3+e4+e5);
            
            e3 = integral2(ifunc(ksi,eta,z),...
                0, 2*obj.a, @(y) 1/4*y-obj.a, @(y) -1/4*y+obj.a,...
                'Method', 'iterated', 'AbsTol',1e-6,'RelTol',1e-3);
            
            r = obj.E0*e1*e2*(e3);
        end
        
        function [r] = Intensity_NInt(obj, ksi, eta, z)
            energy = Energy_NInt(obj, ksi, eta, z);
            r = power(abs(energy), 2);
        end
        
        function [r] = Energy_NInt_quad(obj, ksi, eta, z)
            ifunc = @(k, e, d)...
                (@(x, y) exp(-1j*(2*pi/obj.lambda)*((x*k+y*e)/d)));
            e1 = exp(1j*(2*pi/obj.lambda)*z) / (1j*(2*pi/obj.lambda)*z);
            e2 = exp(1j*(2*pi/obj.lambda) * (ksi*ksi + eta*eta)/(2*z));
            
            %e3 = quad2d(ifunc(ksi,eta,z),...
            %    obj.a/2, obj.a, 0, @(x) -4*x+4*obj.a); 
            %e4 = quad2d(ifunc(ksi,eta,z),...
            %    -obj.a/2, obj.a/2, 0, 2*obj.a);
            %e5 = quad2d(ifunc(ksi,eta,z),...
            %    -obj.a, -obj.a/2, 0, @(x) 4*x+4*obj.a);
            %r = obj.E0*e1*e2*(e3+e4+e5);
            
            e3 = quad2d(ifunc(ksi,eta,z),...
                0, 2*obj.a, @(y) 1/4*y-obj.a, @(y) -1/4*y+obj.a);
            
            r = obj.E0*e1*e2*(e3);
        end
        
        function [r] = Intensity_NInt_quad(obj, ksi, eta, z)
            energy = Energy_NInt_quad(obj, ksi, eta, z);
            r = power(abs(energy), 2);
        end
        
    end
end

