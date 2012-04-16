#include<boost/array.hpp>
#include<boost/timer/timer.hpp>
#include<boost/python.hpp>
#include<fstream>
#include<iostream>
#include<sstream>

#include "NDLattice_v2.hpp"
#include "NDAlgorithm.hpp"
#include "NDIterate.hpp"
#include "random_msk.hpp"
#include "Ising.hpp"

typedef random_class<double, fibonacci> drng;
typedef random_class<int, fibonacci>    irng;

typedef csp::Lattice<int, 3, csp::bounds::periodic, irng> Lattice_int3;
typedef csp::ising::Ising<Lattice_int3, drng> Ising_int3;

struct Parameters
{
    double T_begin, T_end;
    int Tsteps, nsteps;
};

struct pyIsing
{
    boost::multi_array<double, 2> run(int L, Parameters par)
    {
        drng rdg;
        irng rig(0, L);

        Lattice_int3 lattice(L, 1, rig);
        Ising_int3   ising(lattice, par.T_begin, rdg);
        return ising.T_run(par.T_begin, par.T_end, par.Tsteps, par.nsteps);
    }
};

using namespace boost::python;

BOOST_PYTHON_MODULE_INIT(pyIsing)
{
    //class_<irng>         ir("irng", init<int, int>());
    //class_<Lattice_int3> lattice("lattice", init<int, int, irng>());
    //class_<Ising_int3>   ising("ising", init<Lattice_int3, double>());
    //ising.def("run", &Ising_int3::T_run);
    class_<Parameters> par("param");
    class_<pyIsing> pI("pyIsing");
    pI.def("run", &pyIsing::run);
}
