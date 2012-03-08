/*
 * Ising.hpp
 * Functions for Ising Simulations using Lattice.hpp
 * Written for CSP FS 2012 (adapted from the ICP HS 2011 solution)
 * by Rico Häuselmann
 */

#ifndef CSP_ISING
#define CSP_ISING

#include "../Common/NDLattice.hpp"
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
        //template<class Iterator, class R>
        //void system_sweep(Iterator begin, Iterator end, double T, int& Etot, int& Mtot, R rng)
        //{
        //    for(Iterator it_z = begin, it_z < end, ++it_z)
        //    {
        //        for(Iterator it_y = (*it_z).begin(), it_y < (*it_z).end(), ++it_y)
        //        {
        //        }
        //    }
        //}

        template<typename T>
        struct Rng
        {
            virtual T operator() () = 0;
        };

        struct default_rng_ : public Rng<double>
        {
            double operator() () {return drand48();}
        } default_rng;

        template<typename Lattice, typename R = Rng<double> >
        struct system_sweep_
        {
            typedef typename Lattice::element_type element_type;

            // constructor
            system_sweep_(double& Temp, element_type& Etot, element_type& Mtot, R& rng = default_rng) : 
                rng(rng), Temp(Temp), Etot(Etot), Mtot(Mtot) {}

            // inner loop call
            template<typename IndexList>
            void operator() (Lattice& L, IndexList& idx)
            {
                // get the current spin
                element_type element = L(idx);

                // get the sum over all directly neighbored spins
                element_type neighbor_sum = 0;
                for(int dim = 0; dim < idx.size(); ++dim)
                {
                    neighbor_sum += L.get_n_left(idx, dim);
                    neighbor_sum += L.get_n_right(idx, dim);
                }

                // get delta E
                element_type dE = 2 * element * neighbor_sum;

                // flip or don't flip & update E and M
                if(dE < 0 || rng() < std::exp(-dE / Temp))
                {
                    L(idx) = -element;
                    Etot += dE;
                    Mtot += 2 * L(idx);
                }
            }

            // between loops call
            void operator() () {}

            R& rng;
            // variables to update
            double& Temp;
            element_type& Etot;
            element_type& Mtot;
        };

        template<typename Lattice>
        struct get_em_
        {
            typedef typename Lattice::element_type element_type;
            typedef std::pair<element_type, element_type> return_type;

            // constructor
            get_em_() : Etot_(0), Mtot_(0) {}

            // inner loop call
            template<typename IndexList>
            void operator() (Lattice& L, IndexList& idx)
            {
                for(int dim = 0; dim < idx.size(); ++dim)
                {
                    Etot_ += L(idx) * L.get_n_left(idx, dim);
                    Etot_ += L(idx) * L.get_n_right(idx, dim);
                }
                Mtot_ += L(idx);
            }

            //between loops call
            void operator() () {}

            //finalize
            void finalize()
            {
                Etot_ = - Etot_;
            }

            return_type result() 
            {
                return return_type(Etot_, Mtot_);
            }

            // variables to update
            element_type Etot_;
            element_type Mtot_;
        };

        template<class Lattice, class R = Rng<double> >
        class Ising
        {
            typedef typename Lattice::element_type element_type;
            typedef typename get_em_<Lattice>::return_type em_type;

            public:
            Ising(Lattice& L, double Temp, R& rng = default_rng) 
                : L(L), Temp(Temp), rng(rng), system_sweep(Temp, Etot, Mtot, rng),
                    get_em()
            {
                iterate::stencil_iterate<Lattice::dimension>(L, get_em); 
                em_type res = get_em.result();
                Etot = res.first; Mtot = res.second;
                std::cout << "Etot: " << Etot << " Mtot: " << Mtot << std::endl;
            }

            void step()
            {
            }
            
            private:
            Lattice& L;
            double Temp;
            element_type Etot;
            element_type Mtot;
            system_sweep_<Lattice, R> system_sweep;
            R& rng;

            get_em_<Lattice> get_em;
        };
    };
}

#endif
