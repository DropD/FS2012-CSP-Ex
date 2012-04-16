#include <iostream>
#include <fstream>
#include <sstream>

#include <boost/timer/timer.hpp>

#include "../../Common/NDLattice.hpp"
#include "../../Common/NDIterate.hpp"
#include "../../Common/NDAlgorithm.hpp"
#include "../../Common/Ising.hpp"

#include "../../Common/random_msk.hpp"

void ising_run(int L);

/*
 * typedefs
 */

typedef random_class<double, fibonacci> drng;
typedef random_class<int, fibonacci>    irng;

typedef csp::Lattice<int, 3, csp::bounds::periodic, irng> Lattice;

/*
 * main
 */

int main (int argc, char const* argv[])
{
    boost::timer::cpu_timer timer;

    int L;
    std::ifstream param;
    param.open(argv[1]);
    param >> L;
    std::cout << L << std::endl;

    ising_run(L);

    return 0;
}

void ising_run(int L)
{
    std::cout << "starting fsc experiment with L = " << L << std::endl;
    boost::timer::cpu_timer timer;

    /*
     * parameters
     */
    int Tsteps, nsteps;
    double Tbegin, Tend;

    Tbegin = 4.5;
    Tend = 5;
    Tsteps = 100;
    nsteps = 200;

    /*
     * setup
     */
    drng rdg;
    irng rig(0, L);

    Lattice lattice(L, 1, rig);
    csp::ising::Ising<Lattice, drng> ising(lattice, Tbegin, rdg);

    //thermalization
    if(Tbegin > 0)
    {
        for(int i = 0; i < nsteps; ++i)
        {
            ising.step();
        }
    }

    /*
     * run and plot
     */
    std::stringstream fname;
    fname << "ising" << L << ".dat";
    std::ofstream file(fname.str().c_str());
    csp::algorithm::print_ plot(file);

    typedef boost::multi_array<double, 2> plot_type;
    plot_type plot_array = ising.T_run(Tbegin, Tend, Tsteps, nsteps);
    csp::iterate::stencil_iterate<2>(plot_array, plot);
    std::cout << "L = " << L 
        << " spin flips: " 
        << (double) (1.0 * Tsteps * nsteps * L*L*L + (Tbegin ? nsteps*L*L*L : 0))
        << " t = " << timer.format() 
        << std::endl;
}
