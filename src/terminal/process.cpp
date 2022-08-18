#include <terminal/process.hpp>

#include <common/common.hpp>
#include <terminal/memory.hpp>
#include <terminal/cmd.hpp>

#include <sstream>

#include <cstdio>

using namespace std;
using namespace defs;

namespace process{
    stack<pair<arg_type, string>> arg_stack;
    string key_words[] = {"exit", "putn", "puts", "push", "pop", "stack", "size", "ipush", "ipop", "index", "isize", "iswap", "add", "sub", "mul", "div", "mod", "typeof", "at", "rel", "str", "ceil", "floor", "round"};

    void processInput(string input){
        if(input.empty())
            return;

        string cmd = "";
        string args = "";
        string next = "";

        bool get_args = false;
        bool arg_str = false;
        bool arg_str_d = false;
        bool arg_str_force = false;
        bool get_next = false;

        stringstream ss(input);
        char c = '\0';
        char last = '\0';
        while(ss.get(c)){
            if(c == '\t')
                continue;
            
            if(get_next){
                next += c;
            } else if(get_args){
                if(arg_str){
                    if(arg_str_force){
                        arg_str_force = false;
                    } else if(c == '\''){
                        arg_str = false;
                        last = '\0';
                    } else if(c == '\\'){
                        arg_str_force = true;
                    }
                    args += c;
                } else if(arg_str_d){
                    if(arg_str_force){
                        arg_str_force = false;
                    } else if(c == '\"'){
                        arg_str_d = false;
                        last = '\0';
                        args += '\'';
                        continue;
                    } else if(c == '\\'){
                        arg_str_force = true;
                    } else if(c == '\''){
                        args += '\\';
                    }
                    args += c;
                } else if(c == ';'){
                    get_next = true;
                } else if(c == '\''){
                    arg_str = true;
                    args += '\'';
                } else if(c == '\"'){
                    arg_str_d = true;
                    args += '\'';
                } else if(c == ' ' && last == ' '){
                    continue;
                } else{
                    args += c;
                }
                last = c;
            } else{
                if((c == ' '  || c == ';') && cmd.empty()){
                    continue;
                } else if(c == ' '){
                    get_args = true;
                } else if(c == ';'){
                    get_next = true;
                } else{
                    cmd += c;
                }
            }
        }
        
        processCmd(cmd, args);
        clearArgStack();
        processInput(next);
    }

    void processArgs(string args){
        string arg = "";
        arg_type type = ARG_VAR;

        bool get_str = false;
        bool str_force = false;

        bool get_num = false;
        bool num_force = false;

        bool get_var = false;
        bool get_index = false;

        bool arg_error = false;
        bool arg_except_error = false;

        stringstream ss(args);
        char c = '\0';
        while(ss.get(c)){
            if(arg_error && c != ' ')
                continue;
            
            if(get_str){
                if(str_force){
                    str_force = false;
                } else if(c == '\\'){
                    str_force = true;
                } else if(c == '\''){
                    get_str = false;
                    arg_except_error = true;
                }
                arg += c;
            } else if((get_num || get_index) && c != ' '){
                if(common::is_num(c)){
                    arg += c;
                } else if(num_force && c == '.'){
                    arg += c;
                    type = ARG_DOT;
                    num_force = false;
                } else{
                    arg_error = true;
                    printf("Excepted number (arg=\'%s\' c=\'%c\')\n", arg.c_str(), c);
                }
            } else if(c == '\''){
                if(get_var){
                    arg_error = true;
                    printf("Excepted varible name (arg=\'%s\' c=\'%c\')\n", arg.c_str(), c);
                    continue;
                }
                get_str = true;
                str_force = false;
                type = ARG_STR;
                arg += c;
            } else if(c == '$'){
                if(get_var){
                    arg_error = true;
                    printf("Character \'$\' is not allowed in varible name\n");
                    continue;
                }
                get_index = true;
                num_force = false;
                arg += c;
            } else if(common::is_num(c) || c == '-'){
                if(!get_var){
                    get_num = true;
                    num_force = true;
                    type = ARG_INT;
                }
                arg += c;
            } else if(c == ' '){
                if(!arg.empty())
                    arg_stack.push(pair<arg_type, string>(type, arg));
                type = ARG_VAR;
                arg = "";
                arg_error = false;
                arg_except_error = false;

                get_var = false;
                get_num = false;
                get_index = false;
            } else if(arg_except_error){
                arg_error = true;
                printf("Excepted space (after arg=\'%s\' c=\'%c\')\n", arg.c_str(), c);
            } else{
                get_var = true;
                arg += c;
            }
        }

        if(!arg.empty()){
            arg_stack.push(pair<arg_type, string>(type, arg));
        }
    }

    void processCmd(string cmd, string args){
        cmd = common::str_to_lower(cmd);

        bool is_key_word = false;
        for(string kw : key_words){
            if(kw == cmd){
                is_key_word = true;
                break;
            }
        }

        if(!is_key_word){
            printf("\'%s\' is not a key word\n", cmd.c_str());
            return;
        }

        processArgs(args);

        if(cmd == key_words[0]){ //exit
            cmd::cmd_exit(&arg_stack, arg_stack.size());
        } else if(cmd == key_words[1]){ //putn
            cmd::cmd_putn(&arg_stack, arg_stack.size());
        } else if(cmd == key_words[2]){ //puts
            cmd::cmd_puts(&arg_stack, arg_stack.size());
        } else if(cmd == key_words[3]){ //push
            cmd::cmd_push(&arg_stack, arg_stack.size());
        } else if(cmd == key_words[4]){ //pop
            cmd::cmd_pop(&arg_stack, arg_stack.size());
        } else if(cmd == key_words[5]){ //stack
            cmd::cmd_stack(&arg_stack, arg_stack.size());
        } else if(cmd == key_words[6]){ //size
            cmd::cmd_size(&arg_stack, arg_stack.size());
        } else if(cmd == key_words[7]){ //ipush
            cmd::cmd_ipush(&arg_stack, arg_stack.size());
        } else if(cmd == key_words[8]){ //ipop
            cmd::cmd_ipop(&arg_stack, arg_stack.size());
        } else if(cmd == key_words[9]){ //index
            cmd::cmd_index(&arg_stack, arg_stack.size());
        } else if(cmd == key_words[10]){ //isize
            cmd::cmd_isize(&arg_stack, arg_stack.size());
        } else if(cmd == key_words[11]){ //iswap
            cmd::cmd_iswap(&arg_stack, arg_stack.size());
        } else if(cmd == key_words[12]){ //add
            cmd::std_cmd_call(&arg_stack, arg_stack.size(), &cmd::add_err, &cmd::add_prot, &cmd::add_prot_err, 2);
        } else if(cmd == key_words[13]){ //sub
            cmd::cmd_sub(&arg_stack, arg_stack.size());
        } else if(cmd == key_words[14]){ //mul
            cmd::std_cmd_call(&arg_stack, arg_stack.size(), &cmd::mul_err, &cmd::mul_prot, &cmd::mul_prot_err, 2);
        } else if(cmd == key_words[15]){ //div
            cmd::std_cmd_call(&arg_stack, arg_stack.size(), &cmd::div_err, &cmd::div_prot, &cmd::div_prot_err, 2);
        } else if(cmd == key_words[16]){ //mod
            cmd::std_cmd_call(&arg_stack, arg_stack.size(), &cmd::mod_err, &cmd::mod_prot, &cmd::mod_prot_err, 2);
        } else if(cmd == key_words[17]){ //typeof
            cmd::std_cmd_call(&arg_stack, arg_stack.size(), &cmd::typeof_err, &cmd::typeof_prot, &cmd::typeof_prot_err, 1);
        } else if(cmd == key_words[18]){ //at
            cmd::std_cmd_call(&arg_stack, arg_stack.size(), &cmd::at_err, &cmd::at_prot, &cmd::at_prot_err, 2);
        } else if(cmd == key_words[19]){ //rel
            cmd::std_cmd_call(&arg_stack, arg_stack.size(), &cmd::rel_err, &cmd::rel_prot, &cmd::rel_prot_err, 1);
        } else if(cmd == key_words[20]){ //str
            cmd::std_cmd_call(&arg_stack, arg_stack.size(), &cmd::str_err, &cmd::str_prot, &cmd::str_prot_err, 1);
        } else if(cmd == key_words[21]){ //ceil
            cmd::std_cmd_call(&arg_stack, arg_stack.size(), &cmd::ceil_err, &cmd::ceil_prot, &cmd::ceil_prot_err, 1);
        } else if(cmd == key_words[22]){ //floor
            cmd::std_cmd_call(&arg_stack, arg_stack.size(), &cmd::floor_err, &cmd::floor_prot, &cmd::floor_prot_err, 1);
        } else if(cmd == key_words[23]){ //round
            cmd::std_cmd_call(&arg_stack, arg_stack.size(), &cmd::round_err, &cmd::round_prot, &cmd::round_prot_err, 1);
        }
        else{
            printf("Unexcepted key word (this may be a deweloper/undoned key)\n");
        }
    }

    void clearArgStack(){
        while(arg_stack.size())
            arg_stack.pop();
    }
}