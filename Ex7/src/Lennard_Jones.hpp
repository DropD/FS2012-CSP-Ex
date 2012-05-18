#define CSP_MD_LENNARD_JONES

#include "Algorithm.hpp"

#include <vector>

namespace csp
{
    namespace md
    {
        class Lennard_Jones_Algo : public Algorithm
        {
            Lennard_Jones_Algo() : Algorithm("lennard_jones") {}
            Lennard_Jones_Algo(Parameter& param, Configuration& conf, Topology& topo) 
                : Algorithm(param, conf, topo, "lennard_jones") {}
            virtual void apply()
            {
            }
        };

        class Lennard_Jones_Reader : public Reader
        {
            Lennard_Jones_Reader() : Reader("lennard_jones") {}
            Lennard_Jones_Reader(Parameter& param, Configuration& conf, Topology &topo) 
                : Reader(param, conf, topo, "lennard_jones") {}
            virtual void read(std::istream& input)
            {
            }
        };
    }
}

#endif
