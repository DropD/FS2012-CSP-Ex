#ifndef CSP_MD_SIMULATION
#define CSP_MD_SIMULATION

namespace csp
{
    namespace md
    {
        //template<class Force, class Integrator, class Neighbors>
        class Simulation
        {
            public:
            typedef std::vector<int> index_list;
            typedef boost::array<double> point;

            Simulation() {}

            read_parameters(std::istream& input);
            read_configuration(std::istream& input);
            read_topology(std::istream& input);

            add_force(Force f);
            set_integrator(Integrator i);
            set_neighbors(Neighbors n);

            calculate_forces();
            calculate_pos_vel();

            step();
            run();

            private:
            Parameter           param;
            Configuration       conf;
            Topology            topo;

            std::vector<Force>  forces;
            Integrator          integrator;
            Neighbors           neighbors;
        };
    }
}

#endif
