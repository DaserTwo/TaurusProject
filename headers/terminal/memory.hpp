#pragma once

#include <string>
#include <vector>
#include <stack>

#include "defs.hpp"

namespace memory{
    typedef std::pair<defs::var_type, std::string> var_type;

    extern var_type _get;
    extern std::stack<var_type> memory_stack;
    extern std::vector<var_type> index_memory;

    extern bool get_var_error;
    var_type get_var(std::string name);
}