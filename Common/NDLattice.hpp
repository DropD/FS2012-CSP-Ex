/*
 * N Dimensional Lattice Class
 * Written for CSP FS 2012
 * by Rico Häuselmann
 * 24. 02. 2012
 */

#ifndef CSP_ND_LATTICE
#define CSP_ND_LATTICE

#include <boost/smart_ptr.hpp>
#include <boost/array.hpp>
#include <boost/multi_array.hpp>
#include <boost/multi_array/iterator.hpp>
#include <vector>
#include <iostream>

#include "NDIterate.hpp"
#include "NDAlgorithm.hpp"

namespace csp
{
    namespace bounds
    {
        struct periodic
        {
            periodic() {}
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

        struct hc_periodic_lookup
        {
            //boost::array<int, N+2> table;
            boost::shared_array<int> table;
            hc_periodic_lookup(int length) : table(new int[length])
            {
                table[0] = length+1;
                table[length-1] = 0;

                for(int i = 1; i < length+1; ++i)
                {
                    table[i] =  i;
                }
            }
            inline int operator() (int idx, int dim)
            {
                return table[idx];
            }
        };
    }

    template<class T, int D, typename BC = bounds::periodic, typename R = int()>
    class Lattice
    {
        public:
        typedef boost::multi_array<T, D> ndarray;
        typedef typename ndarray::index index;
        typedef boost::array<index, D> shape_type;
        typedef typename ndarray::iterator iterator;
        typedef boost::detail::multi_array::index_gen<D, D-1> index_gen;
        typedef typename ndarray::template array_view<D-1>::type array_view;
        typedef typename ndarray::reference reference;
        typedef T element_type;
        typedef T element;

        public:
        // construction
        Lattice() : _lattice(), bc(_lattice.shape()), rng(rand) {}
        Lattice(ndarray& start_array) : _lattice(start_array), bc(_lattice.shape()), rng(rand) {}
        template<typename S>
        Lattice(S& shape, R& rng = rand) : _lattice(shape), bc(_lattice.shape()), rng(rng) {}
        Lattice(int L, T init_value = 0, R& rng = rand) : _lattice(), bc(), rng(rng) 
        {
            boost::array<int, D> shape;
            std::fill(shape.begin(), shape.end(), L);
            _lattice.resize(shape);
            bc = BC(_lattice.shape());

            if(init_value != 0)
            {
                algorithm::set_value_<T> init(init_value);
                iterate(init);
            }
        }

        // element access USE THIS INSTEAD OF [] !!
        template<typename IndexList> 
        T& operator() (IndexList idx) {return _lattice(idx);}

        // subarray & view access !! EXPERIMENTAL !!
        reference operator[] (index idx) {return _lattice[idx];}
        // this does not yet totally work, and it uses boost::detail
        array_view operator[] (index_gen idl) {return _lattice[idl];}
        ndarray& array() {return _lattice;}

        // stencil interface
        template<typename IndexList> 
        T& get_cell(IndexList idx) {return _lattice(idx);}
        template<typename IndexList> 
        T& get_n_left(IndexList idx, int n)
        {
            idx[n] = bc(idx[n] - 1, n);
            return _lattice(idx);
        }
        template<typename IndexList> 
        T& get_n_right(IndexList idx, int n)
        {
            idx[n] = bc(idx[n] + 1, n);
            return _lattice(idx);
        }
        boost::array<int, D> get_random_index()
        {
            boost::array<int, D> idx;
            for(int i = 0; i < D; ++i)
            {
                idx[i] = rng() % shape()[i];
            }
            return idx;
        }

        // Iterating
        iterator begin() {return _lattice.begin();}
        iterator end() {return _lattice.end();}
        template<typename F>
        void iterate(F& fcn) {iterate::stencil_iterate<D>(*this, fcn);}

        // stuff
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
        //void init_with(T value)
        //{
        //    set_value_<T> set_value(value);
        //    iterate::stencil_iterate<D>(*this, set_value);
        //}
        
        public:
        static const int dimension = D;

        private:
        ndarray _lattice;
        BC bc;
        R& rng;
    };

    /*
    template<class T, typename BC, typename R>
    class Lattice<T, 3, BC, R>
    {
        public:
        typedef boost::multi_array<T, 3> ndarray;
        typedef typename ndarray::index index;
        typedef boost::array<index, 3> shape_type;
        typedef T element;

        public:
        // construction
        Lattice() : _lattice(), bc(_lattice.shape()) {}
        Lattice(ndarray& start_array) : _lattice(start_array), bc(_lattice.shape()) {}
        template<typename S>
        Lattice(S& shape, R& rng = rand) : _lattice(shape), bc(_lattice.shape()), rng(rng) {}

        private:
        ndarray _lattice;
        BC bc;
        R& rng;
    };
    */

}

#endif
