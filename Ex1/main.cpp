#include <boost/array.hpp>
#include <boost/timer/timer.hpp>
#include <fstream>
#include <iostream>
#include <sstream>

#include "../Common/NDLattice.hpp"
#include "../Common/NDAlgorithm.hpp"
#include "../Common/NDIterate.hpp"
#include "../Common/random_msk.hpp"
#include "Ising.hpp"

#include "omp.h"

int main (int argc, char const* argv[])
{
    std::cerr << "starting ising simulation." << std::endl;
    boost::timer::auto_cpu_timer timer(std::cerr);

#ifdef MARIO_RANDOM
    typedef random_class<double, fibonacci> drng;
    typedef random_class<int, fibonacci>    irng;
#else
    srand48(17);
    srand(23);
#endif

    typedef csp::algorithm::set_value_<int> set_int;
#ifdef MARIO_RANDOM
    typedef csp::Lattice<int, 3, csp::bounds::periodic, irng> Lattice;
#else
    typedef csp::Lattice<int, 3> Lattice;
#endif
    using csp::algorithm::print_;
    using csp::algorithm::print;

    // parameters
    int L, Tsteps, nsteps;
    double T, Tbegin, Tend;
    std::ifstream param;
    if(argc > 1)
    {
        param.open(argv[1]);
        param >> L >> Tbegin >> Tend >> Tsteps >> nsteps;
    }
    else
    {
        L = 5;
        Tbegin = 0;
        Tend = 10;
        Tsteps = 100;
        nsteps = 100;
    }
    //assert(L < 20);
    //if(L > 20) L = 19;

#ifdef MARIO_RANDOM
    // rngs
    drng rdg;
    irng rig(0, L);
#endif

    // create & init lattice
    boost::array<int, 3> shape = {L, L, L};
#ifdef MARIO_RANDOM
    Lattice lattice(shape, rig);
#else 
    Lattice lattice(shape);
#endif
    set_int init_1(1);
    lattice.iterate(init_1);

    // instantiate ising simulation
#ifdef MARIO_RANDOM
    csp::ising::Ising<Lattice, drng> ising(lattice, T, rdg);
#else
    csp::ising::Ising<Lattice> ising(lattice, T);
#endif

    // run and write out plot
    std::stringstream fname;
    fname << "ising" << L << ".dat";
    std::ofstream file(fname.str().c_str());
    print_ plot(file);

    typedef boost::multi_array<double, 2> plot_type;
    plot_type em_v_T = ising.T_run(Tbegin, Tend, Tsteps, nsteps);
    csp::iterate::stencil_iterate<2>(em_v_T, plot);
    std::cout << lattice.system_size() << std::endl;
    return 0;
}
