#pragma once

#include <string>

#include <cstdio>

namespace cmd_error{
    class Error{
        std::string synax;
        public:
            Error(std::string synax): synax{synax}{};

            inline void too_few_arg(){
                printf("Too few arguments %s\n", synax.c_str());
            }

            inline void too_many_arg(){
                printf("Too many arguments %s\n", synax.c_str());
            }

            inline void exc_num(){
                printf("Excepted number %s\n", synax.c_str());
            }

            inline void exc_int(){
                printf("Excepted integere %s\n", synax.c_str());
            }

            inline void exc_rel(){
                printf("Excepted real number %s\n", synax.c_str());
            }

            inline void exc_str(){
                printf("Excepted string %s\n", synax.c_str());
            }

            inline void exc_bool(){
                printf("Excepted boolean %s\n", synax.c_str());
            }

            inline void exc_arg(){
                printf("Excepted argument %s\n", synax.c_str());
            }

            inline void bad_arg(){
                printf("Bad argument %s\n", synax.c_str());
            }
    };
}