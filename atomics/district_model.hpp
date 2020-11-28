#ifndef _DISTRICT_MODEL_HPP__
#define _DISTRICT_MODEL_HPP__

#include <limits>
#include <assert.h>
#include <string>
#include <random>
#include <cmath>

#include "./district.hpp"

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
struct district_ports{
    struct report          : public out_port<population> { };
    struct people_out      : public out_port<pair<string, population>> { };

    struct new_travel_rate : public in_port<tuple<string, string, population>> { };
    struct new_params      : public in_port<pair<string, model_params>> { };
    struct people_in       : public in_port<pair<string, population>> { };
};

template<typename TIME>
class district_model{
    public:
    const TIME dt{0.01};
    // ports definition
    using input_ports=std::tuple<
        typename district_ports::new_travel_rate,
        typename district_ports::new_params,
        typename district_ports::people_in
         >;


    using output_ports=std::tuple<
        typename district_ports::report,
        typename district_ports::people_out
         >;

    // state definition
    struct state_type{
        district d;
    };
    state_type state;

    // default constructor
	district_model() = default;

	// constructor with passed variable
    district_model(district var) {
        state.d = var;
	}

    // internal transition
    void internal_transition() const {
        /*intentionally left blank*/
    }

	// external transition
    void external_transition(double, typename make_message_bags<input_ports>::type mbs) {
        /* for each new travel rate, if they are talking to me, update my travel rates */
        for(const auto& [me, id, s] : get_messages<typename district_ports::new_travel_rate>(mbs)){
            if(me == state.d.id){
                state.d.set_connectivity(id, s);
            }
        }

        /* for each new set of morel params, if they are talking to me, update my params */
        for(const auto& [me, params] : get_messages<typename district_ports::new_params>(mbs)){
            if(me == state.d.id){
                state.d.params = params;
            }
        }

        /* for each new set of morel params, if they are talking to me, update my params */
        for(const auto& [me, pop] : get_messages<typename district_ports::people_in>(mbs)){
            if(me == state.d.id){
                state.d.val += pop;
            }
        }
    }


    // time_advance function
    TIME time_advance() const {
		return {dt};
    }

    // output function
    typename make_message_bags<output_ports>::type output() const {
        typename make_message_bags<output_ports>::type bags;
        /* Report your pop before this call */
        get_messages<typename district_ports::report>(bags).push_back(state.d.val);

        /* Calculate how much you should tell yourself to change */
        auto d = delta(state.d.val, state.d.params, dt);

        /*
            For each connection, calculate how many people of what classes are going there from here
            Send them away, and subtract them from the number of people that we are going to have next step
         */
        for(auto [target, s] : state.d.connectivity){
            auto temp = s*state.d.val;
            d -= temp;
            get_messages<typename district_ports::people_out>(bags).push_back({target, temp});
        }

        /* Remember to send the message to your self */
        get_messages<typename district_ports::people_out>(bags).push_back({state.d.id, d});
        return bags;
    }

    // confluence transition
    void confluence_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {
		internal_transition();
        external_transition(e, move(mbs));
    }

    friend ostringstream& operator<<(ostringstream& os, const typename district_model<TIME>::state_type& i) {
        os << i.d;
		return os;
    }

};
#endif // _DISTRICT_MODEL_HPP__
