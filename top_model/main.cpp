
//Time class header
#include <NDTime.hpp>

//C++ libraries
#include <iostream>
#include <string>

//Atomic model headers
#define concept concept_old

//Cadmium Simulator headers
#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/dynamic_model.hpp>
#include <cadmium/modeling/dynamic_coupled.hpp>
#include <cadmium/modeling/dynamic_model_translator.hpp>
#include <cadmium/engine/pdevs_dynamic_runner.hpp>
#include <cadmium/logger/common_loggers.hpp>

//Atomic model for inputs
#include <cadmium/basic_model/pdevs/iestream.hpp>

#undef concept

#include "../atomics/population.hpp"
#include "../atomics/district.hpp"
#include "../atomics/district_model.hpp"
#include "../atomics/params_changer.hpp"


using namespace std;
using namespace cadmium;
using namespace pop;
//using namespace cadmium::basic_models::pdevs;

using TIME = double;

/***** Define input port for coupled models *****/
// none for this test


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



int main(){

	/****** Station Passenger Generator instantiations *******************/
        /* see ../atomics/population.hpp and for a description of what is what. This can also be constructed piecemeal */
    /*
    district sample_d_10{"c=1.0",
        {14.781, 14.781, 0, 0, 2.1011e-8, 0, 2.1011e-8, 0, 1.8887e-7, 1.8887e-7, 1.8887e-7, 1.8887e-7, 0.86834, 0.071428, 0.142857, 0.1259, 0.13266, 0, 0.33029, 0.13978, 0.11624, 0.11624, 1.7826e-5, 0, 1.7826e-5, 1.7826e-5},
        {11081000, 739, 105.1, 1.1642, 27.679, 1, 53.839, 0, 2, 0},
        {}};

    district sample_d_08{"c=0.8",
        {14.781, 14.781, 0, 0, 2.1011e-8, 0, 2.1011e-8, 0, 1.8887e-7, 1.8887e-7, 1.8887e-7, 1.8887e-7, 0.86834, 0.071428, 0.142857, 0.1259, 0.13266, 0, 0.33029, 0.13978, 0.11624, 0.11624, 1.7826e-5, 0, 1.7826e-5, 1.7826e-5},
        {11081000, 739, 105.1, 1.1642, 27.679, 1, 53.839, 0, 2, 0},
        {}};

    district sample_d_05{"c=0.5",
        {14.781, 14.781, 0, 0, 2.1011e-8, 0, 2.1011e-8, 0, 1.8887e-7, 1.8887e-7, 1.8887e-7, 1.8887e-7, 0.86834, 0.071428, 0.142857, 0.1259, 0.13266, 0, 0.33029, 0.13978, 0.11624, 0.11624, 1.7826e-5, 0, 1.7826e-5, 1.7826e-5},
        {11081000, 739, 105.1, 1.1642, 27.679, 1, 53.839, 0, 2, 0},
        {}};

    district sample_d_03{"c=0.3",
        {14.781, 14.781, 0, 0, 2.1011e-8, 0, 2.1011e-8, 0, 1.8887e-7, 1.8887e-7, 1.8887e-7, 1.8887e-7, 0.86834, 0.071428, 0.142857, 0.1259, 0.13266, 0, 0.33029, 0.13978, 0.11624, 0.11624, 1.7826e-5, 0, 1.7826e-5, 1.7826e-5},
        {11081000, 739, 105.1, 1.1642, 27.679, 1, 53.839, 0, 2, 0},
        {}};

    district sample_d_01{"c=0.1",
        {14.781, 14.781, 0, 0, 2.1011e-8, 0, 2.1011e-8, 0, 1.8887e-7, 1.8887e-7, 1.8887e-7, 1.8887e-7, 0.86834, 0.071428, 0.142857, 0.1259, 0.13266, 0, 0.33029, 0.13978, 0.11624, 0.11624, 1.7826e-5, 0, 1.7826e-5, 1.7826e-5},
        {11081000, 739, 105.1, 1.1642, 27.679, 1, 53.839, 0, 2, 0},
        {}};


	shared_ptr<dynamic::modeling::model> rule_changer = dynamic::translate::make_dynamic_atomic_model<params_changer, TIME, vector<tuple<TIME, string, model_params>>>("rule_changer", {
    //  {12, "c=1.0", {14.781*1.0, 14.781*1.0, 0, 0, 2.1011e-8, 0, 2.1011e-8, 0, 1.8887e-7, 1.8887e-7, 1.8887e-7, 1.8887e-7, 0.86834, 0.071428, 0.142857, 0.1259, 0.13266, 0, 0.33029, 0.13978, 0.11624, 0.11624, 1.7826e-5, 0, 1.7826e-5, 1.7826e-5}},
        {12, "c=0.8", {14.781*0.8, 14.781*0.8, 0, 0, 2.1011e-8, 0, 2.1011e-8, 0, 1.8887e-7, 1.8887e-7, 1.8887e-7, 1.8887e-7, 0.86834, 0.071428, 0.142857, 0.1259, 0.13266, 0, 0.33029, 0.13978, 0.11624, 0.11624, 1.7826e-5, 0, 1.7826e-5, 1.7826e-5}},
        {12, "c=0.5", {14.781*0.5, 14.781*0.5, 0, 0, 2.1011e-8, 0, 2.1011e-8, 0, 1.8887e-7, 1.8887e-7, 1.8887e-7, 1.8887e-7, 0.86834, 0.071428, 0.142857, 0.1259, 0.13266, 0, 0.33029, 0.13978, 0.11624, 0.11624, 1.7826e-5, 0, 1.7826e-5, 1.7826e-5}},
        {12, "c=0.3", {14.781*0.3, 14.781*0.3, 0, 0, 2.1011e-8, 0, 2.1011e-8, 0, 1.8887e-7, 1.8887e-7, 1.8887e-7, 1.8887e-7, 0.86834, 0.071428, 0.142857, 0.1259, 0.13266, 0, 0.33029, 0.13978, 0.11624, 0.11624, 1.7826e-5, 0, 1.7826e-5, 1.7826e-5}},
        {12, "c=0.1", {14.781*0.1, 14.781*0.1, 0, 0, 2.1011e-8, 0, 2.1011e-8, 0, 1.8887e-7, 1.8887e-7, 1.8887e-7, 1.8887e-7, 0.86834, 0.071428, 0.142857, 0.1259, 0.13266, 0, 0.33029, 0.13978, 0.11624, 0.11624, 1.7826e-5, 0, 1.7826e-5, 1.7826e-5}}
	    });
    */

        district sample_d_10{"c=1.0",
        {14.781, 2.1011e-8, 1.8887e-7, 0.142857, 0.071428, 0.86834, 0.13266, 0.1259, 0.33029, 0.13978, 0.11624, 1.7826e-5, 0},
        {11081000, 739, 105.1, 1.1642, 27.679, 1, 53.839, 2, 0},
        {}};

    district sample_d_08{"c=0.8",
        {14.781, 2.1011e-8, 1.8887e-7, 0.142857, 0.071428, 0.86834, 0.13266, 0.1259, 0.33029, 0.13978, 0.11624, 1.7826e-5, 0},
        {11081000, 739, 105.1, 1.1642, 27.679, 1, 53.839, 2, 0},
        {}};

    district sample_d_05{"c=0.5",
        {14.781, 2.1011e-8, 1.8887e-7, 0.142857, 0.071428, 0.86834, 0.13266, 0.1259, 0.33029, 0.13978, 0.11624, 1.7826e-5, 0},
        {11081000, 739, 105.1, 1.1642, 27.679, 1, 53.839, 2, 0},
        {}};

    district sample_d_03{"c=0.3",
        {14.781, 2.1011e-8, 1.8887e-7, 0.142857, 0.071428, 0.86834, 0.13266, 0.1259, 0.33029, 0.13978, 0.11624, 1.7826e-5, 0},
        {11081000, 739, 105.1, 1.1642, 27.679, 1, 53.839, 2, 0},
        {}};

    district sample_d_01{"c=0.1",
        {14.781, 2.1011e-8, 1.8887e-7, 0.142857, 0.071428, 0.86834, 0.13266, 0.1259, 0.33029, 0.13978, 0.11624, 1.7826e-5, 0},
        {11081000, 739, 105.1, 1.1642, 27.679, 1, 53.839, 2, 0},
        {}};


	shared_ptr<dynamic::modeling::model> rule_changer = dynamic::translate::make_dynamic_atomic_model<params_changer, TIME, vector<tuple<TIME, string, model_params>>>("rule_changer", {
        {12, "c=1.0", {14.781*1.0, 2.1011e-8, 1.8887e-7, 0.142857, 0.071428, 0.86834, 0.13266, 0.1259, 0.33029, 0.13978, 0.11624, 1.7826e-5, 0}},
        {12, "c=0.8", {14.781*0.8, 2.1011e-8, 1.8887e-7, 0.142857, 0.071428, 0.86834, 0.13266, 0.1259, 0.33029, 0.13978, 0.11624, 1.7826e-5, 0}},
        {12, "c=0.5", {14.781*0.5, 2.1011e-8, 1.8887e-7, 0.142857, 0.071428, 0.86834, 0.13266, 0.1259, 0.33029, 0.13978, 0.11624, 1.7826e-5, 0}},
        {12, "c=0.3", {14.781*0.3, 2.1011e-8, 1.8887e-7, 0.142857, 0.071428, 0.86834, 0.13266, 0.1259, 0.33029, 0.13978, 0.11624, 1.7826e-5, 0}},
        {12, "c=0.1", {14.781*0.1, 2.1011e-8, 1.8887e-7, 0.142857, 0.071428, 0.86834, 0.13266, 0.1259, 0.33029, 0.13978, 0.11624, 1.7826e-5, 0}},
	    });

	shared_ptr<dynamic::modeling::model> c10 = dynamic::translate::make_dynamic_atomic_model<district_model, TIME, district>("c=1.0", std::move(sample_d_10));
	shared_ptr<dynamic::modeling::model> c08 = dynamic::translate::make_dynamic_atomic_model<district_model, TIME, district>("c=0.8", std::move(sample_d_08));
	shared_ptr<dynamic::modeling::model> c05 = dynamic::translate::make_dynamic_atomic_model<district_model, TIME, district>("c=0.5", std::move(sample_d_05));
	shared_ptr<dynamic::modeling::model> c03 = dynamic::translate::make_dynamic_atomic_model<district_model, TIME, district>("c=0.3", std::move(sample_d_03));
	shared_ptr<dynamic::modeling::model> c01 = dynamic::translate::make_dynamic_atomic_model<district_model, TIME, district>("c=0.1", std::move(sample_d_01));

    dynamic::modeling::Ports iports_SEIRD = {};
    dynamic::modeling::Ports oports_SEIRD  = {
            typeid(SEIRD_defs::report)};

    dynamic::modeling::Models submodels_SEIRD  = {
            rule_changer,
            c10, c08, c05, c03, c01};

    dynamic::modeling::EICs eics_SEIRD  = {};
    dynamic::modeling::EOCs eocs_SEIRD  = {
            dynamic::translate::make_EOC<district_ports::report, SEIRD_defs::report>("c=1.0"),
            dynamic::translate::make_EOC<district_ports::report, SEIRD_defs::report>("c=0.8"),
            dynamic::translate::make_EOC<district_ports::report, SEIRD_defs::report>("c=0.5"),
            dynamic::translate::make_EOC<district_ports::report, SEIRD_defs::report>("c=0.3"),
            dynamic::translate::make_EOC<district_ports::report, SEIRD_defs::report>("c=0.1")};

    /* Each model MUST be piped into itself to function */
    dynamic::modeling::ICs ics_SEIRD  = {
            dynamic::translate::make_IC<param_changer_ports::new_params, district_ports::new_params>("rule_changer", "c=1.0"),
            dynamic::translate::make_IC<district_ports::people_out, district_ports::people_in>("c=1.0", "c=1.0"),
            dynamic::translate::make_IC<param_changer_ports::new_params, district_ports::new_params>("rule_changer", "c=0.8"),
            dynamic::translate::make_IC<district_ports::people_out, district_ports::people_in>("c=0.8", "c=0.8"),
            dynamic::translate::make_IC<param_changer_ports::new_params, district_ports::new_params>("rule_changer", "c=0.5"),
            dynamic::translate::make_IC<district_ports::people_out, district_ports::people_in>("c=0.5", "c=0.5"),
            dynamic::translate::make_IC<param_changer_ports::new_params, district_ports::new_params>("rule_changer", "c=0.3"),
            dynamic::translate::make_IC<district_ports::people_out, district_ports::people_in>("c=0.3", "c=0.3"),
            dynamic::translate::make_IC<param_changer_ports::new_params, district_ports::new_params>("rule_changer", "c=0.1"),
            dynamic::translate::make_IC<district_ports::people_out, district_ports::people_in>("c=0.1", "c=0.1"),


            };

    shared_ptr<dynamic::modeling::coupled<TIME>> SEIRD  = make_shared<dynamic::modeling::coupled<TIME>>(
            "SEIRD ", submodels_SEIRD , iports_SEIRD , oports_SEIRD , eics_SEIRD , eocs_SEIRD , ics_SEIRD
        );

    dynamic::engine::runner<TIME, logger_top> r(SEIRD, {0});
    r.run_until(56);
    return 0;
}
