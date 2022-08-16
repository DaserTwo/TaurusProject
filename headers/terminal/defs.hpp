#pragma once

namespace defs{
    typedef enum{
        ARG_INT,
        ARG_DOT,
        ARG_STR,
        ARG_VAR
    } arg_type;

    typedef enum{
        VAR_INT = ARG_INT,
        VAR_DOT = ARG_DOT,
        VAR_STR = ARG_STR,
        VAR_BOOL,
        VAR_TYPE
    } var_type;
}