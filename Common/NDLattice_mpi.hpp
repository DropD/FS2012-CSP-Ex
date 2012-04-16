/*
 * N Dimensional Lattice Class
 * Written for CSP FS 2012
 * by Rico Häuselmann
 * 24. 02. 2012
 */

#ifndef CSP_ND_LATTICE_MPI
#define CSP_ND_LATTICE_MPI

#include <boost/mpi.hpp>
#include <boost/array.hpp>
#include <boost/multi_array.hpp>
#include <boost/multi_array/iterator.hpp>
#include <vector>
#include <iostream>

#include "NDIterate.hpp"

namespace mpi = boost::mpi;

namespace csp
{
    namespace bounds
    {
        struct periodic
        {
            const boost::multi_array_types::size_type* _shape;
            template<typename S>
            periodic(S shape) : _shape(shape) {}
            inline int operator() (int idx, int dim)
            {
                if(idx < 0) return _shape[dim] + idx;
                else if(idx >= _shape[dim]) return idx - _shape[dim];
                else return idx;
            }
        };

        struct mirror
        {
            const boost::multi_array_types::size_type* _shape;
            template<typename S>
            mirror(S shape) : _shape(shape) {}
            inline int operator() (int idx, int dim)
            {
                if(idx < 0) return -idx;
                else if(idx >= _shape[dim]) return 2*_shape[dim] - idx - 2;
                else return idx;
            }
        };
    }

    template<class T, int D, typename BC = bounds::periodic>
    class Lattice
    {
        public:
        typedef boost::multi_array<T, D> ndarray;
        typedef typename ndarray::index index;
        typedef boost::array<int, D> shape_type;
        typedef typename ndarray::iterator iterator;
        typedef boost::detail::multi_array::index_gen<D, D-1> index_gen;
        typedef typename ndarray::template array_view<D-1>::type array_view;
        typedef typename ndarray::reference reference;
        typedef T element_type;
        typedef T element;

        public:
        // construction
        Lattice() : _lattice(), bc(_lattice.shape()) {}
        Lattice(ndarray& start_array) : _lattice(start_array), bc(_lattice.shape()) {}
        template<typename S> Lattice(S& shape) : _lattice(shape), bc(_lattice.shape()) {}
        template<typename S> Lattice(S& shape, mpi::communicator World) 
            : comm(World), g_shape(shape), bc(g_shape) 
        {
            shape_type l_shape = g_shape;
            int p = comm.size();
            // distribute along the least continuous dimension
            if(p <= g_shape[0])
            {
                if(comm.rank() == 0)
                {
                    l_shape[0] = (g_shape[0] / p) + (g_shape[0] % p);
                    l_shape[0] += 2;
                }
                else
                {
                    l_shape[0] = g_shape[0] / p;
                    l_shape[0] += 2;
                }
            }
            else if(p < 2 * g_shape[0] || D == 1)
            {
                // reset the communicator to contain only the processors which are used.
                if(comm.rank() == 0) 
                {
                    std::cerr << "Number of processors is bigger than the outmost dimension, "
                                               << "but less than two times. As a result, not all processors will be used." 
                                               << std::endl;
                }
                bool is_in = comm.rank() < g_shape[0];
                mpi::communicator grid_comm = world.split(is_in);
                comm = grid_comm;
                p = comm.size();
                if(comm.rank() == 0)
                {
                    l_shape[0] = (g_shape[0] / p) + (g_shape[0] % p);
                    l_shape[0] += 2;
                }
                else
                {
                    l_shape[0] = g_shape[0] / p;
                    l_shape[0] += 2;
                }
                
            }
            else
            {
                // we are talking insanely many processors now...
                int q = g_shape[0] * (p % g_shape[0]);
                if(comm.rank() == 0)
                {
                    std::cerr << "Only " << q << " processors will be used." << std::endl;
                }
                bool is_in = comm.rank() < g_shape[0];
                mpi::communicator grid_comm = world.split(is_in);
                comm = grid_comm;
                p = g_shape[0];
                q = q % p;
                if(comm.rank() % g_shape[0] == 0)
                {
                    l_shape[0] = 3;
                    l_shape[1] = (g_shape[1] / p) + (g_shape[1] % q);
                    l_shape[1] += 2;
                }
                else
                {
                    l_shape[0] = 3;
                    l_shape[1] = g_shape[1] / q;
                    l_shape[1] += 2;
                }
            }
            _lattice = ndarray(l_shape);
        }

        /* ----------------------------------------
         * element access USE THIS INSTEAD OF [] !!
         */
        template<typename IndexList> 
        T& operator() (IndexList idx) {return _lattice(idx);}
        /* end of element access
         * ---------------------------------------- */

        /*// subarray & view access !! EXPERIMENTAL !!
        reference operator[] (index idx) {return _lattice[idx];}
        // this does not yet totally work, and it uses boost::detail
        array_view operator[] (index_gen idl) {return _lattice[idl];}
        ndarray& array() {return _lattice;}*/

        /* ----------------------------------------
         * stencil interface
         */
        template<typename IndexList> T& get_cell(IndexList idx) {return _lattice(idx);}
        template<typename IndexList> T& get_n_left(IndexList idx, int n)
        {
            idx[n] = bc(idx[n] - 1, n);
            return _lattice(idx);
        }
        template<typename IndexList> T& get_n_right(IndexList idx, int n)
        {
            idx[n] = bc(idx[n] + 1, n);
            return _lattice(idx);
        }
        shape_type get_random_index()
        {
            shape_type idx;
            for(int i = 0; i < D; ++i)
            {
                idx[i] = rand() % shape()[i];
            }
            return idx;
        }
        /* end of stencil interface
         * ---------------------------------------- */

        /* ----------------------------------------
         * iteration interface
         */
        iterator begin() {return _lattice.begin();}
        iterator end() {return _lattice.end();}
        template<typename F> void iterate(F& fcn) {iterate::stencil_iterate<D>(*this, fcn);}
        //--TODO: linear iteration using _lattice.data() oder .origin()?
        //--TODO: Z-Ordering?
        /* end of iteration interface
         * ---------------------------------------- */

        /* ----------------------------------------
         * get information
         */
        size_t system_size() 
        {
            size_t size = 1;
            const boost::multi_array_types::size_type* shape = _lattice.shape();
            for(int i = 0; i < D; ++i)
            {
                size *= shape[i];
            }
            return size;
        }
        size_t size() { return _lattice.size();}
        const boost::multi_array_types::size_type* shape() {return _lattice.shape();}
        
        public:
        static const int dimension = D;
        //--TODO: adapt this to mirror behaviour of multi_array
        /* end of get information
         * ---------------------------------------- */

        private:
        // core stuff
        ndarray _lattice;
        BC bc;

        // mpi stuff
        mpi::communicator& comm;
        shape_type g_shape;
    };

}

#endif
