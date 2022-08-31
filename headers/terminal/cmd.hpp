#pragma once

#include <string>
#include <stack>

#include "cmd_error.hpp"
#include "memory.hpp"
#include "defs.hpp"

namespace cmd{
    extern bool _prot_err;
    void std_cmd_call(std::stack<std::pair<defs::arg_type, std::string>>* arg_stack, std::size_t argc, cmd_error::Error* err, memory::var_type(*cmd_prot)(std::stack<std::pair<defs::arg_type, std::string>>* arg_stack), bool* cmd_prot_err, int arg_prot_c = 0);
    void std_cmd_call_std_err(std::stack<std::pair<defs::arg_type, std::string>>* arg_stack, std::size_t argc, cmd_error::Error* err, memory::var_type(*cmd_prot)(std::stack<std::pair<defs::arg_type, std::string>>* arg_stack), int arg_prot_c = 0);

    extern cmd_error::Error exit_err;
    void cmd_exit(std::stack<std::pair<defs::arg_type, std::string>>* arg_stack, std::size_t argc);

    extern cmd_error::Error push_err;
    void cmd_push(std::stack<std::pair<defs::arg_type, std::string>>* arg_stack, std::size_t argc);

    extern cmd_error::Error pop_err;
    void cmd_pop(std::stack<std::pair<defs::arg_type, std::string>>* arg_stack, std::size_t argc);

    extern cmd_error::Error stack_err;
    void cmd_stack(std::stack<std::pair<defs::arg_type, std::string>>* arg_stack, std::size_t argc);

    extern cmd_error::Error size_err;
    void cmd_size(std::stack<std::pair<defs::arg_type, std::string>>* arg_stack, std::size_t argc);

    extern cmd_error::Error ipush_err;
    void cmd_ipush(std::stack<std::pair<defs::arg_type, std::string>>* arg_stack, std::size_t argc);

    extern cmd_error::Error ipop_err;
    void cmd_ipop(std::stack<std::pair<defs::arg_type, std::string>>* arg_stack, std::size_t argc);

    extern cmd_error::Error index_err;
    void cmd_index(std::stack<std::pair<defs::arg_type, std::string>>* arg_stack, std::size_t argc);

    extern cmd_error::Error isize_err;
    void cmd_isize(std::stack<std::pair<defs::arg_type, std::string>>* arg_stack, std::size_t argc);

    extern cmd_error::Error iswap_err;
    void cmd_iswap(std::stack<std::pair<defs::arg_type, std::string>>* arg_stack, std::size_t argc);

    extern cmd_error::Error add_err;
    extern bool add_prot_err;
    memory::var_type add_prot(std::stack<std::pair<defs::arg_type, std::string>>* arg_stack); //std prot

    extern cmd_error::Error sub_err;
    extern bool sub_prot_err;
    memory::var_type sub_prot(std::stack<std::pair<defs::arg_type, std::string>>* arg_stack);
    void cmd_sub(std::stack<std::pair<defs::arg_type, std::string>>* arg_stack, std::size_t argc);

    extern cmd_error::Error mul_err;
    extern bool mul_prot_err;
    memory::var_type mul_prot(std::stack<std::pair<defs::arg_type, std::string>>* arg_stack); //std prot

    extern cmd_error::Error div_err;
    extern bool div_prot_err;
    memory::var_type div_prot(std::stack<std::pair<defs::arg_type, std::string>>* arg_stack); //std prot

    extern cmd_error::Error mod_err;
    extern bool mod_prot_err;
    memory::var_type mod_prot(std::stack<std::pair<defs::arg_type, std::string>>* arg_stack); //std prot

    extern cmd_error::Error typeof_err;
    extern bool typeof_prot_err;
    memory::var_type typeof_prot(std::stack<std::pair<defs::arg_type, std::string>>* arg_stack); //std prot

    extern cmd_error::Error at_err;
    extern bool at_prot_err;
    memory::var_type at_prot(std::stack<std::pair<defs::arg_type, std::string>>* arg_stack); //std prot

    extern cmd_error::Error rel_err;
    extern bool rel_prot_err;
    memory::var_type rel_prot(std::stack<std::pair<defs::arg_type, std::string>>* arg_stack); //std prot

    extern cmd_error::Error str_err;
    extern bool str_prot_err;
    memory::var_type str_prot(std::stack<std::pair<defs::arg_type, std::string>>* arg_stack); //std prot

    extern cmd_error::Error ceil_err;
    extern bool ceil_prot_err;
    memory::var_type ceil_prot(std::stack<std::pair<defs::arg_type, std::string>>* arg_stack); //std prot

    extern cmd_error::Error floor_err;
    extern bool floor_prot_err;
    memory::var_type floor_prot(std::stack<std::pair<defs::arg_type, std::string>>* arg_stack); //std prot

    extern cmd_error::Error round_err;
    extern bool round_prot_err;
    memory::var_type round_prot(std::stack<std::pair<defs::arg_type, std::string>>* arg_stack); //std prot

    extern cmd_error::Error process_err;
    extern std::string process_output;
    void cmd_process(std::stack<std::pair<defs::arg_type, std::string>>* arg_stack, std::size_t argc);

    extern cmd_error::Error is_err;
    extern bool is_prot_err;
    memory::var_type is_prot(std::stack<std::pair<defs::arg_type, std::string>>* arg_stack); //std prot

    extern cmd_error::Error cmp_err;
    extern bool cmp_prot_err;
    memory::var_type cmp_prot(std::stack<std::pair<defs::arg_type, std::string>>* arg_stack); //std prot

    extern cmd_error::Error if_err;
    extern std::string if_output;
    void cmd_if(std::stack<std::pair<defs::arg_type, std::string>>* arg_stack, std::size_t argc);

    extern cmd_error::Error put_err;
    void cmd_put(std::stack<std::pair<defs::arg_type, std::string>>* arg_stack, std::size_t argc);

    extern cmd_error::Error putl_err;
    void cmd_putl(std::stack<std::pair<defs::arg_type, std::string>>* arg_stack, std::size_t argc);

    extern cmd_error::Error get_err;
    memory::var_type get_prot(std::stack<std::pair<defs::arg_type, std::string>>* arg_stack); //std_prot

    extern cmd_error::Error getl_err;
    memory::var_type getl_prot(std::stack<std::pair<defs::arg_type, std::string>>* arg_stack); //std_prot

    extern cmd_error::Error fput_err;
    void cmd_fput(std::stack<std::pair<defs::arg_type, std::string>>* arg_stack, std::size_t argc);

    extern cmd_error::Error fputl_err;
    void cmd_fputl(std::stack<std::pair<defs::arg_type, std::string>>* arg_stack, std::size_t argc);

    extern cmd_error::Error fwrite_err;
    void cmd_fwrite(std::stack<std::pair<defs::arg_type, std::string>>* arg_stack, std::size_t argc);

    extern cmd_error::Error fwritel_err;
    void cmd_fwritel(std::stack<std::pair<defs::arg_type, std::string>>* arg_stack, std::size_t argc);

    extern cmd_error::Error fread_err;
    memory::var_type fread_prot(std::stack<std::pair<defs::arg_type, std::string>>* arg_stack); //std_prot
}