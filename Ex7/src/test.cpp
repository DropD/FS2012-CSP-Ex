#include "md_particle.hpp"
#include <iostream>

int main (int argc, char const* argv[])
{
    //csp::md::point<double> ps1 = {1, 0, 0};
    csp::md::point<double> ps2(1, 2, 3);
    //csp::md::point_arr<double> pa1 = {1, 0, 0};
    csp::md::point_arr<double> pa2(2, 3, 4);

    std::cout << "ps2 = " << ps2 << std::endl;
    std::cout << "1 + ps2 = " << 1 + ps2 << std::endl;
    std::cout << "ps2 + ps2 = " << ps2 + ps2 << std::endl;
    std::cout << "pa2 = " << pa2 << std::endl;
    std::cout << pa2 << std::endl;
    std::cout << "pa2 + pa2 = " << pa2 + pa2 << std::endl;
    return 0;
}
