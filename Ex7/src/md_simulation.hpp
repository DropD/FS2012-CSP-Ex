#ifndef CSP_MD_SIMULATION
#define CSP_MD_SIMULATION

#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
#include <iostream>

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

            void read_parameters(std::istream& input);
            void read_configuration(std::istream& input);
            void read_topology(std::istream& input);

            void add_algorithm(boost::shared_ptr<Algorithm> a);
            void add_reader(boost::shared_ptr<Reader> r);
            //set_integrator(Integrator i);
            //set_neighbors(Neighbors n);

            //calculate_forces();
            //calculate_pos_vel();

            void step();
            void run();

            private:
            Parameter           param;
            Configuration       conf;
            Topology            topo;

            std::vector<boost::shared_ptr<Algorithm> >  algorithms;
            std::vector<boost::shared_ptr<Reader> >     readers;
        };

        void Simulation::step()
        {
            using boost::lambda::_1;
            std::for_each(algorithms.begin(), algorithms.end(), boost::bind(&Algorithm::apply, _1));
        }

        void Simulation::run()
        {
            for(double t = param.sys.start_time, t < param.sys.end_time, t += param.sys.time_step)
            {
                step();
                conf.sys.time = t;
                ++conf.sys.step_num;
            }
        }

        void Simulation::add_algorithm(boost::shared_ptr<Algorithm> a)
        {
            algorithms.push_back(a);
        }

        void Simulation::add_reader(boost::shared_ptr<Reader> r)
        {
            readers.push_back(r);
        }

        void Simulation::read_parameters(std::istream& input)
        {
            std::for_each(readers.begin(), readers.end(), boost::bind(&Reader::read_p, _1, input));
        }

        void Simulation::read_configuration(std::istream& input)
        {
            std::for_each(readers.begin(), readers.end(), boost::bind(&Reader::read_c, _1, input));
        }

        void Simulation::read_topology(std::istream& input)
        {
            std::for_each(readers.begin(), readers.end(), boost::bind(&Reader::read_t, _1, input));
        }

    }
}

#endif
