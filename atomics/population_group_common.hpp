

#ifndef _POPULATION_GROUP_COMMON_HPP__
#define _POPULATION_GROUP_COMMON_HPP__

#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>


//                      dt,c, b, q, e, l, n, di,dq,yi,ya,yh,a, t
double constants[14] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

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


    constexpr double S_to_E (double c, double b, double q, double t, double S, double I, double A){return (c*(I+t*A)*S * b*(1-q));} // susceptible to exposed
    constexpr double S_to_Eq(double c, double b, double q, double t, double S, double I, double A){return (c*(I+t*A)*S *   b*q)  ;} // susceptible to quarantined_exposed
    constexpr double S_to_Sq(double c, double b, double q, double t, double S, double I, double A){return (c*(I+t*A)*S * (1-b)*q);} // susceptible to quarantined_susceptible
    constexpr double E_to_I (double e, double n, double E)                                        {return (e*n    *E)            ;} // exposed to symptomatic_infective
    constexpr double E_to_A (double e, double n, double E)                                        {return (e*(1-n)*E)            ;} // exposed to asymptomatic_infective
    constexpr double I_to_H (double di, double I)                                                 {return (di*I)                 ;} // symptomatic_infective to quarantined_infective
    constexpr double I_to_R (double yi, double I)                                                 {return (yi*I)                 ;} // symptomatic_infective to recovered
    constexpr double I_to_D (double a, double I)                                                  {return (a *I)                 ;} // symptomatic_infective to deceased
    constexpr double A_to_R (double ya, double A)                                                 {return (ya*A)                 ;} // asymptomatic_infective to recovered
    constexpr double Sq_to_S(double l, double Sq)                                                 {return (l *Sq)                ;} // quarantined_susceptible to susceptible
    constexpr double Eq_to_H(double dq, double Eq)                                                {return (dq*Eq)                ;} // quarantined_exposed to quarantined_infective
    constexpr double H_to_R (double yh, double H)                                                 {return (yh*H)                 ;} // quarantined_infective to recovered
    constexpr double H_to_D (double a, double H)                                                  {return (a*H)                  ;} // quarantined_infective to deceased


    /*
    inline double S_to_E (double c, double b, double q, double t, double S, double I, double A){
        double delta = (c*(I+t*A)*S * b*(1-q));
        if(delta < 0){
            cerr << "S_to_E fails " << delta << " = (c*(I+A)*S*b*(1-q)) with ( "<<c<<" *( "<<I<<" + "<<t<<" * "<<A<<" )* "<<S<<" * "<<b<<" *(1- "<<q<<" ))\n";
            abort();
        }
        return delta;
    } // susceptible to exposed

    inline double S_to_Eq(double c, double b, double q, double t, double S, double I, double A){
        double delta = (c*(I+t*A)*S *   b*q);
        if(delta < 0){
            cerr << "S_to_Eq fails " << delta << " = (c*(I+A)*S*b*q) with ( "<<c<<" *( "<<I<<" + "<<t<<" * "<<A<<" )* "<<S<<" * "<<b<<" * "<<q<<" )\n";
            abort();
        }
        return delta;
    } // susceptible to quarantined_exposed

    inline double S_to_Sq(double c, double b, double q, double t, double S, double I, double A){
        double delta = (c*(I+t*A)*S * (1-b)*q);
        if(delta < 0){
            cerr << "S_to_Sq fails " << delta << " = (c*(I+A)*S*(1-b)*q) with ( "<<c<<" *( "<<I<<" + "<<t<<" * "<<A<<" )* "<<S<<" *(1- "<<b<<" )* "<<q<<" )\n";
            abort();
        }
        return delta;
    } // susceptible to quarantined_susceptible

    inline double E_to_I (double e, double n, double E){
        double delta = (e*n    *E);
        if(delta < 0){
            cerr << "E_to_I fails "<<delta<<" = (e*n*E) with ( "<<e<<" * "<<n<<" * "<<E<<" )\n";
            abort();
        }
        return delta;
    } // exposed to symptomatic_infective

    inline double E_to_A (double e, double n, double E){
        double delta = (e*(1-n)*E);
        if(delta < 0){
            cerr << "E_to_A fails "<<delta<<" = (e*(1-n)*E) with ( "<<e<<" * (1- "<<n<<" )* "<<E<<" )\n";
            abort();
        }
        return delta;
    } // exposed to asymptomatic_infective

    inline double I_to_H (double di, double I){
        double delta = (di*I);
        if(delta < 0){
            cerr << "I_to_H fails "<<delta<<" = (di*I) with ( "<<di<<" * "<<I<<" )\n";
            abort();
        }
        return delta;
    } // symptomatic_infective to quarantined_infective

    inline double I_to_R (double yi, double I){
        double delta = (yi*I);
        if(delta < 0){
            cerr << "I_to_R fails "<<delta<<" = (yi*I) with ( "<<yi<<" * "<<I<<" )\n";
            abort();
        }
        return delta;
    } // symptomatic_infective to recovered

    inline double I_to_D (double a, double I){
        double delta = (a *I);
        if(delta < 0){
            cerr << "I_to_D fails "<<delta<<" = (a*I) with ( "<<a<<" * "<<I<<" )\n";
            abort();
        }
        return delta;
    } // symptomatic_infective to deceased

    inline double A_to_R (double ya, double A){
        double delta = (ya*A);
        if(delta < 0){
            cerr << "A_to_R fails "<<delta<<" = (ya*A) with ( "<<ya<<" * "<<A<<" )\n";
            abort();
        }
        return delta;
    } // asymptomatic_infective to recovered

    inline double Sq_to_S(double l, double Sq){
        double delta = (l *Sq);
        if(delta < 0){
            cerr << "Sq_to_S fails "<<delta<<" = (l*Sq) with ( "<<l<<" * "<<Sq<<" )\n";
            abort();
        }
        return delta;
    } // quarantined_susceptible to susceptible

    inline double Eq_to_H(double dq, double Eq){
        double delta = (dq*Eq);
        if(delta < 0){
            cerr << "Eq_to_H fails "<<delta<<" = (dq*Eq) with ( "<<dq<<" * "<<Eq<<" )\n";
            abort();
        }
        return delta;
    } // quarantined_exposed to quarantined_infective

    inline double H_to_R (double yh, double H){
        double delta = (yh*H);
        if(delta < 0){
            cerr << "H_to_R fails "<<delta<<" = (yh*H) with ( "<<yh<<" * "<<H<<" )\n";
            abort();
        }
        return delta;
    } // quarantined_infective to recovered

    inline double H_to_D (double a, double H){
        double delta = (a*H);
        if(delta < 0){
            cerr << "H_to_D fails "<<delta<<" = (a*H) with ( "<<a<<" * "<<H<<" )\n";
            abort();
        }
        return delta;

    } // quarantined_infective to deceased
    */
#endif //_POPULATION_GROUP_COMMON_HPP__
