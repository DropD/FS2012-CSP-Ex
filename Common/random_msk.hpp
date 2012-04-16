// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    06.03.2012 18:18:08 CET
// File:    random_msk.hpp

#ifndef __RANDOM_MSK_HEADER
#define __RANDOM_MSK_HEADER




/* How to use the random_msk.hpp
 * 
 * lagged fibonacci raw (without variate_generator) only for doubles
 * 
 * random_class<double, fibonacci> my_rng(start, end) gives a double rng
 * random_class<int, fibonacci> my_rng(start, end) gives a double rng
 * 
 * also ...(end) and ...() implemented
 * 
 * .scale for setting the scale
 * 
 * For convience also implemented
 * random_class<double, fibonacci> my_rng(end) ==> my_rng(0, end)
 * random_class<double, fibonacci> my_rng() ==> my_rng(0, 1)
 * 
 * one can generate a random number generator with a boost::mt19937 via the 
 * variate_generator
 * 
 * random_class<int> my_rng(start, end) gives a int rng
 * 
 * random_class<double> my_rng(start, end) gives a double rng
 * 
 * For convience also implemented
 * random_class<T> my_rng(end) ==> my_rng(0, end)
 * random_class<T> my_rng() ==> my_rng(0, 1)
 * 
 * ALTERNATIVE
 * 
 * for the faster (up to 400%) random number generators rand and rand48 one can write
 * 
 * random_class<int, fast> my_rng; //gives a very fast rng for ints 0 or 1 (rand&1)
 * random_class<int, fast> my_rng(end); //gives a rng, that need a % operation for ints from 0 to end (takes the double time of the 0,1 rng)
 * random_class<int, fast> my_rng(start, end); //equal as above (slow bc % operation, but faster the mt19937)
 * 
 * the same holds for doubles
 * 
 * random_class<double, fast>my_rng; //just a drand48-wrap (no speed-difference with -O2)
 * random_class<double, fast>my_rng(end); //an additional multiplication is needed 
 * random_class<double, fast>my_rng(start, end); //an additional addition and multiplication is needed
 */

#include <boost/random.hpp>
#include <boost/utility.hpp>
#include <boost/type_traits.hpp>
#include <boost/mpl/if.hpp>

#include "time.h"

//+-------------------------------------------------+
//|                 flags for choose                |
//+-------------------------------------------------+
struct fast;
struct mersenne;
struct fibonacci;

/// +---------------------------------------------------+
/// |                   mersenne twisters               |
/// +---------------------------------------------------+
class int_random_class
{
public:
    typedef boost::mt19937 engine_type;
    
    typedef boost::uniform_int<int> int_dist_type;
    typedef boost::variate_generator<boost::mt19937&,int_dist_type> dice_type;
    
    int_random_class(int const & start, int const & end):
            eng(time(NULL)), dice(eng, int_dist_type(start, end))
    {
    }
    inline int operator()()
    {
        return dice();
    }
private:
    engine_type eng;
    dice_type dice;
};

class double_random_class
{
public:
    typedef boost::mt19937 engine_type;
    
    typedef boost::uniform_real<double> dist_type;
    typedef boost::variate_generator<boost::mt19937&,dist_type> rng_type;
    
    double_random_class(double const & start, double const & end):
            eng(time(NULL)), rng(eng, dist_type(start, end))
    {
    }
    inline double operator()()
    {
        return rng();
    }
private:
    engine_type eng;
    rng_type rng;
};


template<typename T, typename V = mersenne>
class random_class;

template<typename T>
class random_class<int, T>: public int_random_class
{
    public:
        random_class(int const & start, int const & end): int_random_class(start, end) {}
        random_class(int const & end): int_random_class(0, end) {}
        random_class(): int_random_class(0, 1) {}
};

template<typename T>
class random_class<double, T>: public double_random_class
{
    public:
        random_class(double const & start, double const & end): double_random_class(start, end) {}
        random_class(double const & end): double_random_class(0, end) {}
        random_class(): double_random_class(0, 1) {}
};

/// +---------------------------------------------------+
/// |                   lagged fibonacci                |
/// +---------------------------------------------------+
template<>
class random_class<double, fibonacci>
{
    public:
        random_class(): shift(0)
        {
            rng.seed(time(NULL));
        }
        random_class(double const & end): scale(end), offset(0), shift(1)
        {
            rng.seed(time(NULL));
        }
        random_class(double const & start, double const & end): scale(end-start), offset(start), shift(2)
        {
            rng.seed(time(NULL));
        }
        inline double operator()() 
        {
            if (shift == 0)
            {
                return rng();
            }
            if (shift == 1)
            {
                return scale*rng();
            }
            return offset + scale*rng();
        }
        void set_scale(double const & scaleIn)
        {
            scale = scaleIn;
        }
    private:
        boost::lagged_fibonacci44497 rng;
        double scale;
        double offset;
        const short int shift;  //shows, what operations are needed (for speedup. Tested!)
};


template<>
class random_class<int, fibonacci>
{
    public:
        random_class(): shift(0)
        {
            rng.seed(time(NULL));
        }
        random_class(int const & end): scale(end), offset(0), shift(1)
        {
            rng.seed(time(NULL));
        }
        random_class(int const & start, int const & end): scale(end-start), offset(start), shift(2)
        {
            rng.seed(time(NULL));
        }
        inline int operator()() 
        {
            if (shift == 0)
            {
                return rng()*2;
            }
            if (shift == 1)
            {
                return scale*rng();
            }
            return offset + scale*rng();
        }
        void set_scale(int const & scaleIn)
        {
            scale = scaleIn;
        }
    private:
        boost::lagged_fibonacci44497 rng;
        int scale;
        int offset;
        const short int shift;  //shows, what operations are needed (for speedup. Tested!)
};
/// +---------------------------------------------------+
/// |                   c-libraries                     |
/// +---------------------------------------------------+
template<>
class random_class<double, fast>
{
    public:
        random_class(): shift(0)
        {
            srand48(time(NULL));
        }
        random_class(double const & end): scale(end), offset(0), shift(1)
        {
            srand48(time(NULL));
        }
        random_class(double const & start, double const & end): scale(end-start), offset(start), shift(2)
        {
            srand48(time(NULL));
        }
        inline double operator()() 
        {
            if (shift == 0)
            {
                return drand48();
            }
            if (shift == 1)
            {
                return scale*drand48();
            }
            return offset + scale*drand48();
        }
    
    private:
        double scale;
        double offset;
        const short int shift;  //shows, what operations are needed (for speedup. Tested!)
};

template<>
class random_class<int, fast>
{
    public:
        random_class(): mod(2), shift(0)
        {
            srand(time(NULL));
        }
        random_class(int const & end): mod(end+1), offset(0), shift(1)
        {
            srand(time(NULL));
        }
        random_class(int const & start, int const & end): mod(end-start+1), offset(start), shift(2)
        {
            srand(time(NULL));
        }
        inline double operator()() 
        {
            if (shift == 0)
            {
                return rand()&1;
            }
            if (shift == 1)
            {
                return rand()%mod;
            }
            return offset + rand()%mod;
        }
    
    private:
        int mod;
        int offset;
        const short int shift;  //shows, what operations are needed (for speedup. Tested!)
};
#endif //__RANDOM_MSK_HEADER
