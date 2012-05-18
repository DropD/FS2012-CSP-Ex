#ifndef CSP_MD_READER
#define CSP_MD_READER

#include "Parameter.hpp"

#include <iostream>
#include <string>

namespace csp
{
    namespace md
    {
        class Reader
        {
            public:
            typedef Parameter result_type;

            Reader (std::string name) : name(name) {}
            Reader (Parameter& param, Configuration& conf, Topology& topo, std::string name)
                : param(param), conf(conf), topo(topo), name(name) {}
            virtual void read(std::istream& input);

            virtual ~Reader() {}

            private:
            std::string     name;
            Parameter&      param;
            Configuration&  conf;
            Topology&       topo;
        };
    }
}

#endif
