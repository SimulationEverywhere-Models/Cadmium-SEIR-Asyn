

#ifndef _POPULATION_GROUP_COMMON_HPP__
#define _POPULATION_GROUP_COMMON_HPP__

#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>

double constants[13] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

using namespace cadmium;
using namespace std;

    struct population_group_defs{
        struct report                 : public out_port<double> { };
        struct susceptible            : public in_port<double> { };
        struct exposed                : public in_port<double> { };
        struct symptomatic_infective  : public in_port<double> { };
        struct asymptomatic_infective : public in_port<double> { };
        struct quarantined_susceptible: public in_port<double> { };
        struct quarantined_exposed    : public in_port<double> { };
        struct quarantined_infective  : public in_port<double> { };
    };



#endif //_POPULATION_GROUP_COMMON_HPP__
