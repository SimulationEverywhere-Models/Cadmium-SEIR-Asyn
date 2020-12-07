//Cadmium Simulator headers
#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/dynamic_model.hpp>
#include <cadmium/modeling/dynamic_model_translator.hpp>
#include <cadmium/engine/pdevs_dynamic_runner.hpp>
#include <cadmium/logger/common_loggers.hpp>

//Time class header
#include <NDTime.hpp>



#include "../atomics/population_group_common.hpp"
//Atomic model headers
#include "../atomics/susceptible.hpp"
#include "../atomics/exposed.hpp"
#include "../atomics/symptomatic_infective.hpp"
#include "../atomics/asymptomatic_infective.hpp"
#include "../atomics/quarantined_exposed.hpp"
#include "../atomics/quarantined_infective.hpp"
#include "../atomics/quarantined_susceptible.hpp"
#include "../atomics/recovered.hpp"
#include "../atomics/deceased.hpp"

//C++ headers
#include <iostream>
#include <iomanip>
#include <chrono>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
#include <regex>

using namespace std;
using namespace cadmium;


using TIME = double;

/***** Define output ports for coupled model *****/
struct SEIRD_defs{
    struct susceptible            : public out_port<double> { };
    struct exposed                : public out_port<double> { };
    struct symptomatic_infective  : public out_port<double> { };
    struct asymptomatic_infective : public out_port<double> { };
    struct quarantined_susceptible: public out_port<double> { };
    struct quarantined_exposed    : public out_port<double> { };
    struct quarantined_infective  : public out_port<double> { };
    struct recovered              : public out_port<double> { };
    struct deceased               : public out_port<double> { };
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

    double dt = 0.01;

    double c  = -1;// Contact rate
    double b  = -1;// Probability of transmission per contact
    double q  = -1;// Quarantined rate of exposed individuals
    double e  = -1;// Transition rate of exposed individuals to the infected class
    double l  = -1;// Rate at which the quarantined uninfected contacts were released into the wider community
    double n  = -1;// Probability of having symptoms among infected individuals
    double di = -1;// Transition rate of symptomatic infected individuals to the uarantined infected class
    double dq = -1;// Transition rate of quarantined exposed individuals to the quarantined infected class
    double yi = -1;// Recovery rate of symptomatic infected individuals
    double ya = -1;// Recovery rate of asymptomatic infected individuals
    double yh = -1;// Recovery rate of quarantined infected individuals
    double a  = -1;// Disease-induced death rate
    double t  = -1;// Conversion between infectivity without symptoms to infectivity with symptoms.

    double S  = -1;// Initial susceptible population
    double E  = -1;// Initial exposed population
    double I  = -1;// Initial symptomatic infective population
    double A  = -1;// Initial asymptomatic infective population
    double Sq = -1;// Initial quarantined susceptible population
    double Eq = -1;// Initial quarantined exposed population
    double H  = -1;// Initial quarantined infective population
    double R  = -1;// Initial recovered population
    double D  = -1;// Initial deceased population


	ifstream inputReader;

	//default filename if none is provided
	if(argc<2){
		inputReader.open("input_data/input.txt");
	}else{ //branch for providing filename
		//std::string temp = argv[1];
		//std::string filePath = "input_data/" + temp;
		inputReader.open(argv[1]);
	}

	if(! inputReader.is_open()){
	    assert(false && "Please input a file");
	}
	//reading input from file

    const std::regex base_regex("^[\\s]*([^=;\\n\\r]+?)\\s*=\\s*([^;\\n\\r]+?)\\s*(([;\\n\\r].*)|$)");
    std::string line;
    std::smatch base_match;
    while(std::getline(inputReader, line)){
        if(std::regex_match(line, base_match, base_regex)){
            auto key = base_match[1];
            auto value = stod(base_match[2]);
            cout << "Set " << key << " to " << value << "\n";

            if(key == "c" ){c  = value;}else
            if(key == "b" ){b  = value;}else
            if(key == "q" ){q  = value;}else
            if(key == "e" ){e  = value;}else
            if(key == "l" ){l  = value;}else
            if(key == "n" ){n  = value;}else
            if(key == "di"){di = value;}else
            if(key == "dq"){dq = value;}else
            if(key == "yi"){yi = value;}else
            if(key == "ya"){ya = value;}else
            if(key == "yh"){yh = value;}else
            if(key == "a" ){a  = value;}else
            if(key == "t" ){t  = value;}else
            if(key == "S" ){S  = value;}else
            if(key == "E" ){E  = value;}else
            if(key == "I" ){I  = value;}else
            if(key == "A" ){A  = value;}else
            if(key == "Sq"){Sq = value;}else
            if(key == "Eq"){Eq = value;}else
            if(key == "H" ){H  = value;}else
            if(key == "R" ){R  = value;}else
            if(key == "D" ){D  = value;}else{
                cerr << key << " is not a valid key\n";
                abort();
            }
        }
    }

    assert( c  >=0 &&
            b  >=0 && b<=1 &&
            q  >=0 &&
            e  >=0 &&
            l  >=0 &&
            n  >=0 && n<=1 &&
            di >=0 &&
            dq >=0 &&
            yi >=0 &&
            ya >=0 &&
            yh >=0 &&
            a  >=0 &&
            t  >=0 &&
            S  >=0 &&
            E  >=0 &&
            I  >=0 &&
            A  >=0 &&
            Sq >=0 &&
            Eq >=0 &&
            H  >=0 &&
            R  >=0 &&
            D  >=0 && "One or more values were omited or not valid");

    constants[0] = dt;
    constants[1] = c;
    constants[2] = b;
    constants[3] = q;
    constants[4] = e;
    constants[5] = l;
    constants[6] = n;
    constants[7] = di;
    constants[8] = dq;
    constants[9] = yi;
    constants[10] = ya;
    constants[11] = yh;
    constants[12] = a;
    constants[13] = t;

    shared_ptr<dynamic::modeling::model> pop_susceptible             = dynamic::translate::make_dynamic_atomic_model<susceptible, double>            ("susceptible"            , move(S));
    shared_ptr<dynamic::modeling::model> pop_exposed                 = dynamic::translate::make_dynamic_atomic_model<exposed, double>                ("exposed"                , move(E));
    shared_ptr<dynamic::modeling::model> pop_symptomatic_infective   = dynamic::translate::make_dynamic_atomic_model<symptomatic_infective, double>  ("symptomatic_infective"  , move(I));
    shared_ptr<dynamic::modeling::model> pop_asymptomatic_infective  = dynamic::translate::make_dynamic_atomic_model<asymptomatic_infective, double> ("asymptomatic_infective" , move(A));
    shared_ptr<dynamic::modeling::model> pop_quarantined_susceptible = dynamic::translate::make_dynamic_atomic_model<quarantined_susceptible, double>("quarantined_susceptible", move(Sq));
    shared_ptr<dynamic::modeling::model> pop_quarantined_exposed     = dynamic::translate::make_dynamic_atomic_model<quarantined_exposed, double>    ("quarantined_exposed"    , move(Eq));
    shared_ptr<dynamic::modeling::model> pop_quarantined_infective   = dynamic::translate::make_dynamic_atomic_model<quarantined_infective, double>  ("quarantined_infective"  , move(H));
    shared_ptr<dynamic::modeling::model> pop_recovered               = dynamic::translate::make_dynamic_atomic_model<recovered, double>              ("recovered"              , move(R));
    shared_ptr<dynamic::modeling::model> pop_deceased                = dynamic::translate::make_dynamic_atomic_model<deceased, double>               ("deceased"               , move(D));


    dynamic::modeling::Ports iports_SEIRD = {};
    dynamic::modeling::Ports oports_SEIRD  = {
            typeid(SEIRD_defs::susceptible),
            typeid(SEIRD_defs::exposed),
            typeid(SEIRD_defs::symptomatic_infective),
            typeid(SEIRD_defs::asymptomatic_infective),
            typeid(SEIRD_defs::quarantined_exposed),
            typeid(SEIRD_defs::quarantined_infective),
            typeid(SEIRD_defs::quarantined_susceptible),
            typeid(SEIRD_defs::recovered),
            typeid(SEIRD_defs::deceased)};


    dynamic::modeling::Models submodels_SEIRD  = {
            pop_susceptible,
            pop_exposed,
            pop_symptomatic_infective,
            pop_asymptomatic_infective,
            pop_quarantined_susceptible,
            pop_quarantined_exposed,
            pop_quarantined_infective,
            pop_recovered,
            pop_deceased};

    dynamic::modeling::EICs eics_SEIRD  = {};
    dynamic::modeling::EOCs eocs_SEIRD  = {
            dynamic::translate::make_EOC<population_group_defs::report,SEIRD_defs::susceptible>            ("susceptible"),
            dynamic::translate::make_EOC<population_group_defs::report,SEIRD_defs::exposed>                ("exposed"),
            dynamic::translate::make_EOC<population_group_defs::report,SEIRD_defs::symptomatic_infective>  ("symptomatic_infective"),
            dynamic::translate::make_EOC<population_group_defs::report,SEIRD_defs::asymptomatic_infective> ("asymptomatic_infective"),
            dynamic::translate::make_EOC<population_group_defs::report,SEIRD_defs::quarantined_susceptible>("quarantined_susceptible"),
            dynamic::translate::make_EOC<population_group_defs::report,SEIRD_defs::quarantined_exposed>    ("quarantined_exposed"),
            dynamic::translate::make_EOC<population_group_defs::report,SEIRD_defs::quarantined_infective>  ("quarantined_infective"),
            dynamic::translate::make_EOC<population_group_defs::report,SEIRD_defs::recovered>              ("recovered"),
            dynamic::translate::make_EOC<population_group_defs::report,SEIRD_defs::deceased>               ("deceased")};

    dynamic::modeling::ICs ics_SEIRD  = {
            dynamic::translate::make_IC<population_group_defs::report,population_group_defs::susceptible>            ("susceptible", "exposed"),
            dynamic::translate::make_IC<population_group_defs::report,population_group_defs::susceptible>            ("susceptible", "quarantined_susceptible"),
            dynamic::translate::make_IC<population_group_defs::report,population_group_defs::susceptible>            ("susceptible", "quarantined_exposed"),
            dynamic::translate::make_IC<population_group_defs::report,population_group_defs::exposed>                ("exposed", "symptomatic_infective"),
            dynamic::translate::make_IC<population_group_defs::report,population_group_defs::exposed>                ("exposed", "asymptomatic_infective"),
            dynamic::translate::make_IC<population_group_defs::report,population_group_defs::symptomatic_infective>  ("symptomatic_infective", "susceptible"),
            dynamic::translate::make_IC<population_group_defs::report,population_group_defs::symptomatic_infective>  ("symptomatic_infective", "exposed"),
            dynamic::translate::make_IC<population_group_defs::report,population_group_defs::symptomatic_infective>  ("symptomatic_infective", "quarantined_susceptible"),
            dynamic::translate::make_IC<population_group_defs::report,population_group_defs::symptomatic_infective>  ("symptomatic_infective", "quarantined_exposed"),
            dynamic::translate::make_IC<population_group_defs::report,population_group_defs::symptomatic_infective>  ("symptomatic_infective", "quarantined_infective"),
            dynamic::translate::make_IC<population_group_defs::report,population_group_defs::symptomatic_infective>  ("symptomatic_infective", "recovered"),
            dynamic::translate::make_IC<population_group_defs::report,population_group_defs::symptomatic_infective>  ("symptomatic_infective", "deceased"),
            dynamic::translate::make_IC<population_group_defs::report,population_group_defs::asymptomatic_infective> ("asymptomatic_infective", "susceptible"),
            dynamic::translate::make_IC<population_group_defs::report,population_group_defs::asymptomatic_infective> ("asymptomatic_infective", "exposed"),
            dynamic::translate::make_IC<population_group_defs::report,population_group_defs::asymptomatic_infective> ("asymptomatic_infective", "quarantined_susceptible"),
            dynamic::translate::make_IC<population_group_defs::report,population_group_defs::asymptomatic_infective> ("asymptomatic_infective", "quarantined_exposed"),
            dynamic::translate::make_IC<population_group_defs::report,population_group_defs::asymptomatic_infective> ("asymptomatic_infective", "recovered"),
            dynamic::translate::make_IC<population_group_defs::report,population_group_defs::quarantined_susceptible>("quarantined_susceptible", "susceptible"),
            dynamic::translate::make_IC<population_group_defs::report,population_group_defs::quarantined_exposed>    ("quarantined_exposed", "quarantined_infective"),
            dynamic::translate::make_IC<population_group_defs::report,population_group_defs::quarantined_infective>  ("quarantined_infective", "recovered"),
            dynamic::translate::make_IC<population_group_defs::report,population_group_defs::quarantined_infective>  ("quarantined_infective", "deceased")};
    shared_ptr<dynamic::modeling::coupled<TIME>> SEIRD  = make_shared<dynamic::modeling::coupled<TIME>>(
            "SEIRD ", submodels_SEIRD , iports_SEIRD , oports_SEIRD , eics_SEIRD , eocs_SEIRD , ics_SEIRD
        );


    dynamic::engine::runner<double, logger_top> r(SEIRD, {0});
    r.run_until(365);
    return 0;
}


















