
#include "../atomics/population.hpp"
#include "../atomics/district.hpp"
#include "../atomics/district_model.hpp"


//C++ headers
#include <iostream>
#include <iomanip>
#include <chrono>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
#include <regex>


#define concept concept_old
//Cadmium Simulator headers
#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/dynamic_model.hpp>
#include <cadmium/modeling/dynamic_model_translator.hpp>
#include <cadmium/engine/pdevs_dynamic_runner.hpp>
#include <cadmium/logger/common_loggers.hpp>

#undef concept

//Time class header
#include <NDTime.hpp>

using namespace std;
using namespace cadmium;
using namespace pop;


using TIME = double;

/***** Define output ports for coupled model *****/
struct SEIRD_defs{
    struct report : public out_port<population> { };
};

/*************** Loggers *******************/
    static ofstream out_messages("output_messages.txt");
    struct oss_sink_messages{
        static ostream& sink(){
            return out_messages;
        }
    };
    static ofstream out_state("output_state.txt");
    struct oss_sink_state{
        static ostream& sink(){
            return out_state;
        }
    };

    using state=logger::logger<logger::logger_state, dynamic::logger::formatter<TIME>, oss_sink_state>;
    using log_messages=logger::logger<logger::logger_messages, dynamic::logger::formatter<TIME>, oss_sink_messages>;
    using global_time_mes=logger::logger<logger::logger_global_time, dynamic::logger::formatter<TIME>, oss_sink_messages>;
    using global_time_sta=logger::logger<logger::logger_global_time, dynamic::logger::formatter<TIME>, oss_sink_state>;

    using logger_top=logger::multilogger<state, log_messages, global_time_mes, global_time_sta>;
/******************************************************/


int main(int argc, char ** argv) {

    /* see ../atomics/population.hpp for a description og what is what. This can also be constructed piecemeal */
    pop::district sample{"sample",
        {14.781, 14.781, 0, 0, 2.1011e-8, 0, 2.1011e-8, 0, 1.8887e-7, 1.8887e-7, 1.8887e-7, 1.8887e-7, 0.86834, 0.071428, 0.142857, 0.1259, 0.13266, 0, 0.33029, 0.13978, 0.11624, 0.11624, 1.7826e-5, 0, 1.7826e-5, 1.7826e-5},
        {11081000, 739, 105.1, 1.1642, 27.679, 1, 53.839, 0, 2, 0},
        {}};

    shared_ptr<dynamic::modeling::model> pop_sample = dynamic::translate::make_dynamic_atomic_model<district_model, TIME, pop::district>("sample", sample);

    dynamic::modeling::Ports iports_SEIRD = {};
    dynamic::modeling::Ports oports_SEIRD  = {
            typeid(SEIRD_defs::report)};

    dynamic::modeling::Models submodels_SEIRD  = {
            pop_sample};

    dynamic::modeling::EICs eics_SEIRD  = {};
    dynamic::modeling::EOCs eocs_SEIRD  = {
            dynamic::translate::make_EOC<district_ports::report, SEIRD_defs::report>("sample")};

    /* Each model MUST be piped into itself to function */
    dynamic::modeling::ICs ics_SEIRD  = {
            dynamic::translate::make_IC<district_ports::people_out, district_ports::people_in>("sample", "sample")};

    shared_ptr<dynamic::modeling::coupled<TIME>> SEIRD  = make_shared<dynamic::modeling::coupled<TIME>>(
            "SEIRD ", submodels_SEIRD , iports_SEIRD , oports_SEIRD , eics_SEIRD , eocs_SEIRD , ics_SEIRD
        );

    dynamic::engine::runner<double, logger_top> r(SEIRD, {0});
    r.run_until(365);
    return 0;
}


















