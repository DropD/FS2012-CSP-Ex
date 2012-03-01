/*
 * N Dimensional Lattice Class
 * Written for CSP FS 2012
 * by Rico Häuselmann
 * 24. 02. 2012
 */

#ifndef CSP
#define CSP

#include <boost/array.hpp>
#include <boost/multi_array.hpp>
#include <boost/multi_array/iterator.hpp>
#include <vector>
#include <iostream>

namespace csp
{
    template<int D>
    struct iterate
    {
        template<typename Iterator, typename F>
        inline iterate(Iterator begin, Iterator end, F fcn)
        {
            for(Iterator it = begin; it < end; ++it)
            {
                iterate<D-1>((*it).begin(), (*it).end(), fcn);
            }
            fcn();
        }
    };

    template<>
    struct iterate<1>
    {
        template<typename Iterator, typename F>
        inline iterate(Iterator begin, Iterator end, F fcn)
        {
            for(Iterator it = begin; it < end; ++it)
            {
                fcn(*it);
            }
            fcn();
        }
    };

    // OR (supporting stencils:
    template<int D>
    struct iterate_idx
    {
        template<typename Lattice, typename F>
        inline iterate_idx(Lattice& L,  F fcn)
        {
            boost::array<int, D> idx;
            for(int i = 0; i < L.size(); ++i)
            {
                idx[0] = i;
                iterate_idx<D-1>(L, idx, fcn);
            }
            fcn();
        }
        template<typename Lattice, typename IndexList, typename F>
        inline iterate_idx(Lattice& L,  IndexList& idx, F fcn)
        {
            int dim = idx.size() - D;
            for(int i = 0; i < L.shape()[dim]; ++i)
            {
                idx[dim] = i;
                iterate_idx<D-1>(L, idx, fcn);
            }
            fcn();
        }
    };

    template<>
    struct iterate_idx<1>
    {
        template<typename Lattice, typename F>
        inline iterate_idx(Lattice& L, F fcn)
        {
            for(int i = 0; i < L.size(); ++i)
            {
                fcn(L[i]);
            }
            fcn();
        }
        template<typename Lattice, typename IndexList, typename F>
        inline iterate_idx(Lattice& L, IndexList& idx, F fcn)
        {
            int dim = idx.size() - 1;
            for(int i = 0; i < L.shape()[dim]; ++i)
            {
                idx[dim] = i;
                fcn(L, idx);
            }
            fcn();
        }
    };

    //struct boundary_conditions
    //{
    //    virtual int operator() (int idx, int dim) = 0;
    //}

    namespace bounds
    {
        template<typename T, int D>
        struct periodic
        {
            //boost::array<long int, D> _shape;
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

        template<typename S>
        struct zero_boundaries
        {
            S _shape;
            zero_boundaries(S shape) : _shape(shape) {}
            inline int operator() (int idx, int dim)
            {
                if(0 <= idx < _shape[dim]) return idx;
                else return 0;
            }
        };

        template<typename S>
        struct mirror_boundaries
        {
            S _shape;
            mirror_boundaries(S shape) : _shape(shape) {}
            inline int operator() (int idx, int dim)
            {
                if(idx < 0) return -idx;
                else if(idx > _shape[dim]) return 2*_shape[dim] - idx;
                else return 0;
            }
        };
    };

    template<class T, int D, typename BC = bounds::periodic<T, D> >
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

        public:
        // construction
        Lattice() : _lattice(), bc(_lattice.shape()) {}
        Lattice(ndarray& start_array) : _lattice(start_array), bc(_lattice.shape()) {}
        template<typename S>
        Lattice(S& shape) : _lattice(shape), bc(_lattice.shape()) {}
        // still does not work... :( I wonder why...
        //Lattice(int size)
        //{
        //    boost::array<int, D> shape;
        //    std::fill(shape.begin(), shape.end(), size);
        //    _lattice = ndarray(shape);
        //}

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

        // Iterating
        iterator begin() {return _lattice.begin();}
        iterator end() {return _lattice.end();}
        template<typename F>
        void iterate(F fcn) {iterate<D>(begin(), end(), fcn);}

        // stuff
        size_t size() {return _lattice.size();}
        const boost::multi_array_types::size_type* shape() {return _lattice.shape();}

        private:
        ndarray _lattice;
        BC bc;
    };

    //template<class T>
    //class Lattice3 : public Lattice<T, 3>
    //{
    //};

    //template<class T>
    //class Lattice2 : public Lattice<T, 2>
    //{
    //};

    //template <typename T, typename TPtr, typename NumDims, typename Reference>
    //class lattice_iterator 
    //    : public boost::detail::multi_array::array_iterator<
    //                                            T
    //                                          , TPtr
    //                                          , NumDims
    //                                          , Reference
    //                                        >
    //{
    //};

    // try extending array_iterator first
    //template <typename T, typename TPtr, typename NumDims, typename Reference>
    //class lattice_iterator;

    //template <typename T, typename TPtr, typename NumDims, typename Reference>
    //class lattice_iterator : public iterator_facade<
    //                                    lattice_iterator<T, TPtr, NumDims, Reference>
    //                                  , typename associated_types<T, NumDims>::value_type
    //                                  , boost::random_access_traversal_tag
    //                                  , Reference
    //                                >

}

#endif
