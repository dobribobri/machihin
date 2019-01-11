#include <QCoreApplication>

#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <limits>
#include <math.h>
#include "trapezium.h"
//#include "test.h"

std::vector<std::string> split_line(std::string line, std::string delim = " ") {
    std::remove(line.begin(), line.end(), '\n');
    std::vector<std::string> ret;
    size_t pos;
    while ((pos = line.find(delim)) != std::string::npos) {
        std::string elem = line.substr(0, pos);
        line.erase(0, pos + 1);
        ret.push_back(elem);
    }
    std::remove(line.begin(), line.end(), ' ');
    if (line != "") { ret.push_back(line); }
    return ret;
}

double stod_sc_n(std::string a) {
    double d = 0.;
    std::replace(a.begin(), a.end(), '.', ',');
    size_t pos = a.find("e");
    try {
        if (pos != std::string::npos) {
            d = std::stod(a.substr(0, pos));
            a.erase(0, pos + 1);
            d *= pow(10, std::stod(a));
        } else {
            d = std::stod(a);
        }
    } catch (...) {
        std::cout << "Error!" << std::endl;
    }
    return d;
}

std::vector<double> get_limits(std::string path) {
    std::ifstream in_result(path);
    std::string line;
    double min = std::numeric_limits<double>::max();
    double max = std::numeric_limits<double>::min();
    while (getline(in_result, line)) {
        for (std::string a : split_line(line)) {
            double d = stod_sc_n(a);
            //std::cout << d << " ";
            if (d < min) { min = d; }
            if (d > max) { max = d; }
        }
        //std::cout << "\n------------------------------------------------------------\n";
    }
    std::vector<double> ret;
    ret.push_back(min);
    ret.push_back(max);
    in_result.close();
    std::cout << path << " limits:  min = " << min << ",\t max = " << max << std::endl;
    return ret;
}

void scale(std::string path, std::vector<double> limits, double new_min, double new_max) {
    double min = limits[0];
    double max = limits[1];
    std::ifstream in_result(path);
    std::string line;
    std::ofstream out("scaled_" + path);
    while (getline(in_result, line)) {
        for (std::string a : split_line(line)) {
            double x = stod_sc_n(a);
            x = new_min + (new_max - new_min) / (max - min) * (x - min);
            std::ostringstream os;
            os << x;
            out << os.str() << " ";
        }
        out << "\n";
    }
    in_result.close();
    out.close();
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
/*
    std::cout << " ==== TEST ==== " << std::endl;
    std::cout << "Sqrt(x+y) 0<x<10, 0<y<10" << std::endl;
    std::complex<double> ans;
    ans = NIntegrate(0., 10., 0., 10., methods::RandMC);
    std::cout << "RandMC : " << ans << std::endl;
    ans = NIntegrate(0, 10, 0, 10, methods::AvgRect);
    std::cout << "AvgRect : " << ans << std::endl;
    ans = NIntegrate(0, 10, 0, 10, methods::Simpson);
    std::cout << "Simpson : " << ans << std::endl;
    std::cout << " ===== OK ===== " << std::endl;
*/
    //setlocale(LC_ALL,"en_us");

    std::string path = "result.txt";
    std::string path_ = "result_a.txt";

    Trapezium* T = new Trapezium(5.*pow(10.,-7.), 5.*pow(10.,-3.), 1);
    T->Intensity(path);
    T->Intensity_(path_);

    std::cout << "scaling " << path << " ... " << std::endl;
    scale(path, get_limits(path), 0, 1000);
    std::cout << "scaling " << path_ << " ... " << std::endl;
    scale(path_, get_limits(path_), 0, 1000);

    FILE* plots[4];
    std::string z;
    plots[0] = popen("gnuplot -persistent", "w");
    fprintf(plots[0], "reset\n");
    fprintf(plots[0], "set title 'Plot #0 : NumIntegrate (scaled 0-1000, Z-range 0-1000)'\n");
    fprintf(plots[0], "unset key\n");
    fprintf(plots[0], "unset colorbox\n");
    fprintf(plots[0], "unset tics\n");
    fprintf(plots[0], "set palette defined (0 '#352a87', 1 '#0363e1', 2 '#1485d4', 3 '#06a7c6', 4 '#38b99e', 5 '#92bf73', 6 '#d9ba56', 7 '#fcce2e', 8 '#f9fb0e')\n");
    fprintf(plots[0], "set pm3d map\n");
    fprintf(plots[0], "set pm3d interpolate 0,0\n");
    fprintf(plots[0], "set zrange [0:1000]\n");
    z = "splot '" + path + "' matrix\n";
    fprintf(plots[0], "%s", z.c_str());

    plots[1] = popen("gnuplot -persistent", "w");
    fprintf(plots[1], "reset\n");
    fprintf(plots[1], "set title 'Plot #1 : NumIntegrate (scaled 0-1000, Z-range 0-500)'\n");
    fprintf(plots[1], "unset key\n");
    fprintf(plots[1], "unset colorbox\n");
    fprintf(plots[1], "unset tics\n");
    fprintf(plots[1], "set palette defined (0 '#352a87', 1 '#0363e1', 2 '#1485d4', 3 '#06a7c6', 4 '#38b99e', 5 '#92bf73', 6 '#d9ba56', 7 '#fcce2e', 8 '#f9fb0e')\n");
    fprintf(plots[1], "set pm3d map\n");
    fprintf(plots[1], "set pm3d interpolate 0,0\n");
    fprintf(plots[1], "set zrange [0:500]\n");
    z = "splot 'scaled_" + path + "' matrix\n";
    fprintf(plots[1], "%s", z.c_str());

    plots[2] = popen("gnuplot -persistent", "w");
    fprintf(plots[2], "reset\n");
    fprintf(plots[2], "set title 'Plot #2 : Analytical (scaled 0-1000, Z-range 0-1000)'\n");
    fprintf(plots[2], "unset key\n");
    fprintf(plots[2], "unset colorbox\n");
    fprintf(plots[2], "unset tics\n");
    fprintf(plots[2], "set palette defined (0 '#352a87', 1 '#0363e1', 2 '#1485d4', 3 '#06a7c6', 4 '#38b99e', 5 '#92bf73', 6 '#d9ba56', 7 '#fcce2e', 8 '#f9fb0e')\n");
    fprintf(plots[2], "set pm3d map\n");
    fprintf(plots[2], "set pm3d interpolate 0,0\n");
    fprintf(plots[2], "set zrange [0:1000]\n");
    z = "splot '" + path_ + "' matrix\n";
    fprintf(plots[2], "%s", z.c_str());

    plots[3] = popen("gnuplot -persistent", "w");
    fprintf(plots[3], "reset\n");
    fprintf(plots[3], "set title 'Plot #3 : Analytical (scaled 0-1000, Z-range 0-500)'\n");
    fprintf(plots[3], "unset key\n");
    fprintf(plots[3], "unset colorbox\n");
    fprintf(plots[3], "unset tics\n");
    fprintf(plots[3], "set palette defined (0 '#352a87', 1 '#0363e1', 2 '#1485d4', 3 '#06a7c6', 4 '#38b99e', 5 '#92bf73', 6 '#d9ba56', 7 '#fcce2e', 8 '#f9fb0e')\n");
    fprintf(plots[3], "set pm3d map\n");
    fprintf(plots[3], "set pm3d interpolate 0,0\n");
    fprintf(plots[3], "set zrange [0:500]\n");
    z = "splot 'scaled_" + path_ + "' matrix\n";
    fprintf(plots[3], "%s", z.c_str());

    pclose(plots[0]);
    pclose(plots[1]);
    pclose(plots[2]);
    pclose(plots[3]);

    return a.exec();
}
