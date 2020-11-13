

#ifndef _POPULATION_GROUP__HPP
#define _POPULATION_GROUP__HPP

#include <concepts>
#include <compare>

#include "../atomics/population_group_common.hpp"

//using namespace std;

struct model_params;
struct population;


constexpr population operator*(auto&& lhs, auto&& rhs) requires (std::integral<std::remove_cvref_t<decltype(lhs)>> || std::floating_point<std::remove_cvref_t<decltype(lhs)>>) && std::same_as<population, std::remove_cvref_t<decltype(rhs)>>;


constexpr double S_to_E (const population* pop, const model_params& params); // susceptible to exposed
constexpr double S_to_Eq(const population* pop, const model_params& params); // susceptible to quarantined_exposed
constexpr double S_to_Sq(const population* pop, const model_params& params); // susceptible to quarantined_susceptible
constexpr double E_to_I (const population* pop, const model_params& params); // exposed to symptomatic_infective
constexpr double E_to_A (const population* pop, const model_params& params); // exposed to asymptomatic_infective
constexpr double I_to_H (const population* pop, const model_params& params); // symptomatic_infective to quarantined_infective
constexpr double I_to_R (const population* pop, const model_params& params); // symptomatic_infective to recovered
constexpr double I_to_D (const population* pop, const model_params& params); // symptomatic_infective to deceased
constexpr double A_to_R (const population* pop, const model_params& params); // asymptomatic_infective to recovered
constexpr double Sq_to_S(const population* pop, const model_params& params); // quarantined_susceptible to susceptible
constexpr double Eq_to_H(const population* pop, const model_params& params); // quarantined_exposed to quarantined_infective
constexpr double H_to_R (const population* pop, const model_params& params); // quarantined_infective to recovered
constexpr double H_to_D (const population* pop, const model_params& params); // quarantined_infective to deceased

//constexpr population population::operator*(const double scalar, const population& pop);
//constexpr population population::operator*(const double scalar, const population* pop);


struct model_params {
    double c ; 	/* Contact rate per person per day */
    double b ;	/* Probability of transmission per contact */
    double q ;  /* Probability of being quarantined per contact */
    double e ;  /* Proportion of exposed individuals becoming infective per day */
    double l ;  /* Proportion of quarantined uninfected individuals being released per day */
    double n ;  /* Probability of having symptoms among infected individuals */
    double di;  /* Proportion of symptomatic infective individuals going into quarantine per day */
    double dq;  /* Proportion of quarantined exposed individuals progressing to quarantined infective per day */
    double yi;  /* Proportion of symptomatic infective individuals recovering per day */
    double ya;  /* Proportion of asymptomatic infective individuals recovering per day */
    double yh;  /* Proportion of quarantined infective individuals recovering per day */
    double a ;  /* Proportion of symptomatic infective and quarantined infective individuals dieing per day */
    double t ;  /* How infective are asymptomatic infective people compaired to symptomatic infective people */

    auto operator<=>(const model_params&) const = default;
    bool operator==(const model_params&) const = default;
};



struct population {
    double s ; /* susceptible population             */
    double e ; /* exposed population                 */
    double i ; /* symptomatic infective population   */
    double a ; /* asymptomatic infective population  */
    double sq; /* quarantined susceptible population */
    double eq; /* quarantined exposed population     */
    double h ; /* quarantined infective population   */
    double r ; /* recovered population               */
    double d ; /* deceased population                */

public:

    constexpr population& operator+=(auto&& rhs) requires std::same_as<population, std::remove_cvref_t<decltype(rhs)>> {
        s  += rhs.s;
        e  += rhs.e;
        i  += rhs.i;
        a  += rhs.a;
        sq += rhs.sq;
        eq += rhs.eq;
        h  += rhs.h;
        r  += rhs.r;
        d  += rhs.d;
        return *this;
    }

    constexpr population& operator-=(auto&& rhs) requires std::same_as<population, std::remove_cvref_t<decltype(rhs)>> {
        s  -= rhs.s;
        e  -= rhs.e;
        i  -= rhs.i;
        a  -= rhs.a;
        sq -= rhs.sq;
        eq -= rhs.eq;
        h  -= rhs.h;
        r  -= rhs.r;
        d  -= rhs.d;
        return *this;
    }

    constexpr population& operator*=(auto&& rhs) requires std::same_as<population, std::remove_cvref_t<decltype(rhs)>> {
        s  *= rhs.s;
        e  *= rhs.e;
        i  *= rhs.i;
        a  *= rhs.a;
        sq *= rhs.sq;
        eq *= rhs.eq;
        h  *= rhs.h;
        r  *= rhs.r;
        d  *= rhs.d;
        return *this;
    }

    constexpr population& operator*=(auto&& rhs) requires std::integral<std::remove_cvref_t<decltype(rhs)>> || std::floating_point<std::remove_cvref_t<decltype(rhs)>>{
        s  *= rhs;
        e  *= rhs;
        i  *= rhs;
        a  *= rhs;
        sq *= rhs;
        eq *= rhs;
        h  *= rhs;
        r  *= rhs;
        d  *= rhs;
        return *this;
    }

    constexpr population& operator+(auto&& rhs) requires std::same_as<population, std::remove_cvref_t<decltype(rhs)>> {
        population temp{*this};
        return temp += rhs;
    }

    constexpr population& operator-(auto&& rhs) requires std::same_as<population, std::remove_cvref_t<decltype(rhs)>> {
        population temp{*this};
        return temp -= rhs;
    }


    constexpr population operator*(auto&& rhs) requires std::integral<std::remove_cvref_t<decltype(rhs)>> || std::floating_point<std::remove_cvref_t<decltype(rhs)>>{
        population temp{*this};
        return temp *= rhs;
    }

    constexpr population operator+() {
        return {*this};
    }

    constexpr population operator-() {
        return (*this)*(-1);
    }

    constexpr double total(){
        return s+e+i+a+sq+eq+h+r+d;
    }

    constexpr population delta(const model_params& params){
        return {
                -S_to_E (this, params)
                -S_to_Eq(this, params)
                -S_to_Sq(this, params)
                +Sq_to_S(this, params)
            ,
                -E_to_I (this, params)
                -E_to_A (this, params)
                +S_to_E (this, params)
            ,
                -I_to_H (this, params)
                -I_to_R (this, params)
                -I_to_D (this, params)
                +E_to_I (this, params)
            ,
                -A_to_R (this, params)
                +E_to_A (this, params)
            ,
                -Sq_to_S(this, params)
                +S_to_Sq(this, params)
            ,
                -Eq_to_H(this, params)
                +S_to_Eq(this, params)
            ,
                -H_to_R (this, params)
                -H_to_D (this, params)
                +I_to_H (this, params)
                +Eq_to_H(this, params)
            ,
                +I_to_R (this, params)
                +A_to_R (this, params)
                +H_to_R (this, params)
            ,
                +I_to_D (this, params)
                +H_to_D (this, params)
            };
    }

    constexpr population delta(const model_params& params, auto&& dt) requires(std::integral<std::remove_cvref_t<decltype(dt)>> || std::floating_point<std::remove_cvref_t<decltype(dt)>>){
        population out = delta(params);
        out *= dt;
        return out;
    }

    constexpr population step(const model_params& params){
        population out = delta(params);
        out += *this;
        return out;
    }

    constexpr population step(const model_params& params, auto&& dt) requires(std::integral<std::remove_cvref_t<decltype(dt)>> || std::floating_point<std::remove_cvref_t<decltype(dt)>>){
        population out = delta(params);
        out *= dt;
        out += *this;
        return out;
    }

    constexpr auto operator<=>(const population&) const = default;
    constexpr bool operator==(const population&) const = default;
};

constexpr population operator*(auto&& lhs, auto&& rhs) requires (std::integral<std::remove_cvref_t<decltype(lhs)>> || std::floating_point<std::remove_cvref_t<decltype(lhs)>>) && std::same_as<population, std::remove_cvref_t<decltype(rhs)>>{
    return rhs*lhs;
}



/* For now, defined in terms of the old equations, may invert later */
constexpr double S_to_E (const population* pop, const model_params& params){return S_to_E (params.c, params.b, params.q, params.t, pop->s, pop->i, pop->a);};
constexpr double S_to_Eq(const population* pop, const model_params& params){return S_to_Eq(params.c, params.b, params.q, params.t, pop->s, pop->i, pop->a);};
constexpr double S_to_Sq(const population* pop, const model_params& params){return S_to_Sq(params.c, params.b, params.q, params.t, pop->s, pop->i, pop->a);};
constexpr double E_to_I (const population* pop, const model_params& params){return E_to_I (params.e, params.n, pop->e);};
constexpr double E_to_A (const population* pop, const model_params& params){return E_to_A (params.e, params.n, pop->e);};
constexpr double I_to_H (const population* pop, const model_params& params){return I_to_H (params.di, pop->i);};
constexpr double I_to_R (const population* pop, const model_params& params){return I_to_R (params.yi, pop->i);};
constexpr double I_to_D (const population* pop, const model_params& params){return I_to_D (params.a,  pop->i);};
constexpr double A_to_R (const population* pop, const model_params& params){return A_to_R (params.ya, pop->a);};
constexpr double Sq_to_S(const population* pop, const model_params& params){return Sq_to_S(params.l,  pop->sq);};
constexpr double Eq_to_H(const population* pop, const model_params& params){return Eq_to_H(params.dq, pop->eq);};
constexpr double H_to_R (const population* pop, const model_params& params){return H_to_R (params.yh, pop->h);};
constexpr double H_to_D (const population* pop, const model_params& params){return H_to_D (params.a, pop->h);};

#endif /* _POPULATION_GROUP__HPP */
