/*
 * N Dimensional Lattice Class
 * Written for CSP FS 2012
 * by Rico Häuselmann
 * 24. 02. 2012
 */

#ifndef CSP
#define CSP

#include <boost/multi_array.hpp>

namespace csp
{
    template<int D, class T>
    class Lattice
    {
        private:
        typedef boost::multi_array<T, D> ndarray;
        typedef ndarray::index index;
        typedef boost::array<index, D> shape_type;
        ndarray _lattice;

        public:
        Lattice() : _lattice() {}
        Lattice(ndarray start_array) : _lattice(start_array) {}
        Lattice(shape_type shape) : _lattice(boost::shape_type(shape)) {}
    }
}

#endif
