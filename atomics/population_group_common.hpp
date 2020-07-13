

#ifndef _POPULATION_GROUP_COMMON_HPP__
#define _POPULATION_GROUP_COMMON_HPP__

#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>


//                      dt,c, b, q, e, l, n, di,dq,yi,ya,yh,a
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

    /*
    inline double S_to_E (double c, double b, double q, double S, double I, double A){return (c*(I+A)*S * b*(1-q));} // susceptible to exposed
    inline double S_to_Eq(double c, double b, double q, double S, double I, double A){return (c*(I+A)*S *   b*q)  ;} // susceptible to quarantined_exposed
    inline double S_to_Sq(double c, double b, double q, double S, double I, double A){return (c*(I+A)*S * (1-b)*q);} // susceptible to quarantined_susceptible
    inline double E_to_I (double e, double n, double E)                              {return (e*n    *E)          ;} // exposed to symptomatic_infective
    inline double E_to_A (double e, double n, double E)                              {return (e*(1-n)*E)          ;} // exposed to asymptomatic_infective
    inline double I_to_H (double di, double I)                                       {return (di*I)               ;} // symptomatic_infective to quarantined_infective
    inline double I_to_R (double yi, double I)                                       {return (yi*I)               ;} // symptomatic_infective to recovered
    inline double I_to_D (double a, double I)                                        {return (a *I)               ;} // symptomatic_infective to deceased
    inline double A_to_R (double ya, double A)                                       {return (ya*A)               ;} // asymptomatic_infective to recovered
    inline double Sq_to_S(double l, double Sq)                                       {return (l *Sq)              ;} // quarantined_susceptible to susceptible
    inline double Eq_to_H(double dq, double Eq)                                      {return (dq*Eq)              ;} // quarantined_exposed to quarantined_infective
    inline double H_to_R (double yh, double H)                                       {return (yh*H)               ;} // quarantined_infective to recovered
    inline double H_to_D (double a, double H)                                        {return (a*H)                ;} // quarantined_infective to deceased
    */

    inline double S_to_E (double c, double b, double q, double S, double I, double A){
        double t = (c*(I+A)*S * b*(1-q));
        if(t < 0){
            cerr << "S_to_E fails " << t << " = (c*(I+A)*S*b*(1-q)) with ( "<<c<<" *( "<<I<<" + "<<A<<" )* "<<S<<" * "<<b<<" *(1- "<<q<<" ))\n";
            abort();
        }
        return t;
    } // susceptible to exposed

    inline double S_to_Eq(double c, double b, double q, double S, double I, double A){
        double t = (c*(I+A)*S *   b*q);
        if(t < 0){
            cerr << "S_to_Eq fails " << t << " = (c*(I+A)*S*b*q) with ( "<<c<<" *( "<<I<<" + "<<A<<" )* "<<S<<" * "<<b<<" * "<<q<<" )\n";
            abort();
        }
        return t;
    } // susceptible to quarantined_exposed

    inline double S_to_Sq(double c, double b, double q, double S, double I, double A){
        double t = (c*(I+A)*S * (1-b)*q);
        if(t < 0){
            cerr << "S_to_Sq fails " << t << " = (c*(I+A)*S*(1-b)*q) with ( "<<c<<" *( "<<I<<" + "<<A<<" )* "<<S<<" *(1- "<<b<<" )* "<<q<<" )\n";
            abort();
        }
        return t;
    } // susceptible to quarantined_susceptible

    inline double E_to_I (double e, double n, double E){
        double t = (e*n    *E);
        if(t < 0){
            cerr << "E_to_I fails "<<t<<" = (e*n*E) with ( "<<e<<" * "<<n<<" * "<<E<<" )\n";
            abort();
        }
        return t;
    } // exposed to symptomatic_infective

    inline double E_to_A (double e, double n, double E){
        double t = (e*(1-n)*E);
        if(t < 0){
            cerr << "E_to_A fails "<<t<<" = (e*(1-n)*E) with ( "<<e<<" * (1- "<<n<<" )* "<<E<<" )\n";
            abort();
        }
        return t;
    } // exposed to asymptomatic_infective

    inline double I_to_H (double di, double I){
        double t = (di*I);
        if(t < 0){
            cerr << "I_to_H fails "<<t<<" = (di*I) with ( "<<di<<" * "<<I<<" )\n";
            abort();
        }
        return t;
    } // symptomatic_infective to quarantined_infective

    inline double I_to_R (double yi, double I){
        double t = (yi*I);
        if(t < 0){
            cerr << "I_to_R fails "<<t<<" = (yi*I) with ( "<<yi<<" * "<<I<<" )\n";
            abort();
        }
        return t;
    } // symptomatic_infective to recovered

    inline double I_to_D (double a, double I){
        double t = (a *I);
        if(t < 0){
            cerr << "I_to_D fails "<<t<<" = (a*I) with ( "<<a<<" * "<<I<<" )\n";
            abort();
        }
        return t;
    } // symptomatic_infective to deceased

    inline double A_to_R (double ya, double A){
        double t = (ya*A);
        if(t < 0){
            cerr << "A_to_R fails "<<t<<" = (ya*A) with ( "<<ya<<" * "<<A<<" )\n";
            abort();
        }
        return t;
    } // asymptomatic_infective to recovered

    inline double Sq_to_S(double l, double Sq){
        double t = (l *Sq);
        if(t < 0){
            cerr << "Sq_to_S fails "<<t<<" = (l*Sq) with ( "<<l<<" * "<<Sq<<" )\n";
            abort();
        }
        return t;
    } // quarantined_susceptible to susceptible

    inline double Eq_to_H(double dq, double Eq){
        double t = (dq*Eq);
        if(t < 0){
            cerr << "Eq_to_H fails "<<t<<" = (dq*Eq) with ( "<<dq<<" * "<<Eq<<" )\n";
            abort();
        }
        return t;
    } // quarantined_exposed to quarantined_infective

    inline double H_to_R (double yh, double H){
        double t = (yh*H);
        if(t < 0){
            cerr << "H_to_R fails "<<t<<" = (yh*H) with ( "<<yh<<" * "<<H<<" )\n";
            abort();
        }
        return t;
    } // quarantined_infective to recovered

    inline double H_to_D (double a, double H){
        double t = (a*H);
        if(t < 0){
            cerr << "H_to_D fails "<<t<<" = (a*H) with ( "<<a<<" * "<<H<<" )\n";
            abort();
        }
        return t;

    } // quarantined_infective to deceased
    /*

    S_to_E (c, b, q, state.S, state.I, state.A)
    S_to_Eq(c, b, q, state.S, state.I, state.A)
    S_to_Sq(c, b, q, state.S, state.I, state.A)
    E_to_I (e, n, state.E)
    E_to_A (e, n, state.E)
    I_to_H (di, state.I)
    I_to_R (yi, state.I)
    I_to_D (a, state.I)
    A_to_R (ya, state.A)
    Sq_to_S(l, state.Sq)
    Eq_to_H(dq, state.Eq)
    H_to_R (yh, state.H)
    H_to_D (a, state.H)

    */


#endif //_POPULATION_GROUP_COMMON_HPP__
