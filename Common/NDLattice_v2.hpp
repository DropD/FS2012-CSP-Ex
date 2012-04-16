#ifndef CSP_ND_LATTICE_V2
#define CSP_ND_LATTICE_V2

#include <cmath>
#include <omp.h>

#include <boost/multi_array.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/timer/timer.hpp>
#include <iostream>
#include <iomanip>

#include "NDAlgorithm.hpp"
#include "NDIterate.hpp"

namespace csp_v2
{
    int tmsg_offset = 30;

    namespace algorithm
    {
    }

    namespace iterate
    {
        template<int D, class HC = boost::mpl::bool_<false> >
        struct stencil_iterate;

        template<int D>
        struct stencil_iterate<D, boost::mpl::bool_<false> >
        {
            stencil_iterate() {}
            template<class Lattice, class F>
            void inline operator() (Lattice& L, F& fcn)
            {
                //std::cout << "non-hc iteration over " << D << " dimensions starting.";
                //it_timer.start()
                boost::array<int, D> idx;
                stencil_iterate<D-1> next_it;
                for(int i = 0; i < L.size(); ++i)
                {
                    idx[0] = i;
                    next_it(L, idx, fcn);
                }
                fcn();
                fcn.finalize();
                //std::cout << std::setw(tmsg_offset) << "iteration time: " << it_timer.format() << std::endl;

            }
            template<class Lattice, class IndexList, class F>
            void inline operator() (Lattice& L, IndexList idx, F& fcn)
            {
                //it_timer.start();
                int dim = idx.size() - D;
                stencil_iterate<D-1> next_it;
                for(int i = 0; i < L.shape()[dim]; ++i)
                {
                    idx[dim] = i;
                    next_it(L, idx, fcn);
                }
                fcn();
                //std::cout << std::setw(tmsg_offset) << "subiteration " << dim << " -- time: " << it_timer.format() << std::endl;
            }

            //private:
            //boost::timer::cpu_timer it_timer;
        };

        template<>
        struct stencil_iterate<1, boost::mpl::bool_<false> >
        {
            inline stencil_iterate() {}
            template<class Lattice, class F>
            void inline operator() (Lattice& L, F& fcn)
            {
                //std::cout << "non-hc iteration over " << 1 << " dimensions starting.";
                //boost::timer::cpu_timer it_timer;
                for(int i = 0; i < L.size(); ++i)
                {
                    fcn(L[i]);
                }
                fcn();
                fcn.finalize();
                //std::cout << std::setw(tmsg_offset) << "iteration time: " << it_timer.format() << std::endl;
            }
            template<class Lattice, class IndexList, class F>
            void inline operator() (Lattice& L, IndexList idx, F& fcn)
            {
                //boost::timer::cpu_timer it_timer;
                int dim = idx.size() - 1;
                for(int i = 0; i < L.shape()[dim]; ++i)
                {
                    idx[dim] = i;
                    fcn(L, idx);
                }
                fcn();
                //std::cout << std::setw(tmsg_offset) << "subiteration " << dim << " -- time: " << it_timer.format() << std::endl;
            }
        };

        template<int D>
        struct stencil_iterate<D, boost::mpl::bool_<true> >
        {
            inline stencil_iterate() {}
            template<class Lattice, class F>
            void inline operator() (Lattice& L, F& fcn)
            {
                //std::cout << "hc iteration over " << D << " dimensions starting.";
                //boost::timer::cpu_timer it_timer;
                boost::array<int, D> idx;
                stencil_iterate<D-1> next_it;
                for(int i = 0; i < L.size(); ++i)
                {
                    idx[0] = i;
                    next_it(L, idx, fcn);
                }
                fcn();
                fcn.finalize();
                //std::cout << std::setw(tmsg_offset) << "iteration time: " << it_timer.format() << std::endl;
            }
            template<class Lattice, class IndexList, class F>
            void inline operator() (Lattice& L, IndexList idx, F& fcn)
            {
                //boost::timer::cpu_timer it_timer;
                int dim = idx.size() - D;
                stencil_iterate<D-1> next_it;
                for(int i = 0; i < L.size(); ++i)
                {
                    idx[dim] = i;
                    next_it(L, idx, fcn);
                }
                fcn();
                //std::cout << std::setw(tmsg_offset) << "subiteration " << dim << " -- time: " << it_timer.format() << std::endl;
            }
        };

        template<>
        struct stencil_iterate<3, boost::mpl::bool_<true> >
        {
            inline stencil_iterate() {}
            template<class Lattice, class F>
            void inline operator() (Lattice& L, F& fcn)
            {
                int p;
                #pragma omp parallel shared(p)
                {
                    p = omp_get_num_threads();
                }
                std::cout << "iterating <3, true> on " << p << " cores" << std::endl;

                std::cout << "hc iteration over " << 3 << " dimensions starting.";
                boost::timer::cpu_timer itx_timer; itx_timer.stop();
                boost::timer::cpu_timer ity_timer; ity_timer.stop();
                boost::timer::cpu_timer itz_timer; itz_timer.stop();
                itx_timer.start();
                for(int x = 0; x < L.size(); ++x)
                {
                    itx_timer.stop();
                    ity_timer.start();
                    for(int y = 0; y < L.size(); ++y)
                    {
                        ity_timer.stop();
                        itz_timer.start();
                        #pragma omp parallel for
                        for(int z = 0; z < L.size(); ++z)
                        {
                            fcn(L, x, y, z);
                        }
                        itz_timer.stop();
                        ity_timer.start();
                        fcn();
                    }
                    ity_timer.stop();
                    itz_timer.start();
                    fcn();
                }
                itz_timer.stop();
                fcn();
                fcn.finalize();
                std::cout << std::setw(tmsg_offset) << "subiteration " << 2 << " -- time: " << itz_timer.format() << std::endl;
                std::cout << std::setw(tmsg_offset) << "subiteration " << 1 << " -- time: " << ity_timer.format() << std::endl;
                std::cout << std::setw(tmsg_offset) << "iteration time: " << itx_timer.format() << std::endl;
            }
            template<class Lattice, class IndexList, class F>
            void inline operator() (Lattice& L, IndexList idx, F& fcn)
            {
                //boost::timer::cpu_timer it_timer;
                int dim = idx.size() - 3;
                for(int i = 0; i < L.shape()[dim]; ++i)
                {
                    idx[dim] = i;
                    stencil_iterate<2>(L, idx, fcn);
                }
                fcn();
                //std::cout << std::setw(tmsg_offset) << "subiteration " << dim << " -- time: " << it_timer.format() << std::endl;
            }
            
        };

        template<int D>
        struct stencil_iterate_v2
        {
            template<class Lattice, class F>
            void operator()(Lattice& L, F& fcn)
            {
            }
        };
    }

    namespace bounds
    {
        template<int D, class HC = boost::mpl::bool_<false> >
        struct periodic;

        template<int D>
        struct periodic<D, boost::mpl::bool_<false> >
        {
            template<typename IndexList>
            periodic(IndexList shape) : _shape(shape) {}
            periodic(int length) : _shape()
            {
                std::fill(_shape.begin(), _shape.end(), length);
            }

            inline int operator() (int idx, int dim)
            {
                if(idx < 0) return _shape[dim] + idx;
                else if(idx >= _shape[dim]) return idx - _shape[dim];
                else return idx;
            }
            template<class IndexList>
            inline IndexList operator() (IndexList idx)
            {
                for(int i = 0; i < idx.size(); ++i)
                {
                    idx[i] = operator()(idx[i], i);
                }
                return idx;
            }
            inline int operator[] (int idx) {return operator()(idx, 0);}

            private:
            boost::array<int, D> _shape;
        };

        template<int D>
        struct periodic<D, boost::mpl::bool_<true> >
        {
            periodic(int length) : _lookup()
            {
//                std::cout << "DEBUG PERIODIC.Const.1.0" << std::endl;
                boost::array<int, 1> shape = {length+1};
//                std::cout << "DEBUG PERIODIC.Const.1.1" << std::endl;
                _lookup.resize(shape);
//                std::cout << "DEBUG PERIODIC.Const.1.2" << std::endl;
                _lookup[0] = length - 1;
                _lookup[length] = 0;
                for(int i = 1; i < length; ++i)
                {
                    _lookup[i] = i;
                }
            }

            inline int operator[] (int idx) 
            { 
//                std::cout << "DEBUG 1.0" << std::endl;
                int res = _lookup[idx+1];
//                std::cout << "DEBUG 1.1" << std::endl;
                return res;
            }

            private:
            boost::multi_array<int, 1> _lookup;
        };
    }

    template<class T, int D, class HC, class BC>
    struct sublattice;

    template<class T 
             , int D
             , class HC = boost::mpl::bool_<false>
             , class BC = bounds::periodic<D, HC>
             , class R = int()>
    class Lattice
    {
        public:
        typedef boost::multi_array<T, D> ndarray;
        typedef boost::array<T, D> shape_type;
        typedef typename ndarray::template subarray<D-1>::type subarray; 
        typedef T element;
        //friend struct sublattice<T, D-1>;

        public:
        // constructors
        Lattice();
        Lattice(int L, T init_value = 0);
        Lattice(shape_type shape, T init_value = 0, boost::disable_if<HC, int> dummy = 0);

        // element access
        template<typename IndexList>
        inline T& operator() (IndexList idx) {return _lattice(idx);}
        // element access shortcuts
        //inline subarray operator[](int x) {return _lattice[x];}
        inline sublattice<T, D-1, HC, BC> operator[](int x) 
        {
            return sublattice<T, D-1, HC, BC>(_lattice[bc[x]], bc);
        }
        //inline typename ndarray::template subarray<D-2>::type 
        //    operator()(int x, int y) {return _lattice[x][y];}
        //inline typename ndarray::template subarray<D-3>::type
        //    operator()(int x, int y, int z) {return _lattice[x][y][z];}

        // stencil interface
        template<typename IndexList>
        inline T& get_cell(IndexList idx) { return _lattice(bc(idx)); }
        //inline typename boost::enable_if<boost::mpl::bool_<D == 1>, T&>::type
        //    get_cell(int x) { return _lattice[bc[x]]; }
        //inline typename boost::enable_if<boost::mpl::bool_<D == 2>, T&>::type
        //    get_cell(int x, int y) { return _lattice[bc[x]][bc[y]]; }
        //inline typename boost::disable_if<boost::mpl::bool_<D == 2>, void>::type 
        //    get_cell(int x, int y) { throw "invalid method"; }
        //inline typename boost::enable_if<boost::mpl::bool_<D == 3>, T&>::type 
        //T& get_cell(int x, int y, int z) { return _lattice[bc[x]][bc[y]][bc[z]]; }

        template<typename IndexList>
        T& get_n_left(IndexList idx, int n)
        {
            idx[n] = bc(idx[n]-1, n);
            return _lattice(idx);
        }

        template<typename IndexList>
        T& get_n_right(IndexList idx, int n)
        {
            idx[n] = bc(idx[n]+1, n);
            return _lattice(idx);
        }

        shape_type get_random_index();

        // iterating
        template<typename F>
        void iterate(F& fcn) 
        {
            iterate::stencil_iterate<D, HC> iterator;
            iterator(*this, fcn);
        }

        // stuff
        const size_t system_size() {return system_size;}
        const size_t size() {return _lattice.size();}
        const boost::multi_array_types::size_type* shape() {return _lattice.shape();}

        public:
        static const int dimension = D;
        static const bool is_hypercube = HC();

        private:
        ndarray _lattice;
        BC bc;
        size_t _system_size;
    };

    template<class T, int D, class HC, class BC, class R>
    Lattice<T, D, HC, BC, R>::Lattice() : _lattice(), bc() {}

    template<class T, int D, class HC, class BC, class R>
    Lattice<T, D, HC, BC, R>::Lattice(int L, T init_value) : _lattice(), bc(L)
    {
//        std::cout << "DEBUG NDLv2.Cons.1.0.0" << std::endl;
        boost::array<int, D> shape;
//        std::cout << "DEBUG NDLv2.Cons.1.0.1" << " shape.size(): " << shape.size() << std::endl;
        std::fill(shape.begin(), shape.end(), L);
//        std::cout << "DEBUG NDLv2.Cons.1.0.2" << " shape[2]: " << shape[2] << std::endl;
        //_lattice = boost::multi_array<T, D>(shape);
        //boost::multi_array<T, D> test_array(shape);
        _lattice.resize(shape);
//        std::cout << "DEBUG NDLv2.Cons.1.0.3" << std::endl;

//        std::cout << "DEBUG NDLv2.Cons.1.1" << std::endl;
        if(init_value != 0)
        {
            csp::algorithm::set_value_<T> init(init_value);
            iterate(init);
        }

//        std::cout << "DEBUG NDLv2.Cons.1.2" << std::endl;
        size_t size = 1;
        for(int i = 0; i <D; ++i)
            size *= L;
        _system_size = size;
//        std::cout << "DEBUG NDLv2.Cons.2.0" << std::endl;
    }

    template<class T, int D, class HC, class BC, class R>
    Lattice<T, D, HC, BC, R>::Lattice(shape_type shape, T init_value, boost::disable_if<HC, int> dummy) 
    : _lattice(shape), bc(shape)
    {
        if(init_value != 0)
        {
            csp::algorithm::set_value_<T> init(init_value);
            iterate(init);
        }

        size_t size = 1;
        for(int i = 0; i < D; ++i)
            size *= shape[i];
        _system_size = size;
    }

    //template<class T, int D, class HC, class BC>
    //struct sublattice;

    template<class T, int D, class BC>
    struct sublattice<T, D, boost::mpl::bool_<true>, BC>
    {
        public:
        typedef typename Lattice<T, D+1>::subarray subarray;
        //friend struct sublattice<T, D-1>;

        sublattice(subarray subarr, BC& bc) : _sublattice(subarr), bc(bc) {}
        inline sublattice<T, D-1, boost::mpl::bool_<true>, BC> operator[] (int x) 
        {
            return sublattice<T, D-1, boost::mpl::bool_<true>, BC>(_sublattice[bc[x]], bc);
        }

        private:
        subarray _sublattice;
        BC& bc;
    };

    template<class T, class BC>
    struct sublattice<T, 1, boost::mpl::bool_<true>, BC>
    {
        public:
        typedef typename Lattice<T, 2>::subarray subarray;

        sublattice(subarray subarr, BC& bc) : _sublattice(subarr), bc(bc) {}
        inline T& operator[] (int x) 
        {
            return _sublattice[bc[x]];
        }

        private:
        subarray _sublattice;
        BC& bc;
    };

    template<class T, int D, class BC>
    struct sublattice<T, D, boost::mpl::bool_<false>, BC>
    {
        public:
        typedef typename Lattice<T, D+1>::subarray subarray;

        sublattice(subarray subarr, BC& bc) : _sublattice(subarr), bc(bc), _dim(D) {}
        sublattice(subarray subarr, BC& bc, int dim) : _sublattice(subarr), bc(bc), _dim(dim) {}
        inline sublattice<T, D-1, boost::mpl::bool_<false>, BC> operator[] (int x)
        {
            return sublattice<T, D-1, boost::mpl::bool_<false>, BC>(_sublattice[bc(x, _dim - D)], bc);
        }

        private:
        subarray _sublattice;
        BC& bc;
        int _dim;
    };

    template<class T, class BC>
    struct sublattice<T, 1, boost::mpl::bool_<false>, BC>
    {
        public:
        typedef typename Lattice<T, 2>::subarray subarray;

        sublattice(subarray subarr, BC& bc) : _sublattice(subarr), bc(bc), _dim(1) {}
        sublattice(subarray subarr, BC& bc, int dim) : _sublattice(subarr), bc(bc), _dim(dim) {}
        inline T& operator[] (int x)
        {
            return _sublattice[bc(x, _dim - 1)];
        }

        private:
        subarray _sublattice;
        BC& bc;
        int _dim;
    };
}

#endif
