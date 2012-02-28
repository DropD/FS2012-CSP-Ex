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

    // OR:
    template<int D>
    struct iterate_idx
    {
        template<typename Lattice, typename F>
        inline iterate_idx(Lattice L,  F fcn)
        {
            for(int i = 0; i < L.size(); ++i)
            {
                iterate_idx<D-1>(L, boost::indices[i], fcn);
            }
            fcn();
        }
        template<typename Lattice, typename IndexList, typename F>
        inline iterate_idx(Lattice L,  IndexList idx, F fcn)
        {
            for(int i = 0; i < L.size(); ++i)
            {
                iterate_idx<D-1>(L, idx[i], fcn);
            }
            fcn();
        }
    };

    template<>
    struct iterate_idx<1>
    {
        template<typename Lattice, typename F>
        inline iterate_idx(Lattice L, F fcn)
        {
            for(int i = 0; i < L.size(); ++i)
            {
                fcn(L[i]);
            }
            fcn();
        }
        template<typename Lattice, typename IndexList, typename F>
        inline iterate_idx(Lattice L, IndexList idx, F fcn)
        {
            for(int i = 0; i < L.size(); ++i)
            {
                fcn(L[idx][i]);
            }
            fcn();
        }
    };

    template<class T, int D>
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

        public:
        // construction
        Lattice() : _lattice() {}
        Lattice(ndarray& start_array) : _lattice(start_array) {}
        Lattice(boost::array<int, D>& shape) : _lattice(shape) {}
        Lattice(shape_type& shape) : _lattice(shape) {}
        // does not work... :(
        //Lattice(int size)
        //{
        //    shape_type shape;
        //    std::fill(shape.begin(), shape.end(), size);
        //    _lattice = ndarray(shape);
        //}

        // element access
        template<typename IndexList> 
        T& operator() (IndexList idx) {return _lattice(idx);}

        // subarray & view access
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
            idx[n] -= 1;
            return _lattice(idx);
        }

        // Iterating
        iterator begin() {return _lattice.begin();}
        iterator end() {return _lattice.end();}
        template<typename F>
        void iterate(F fcn) {iterate<D>(begin(), end(), fcn);}

        // stuff
        size_t size() {return _lattice.size();}

        private:
        ndarray _lattice;
    };

    template<class T>
    class Lattice3 : public Lattice<T, 3>
    {
    };

    template<class T>
    class Lattice2 : public Lattice<T, 2>
    {
    };

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
