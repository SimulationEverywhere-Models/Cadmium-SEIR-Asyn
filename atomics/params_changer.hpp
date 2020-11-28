
#ifndef _PARAMS_CHANGER__HPP
#define _PARAMS_CHANGER__HPP

#include "../atomics/population.hpp"


#define concept concept_old
#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>
#undef concept


using namespace cadmium;
using namespace std;
using namespace pop;


//Port definition
struct param_changer_ports{
    struct new_params      : public out_port<pair<string, model_params>> { };
};


template<typename TIME>
class params_changer{
    public:
    // ports definition
    using input_ports=std::tuple<
         >;


    using output_ports=std::tuple<
        typename param_changer_ports::new_params
         >;


    struct state_type{
        vector<tuple<TIME, string, model_params>> q;
        TIME cur_time;
    };
    state_type state;


    // default constructor
	params_changer() = default;

	// constructor with passed variable
    params_changer(vector<tuple<TIME, string, model_params>> rule_change_set) : state{rule_change_set, 0}{
        sort(begin(state.q), end(state.q), [](const auto& lhs, const auto& rhs){return get<0>(lhs) < get<0>(rhs);});
	}


    // internal transition
    void internal_transition() {
        if(state.q.size()){
            state.cur_time = get<0>(state.q[0]);
            state.q.erase(begin(state.q));
        }
    }

	// external transition
    void external_transition(TIME time_elapsed, typename make_message_bags<input_ports>::type) {
        state.cur_time += time_elapsed;
    }


    // time_advance function
    TIME time_advance() const {
        if(state.q.size()){
            return get<0>(state.q[0])-state.cur_time;
        }else{
		    return std::numeric_limits<TIME>::infinity();
		}
    }

    // output function
    typename make_message_bags<output_ports>::type output() const {
        typename make_message_bags<output_ports>::type bags;
        /* send 1 new set of params */
        get_messages<typename param_changer_ports::new_params>(bags).push_back({get<1>(state.q[0]), get<2>(state.q[0])});

        return bags;
    }

    // confluence transition
    void confluence_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {
		internal_transition();
        external_transition({0}, move(mbs));
    }

    friend ostringstream& operator<<(ostringstream& os, const typename params_changer<TIME>::state_type& i) {
        os << i.q.size() << " to go";

		return os;
    }

};

#endif /* _PARAMS_CHANGER__HPP */
