/*
 * N Dimensional iterations
 * Written for CSP FS 2012
 * by Rico Häuselmann
 * 07. 03. 2012
 */

#ifndef CSP_ND_ITERATION
#define CSP_ND_ITERATION

namespace csp
{
    namespace iterate
    {
        template<int D>
        struct stencil_iterate
        {
            template<typename Lattice, typename F>
            inline stencil_iterate(Lattice& L,  F& fcn)
            {
                boost::array<int, D> idx;
                for(int i = 0; i < L.size(); ++i)
                {
                    idx[0] = i;
                    stencil_iterate<D-1>(L, idx, fcn);
                }
                fcn();
                fcn.finalize();
            }
            template<typename Lattice, typename IndexList, typename F>
            inline stencil_iterate(Lattice& L,  IndexList& idx, F& fcn)
            {
                int dim = idx.size() - D;
                for(int i = 0; i < L.shape()[dim]; ++i)
                {
                    idx[dim] = i;
                    stencil_iterate<D-1>(L, idx, fcn);
                }
                fcn();
            }
        };

        template<>
        struct stencil_iterate<1>
        {
            template<typename Lattice, typename F>
            inline stencil_iterate(Lattice& L, F& fcn)
            {
                for(int i = 0; i < L.size(); ++i)
                {
                    fcn(L[i]);
                }
                fcn();
            }
            template<typename Lattice, typename IndexList, typename F>
            inline stencil_iterate(Lattice& L, IndexList& idx, F& fcn)
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

        template<int D>
        struct old
        {
            template<typename Iterator, typename F>
            inline old(Iterator begin, Iterator end, F& fcn)
            {
                for(Iterator it = begin; it < end; ++it)
                {
                    old<D-1>((*it).begin(), (*it).end(), fcn);
                }
                fcn();
            }
        };

        template<>
        struct old<1>
        {
            template<typename Iterator, typename F>
            inline old(Iterator begin, Iterator end, F& fcn)
            {
                for(Iterator it = begin; it < end; ++it)
                {
                    fcn(*it);
                }
                fcn();
            }
        };
    }
}

#endif
