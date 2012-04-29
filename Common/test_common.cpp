/*
 * Test suite for common clases
 * for CSP FS 2012 exercises
 * by Rico Häuselmann 24. 02. 2012
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <boost/random.hpp>
#include <boost/timer/timer.hpp>
#include <boost/mpl/bool.hpp>

#include "NDLattice.hpp"
#include "NDLattice_v2.hpp"
#include "NDIterate.hpp"
#include "NDAlgorithm.hpp"
#include "random_msk.hpp"

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
void test_speed();
void test_v2();

int main (int argc, char const* argv[])
{
    /*
     * Create a random index generator
     */
    randint rig;

    /*
     * testing constructors
     */
    //std::cout << "- check constructors" << std::endl;
    //test_constructors(rig);

    /*
     * testing data access
     */
    //std::cout << "- check data access" << std::endl;
    //test_access();

    /*
     * testing iterations & stencils
     */
    //std::cout << "- check iteration over Lattice dimensions with stencil" << std::endl;
    //test_stencils();

    /*
     * testing speed
     */
    std::cout << "- check speed of various operations" << std::endl;
    test_speed();

    /*
     * testing v2
     */
    //std::cout << "- check basics of v2" << std::endl;
    //test_v2();

    // random tests

    return 0;
}

#define T_MSG(msg, timer)           \
    std::cout << msg;               \
    std::cout << timer.format();    

void test_v2()
{
    boost::timer::cpu_timer ctimer;

    //boost::array<int, 3> shape_v2 = {10, 10, 10};
    csp_v2::Lattice<int, 3, boost::mpl::bool_<true> > lv2(10, 1);
    //csp_v2::Lattice<int, 3, boost::mpl::bool_<false> > lv2_s(shape_v2);
    
    std::cout << "|-+ construct 400*400*400 int lattice and set all elements 1" << std::endl;
    ctimer = boost::timer::cpu_timer();
    ctimer.stop();
    csp_v2::Lattice<int, 3, boost::mpl::bool_<true> > lv2_hc(400, 1);
    T_MSG(       "  |-- constructed lattice & set all elements: ", ctimer);

    std::cout << lv2[0][0][0] << std::endl;
    //std::cout << "DEBUG MAIN.1.0" << std::endl;
    std::cout << lv2_hc[0][0][0] << std::endl;
    //std::cout << "DEBUG MAIN.1.1" << std::endl;
    csp_v2::Lattice<int, 5, boost::mpl::false_> lv2_5(4);
    std::cout << lv2_5[0][1][2][3][0] << std::endl;
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

//void message(std::string msg, boost::timer::auto_cpu_timer timer)
//{
//    timer.stop();
//    std::cout << msg;
//    timer.report();
//    timer.resume();
//}

void test_speed()
{
    int L;
    boost::timer::cpu_timer ctimer;
    boost::timer::cpu_timer itimer;

    csp::algorithm::set_value_<int> init_one(1);
    csp::algorithm::add_direct_neighbors nn_sum;
    /*
     * 6D, L=10, int
     */
    //L = 20;
    //std::cout << "|-+ int, 6D, L=" << L << " N=" << L*L*L*L*L*L << std::endl;

    //boost::array<int, 6> speed_6d_shape = {L, L, L, L, L, L};

    //ctimer = boost::timer::cpu_timer();
    //csp::Lattice<int, 6> speed_6d_lattice(speed_6d_shape);
    //ctimer.stop();
    //T_MSG(       "  |-- constructed lattice: ", ctimer);

    //itimer = boost::timer::cpu_timer();
    //speed_6d_lattice.iterate(init_one);
    //itimer.stop();
    //T_MSG(       "  |-- set all elements one: ", itimer);

    //itimer = boost::timer::cpu_timer();
    //speed_6d_lattice.iterate(nn_sum);
    //itimer.stop();
    //T_MSG(       "  |-- added nn for all elements: ", itimer);

    //std::cout << "  |-- first element: " << speed_6d_lattice[0][0][0][0][0][0] << std::endl;

    /*
     * 3D, L=400, int
     */
    L = 400;
    std::cout << "|-+ int, 3D, L=" << L << " N=" << L * L * L << std::endl;

    //boost::array<int, 3> speed_3d_shape = {L, L, L};

    ctimer = boost::timer::cpu_timer();
    csp::Lattice<int, 3> speed_3d_lattice(L);
    ctimer.stop();
    T_MSG(       "  |-- constructed lattice: ", ctimer);

    itimer = boost::timer::cpu_timer();
    speed_3d_lattice.iterate(init_one);
    itimer.stop();
    T_MSG(       "  |-- set all elements one: ", itimer);

    itimer = boost::timer::cpu_timer();
    speed_3d_lattice.iterate(nn_sum);
    itimer.stop();
    T_MSG(       "  |-- added nn for all elements: ", itimer);

    /*
     * 3D, L=400, int, non-hc
     */
    L = 400;
    std::cout << "|-+ V2: non-hc, int, 3D, L=" << L << " N=" << L * L * L << std::endl;

    ctimer = boost::timer::cpu_timer();
    csp_v2::Lattice<int, 3, boost::mpl::bool_<false> > speed_3d_lattice_v2(L);
    ctimer.stop();
    T_MSG(       "  |-- constructed lattice: ", ctimer);

    itimer = boost::timer::cpu_timer();
    speed_3d_lattice_v2.iterate(init_one);
    itimer.stop();
    T_MSG(       "  |-- set all elements one: ", itimer);

    itimer = boost::timer::cpu_timer();
    speed_3d_lattice_v2.iterate(nn_sum);
    itimer.stop();
    T_MSG(       "  |-- added nn for all elements: ", itimer);

    /*
     * 3D, L=400, int, hc
     */
    L = 400;
    std::cout << "|-+ V2: hc, int, 3D, L=" << L << " N=" << L * L * L << std::endl;

    ctimer = boost::timer::cpu_timer();
    csp_v2::Lattice<int, 3, boost::mpl::bool_<true> > speed_3d_hc_lattice_v2(L);
    ctimer.stop();
    T_MSG(       "  |-- constructed lattice: ", ctimer);

    itimer = boost::timer::cpu_timer();
    speed_3d_hc_lattice_v2.iterate(init_one);
    itimer.stop();
    T_MSG(       "  |-- set all elements one: ", itimer);

    itimer = boost::timer::cpu_timer();
    speed_3d_hc_lattice_v2.iterate(nn_sum);
    itimer.stop();
    T_MSG(       "  |-- added nn for all elements: ", itimer);

    /*
     * no overhead comparison
     */
    //L = 400;
    //std::cout << "|-+ Comparison: int, 3D, L=" << L << " N=" << L * L * L 
    //          << " simple multi_array & for loop "<< std::endl;

    //boost::array<int, 1> bc_shape = {L+2};

    //ctimer = boost::timer::cpu_timer();
    //boost::multi_array<int, 3> speed_3d_array(speed_3d_shape);
    ////boost::array<boost::array<boost::array<int, 400>, 400>, 400> speed_3d_array;
    //boost::multi_array<int, 1> bc(bc_shape);
    ////boost::array<int, 402> bc;
    //bc[0] = L-1;
    //bc[L+1] = 0;
    //for(int i = 1; i < L+1; ++i)
    //{
    //    bc[i] = i - 1;
    //}
    //ctimer.stop();
    //T_MSG(       "  |-- constructed array & bc_array: ", ctimer);

    //itimer = boost::timer::cpu_timer();
    //for(int x = 0; x < L; ++x)
    //{
    //    for(int y = 0; y < L; ++y)
    //    {
    //        for(int z = 0; z < L; ++z)
    //        {
    //            speed_3d_array[x][y][z] = 1;
    //        }
    //    }
    //}
    //itimer.stop();
    //T_MSG(       "  |-- set all elements one: ", itimer);

    //itimer = boost::timer::cpu_timer();
    //for(int x = 0; x < L; ++x)
    //{
    //    for(int y = 0; y < L; ++y)
    //    {
    //        for(int z = 0; z < L; ++z)
    //        {
    //            speed_3d_array[x][y][z] += speed_3d_array[x][y][bc[z-1+1]]
    //                + speed_3d_array[x][y][bc[z+1+1]]
    //                + speed_3d_array[x][bc[y-1+1]][z]
    //                + speed_3d_array[x][bc[y+1+1]][z]
    //                + speed_3d_array[bc[x-1+1]][y][z]
    //                + speed_3d_array[bc[x+1+1]][y][z];
    //        }
    //    }
    //}
    //itimer.stop();
    //T_MSG(       "  |-- added nn for all elements: ", itimer);
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
    lattice.iterate(print);
    for(int i = 0; i < 3; ++i)
    {
        lattice.iterate(sdn);
        lattice.iterate(print);
    }

    boost::array<int, 2> shape2 = {{5, 5}};
    boost::array<int, 2> idx2 = {{2, 2}};

    csp::Lattice<double, 2, csp::bounds::mirror> lattice2(shape2);
    set_value_<double> set_0_01(0.01);
    lattice2.iterate(set_0_01);
    lattice2(idx2) = 0.1;

    std::cout << "|-- 5x5 double lattice, add direct neighbors, mirror boundaries" << std::endl;
    lattice2.iterate(print);
    lattice2.iterate(sum_prod_nn);
    std::cout << "sum of nn_prods: " << sum_prod_nn.result() << std::endl;
    //std::cout << "sum of nn_prosds: " << sum_prod_nn(lattice2) << std::endl;
    for(int i = 0; i < 3; ++i)
    {
        lattice2.iterate(sdn);
        lattice2.iterate(print);
    }
}
