/*
 * Ising.hpp
 * Functions for Ising Simulations using Lattice.hpp
 * Written for CSP FS 2012 (adapted from the ICP HS 2011 solution)
 * by Rico Häuselmann
 */

#ifndef CSP_ISING
#define CSP_ISING

#include "NDLattice.hpp"
#include <cmath>

#define kB 1.

namespace icp
{
    namespace ising
    {
        template<class Lattice, class R>
        void system_sweep(Lattice& L, double T, int& Etot, int& Mtot, R rng)
        {
            for(int y=0; y<L.size(); ++y)
            {
                for(int x=0; x<L.size(); ++x)
                {
                    int dE = 2 * L.get_cell(x, y) * L.sum_neighbors(x, y);

                    if(dE < 0 || rng() < std::exp(-dE / T))
                    {
                        L.flip_cell(x, y);
                        Etot += dE;
                        Mtot += 2 * L.get_cell(x, y);
                    }
                }
            }
        }
    }
}

namespace csp
{
    namespace ising
    {
        template<class Iterator, class R>
        void system_sweep(Iterator begin, Iterator end, double T, int& Etot, int& Mtot, R rng)
        {
            for(Iterator it_z = begin, it_z < end, ++it_z)
            {
                for(Iterator it_y = (*it_z).begin(), it_y < (*it_z).end(), ++it_y)
                {
                }
            }
        }

        template<class Lattice, int D, class R>
        class Ising
        {
            pulic:
            Ising(double T, R rng) : rng(rng) {}
            void system_sweep() 
            {
                csp::iterate<D>
            }
            
            private:
            Lattice L;
            R rng;
            int Etot;
            int Mtot;
        }
    }
}

#endif
