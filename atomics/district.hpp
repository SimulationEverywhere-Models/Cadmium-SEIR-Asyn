
#ifndef _DISTRICT__HPP
#define _DISTRICT__HPP

#include "./population.hpp"

#include <iostream>
#include <iomanip>
#include <optional>
#include <vector>



namespace pop {

    template<typename T>
    concept streamable =
        requires(T a){
            std::cout << a;
            std::cin >> a;
        };

    template<typename LHS, typename RHS>
        concept multipliable =
            requires(LHS lhs, RHS rhs){
                rhs *= lhs;
                rhs = lhs*rhs;
            };

    template<typename T>
        concept addable =
            requires(T a){
                a += a;
                a = a + a;
                a -= a;
                a = a - a;
            };

    template<typename VALUE, typename PARAMS>
        concept stepable =
            requires(VALUE v, PARAMS p){
                v = step(v, p, 1);
                v = delta(v, p, 1);
            };

    template<typename IDENTIFIER, typename PARAMS, typename VALUE, typename SCALAR=VALUE>
        requires(
            std::equality_comparable<IDENTIFIER>,
            streamable<IDENTIFIER> && streamable<PARAMS> && streamable<VALUE> && streamable<SCALAR> &&
            addable<VALUE> && multipliable<SCALAR, VALUE> && stepable<VALUE, PARAMS>
        )
    struct district_template {
        IDENTIFIER id;
        PARAMS params;
        VALUE  val;
        std::vector<std::pair<IDENTIFIER, SCALAR>> connectivity;

        std::optional<SCALAR> get_connectivity(IDENTIFIER target){
            for(const auto& [t, s] : connectivity){
                if(t == target){
                    return s;
                }
            }
            return {};
        }

        std::optional<VALUE> get_travel(IDENTIFIER target){
            auto s = get_connectivity(target);
            if(s){
                return (*s)*val;
            }else{
                return {};
            }
        }

        void set_connectivity(IDENTIFIER target, SCALAR scalar){
            for(auto& [t, s] : connectivity){
                if(t == target){
                    s = scalar;
                    return;
                }
            }
            connectivity.push_back({target, scalar});
        }

        friend std::ostream& operator<<(std::ostream& os, const district_template<IDENTIFIER, PARAMS, VALUE, SCALAR>& d){
            os << "[";
            if constexpr(std::same_as<IDENTIFIER, std::string>){
                os << std::quoted(d.id);
            }else{
                os << d.id;
            }
            os << ", ";
            if constexpr(std::same_as<PARAMS, std::string>){
                os << std::quoted(d.params);
            }else{
                os << d.params;
            }
            os << ", ";
            if constexpr(std::same_as<VALUE, std::string>){
                os << std::quoted(d.val);
            }else{
                os << d.val;
            }
            os << ", " << d.connectivity.size() << ", [";

            bool first_one = true;
            for(const auto& [t, s] : d.connectivity){
                if(!first_one){os << ", ";}else{first_one = false;}

                if constexpr(std::same_as<IDENTIFIER, std::string>){
                    os << std::quoted(t);
                }else{
                    os << t;
                }
                os << ", ";
                if constexpr(std::same_as<SCALAR, std::string>){
                    os << std::quoted(s);
                }else{
                    os << s;
                }
            }
            os << "]]";
            return os;
        }

        friend std::istream& operator>>(std::istream& is, district_template<IDENTIFIER, PARAMS, VALUE, SCALAR>& d){
            is.ignore(std::numeric_limits<std::streamsize>::max(), '[');


            if constexpr(std::same_as<IDENTIFIER, std::string>){
                is >> std::quoted(d.id);
            }else{
                is >> d.id;
            }

            is.ignore(std::numeric_limits<std::streamsize>::max(), ',');

            if constexpr(std::same_as<PARAMS, std::string>){
                is >> std::quoted(d.params);
            }else{
                is >> d.params;
            }

            is.ignore(std::numeric_limits<std::streamsize>::max(), ',');

            if constexpr(std::same_as<VALUE, std::string>){
                is >> std::quoted(d.val);
            }else{
                is >> d.val;
            }
            is.ignore(std::numeric_limits<std::streamsize>::max(), ',');

            size_t cons{0};

            is >> cons;

            d.connectivity.clear();

            for(size_t i = 0; i<cons; i++){

                std::pair<IDENTIFIER, SCALAR> p;
                auto& [t, s] = p;

                is.ignore(std::numeric_limits<std::streamsize>::max(), '[');
                if constexpr(std::same_as<IDENTIFIER, std::string>){
                    is >> std::quoted(t);
                }else{
                    is >> t;
                }

                is.ignore(std::numeric_limits<std::streamsize>::max(), ',');
                if constexpr(std::same_as<SCALAR, std::string>){
                    is >> std::quoted(s);
                }else{
                    is >> s;
                }

            }

            is.ignore(std::numeric_limits<std::streamsize>::max(), ']');
            is.ignore(std::numeric_limits<std::streamsize>::max(), ']');

            return is;
        }
    };



    typedef pop::district_template<std::string, pop::model_params, pop::population> district;
}
#endif /* _DISTRICT__HPP */
