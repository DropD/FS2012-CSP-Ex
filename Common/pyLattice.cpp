#include <boost/python.hpp>

using namespace boost::python;

BOOST_PYTHON_MODULE_INIT(pyLattice)
{
    class_<csp_v2::Lattice<int, 3> > lattice_int3("lattice_int3", init<int, int>());
    lattice_int3.def(
}
