/*
 * Test suite for common clases
 * for CSP FS 2012 exercises
 * by Rico Häuselmann 24. 02. 2012
 */

#include <iostream>

#include "NDLattice.hpp"

struct print_
{
    template<typename Element>
    void operator() (Element e) { std::cout << e << " "; }
    template<typename Lattice, typename IndexList>
    void operator() (Lattice& L, IndexList idx) 
    { 
        typename Lattice::element_type e = L(idx);
        for(int i = 0; i < idx.size(); ++i)
        {
            int temp = idx[i];
            idx[i] = (idx[i] + 1) < L.shape()[i] ? (idx[i] + 1) : 0;
            e += L(idx);
            idx[i] = (idx[i] - 2) > 0 ? (idx[i] -2) : (L.shape()[i] - 1);
            e += L(idx);
            idx[i] = temp;
        }
        std::cout << e << " "; 
    }
    void operator() () { std::cout << std::endl; }
    print_() {}
} print;

struct none_
{
    template<typename Element>
    void operator() (Element e) { }
    void operator() () {}
    none_() {}
} none;

int main (int argc, char const* argv[])
{
    //print_ print;
    //none_ none;
    int shapear[3] = {5, 5, 5};
    boost::array<boost::multi_array<int, 3>::index, 3> shape = {{7, 6, 5}};
    boost::array<int, 3> shape2 = {{5, 5, 5}};

    csp::Lattice<int, 3> lattice(shape);
    //csp::Lattice<int, 3> lattice2(5);
    csp::Lattice<int, 3> lattice3(shape2);

    boost::multi_array<int, 3> mar(shape);

    boost::array<int, 3> idx = {{0, 0, 0}};
    lattice(idx) = 5;
    lattice[0][0][0] = 6;
    lattice[6][5][4] = 6;
    //lattice.array()[0][0][0] = 4;
    if(lattice[0][0][0] == 5) std::cout << "True" << std::endl;
    else std::cout << "False" << std::endl;

    std::cout << "size: " << std::distance(mar.begin(), mar.end()) << std::endl;
    std::cout << "size: " << std::distance(mar[0].begin(), mar[0].end()) << std::endl;
    std::cout << "size: " << std::distance(mar[0][0].begin(), mar[0][0].end()) << std::endl;

    mar[0][0][0] = 4;
    std::cout << mar[0][0][0] << std::endl;
    std::cout << *(*(*mar.begin()).begin()).begin() << std::endl;
    std::cout << *(*(*lattice.begin()).begin()).begin() << std::endl;

    //iterations
    std::cout << "check iteration over Lattice dimensions" << std::endl;
    //csp::iterate<1>(lattice.begin(), lattice.end(), none);
    csp::iterate<3>(lattice.begin(), lattice.end(), print);
    //csp::iterate_idx<1>(mar[0][0], boost::array<int, 2>(0, 0, 0), print);
    std::cout << "check iteration over Lattice dimensions with stencil" << std::endl;
    csp::iterate_idx<3>(lattice, print);
    //boost::array<int, 2> idx2 = {{0, 0}};
    //std::cout << lattice.shape()[1] << std::endl;

    typedef boost::multi_array_types::index_range range;

    boost::multi_array<int, 3>::array_view<2>::type myview = 
        mar[boost::indices[range(0,2)][1][range(0, 4, 2)] ];

    //boost::multi_array<int, 3>::index_gen idgen =
    boost::detail::multi_array::index_gen<3, 2> idgen =
        boost::indices[range(0,2)][1][range(0, 4, 2)];

    boost::multi_array<int, 3>::array_view<2>::type myview2 = 
        lattice[boost::indices[range(0,2)][1][range(0, 4, 2)] ];

    // random tests

    return 0;
}
