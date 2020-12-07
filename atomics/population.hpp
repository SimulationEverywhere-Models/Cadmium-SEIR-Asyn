

#ifndef _POPULATION__HPP
#define _POPULATION__HPP

#include <concepts>
#include <compare>
#include <iostream>
#include <limits>

/*


__attribute__((flatten)) => tells the compiler to inline every made by this function

constexpr => tells the compiler that this is an inlineable function that will allways return the same output givin the same input, and that it can be run at compile time

auto operator<=>(const foo&) const = default; => a shortcut for implementing all of < > <= >=, using structural comparasons.


const everything that can be const

perfer non-friend non-member functions

*/
//#define constexpr inline

namespace pop {

    struct model_params;
    struct population;

    constexpr population step(const population& pop, const model_params& params, const double dt);
    constexpr population delta(const population& pop, const model_params& params, const double dt);

    struct model_params {
        double contact_rate_i;
        double contact_rate_a;
        double contact_rate_iq;
        double contact_rate_aq;

        double infectivity_from_contact_i;
        double infectivity_from_contact_a;
        double infectivity_from_contact_iq;
        double infectivity_from_contact_aq;

        double quarantined_chance_from_contact_i;
        double quarantined_chance_from_contact_a;
        double quarantined_chance_from_contact_iq;
        double quarantined_chance_from_contact_aq;

        double proportion_with_symptoms;

        double advacement_sq;

        double advacement_e;
        double advacement_eq;

        double self_quarantined_rate_i;
        double self_quarantined_rate_a;

        double recovery_rate_i;
        double recovery_rate_a;
        double recovery_rate_iq;
        double recovery_rate_aq;

        double death_rate_i;
        double death_rate_a;
        double death_rate_iq;
        double death_rate_aq;

        auto operator<=>(const model_params&) const = default;
        bool operator==(const model_params&) const = default;
    };

    struct population {
        double susceptible;
        double susceptible_q;
        double exposed;
        double exposed_q;
        double infective;
        double infective_q;
        double asymptomatic;
        double asymptomatic_q;
        double recovered;
        double deceased;

        auto operator<=>(const population&) const = default;
        bool operator==(const population&) const = default;

        /*
        __attribute__((flatten)) constexpr population step(const model_params& params, const double dt) const{
            return pop::step(*this, params, dt);
        }
        __attribute__((flatten)) constexpr population delta(const model_params& params, const double dt) const{
            return pop::delta(*this, params, dt);
        }*/
    };


    std::ostream& operator<<(std::ostream& os, const model_params& mp){
        return os << "["
           << mp.contact_rate_i << ", "
           << mp.contact_rate_a << ", "
           << mp.contact_rate_iq << ", "
           << mp.contact_rate_aq << ", "
           << mp.infectivity_from_contact_i << ", "
           << mp.infectivity_from_contact_a << ", "
           << mp.infectivity_from_contact_iq << ", "
           << mp.infectivity_from_contact_aq << ", "
           << mp.quarantined_chance_from_contact_i << ", "
           << mp.quarantined_chance_from_contact_a << ", "
           << mp.quarantined_chance_from_contact_iq << ", "
           << mp.quarantined_chance_from_contact_aq << ", "
           << mp.proportion_with_symptoms << ", "
           << mp.advacement_sq << ", "
           << mp.advacement_e << ", "
           << mp.advacement_eq << ", "
           << mp.self_quarantined_rate_i << ", "
           << mp.self_quarantined_rate_a << ", "
           << mp.recovery_rate_i << ", "
           << mp.recovery_rate_a << ", "
           << mp.recovery_rate_iq << ", "
           << mp.recovery_rate_aq << ", "
           << mp.death_rate_i << ", "
           << mp.death_rate_a << ", "
           << mp.death_rate_iq << ", "
           << mp.death_rate_aq << "]";
    }


    std::istream& operator>>(std::istream& is, model_params& mp){
        is.ignore(std::numeric_limits<std::streamsize>::max(), '[');

        (is >> mp.contact_rate_i)                   .ignore(std::numeric_limits<std::streamsize>::max(), ',');
        (is>> mp.contact_rate_a)                    .ignore(std::numeric_limits<std::streamsize>::max(), ',');
        (is>> mp.contact_rate_iq)                   .ignore(std::numeric_limits<std::streamsize>::max(), ',');
        (is>> mp.contact_rate_aq)                   .ignore(std::numeric_limits<std::streamsize>::max(), ',');
        (is>> mp.infectivity_from_contact_i)        .ignore(std::numeric_limits<std::streamsize>::max(), ',');
        (is>> mp.infectivity_from_contact_a)        .ignore(std::numeric_limits<std::streamsize>::max(), ',');
        (is>> mp.infectivity_from_contact_iq)       .ignore(std::numeric_limits<std::streamsize>::max(), ',');
        (is>> mp.infectivity_from_contact_aq)       .ignore(std::numeric_limits<std::streamsize>::max(), ',');
        (is>> mp.quarantined_chance_from_contact_i) .ignore(std::numeric_limits<std::streamsize>::max(), ',');
        (is>> mp.quarantined_chance_from_contact_a) .ignore(std::numeric_limits<std::streamsize>::max(), ',');
        (is>> mp.quarantined_chance_from_contact_iq).ignore(std::numeric_limits<std::streamsize>::max(), ',');
        (is>> mp.quarantined_chance_from_contact_aq).ignore(std::numeric_limits<std::streamsize>::max(), ',');
        (is>> mp.proportion_with_symptoms)          .ignore(std::numeric_limits<std::streamsize>::max(), ',');
        (is>> mp.advacement_sq)                     .ignore(std::numeric_limits<std::streamsize>::max(), ',');
        (is>> mp.advacement_e)                      .ignore(std::numeric_limits<std::streamsize>::max(), ',');
        (is>> mp.advacement_eq)                     .ignore(std::numeric_limits<std::streamsize>::max(), ',');
        (is>> mp.self_quarantined_rate_i)           .ignore(std::numeric_limits<std::streamsize>::max(), ',');
        (is>> mp.self_quarantined_rate_a)           .ignore(std::numeric_limits<std::streamsize>::max(), ',');
        (is>> mp.recovery_rate_i)                   .ignore(std::numeric_limits<std::streamsize>::max(), ',');
        (is>> mp.recovery_rate_a)                   .ignore(std::numeric_limits<std::streamsize>::max(), ',');
        (is>> mp.recovery_rate_iq)                  .ignore(std::numeric_limits<std::streamsize>::max(), ',');
        (is>> mp.recovery_rate_aq)                  .ignore(std::numeric_limits<std::streamsize>::max(), ',');
        (is>> mp.death_rate_i)                      .ignore(std::numeric_limits<std::streamsize>::max(), ',');
        (is>> mp.death_rate_a)                      .ignore(std::numeric_limits<std::streamsize>::max(), ',');
        (is>> mp.death_rate_iq)                     .ignore(std::numeric_limits<std::streamsize>::max(), ',');
        (is>> mp.death_rate_aq)                     .ignore(std::numeric_limits<std::streamsize>::max(), ']');

        return is;

    }


    std::ostream& operator<<(std::ostream& os, const population& pop){
        return os << "["
                  << pop.susceptible << ", "
                  << pop.susceptible_q << ", "
                  << pop.exposed << ", "
                  << pop.exposed_q << ", "
                  << pop.infective << ", "
                  << pop.infective_q << ", "
                  << pop.asymptomatic << ", "
                  << pop.asymptomatic_q << ", "
                  << pop.recovered << ", "
                  << pop.deceased << "]";
    }


    std::istream& operator>>(std::istream& is, population& pop){
        is.ignore(std::numeric_limits<std::streamsize>::max(), '[');

        (is >> pop.susceptible)   .ignore(std::numeric_limits<std::streamsize>::max(), ',');
        (is >> pop.susceptible_q) .ignore(std::numeric_limits<std::streamsize>::max(), ',');
        (is >> pop.exposed)       .ignore(std::numeric_limits<std::streamsize>::max(), ',');
        (is >> pop.exposed_q)     .ignore(std::numeric_limits<std::streamsize>::max(), ',');
        (is >> pop.infective)     .ignore(std::numeric_limits<std::streamsize>::max(), ',');
        (is >> pop.infective_q)   .ignore(std::numeric_limits<std::streamsize>::max(), ',');
        (is >> pop.asymptomatic)  .ignore(std::numeric_limits<std::streamsize>::max(), ',');
        (is >> pop.asymptomatic_q).ignore(std::numeric_limits<std::streamsize>::max(), ',');
        (is >> pop.recovered)     .ignore(std::numeric_limits<std::streamsize>::max(), ',');
        (is >> pop.deceased)      .ignore(std::numeric_limits<std::streamsize>::max(), ']');

        return is;

    }


    constexpr population& operator+=(auto& lhs, auto&& rhs) requires(std::same_as<population, std::remove_cvref_t<decltype(lhs)>> && std::same_as<population, std::remove_cvref_t<decltype(rhs)>>){
        lhs.susceptible    += rhs.susceptible;
        lhs.susceptible_q  += rhs.susceptible_q;
        lhs.exposed        += rhs.exposed;
        lhs.exposed_q      += rhs.exposed_q;
        lhs.infective      += rhs.infective;
        lhs.infective_q    += rhs.infective_q;
        lhs.asymptomatic   += rhs.asymptomatic;
        lhs.asymptomatic_q += rhs.asymptomatic_q;
        lhs.recovered      += rhs.recovered;
        lhs.deceased       += rhs.deceased;
        return lhs;
    }

    constexpr population& operator-=(auto& lhs, auto&& rhs) requires(std::same_as<population, std::remove_cvref_t<decltype(lhs)>> && std::same_as<population, std::remove_cvref_t<decltype(rhs)>>){
        lhs.susceptible    -= rhs.susceptible;
        lhs.susceptible_q  -= rhs.susceptible_q;
        lhs.exposed        -= rhs.exposed;
        lhs.exposed_q      -= rhs.exposed_q;
        lhs.infective      -= rhs.infective;
        lhs.infective_q    -= rhs.infective_q;
        lhs.asymptomatic   -= rhs.asymptomatic;
        lhs.asymptomatic_q -= rhs.asymptomatic_q;
        lhs.recovered      -= rhs.recovered;
        lhs.deceased       -= rhs.deceased;
        return lhs;
    }

    constexpr population& operator*=(auto& lhs, auto&& rhs) requires(std::same_as<population, std::remove_cvref_t<decltype(lhs)>> && std::same_as<population, std::remove_cvref_t<decltype(rhs)>>){
        lhs.susceptible    *= rhs.susceptible;
        lhs.susceptible_q  *= rhs.susceptible_q;
        lhs.exposed        *= rhs.exposed;
        lhs.exposed_q      *= rhs.exposed_q;
        lhs.infective      *= rhs.infective;
        lhs.infective_q    *= rhs.infective_q;
        lhs.asymptomatic   *= rhs.asymptomatic;
        lhs.asymptomatic_q *= rhs.asymptomatic_q;
        lhs.recovered      *= rhs.recovered;
        lhs.deceased       *= rhs.deceased;
        return lhs;
    }

    constexpr population& operator*=(auto& lhs, auto&& rhs) requires(std::same_as<population, std::remove_cvref_t<decltype(lhs)>> && (std::integral<std::remove_cvref_t<decltype(rhs)>> || std::floating_point<std::remove_cvref_t<decltype(rhs)>>)){
        lhs.susceptible    *= rhs;
        lhs.susceptible_q  *= rhs;
        lhs.exposed        *= rhs;
        lhs.exposed_q      *= rhs;
        lhs.infective      *= rhs;
        lhs.infective_q    *= rhs;
        lhs.asymptomatic   *= rhs;
        lhs.asymptomatic_q *= rhs;
        lhs.recovered      *= rhs;
        lhs.deceased       *= rhs;
        return lhs;
    }

    __attribute__((flatten)) constexpr population operator+(auto&& lhs, auto&& rhs) requires(std::same_as<population, std::remove_cvref_t<decltype(lhs)>> && std::same_as<population, std::remove_cvref_t<decltype(rhs)>>){
        population temp{lhs};
        return temp += rhs;
    }

    __attribute__((flatten)) constexpr population operator-(auto&& lhs, auto&& rhs) requires(std::same_as<population, std::remove_cvref_t<decltype(lhs)>> && std::same_as<population, std::remove_cvref_t<decltype(rhs)>>){
        population temp{lhs};
        return temp -= rhs;
    }

    __attribute__((flatten)) constexpr population operator*(auto&& lhs, auto&& rhs)
            requires(std::same_as<population, std::remove_cvref_t<decltype(lhs)>> && (
                std::same_as<population, std::remove_cvref_t<decltype(rhs)>> ||
                std::integral<std::remove_cvref_t<decltype(rhs)>> ||
                std::floating_point<std::remove_cvref_t<decltype(rhs)>>))
    {
        population temp{lhs};
        return temp *= rhs;
    }

    __attribute__((flatten)) constexpr population operator*(auto& lhs, auto&& rhs)
            requires((std::integral<std::remove_cvref_t<decltype(lhs)>> || std::floating_point<std::remove_cvref_t<decltype(lhs)>>)
                && std::same_as<population, std::remove_cvref_t<decltype(rhs)>>)
    {
        return rhs*lhs;
    }


    __attribute__((flatten)) constexpr population operator+(auto&& p) requires std::same_as<population, std::remove_cvref_t<decltype(p)>> {
        return {p};
    }

    __attribute__((flatten)) constexpr population operator-(auto&& p) requires std::same_as<population, std::remove_cvref_t<decltype(p)>> {
        return p*(-1);
    }

    /*
    params.contact_rate_i                     = c
    params.contact_rate_a                     = c
    params.contact_rate_iq                    = 0
    params.contact_rate_aq                    = 0

    params.infectivity_from_contact_i         = b
    params.infectivity_from_contact_a         = b*t
    params.infectivity_from_contact_iq        = b
    params.infectivity_from_contact_aq        = b*t

    params.quarantined_chance_from_contact_i  = q
    params.quarantined_chance_from_contact_a  = q
    params.quarantined_chance_from_contact_iq = q
    params.quarantined_chance_from_contact_aq = q

    params.proportion_with_symptoms           = n

    params.advacement_sq                      = l

    params.advacement_e                       = e
    params.advacement_eq                      = dq

    params.self_quarantined_rate_i            = di
    params.self_quarantined_rate_a            = 0

    params.recovery_rate_i                    = yi
    params.recovery_rate_a                    = ya
    params.recovery_rate_iq                   = yh
    params.recovery_rate_aq                   = yh

    params.death_rate_i                       = a
    params.death_rate_a                       = 0
    params.death_rate_iq                      = a
    params.death_rate_aq                      = a

    pop.susceptible    = S
    pop.susceptible_q  = Sq
    pop.exposed        = E
    pop.exposed_q      = Eq
    pop.infective      = I
    pop.infective_q    = H
    pop.asymptomatic   = A
    pop.asymptomatic_q = 0
    pop.recovered      = R
    pop.deceased       = D
    */

    constexpr double susceptible_to_exposed      (const population& pop, const model_params& params){
        return pop.susceptible * pop.infective      * params.contact_rate_i  * params.infectivity_from_contact_i  * (1-params.quarantined_chance_from_contact_i)  +
               pop.susceptible * pop.asymptomatic   * params.contact_rate_a  * params.infectivity_from_contact_a  * (1-params.quarantined_chance_from_contact_a)  +
               pop.susceptible * pop.infective_q    * params.contact_rate_iq * params.infectivity_from_contact_iq * (1-params.quarantined_chance_from_contact_iq) +
               pop.susceptible * pop.asymptomatic_q * params.contact_rate_aq * params.infectivity_from_contact_aq * (1-params.quarantined_chance_from_contact_aq);
    }

    constexpr double susceptible_to_exposed_q    (const population& pop, const model_params& params){
        return pop.susceptible * pop.infective      * params.contact_rate_i  * params.infectivity_from_contact_i  * params.quarantined_chance_from_contact_i  +
               pop.susceptible * pop.asymptomatic   * params.contact_rate_a  * params.infectivity_from_contact_a  * params.quarantined_chance_from_contact_a  +
               pop.susceptible * pop.infective_q    * params.contact_rate_iq * params.infectivity_from_contact_iq * params.quarantined_chance_from_contact_iq +
               pop.susceptible * pop.asymptomatic_q * params.contact_rate_aq * params.infectivity_from_contact_aq * params.quarantined_chance_from_contact_aq;
    }

    constexpr double susceptible_to_susceptible_q(const population& pop, const model_params& params){
        return pop.susceptible * pop.infective      * params.contact_rate_i  * (1-params.infectivity_from_contact_i)  * params.quarantined_chance_from_contact_i  +
               pop.susceptible * pop.asymptomatic   * params.contact_rate_a  * (1-params.infectivity_from_contact_a)  * params.quarantined_chance_from_contact_a  +
               pop.susceptible * pop.infective_q    * params.contact_rate_iq * (1-params.infectivity_from_contact_iq) * params.quarantined_chance_from_contact_iq +
               pop.susceptible * pop.asymptomatic_q * params.contact_rate_aq * (1-params.infectivity_from_contact_aq) * params.quarantined_chance_from_contact_aq;
    }

    constexpr double susceptible_q_to_susceptible(const population& pop, const model_params& params){return pop.susceptible_q*params.advacement_sq;}

    constexpr double exposed_to_infective       (const population& pop, const model_params& params){return pop.exposed  *params.advacement_e *   params.proportion_with_symptoms ;}
    constexpr double exposed_to_asymptomatic    (const population& pop, const model_params& params){return pop.exposed  *params.advacement_e *(1-params.proportion_with_symptoms);}
    constexpr double exposed_q_to_infective_q   (const population& pop, const model_params& params){return pop.exposed_q*params.advacement_eq*   params.proportion_with_symptoms ;}
    constexpr double exposed_q_to_asymptomatic_q(const population& pop, const model_params& params){return pop.exposed_q*params.advacement_eq*(1-params.proportion_with_symptoms);}

    constexpr double infective_to_infective_q(const population& pop, const model_params& params){return pop.infective*params.self_quarantined_rate_i;}
    constexpr double infective_to_recovered  (const population& pop, const model_params& params){return pop.infective*params.recovery_rate_i;}
    constexpr double infective_to_deceased   (const population& pop, const model_params& params){return pop.infective*params.death_rate_i;}

    constexpr double infective_q_to_recovered(const population& pop, const model_params& params){return pop.infective_q*params.recovery_rate_iq;}
    constexpr double infective_q_to_deceased (const population& pop, const model_params& params){return pop.infective_q*params.death_rate_iq;}

    constexpr double asymptomatic_to_asymptomatic_q(const population& pop, const model_params& params){return pop.asymptomatic*params.self_quarantined_rate_a;}
    constexpr double asymptomatic_to_recovered     (const population& pop, const model_params& params){return pop.asymptomatic*params.recovery_rate_a;}
    constexpr double asymptomatic_to_deceased      (const population& pop, const model_params& params){return pop.asymptomatic*params.death_rate_a;}

    constexpr double asymptomatic_q_to_recovered(const population& pop, const model_params& params){return pop.asymptomatic_q*params.recovery_rate_aq;}
    constexpr double asymptomatic_q_to_deceased (const population& pop, const model_params& params){return pop.asymptomatic_q*params.death_rate_aq;}


    __attribute__((flatten)) constexpr population step(const population& pop, const model_params& params, const double dt=1.0){
        return pop+delta(pop, params, dt);
    }

    __attribute__((flatten)) constexpr  population delta(const population& pop, const model_params& params, const double dt=1.0){
        population temp{/* susceptible */
            +susceptible_q_to_susceptible(pop, params)
            -susceptible_to_susceptible_q(pop, params)
            -susceptible_to_exposed(pop, params)
            -susceptible_to_exposed_q(pop, params)
            ,/* susceptible_q */
            +susceptible_to_susceptible_q(pop, params)
            -susceptible_q_to_susceptible(pop, params)
            ,/* exposed */
            +susceptible_to_exposed(pop, params)
            -exposed_to_infective(pop, params)
            -exposed_to_asymptomatic(pop, params)
            ,/* exposed_q */
            +susceptible_to_exposed_q(pop, params)
            -exposed_q_to_infective_q(pop, params)
            -exposed_q_to_asymptomatic_q(pop, params)
            ,/* infective */
            +exposed_to_infective(pop, params)
            -infective_to_infective_q(pop, params)
            -infective_to_recovered(pop, params)
            -infective_to_deceased(pop, params)
            ,/* infective_q */
            +exposed_q_to_infective_q(pop, params)
            +infective_to_infective_q(pop, params)
            -infective_q_to_recovered(pop, params)
            -infective_q_to_deceased(pop, params)
            ,/* asymptomatic */
            +exposed_to_asymptomatic(pop, params)
            -asymptomatic_to_asymptomatic_q(pop, params)
            -asymptomatic_to_recovered(pop, params)
            -asymptomatic_to_deceased(pop, params)
            ,/* asymptomatic_q */
            +exposed_q_to_asymptomatic_q(pop, params)
            +asymptomatic_to_asymptomatic_q(pop, params)
            -asymptomatic_q_to_recovered(pop, params)
            -asymptomatic_q_to_deceased(pop, params)
            ,/* recovered */
            +infective_to_recovered(pop, params)
            +infective_q_to_recovered(pop, params)
            +asymptomatic_to_recovered(pop, params)
            +asymptomatic_q_to_recovered(pop, params)
            ,/* deceased */
            +infective_to_deceased(pop, params)
            +infective_q_to_deceased(pop, params)
            +asymptomatic_to_deceased(pop, params)
            +asymptomatic_q_to_deceased(pop, params)
        };
        temp *= dt;
        return temp;
    }

};


//#undef constexpr
#endif /* _POPULATION__HPP */
