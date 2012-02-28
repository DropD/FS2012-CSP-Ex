/*
 * Lattice.hpp
 * Lattice class for icp exercises
 * by Rico Häuselmann
 */

#ifndef ICP_LATTICE
#define ICP_LATTICE

#include <vector>
#include <iostream>
#include <iomanip>
#include <assert.h>
#include <exception>
#include <algorithm>

namespace icp
{
    class oob_exception : public std::exception
    {
        virtual const char* what() const throw()
        {
            return "index out of bounds.";
        }
    } out_of_bounds;

    struct test_element
    {
        private:
        int k;

        public:
        test_element(int k) : k(k) { }
        bool operator() (const int &val)
        {
            return (val == k);
        }
    };

    class Lattice
    {
        protected:
        std::vector<std::vector<int> > _lattice;
        int L;

        public:
        Lattice() {}
        Lattice(int L) : L(L) 
        {
            _lattice = std::vector<std::vector<int> >(L+2, std::vector<int>(L+2, 0));
        }
        Lattice(int L, int init) : L(L) 
        {
            _lattice = std::vector<std::vector<int> >(L+2, std::vector<int>(L+2, init));
        }

        std::ostream& print(std::ostream& os)
        {
            os << std::endl;
            for(int i=0; i<L; ++i)
            {
                for(int j=0; j<L; ++j)
                {
                    int cv = get_cell(j, i);
                    os << std::setw(4);
                    if(cv) os << cv;
                    else os << " ";
                }
                os << std::endl;
            }
            return os;
        }

        std::ostream& print_all(std::ostream& os)
        {
            os << std::endl;
            for(int i=0; i<L; ++i)
            {
                for(int j=0; j<L; ++j)
                {
                    int cv = get_cell(j, i);
                    os << std::setw(4) << cv;
                }
                os << std::endl;
            }
            return os;
        }

        std::ostream& print_csv(std::ostream& os)
        {
            os << std::endl;
            for(int i=0; i<L; ++i)
            {
                for(int j=0; j<L; ++j)
                {
                    int cv = get_cell(j, i);
                    os << cv << " ";
                }
                os << std::endl;
            }
            return os;
        }

        template<class R>
        void fill(double p, R rng)
        {
            #pragma omp parallel for
            for(int i=0; i<L; ++i)
            {
                for(int j=0; j<L; ++j)
                {
                    if(rng() < p) set_cell(j, i, 1);
                    else set_cell(j, i, 0);
                }
            }
        }

        template<class F>
        void apply_down_right(F fkt)
        {
            for(int i=0; i<L; ++i)
            {
                for(int j=0; j<L; ++j)
                {
                    set_cell(j, i, fkt(get_cell(j, i)));
                }
            }
        }

        bool percolates(int k)
        {
            //std::cout << "percolates(" << k << ")" << std::endl;
            //test_element is_k(k);
            bool first_row = 0; //= std::any_of(_lattice[1].begin(), _lattice[1].end(), is_k);
            bool last_row = 0;  //= std::any_of(_lattice[L].begin(), _lattice[L].end(), is_k);

            int i = 0;
            while((first_row == 0 || last_row == 0) && i < L)
            {
                if(get_cell(i, 0) == k)   first_row = 1;
                if(get_cell(i, L-1) == k) last_row  = 1;
                ++i;
            }

            return (first_row && last_row);
        }

        int count_in_center_region(int R, int k)
        {
            int xy_start = (L - R) / 2.;
            int xy_stop = xy_start + R;
            int count = 0;
            for(int y = xy_start; y < xy_stop; ++y)
            {
                for(int x = xy_start; x < xy_stop; ++x)
                {
                    if(get_cell(x, y) == k) ++count;
                }
            }
            return count;
        }

        bool any_neighbor(int x, int y)
        {
            return get_top(x, y) > 0 || get_right(x, y) > 0 || get_left(x, y) > 0 || get_bottom(x, y) > 0;
        }

        int sum_neighbors(int x, int y)
        {
            //std::cout << "sn " << get_top(x, y) << " " << get_right(x,y) << " " << get_left(x,y) << " " << get_bottom(x,y) << std::endl;
            return get_top(x, y) + get_right(x, y) + get_left(x, y) + get_bottom(x, y);
        }

        virtual inline void set_cell(int x, int y, int value) 
        { 
            if(x < 0 || x >= L|| y < 0 || y >= L) throw out_of_bounds;

            _lattice[y+1][x+1] = value; 
        }

        inline void flip_cell (int x, int y) 
        { 
            set_cell(x, y, - get_cell(x, y));
        }

        virtual inline int get_cell(int x, int y) 
        {
            if(x < 0 || x >= L|| y < 0 || y >= L) throw out_of_bounds;

            return _lattice[y+1][x+1]; 
        }

        virtual inline int get_top(int x, int y)
        {
            if(x < 0 || x >= L|| y < 0 || y >= L) throw out_of_bounds;

            return _lattice[y][x+1]; 
        }

        virtual inline int get_left(int x, int y)
        {
            if(x < 0 || x >= L|| y < 0 || y >= L) throw out_of_bounds;

            return _lattice[y+1][x]; 
        }

        virtual inline int get_right(int x, int y)
        {
            if(x < 0 || x >= L|| y < 0 || y >= L) throw out_of_bounds;

            return _lattice[y+1][x+2]; 
        }

        virtual inline int get_bottom(int x, int y)
        {
            if(x < 0 || x >= L|| y < 0 || y >= L) throw out_of_bounds;

            return _lattice[y+2][x+1]; 
        }

        int size() {return L;}
    };

    class PeriodicLattice : public Lattice
    {
        protected:
        //std::vector<std::vector<int> > _lattice;
        //int L;

        public:
        PeriodicLattice(int L)
        {
            this->L = L;
            _lattice = std::vector<std::vector<int> >(L, std::vector<int>(L, 0));
        }
        PeriodicLattice(int L, int init)
        {
            this->L = L;
            _lattice = std::vector<std::vector<int> >(L, std::vector<int>(L, init));
        }

        inline void set_cell(int x, int y, int value) 
        { 
            if(x < 0 || x >= L || y < 0 || y >= L) throw out_of_bounds;

            _lattice[y][x] = value; 
        }

        inline int get_cell(int x, int y) 
        {
            if(x < 0 || x >= L|| y < 0 || y >= L) throw out_of_bounds;

            return _lattice[y][x]; 
        }

        inline int get_top(int x, int y)
        {
            if(x < 0 || x >= L|| y < 0 || y >= L) throw out_of_bounds;

            if(y == 0) return _lattice[L-1][x];
            else return _lattice[y-1][x]; 
        }

        inline int get_left(int x, int y)
        {
            if(x < 0 || x >= L|| y < 0 || y >= L) throw out_of_bounds;

            if(x == 0) return _lattice[y][L-1];
            else return _lattice[y][x-1]; 
        }

        inline int get_right(int x, int y)
        {
            if(x < 0 || x >= L|| y < 0 || y >= L) throw out_of_bounds;

            if(x == L-1) return _lattice[y][0];
            else return _lattice[y][x+1]; 
        }

        inline int get_bottom(int x, int y)
        {
            if(x < 0 || x >= L|| y < 0 || y >= L) throw out_of_bounds;

            if(y == L-1) return _lattice[0][x];
            else return _lattice[y+1][x]; 
        }
    };
}

#endif
