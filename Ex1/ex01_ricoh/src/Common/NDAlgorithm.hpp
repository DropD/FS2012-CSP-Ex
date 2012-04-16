/*
 * N Dimensional algorithms
 * Written for CSP FS 2012
 * by Rico Häuselmann
 * 07. 03. 2012
 */

#ifndef CSP_ND_ALGORITHM
#define CSP_ND_ALGORITHM

#include <iostream>
#include <iomanip>

namespace csp
{
    namespace algorithm
    {
        template<typename T>
        struct set_value_
        {
            set_value_(T value) : value(value) {}

            template<typename Lattice, typename IndexList>
            inline void operator() (Lattice& L, IndexList idx)
            {
                L(idx) = value;
            }
            inline void operator() () {}
            inline void finalize() {}

            private:
            T value;
        };

        struct print_
        {
            print_(std::ostream& os = std::cout) : os(os) {}
            template<typename Lattice, typename IndexList>
            inline void operator() (Lattice& L, IndexList& idx) 
            { 
                typename Lattice::element e = L(idx);
                os << e << " "; 
            }
            inline void operator() () { os << std::endl; }
            inline void finalize() {}

            private:
            std::ostream& os;
        } print;

        struct add_direct_neighbors
        {
            template<typename Lattice, typename IndexList>
            inline void operator() (Lattice& L, IndexList& idx) 
            {
                typename Lattice::element delta = 0;
                for(int dim = 0; dim < idx.size(); ++dim)
                {
                    delta += L.get_n_left(idx, dim);
                    delta += L.get_n_right(idx, dim);
                }
                L(idx) += delta;
            }
            inline void operator() () {}
            inline void finalize() {}
        };

        template<typename T>
        struct sum_prod_nn_
        {
            private:
            T sum;

            public:
            sum_prod_nn_() : sum(0) {}

            template<typename Lattice, typename IndexList>
            inline void operator() (Lattice& L, IndexList& idx)
            {
                typedef typename Lattice::element element;
                for(int dim = 0; dim < idx.size(); ++dim)
                {
                    sum += L(idx) * L.get_n_left(idx, dim);
                    sum += L(idx) * L.get_n_right(idx, dim);
                }
            }
            inline void operator() () {}
            inline void finalize() {}
            T result() {return sum;}
        };

        // y u no work?
        //
        //template<typename Lattice>
        //typename Lattice::element sum_prod_nn(Lattice L)
        //{
        //    sum_prod_nn_<typename Lattice::element> sum_prod_nn;
        //    csp::iterate::stencil_iterate<2>(L, sum_prod_nn);
        //    return sum_prod_nn.result();
        //}
    }
}

#endif
