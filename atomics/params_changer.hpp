
#ifdef _PARAMS_CHANGER__HPP
#define _PARAMS_CHANGER__HPP

#include "../atomics/population.hpp"


#define concept concept_old
#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>
#undef concept


using namespace cadmium;
using namespace std;
using namespace pop;

//struct district{
//    int n;
//    int k;
//};

//Port definition
struct param_changer_ports{
    struct new_params      : public out_port<pair<string, model_params>> { };
};

#endif /* _PARAMS_CHANGER__HPP */
