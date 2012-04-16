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

namespace csp
{
    namespace ising
    {
        typedef long long int energy;

        inline void update_exp(double T, boost::array<double, 7>& exp)
        {
            for(int i = 0; i < 7; ++i)
            {
                exp[i] = std::exp(2 * (2 * i - 6) / T);
            }
        }

        inline double cached_exp(int dE, boost::array<double, 7>& exp)
        {
            switch(dE)
            {
                case -12:
                    return exp[0];
                    break;
                case -8:
                    return exp[1];
                    break;
                case -4:
                    return exp[2];
                    break;
                case 0:
                    return exp[3];
                    break;
                case 4:
                    return exp[4];
                    break;
                case 8:
                    return exp[5];
                    break;
                case 12:
                    return exp[6];
                    break;
                default:
                    throw "Impossible dE value in cached_exp";
            }
        }

        template<typename T>
        struct Rng
        {
            virtual T operator() () = 0;
        };

        struct default_rng_ : public Rng<double>
        {
            double operator() () {return drand48();}
        } default_rng;

        struct default_irng_ : public Rng<int>
        {
            int operator() () {return rand();}
        } default_irng;

        template<typename Lattice, typename R = double()>
        struct spin_flip_
        {
            typedef typename Lattice::element element;

            // constructor
            spin_flip_(double& Temp, energy& Etot, energy& Mtot, 
                       boost::array<double, 7>& exp, R& rng = drand48) : 
                rng(rng), Temp(Temp), Etot(Etot), Mtot(Mtot), exp(exp) {}

            // inner loop call
            template<typename IndexList>
            void operator() (Lattice& L, IndexList& idx)
            {
                // get the current spin
                element e = L(idx);

                // get the sum over all directly neighbored spins
                element neighbor_sum = 0;
                for(int dim = 0; dim < idx.size(); ++dim)
                {
                    neighbor_sum += L.get_n_left(idx, dim);
                    neighbor_sum += L.get_n_right(idx, dim);
                }

                // get delta E
                element dE = 2 * e * neighbor_sum;

                // flip or don't flip & update E and M
                if(dE < 0 || rng() < cached_exp(-dE, exp))
                {
                    L(idx) = -e;
                    Etot += dE;
                    Mtot += 2 * L(idx);
                }
            }

            // between loops call
            void operator() () {}

            void finalize() {}
        
            R& rng;
            // variables to update
            double& Temp;
            energy& Etot;
            energy& Mtot;

            // exp cache
            boost::array<double, 7>& exp;
        };

        template<typename Lattice>
        struct get_em_
        {
            typedef typename Lattice::element element;
            typedef std::pair<energy, energy> return_type;

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
            energy Etot_;
            energy Mtot_;
        };

        template<class Lattice, typename R = double()>
        class Ising
        {
            typedef typename Lattice::element element;
            typedef long long int energy;
            typedef typename get_em_<Lattice>::return_type em_type;

            public:
            // constructor
            Ising(Lattice& L, double _Temp = 0., R& _rng = drand48) 
                : L(L), Temp(_Temp), rng(_rng), 
                    spin_flip(Temp, Etot, Mtot, exp, _rng), get_em()
            {
                L.iterate(get_em); 
                em_type res = get_em.result();
                Etot = res.first; Mtot = res.second;
                //std::cout << "T: " << Temp << " Etot: " << Etot << " Mtot: " << Mtot << std::endl;
            }

            void step()
            {
                update_exp(Temp, exp);

                for(int i = 0; i < L.system_size(); ++i)
                {
                    boost::array<int, Lattice::dimension> idx;
                    idx = L.get_random_index();
                    spin_flip(L, idx);
                }
                //L.iterate(spin_flip);
                //std::cout << "T: " << Temp << " Etot: " << Etot << " Mtot: " << Mtot << std::endl;
            }

            boost::multi_array<double, 2> T_run(double T_begin, double T_end, int Tsteps, int nsteps)
            {
                // create result array
                double dT = (T_end - T_begin) / (double)(Tsteps);
                boost::array<int, 2> shape = {Tsteps, 5};
                boost::multi_array<double, 2> results(shape);

                // loop over T
                for(int t = 0; t < Tsteps; ++t)
                {
                    Temp = T_begin + t*dT;
                    double Esum    = 0;
                    double E2sum   = 0;
                    double Msum    = 0;
                    double M2sum   = 0;

                    double N = (double)(nsteps);
                    double S = (double)(L.system_size());

                    // make nsteps sweeps and measure
                    for(int i = 0; i < nsteps; ++i)
                    {
                        step();

                        Esum    += Etot;
                        E2sum   += Etot * Etot;
                        Msum    += Mtot;
                        M2sum   += Mtot * Mtot;

                        //if(i % 10 == 0) std::cout << "T = " << Temp << " Etot2 = " << Etot*Etot << " E2sum = " << E2sum/i << " Esum2 = " << Esum*Esum/i/i << std::endl;
                    }

                    // process data
                    double E_avg = Esum           / N;
                    double M_avg = std::abs(Msum) / N;
                    double Xi    = ((M2sum / N) - (M_avg * M_avg)) / Temp;
                    double Cv    = ((E2sum / N) - (E_avg * E_avg)) / (Temp * Temp);

                    // save results
                    results[t][0] = Temp;
                    results[t][1] = E_avg / S;
                    results[t][2] = M_avg / S;
                    results[t][3] = Xi    / S;
                    results[t][4] = Cv    / S;
                }
                return results;
            }
            
            private:
            Lattice& L;
            double Temp;
            energy Etot;
            energy Mtot;
            spin_flip_<Lattice, R> spin_flip;
            R& rng;
            boost::array<double, 7> exp;

            get_em_<Lattice> get_em;
        };
    };
}

#endif
