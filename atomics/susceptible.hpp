/**
* Griffin Barrett
* ARSLab - Carleton University
*
*/

#ifndef _SUSCEPTIBLE_HPP__
#define _SUSCEPTIBLE_HPP__

#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>

#include <limits>
#include <assert.h>
#include <string>


//All population groups use the same set of port types
//They are defined in this ancillary header
#include "../atomics/population_group_common.hpp"

using namespace cadmium;
using namespace std;

    template<typename TIME>
    class susceptible{
        public:

            double dt; //integrative time
            double c; // Contact rate
            double b; // Probability of transmission per contact
            double q; // Quarantined rate of exposed individuals
            //double e; // Transition rate of exposed individuals to the infected class
            double l; // Rate at which the quarantined uninfected contacts were released into the wider community
            //double n; // Probability of having symptoms among infected individuals
            //double di;// Transition rate of symptomatic infected individuals to the uarantined infected class
            //double dq;// Transition rate of quarantined exposed individuals to the quarantined infected class
            //double yi;// Recovery rate of symptomatic infected individuals
            //double ya;// Recovery rate of asymptomatic infected individuals
            //double yh;// Recovery rate of quarantined infected individuals
            //double a;// Disease-induced death rate

            // default constructor
            susceptible() noexcept{
              assert(false && "default constructor is not available");
            }

            susceptible(double initial_population){
                dt = constants[0]; //integrative time
                c  = constants[1]; // Contact rate
                b  = constants[2]; // Probability of transmission per contact
                q  = constants[3]; // Quarantined rate of exposed individuals
                //e  = constants[4]; // Transition rate of exposed individuals to the infected class
                l  = constants[5]; // Rate at which the quarantined uninfected contacts were released into the wider community
                //n  = constants[6]; // Probability of having symptoms among infected individuals
                //di = constants[7]; // Transition rate of symptomatic infected individuals to the uarantined infected class
                //dq = constants[8]; // Transition rate of quarantined exposed individuals to the quarantined infected class
                //yi = constants[9]; // Recovery rate of symptomatic infected individuals
                //ya = constants[10];// Recovery rate of asymptomatic infected individuals
                //yh = constants[11];// Recovery rate of quarantined infected individuals
                //a  = constants[12];// Disease-induced death rate

                state.population       = initial_population;
                state.population_delta = 0;
                state.report_queued    = true;
            }
            // state definition
            struct state_type{
                double population;
                double population_delta;

                //double S;
                //double E;
                double I;
                double A;
                double Sq;
                //double Eq;
                //double H;

                bool report_queued;
            };

            state_type state;
            // ports definition
            using input_ports=std::tuple<typename
                                                    //population_group_defs::susceptible,
                                                    //population_group_defs::exposed,
                                                    population_group_defs::symptomatic_infective,
                                                    population_group_defs::asymptomatic_infective,
                                                    population_group_defs::quarantined_susceptible//,
                                                    //population_group_defs::quarantined_exposed,
                                                    //population_group_defs::quarantined_infective
                                                >;

            using output_ports=std::tuple<typename  population_group_defs::report>;

            // internal transition
            void internal_transition() {
                state.population = max(0.0, state.population+state.population_delta);
                state.population_delta = 0;
                state.report_queued = false;
            }

            // external transition
            void external_transition(TIME t, typename make_message_bags<input_ports>::type mbs) {
                //for(auto el : get_messages<typename population_group_defs::susceptible>            (mbs)){state.S  = el;}
                //for(auto el : get_messages<typename population_group_defs::exposed>                (mbs)){state.E  = el;}
                for(auto el : get_messages<typename population_group_defs::symptomatic_infective>  (mbs)){state.I  = el;}
                for(auto el : get_messages<typename population_group_defs::asymptomatic_infective> (mbs)){state.A  = el;}
                for(auto el : get_messages<typename population_group_defs::quarantined_susceptible>(mbs)){state.Sq = el;}
                //for(auto el : get_messages<typename population_group_defs::quarantined_exposed>    (mbs)){state.Eq = el;}
                //for(auto el : get_messages<typename population_group_defs::quarantined_infective>  (mbs)){state.H  = el;}

                //susceptible
                state.population_delta = (
                    -S_to_E (c, b, q, state.population, state.I, state.A)
                    -S_to_Eq(c, b, q, state.population, state.I, state.A)
                    -S_to_Sq(c, b, q, state.population, state.I, state.A)
                    +Sq_to_S(l, state.Sq)
                )*dt;

                state.report_queued = true;
            }

            // confluence transition
            void confluence_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {
                internal_transition();
                external_transition(TIME(), std::move(mbs));
            }


            // output function
            typename make_message_bags<output_ports>::type output() const {
                typename make_message_bags<output_ports>::type bags;
                if(state.report_queued == true){
                    get_messages<typename population_group_defs::report>(bags).push_back(max(0.0, state.population+state.population_delta));
                }
                return bags;
            }

            // time_advance function
            TIME time_advance() const {
                if(state.report_queued == true){
                    return TIME(dt);
                }else{
                    return std::numeric_limits<TIME>::infinity();
                }
            }

            friend std::ostringstream& operator<<(std::ostringstream& os, const typename susceptible<TIME>::state_type& i) {
                 os.precision(2);
                os << fixed;
                os << "{\"susceptible\":" << max(0.0, i.population+i.population_delta) << "}";
            return os;
            }
        };


#endif // _SUSCEPTIBLE_HPP__





