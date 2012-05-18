#ifndef CSP_MD_FORCE
#define CSP_MD_FORCE

#include <vector>
#include <string>

namespace csp
{
    namespace md
    {
        class Algorithm
        {
            public:
            typedef std::vector<int> index_list;

            Force(std::string name) : name(name) {}
            Force(Parameter& param, Configuration& conf, Topology& topo, std::string name) 
                : param(param), conf(conf), topo(topo), name(name) {}
            virtual void apply();

            virtual ~Algorithm() {}

            private:
            std::string     name;
            Parameter&      param;
            Configuration&  conf;
            Topology&       topo;
        };
    }
}

#endif
