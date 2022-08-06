#pragma once

namespace defs{
    typedef enum{
        ARG_NUM,
        ARG_DOT,
        ARG_STR,
        ARG_VAR
    } arg_type;

    typedef enum{
        VAR_NUM = ARG_NUM,
        VAR_DOT = ARG_DOT,
        VAR_STR = ARG_STR,
        VAR_BOOL
    } var_type;
}