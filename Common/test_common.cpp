/*
 * Test suite for common clases
 * for CSP FS 2012 exercises
 * by Rico Häuselmann 24. 02. 2012
 */

#include <iostream>
#include <boost/random.hpp>

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
            //idx[i] = (idx[i] + 1) < L.shape()[i] ? (idx[i] + 1) : 0;
            //using new stencil interface
            e += L.get_n_left(idx, i);
            //idx[i] = (idx[i] - 2) > 0 ? (idx[i] -2) : (L.shape()[i] - 1);
            //using new stencil interface
            e += L.get_n_right(idx, i);
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

int main (int argc, char const* argv[])
{
    /*
     * Create a random index generator
     */
    randint rig;

    /*
     * testing constructors
     */
    test_constructors(rig);

    boost::array<boost::multi_array<int, 3>::index, 3> shape = {{7, 6, 5}};

    boost::multi_array<int, 3> mar(shape);
    csp::Lattice<int, 3> lattice(shape);

    boost::array<int, 3> idx = {{0, 0, 0}};
    lattice(idx) = 5;
    lattice[0][0][0] = 6;
    lattice[6][5][4] = 6;
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
}

template <typename R>
void test_access(R rng)
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
}
