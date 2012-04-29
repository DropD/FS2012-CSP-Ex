#ifndef CSP_MD_PARTICLE
#define CSP_MD_PARTICLE

#include <boost/array.hpp>
#include <iostream>

namespace csp
{
    namespace md
    {
        template<typename T>
        struct point
        {
            point() {}
            point(double x, double y, double z) : x(x), y(y), z(z) {}
            T operator[](int i)
            {
                switch(i)
                {
                    case 0:
                        return x;
                    case 1:
                        return y;
                    case 2:
                        return z;
                }
            }
            T x, y, z;
        };

        template<typename T>
        struct point_arr
        {
            point_arr() {}
            point_arr(double x, double y, double z)
            {
                xyz[0] = x;
                xyz[1] = y;
                xyz[2] = z;
            }
            T operator[](int i) { return xyz[i]; }

            boost::array<T, 3> xyz;
        };

        template<typename T, typename U>
        inline point<U> operator+(T a, point<U> b)
        {
            return point<U>(a + b.x, a + b.y, a + b.z);
        }

        template<typename T>
        inline point<T> operator+(point<T> a, point<T> b)
        {
            return point<T>(a.x + b.x, a.y + b.y, a.z + b.z);
        }

        template<typename T>
        inline point_arr<T> operator+(point_arr<T> a, point_arr<T> b)
        {
            return point_arr<T>(
                    a[0] + b[0],
                    a[1] + b[1],
                    a[2] + b[2]);
        }

        template<typename T>
        inline std::ostream& operator<<(std::ostream& os, point<T> pt)
        {
            os << "point: ( " << pt.x << " | " << pt.y << " | " << pt.z << " )";
            return os;
        }

        template<typename T>
        inline std::ostream& operator<<(std::ostream& os, point_arr<T> pta)
        {
            os << "point: ( " << pta[0] << " | " << pta[1] << " | " << pta[2] << " )";
            return os;
        }

        //template<typename U>
        //inline point<U> operator+<point<U>, point<U> >(point<U> a, point<U> b)
        //{
        //    return point<U>(a.x + b.x, a.y + b.y, a.z + b.z);
        //}

        //template<typename T>
        //inline point<T> operator-(point<T> b)
        //{
        //    return point<T>(-b.x, -b.y, -b.z);
        //}

        //template<typename T, typename U>
        //inline point<U> operator-(T a, point<U> b)
        //{
        //    return a + (-b);
        //}

        //template<typename T, typename U>
        //inline point<U> operator*(T a, point<U> b)
        //{
        //    return point<U>(a * b.x, a + b.y, a + b.z);
        //}

        //template<typename U>
        //inline U operator*<point<U> >(point<U> a, point<U> b)
        //{
        //    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
        //}

        //template<T>
        //std::vector<point<T> > operator+(std::vector<point<T> > a, std::vector<point<T> > b)
        //{
        //    std::vector<point<T> >
        //}
    }
}

#endif
