#include<boost/array.hpp>
#include<boost/timer/timer.hpp>
#include<fstream>
#include<iostream>
#include<sstream>

#include "NDLattice_v2.hpp"
#include "NDAlgorithm.hpp"
#include "NDIterate.hpp"
#include "random_msk.hpp"
#include "Ising.hpp"


typedef csp::Lattice<int, 3> lattice_int3;
typedef csp::ising::Ising
