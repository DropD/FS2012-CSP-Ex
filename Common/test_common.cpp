/*
 * Test suite for common clases
 * for CSP FS 2012 exercises
 * by Rico Häuselmann 24. 02. 2012
 */

#include <iostream>
#include <iomanip>
#include <boost/random.hpp>

#include "NDLattice.hpp"
#include "NDIterate.hpp"
#include "NDAlgorithm.hpp"

struct none_
{
    template<typename Element>
    void operator() (Element e) { }
    void operator() () {}
    none_() {}
} none;

struct randint
{
    boost::random::mt19937 eng;
    boost::random::uniform_int_distribution<> dist;
    randint() : dist(1, 10) {}
    int operator()()
    {
        return dist(eng);
    }
};

template <typename R>
void test_constructors(R rng);
void test_access();
void test_stencils();

int main (int argc, char const* argv[])
{
    /*
     * Create a random index generator
     */
    randint rig;

    /*
     * testing constructors
     */
    std::cout << "- check constructors" << std::endl;
    test_constructors(rig);

    /*
     * testing data access
     */
    std::cout << "- check data access" << std::endl;
    test_access();

    /*
     * testing iterations & stencils
     */
    std::cout << "- check iteration over Lattice dimensions with stencil" << std::endl;
    test_stencils();

    //boost::array<boost::multi_array<int, 3>::index, 3> shape = {{7, 6, 5}};

    //boost::multi_array<int, 3> mar(shape);
    //csp::Lattice<int, 3> lattice(shape);

    //boost::array<int, 3> idx = {{0, 0, 0}};
    //lattice(idx) = 5;
    //lattice[0][0][0] = 6;
    //lattice[6][5][4] = 6;
    //if(lattice[0][0][0] == 5) std::cout << "True" << std::endl;
    //else std::cout << "False" << std::endl;

    //std::cout << "size: " << std::distance(mar.begin(), mar.end()) << std::endl;
    //std::cout << "size: " << std::distance(mar[0].begin(), mar[0].end()) << std::endl;
    //std::cout << "size: " << std::distance(mar[0][0].begin(), mar[0][0].end()) << std::endl;

    //mar[0][0][0] = 4;
    //std::cout << mar[0][0][0] << std::endl;
    //std::cout << *(*(*mar.begin()).begin()).begin() << std::endl;
    //std::cout << *(*(*lattice.begin()).begin()).begin() << std::endl;

    //typedef boost::multi_array_types::index_range range;

    //boost::multi_array<int, 3>::array_view<2>::type myview = 
    //    mar[boost::indices[range(0,2)][1][range(0, 4, 2)] ];

    ////boost::multi_array<int, 3>::index_gen idgen =
    //boost::detail::multi_array::index_gen<3, 2> idgen =
    //    boost::indices[range(0,2)][1][range(0, 4, 2)];

    //boost::multi_array<int, 3>::array_view<2>::type myview2 = 
    //    lattice[boost::indices[range(0,2)][1][range(0, 4, 2)] ];

    // random tests

    return 0;
}

bool is_not_ten(int x)
{
    return x == 10 ? 1 : 0;
}

template <typename R>
void test_constructors(R rng)
{
    typedef std::complex<double> complex;
    csp::Lattice<int, 0> lattice_i1;
    csp::Lattice<double, 1> lattice_d1;
    csp::Lattice<complex, 2> lattice_c1;

    csp::Lattice<int, 3> lattice_i2();
    csp::Lattice<double, 4> lattice_d2();
    csp::Lattice<complex, 5> lattice_c2();

    boost::array<int, 6> shape;
    std::generate(shape.begin(), shape.end(), rng);

    csp::Lattice<int, 6> lattice_i3(shape);
    if(!std::equal(shape.begin(), shape.end(), lattice_i3.shape()))
        throw "shape of created lattice is not equal to given shape!";
    csp::Lattice<double, 6> lattice_d3(shape);
    if(!std::equal(shape.begin(), shape.end(), lattice_i3.shape()))
        throw "shape of created lattice is not equal to given shape!";
    csp::Lattice<complex, 6> lattice_c3(shape);
    if(!std::equal(shape.begin(), shape.end(), lattice_i3.shape()))
        throw "shape of created lattice is not equal to given shape!";

    //// maybe next time
    //csp::Lattice<int, 7> lattice_i4(10);
    //csp::Lattice<double, 7> lattice_d4(10);
    //csp::Lattice<complex, 7> lattice_c4(10);
    std::cout << "|-- constructors ok" << std::endl;
}

void test_access()
{
    boost::array<int, 5> idx; 
    boost::array<int, 5> shape = {{3, 3, 3, 3, 3}};
    std::fill(idx.begin(), idx.end(), 0);

    csp::Lattice<int, 5> lattice(shape);

    if(lattice[0][0][0][0][0] != lattice(idx))
        throw "access by [] differs from ()";

    lattice[1][1][1][1][1] = 4;
    if(lattice[1][1][1][1][1] != 4)
        throw "writing using [] doesn't work";

    lattice(idx) = -42;
    if(lattice(idx) != -42)
        throw "writing using () doesn't work";

    std::cout << "|-- data access ok." << std::endl;
}

void test_stencils()
{
    using csp::algorithm::print;
    using csp::algorithm::add_direct_neighbors;
    using csp::algorithm::set_value_;
    using csp::algorithm::sum_prod_nn_;

    add_direct_neighbors sdn;
    sum_prod_nn_<double> sum_prod_nn;

    boost::array<int, 2> shape = {{2, 2}};
    boost::array<int, 2> idx = {{0, 0}};

    csp::Lattice<int, 2> lattice(shape);
    lattice(idx) = 1;

    std::cout << "|-- 2x2 int lattice, add direct neighbors" << std::endl;
    csp::iterate::stencil_iterate<2>(lattice, print);
    for(int i = 0; i < 3; ++i)
    {
        csp::iterate::stencil_iterate<2>(lattice, sdn);
        csp::iterate::stencil_iterate<2>(lattice, print);
    }

    boost::array<int, 2> shape2 = {{5, 5}};
    boost::array<int, 2> idx2 = {{2, 2}};

    csp::Lattice<double, 2, csp::bounds::mirror> lattice2(shape2);
    set_value_<double> set_0_01(0.01);
    csp::iterate::stencil_iterate<2>(lattice2, set_0_01);
    lattice2(idx2) = 0.1;

    std::cout << "|-- 5x5 double lattice, add direct neighbors, mirror boundaries" << std::endl;
    csp::iterate::stencil_iterate<2>(lattice2, print);
    csp::iterate::stencil_iterate<2>(lattice2, sum_prod_nn);
    std::cout << "sum of nn_prods: " << sum_prod_nn.result() << std::endl;
    //std::cout << "sum of nn_prosds: " << sum_prod_nn(lattice2) << std::endl;
    for(int i = 0; i < 3; ++i)
    {
        csp::iterate::stencil_iterate<2>(lattice2, sdn);
        csp::iterate::stencil_iterate<2>(lattice2, print);
    }
}
