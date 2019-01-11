#ifndef INTEGRAL_H
#define INTEGRAL_H

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <complex>
#include <random>
#include <time.h>
#include <functional>
#include <fstream>
#include <sstream>


enum methods { RandMC, AvgRect, Simpson };
typedef int Method;

class Trapezium
{
    private:
        std::complex<double> I = std::complex<double>(0.,1.);
        double lambda = 5.*pow(10,-7);
        double a = 5.*pow(10,-3);
        double E0 = pow(10,18);

        std::complex<double> func(double& ksi, double& eta, double& z, double x, double y);

        std::complex<double> NIntegrate(double& ksi, double& eta, double& z,
                        double from_x, double to_x, double from_y, double to_y,
                        const std::function<bool(double&,double&)>& bAreaCondition, Method m);

        std::complex<double> NIntegrate(double& ksi, double& eta, double& z,
                        double from_x, double to_x, double from_y, double to_y, Method m);


        std::complex<double> Energy_(double ksi, double eta, double z);

    public:
        Trapezium() { }
        Trapezium(double _lambda, double _a, double _E0) : lambda(_lambda), a(_a), E0(_E0) { }

        double Intensity(double ksi, double eta, double z);
        void Intensity(std::string fpath, double step = 0.05, double z = 10.,
                       double ksiL = -1., double ksiR = 1., double etaL= -1., double etaR = 1.);
        double Intensity_(double ksi, double eta, double z);
        void Intensity_(std::string fpath, double step = 0.05, double z = 10.,
                       double ksiL = -1., double ksiR = 1., double etaL= -1., double etaR = 1.);

        void setE0(double _E0) { this->E0 = _E0; }
};

#endif // INTEGRAL_H
