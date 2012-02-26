/*
 * Ising.hpp
 * Functions for Ising Simulations using Lattice.hpp
 * by Rico Häuselmann
 */

#ifndef ICP_ISING
#define ICP_ISING

#include "Lattice.hpp"
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

#endif
