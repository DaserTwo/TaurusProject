#pragma once

#include <string>
#include <stack>

#include "defs.hpp"

namespace process{
    extern std::stack<std::pair<defs::arg_type, std::string>> arg_stack;
    extern std::string key_words[];

    void processInput(std::string input);
    void processArgs(std::string args);
    void processCmd(std::string cmd, std::string args);

    void clearArgStack();
}