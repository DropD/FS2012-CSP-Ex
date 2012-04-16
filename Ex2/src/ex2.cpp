#include <boost/timer/timer.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

#include <omp.h>

#include "../../Common/NDLattice.hpp"
#include "../../Common/NDIterate.hpp"
#include "../../Common/NDAlgorithm.hpp"
#include "../../Common/Ising.hpp"

#include "../../Common/random_msk.hpp"

/*
 * typedefs
 */

typedef random_class<double, fibonacci> drng;
typedef random_class<int, fibonacci>    irng;

typedef csp::Lattice<int, 3, csp::bounds::periodic, irng> Lattice;

int main (int argc, char const* argv[])
{
    /*
     * parameters
     */
    int L, Tsteps, nsteps;
    double Tbegin, Tend;
    //std::ifstream param;
    if(argc > 1)
    {
        //param.open(argv[1]);
        //param >> L; //>> Tbegin >> Tend >> Tsteps >> nsteps;
        L = atoi(argv[1]);
    }
    else
    {
        L = 10;
    }
    Tbegin = 4;
    Tend = 5;
    Tsteps = 200;
    nsteps = 400;
       

    //for(L = 10; L < 55; L += 5)
    //{
        boost::timer::cpu_timer timer;
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
            << " spin flips: " << (double) (1.0 * Tsteps * nsteps * L*L*L + (Tbegin ? nsteps*L*L*L : 0))
            << " t = " << timer.format() << std::endl;
    //}

    return 0;
}
