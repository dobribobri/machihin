#include "trapezium.h"


double Trapezium::Intensity(double ksi, double eta, double z) {
    std::complex<double> elem0(E0, 0.);
    std::complex<double> elem1 = exp(I*(2*M_PI/lambda)*z) / (I*(2*M_PI/lambda)*z);
    std::complex<double> elem2 = exp(I*(2*M_PI/lambda) * (ksi*ksi+eta*eta)/(2*z));
    Method m = methods::RandMC;
    std::complex<double> elem3 = NIntegrate(ksi, eta, z,
                                            /*X:*/ a/2, a,      /*Y:*/ 0., 2*a,
                                            [&](double& x, double& y){ return y <= -4*x + 4*a; },
                                            m);
    std::complex<double> elem4 = NIntegrate(ksi, eta, z,
                                            /*X:*/ -a/2, a/2,   /*Y:*/ 0., 2*a,
                                            m);
    std::complex<double> elem5 = NIntegrate(ksi, eta, z,
                                            /*X:*/ -a, -a/2,    /*Y:*/ 0., 2*a,
                                            [&](double& x, double& y){ return y <= 4*x + 4*a; },
                                            m);
    std::complex<double> energy = elem0*elem1*elem2 * (elem3 + elem4 + elem5);
    return pow(abs(energy),2);
}

void Trapezium::Intensity(std::string fpath, double step, double z,
                          double ksiL, double ksiR, double etaL, double etaR) {
    std::ofstream file(fpath);
    double total = ((ksiR-ksiL)/step);
    int k = 0;
    for (double ksi = ksiL; ksi <= ksiR; ksi += step) {
        for (double eta = etaL; eta <= etaR; eta += step) {
            //std::ostringstream s_ksi, s_eta;
            //s_ksi << ksi; s_eta << eta;
            double I = this->Intensity(ksi, eta, z);
            //file << s_ksi.str() << " " << s_eta.str() << " " << I << "\n";
            file << I << " ";
            std::cout << I << " ";
        }
        file << "\n";
        k++;
        std::cout << std::endl;
        std::cout << "------------------------------------------------------------" << std::endl;
        std::cout << k/total*100 << "%" << std::endl;
        std::cout << "------------------------------------------------------------" << std::endl;
    }
    file.close();
}

std::complex<double> Trapezium::func(double& ksi, double& eta, double& z, double x, double y) {
    return exp(-I*(2*M_PI/lambda) * (x*ksi+y*eta)/z);
}

std::complex<double> Trapezium::NIntegrate(double& ksi, double& eta, double& z,
                                           double from_x, double to_x, double from_y, double to_y,
                                           const std::function<bool(double&,double&)>& bAreaCondition, Method m) {
    std::complex<double> S(0.,0.);
    if (m == methods::RandMC) {
        srand(static_cast<unsigned int>(time(nullptr)));
        int N = int(pow(10, 6));
        int k = 0;
        for (int i = 0; i < N; i++) {
            double x = from_x + (to_x - from_x) * rand() / RAND_MAX;
            double y = from_y + (to_y - from_y) * rand() / RAND_MAX;
            if (bAreaCondition(x, y)) {
                S += func(ksi, eta, z, x, y);
                k++;
            }
        }
        S *= (to_x - from_x)*(to_y - from_y)/k;
        return S;
    }
    return S;
}

std::complex<double> Trapezium::NIntegrate(double& ksi, double& eta, double& z,
                                           double from_x, double to_x, double from_y, double to_y, Method m) {
    return NIntegrate(ksi, eta, z, from_x, to_x, from_y, to_y,
                      [](double x, double y){ (void)x; (void)y; return true; }, m);
}

std::complex<double> Trapezium::Energy_(double ksi, double eta, double z) {
    std::complex<double> elem0 = I * exp(I*M_PI*(2.*z*z+eta*eta+ksi*ksi)/(z*lambda)) *
            E0 * z *lambda*lambda*lambda;
    std::complex<double> elem1 = (exp(-2.*I*a*M_PI*ksi/(z*lambda)) -
                                  exp(-I*a*M_PI*(4.*eta + ksi)/(z*lambda)))/(4.*eta - ksi);
    std::complex<double> elem2 = (exp(-I*a*M_PI*(4.*eta - ksi)/(z*lambda)) *
                                  (-1. + exp(I*a*M_PI*(4.*eta + ksi)/(z*lambda))))/(4.*eta + ksi);
    std::complex<double> elem3 = 2 * M_PI*M_PI*M_PI * ksi;
    std::complex<double> energy = elem0 * (elem1 - elem2) / elem3;
    return energy;
}

double Trapezium::Intensity_(double ksi, double eta, double z) {
    std::complex<double> energy = this->Energy_(ksi, eta, z);
    return pow(abs(energy), 2);
}

void Trapezium::Intensity_(std::string fpath, double step, double z,
                          double ksiL, double ksiR, double etaL, double etaR) {
    std::ofstream file(fpath);
    double total = ((ksiR-ksiL)/step);
    int k = 0;
    for (double ksi = ksiL; ksi <= ksiR; ksi += step) {
        for (double eta = etaL; eta <= etaR; eta += step) {
            //std::ostringstream s_ksi, s_eta;
            //s_ksi << ksi; s_eta << eta;
            double I = this->Intensity_(ksi, eta, z);
            //file << s_ksi.str() << " " << s_eta.str() << " " << I << "\n";
            file << I << " ";
            std::cout << I << " ";
        }
        file << "\n";
        k++;
        std::cout << std::endl;
        std::cout << "------------------------------------------------------------" << std::endl;
        std::cout << k/total*100 << "%" << std::endl;
        std::cout << "------------------------------------------------------------" << std::endl;
    }
    file.close();
}

