#include <terminal/process.hpp>

#include <common/common.hpp>
#include <terminal/memory.hpp>

#include <sstream>

#include <cstdio>

using namespace std;
using namespace defs;

namespace process{
    stack<pair<arg_type, string>> arg_stack;
    string key_words[] = {"exit", "putn", "puts", "push", "pop", "get", "index", "pushi", "popi", "sizei", "swapi"};

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
                    type = ARG_NUM;
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

        if(cmd == "exit"){
            if(arg_stack.size() == 0){
                exit(0);
            } else if(arg_stack.size() == 1){
                if(arg_stack.top().first == ARG_NUM){
                    int code = stoi(arg_stack.top().second);
                    exit(code);
                } else if(arg_stack.top().first == ARG_VAR){
                    memory::var_type buffer = memory::get_var(arg_stack.top().second);
                    if(memory::get_var_error){
                        return;
                    } else if(buffer.first != defs::VAR_NUM){
                        printf("Excepted number (exit [code:NUM]) or (exit [void])\n");
                        return;
                    }
                    int code = stoi(buffer.second);
                    exit(code);
                } else{
                    printf("Excepted number (exit [code:NUM]) or (exit [void])\n");
                }
            } else{
                printf("Too many arguments (exit [code:NUM]) or (exit [void])\n");
            }
        } else if(cmd == "putn"){
            if(arg_stack.size() == 0){
                printf("Excepted number (putn REL])\n");
            } else if(arg_stack.size() == 1){
                memory::var_type buffer;
                if(arg_stack.top().first == ARG_VAR){
                    buffer = memory::get_var(arg_stack.top().second);
                    if(memory::get_var_error){
                        return;
                    }
                } else{
                    buffer = memory::var_type((var_type)arg_stack.top().first, arg_stack.top().second);
                }

                if(buffer.first == VAR_NUM || VAR_BOOL){
                    long long n = stoll(buffer.second);
                    printf("%lli", n);
                } else if(buffer.first == VAR_DOT){
                    long double n = stold(buffer.second);
                    printf("%Lf", n);
                } else{
                    printf("Excepted number (putn [n:REL])\n");
                }
            } else{
                printf("Too many arguments (putn [n:REL])\n");
            }
        } else if(cmd == "puts"){
            if(arg_stack.size() == 0){
                printf("Excepted string (puts [s:STR])\n");
            } else if(arg_stack.size() == 1){
                if(arg_stack.top().first == ARG_STR){
                    string s = common::clear_str_format(arg_stack.top().second);
                    printf("%s", s.c_str());
                } else if(arg_stack.top().first == ARG_VAR){
                    memory::var_type buffer = memory::get_var(arg_stack.top().second);
                    if(memory::get_var_error){
                        return;
                    } else if(buffer.first == VAR_BOOL){
                        printf("%s", (buffer.second == "0")?"false":"true");
                    } else if(buffer.first != VAR_STR){
                        printf("Excepted string (puts [s:STR])\n");
                        return;
                    }
                    string s = common::clear_str_format(buffer.second);
                    printf("%s", s.c_str());
                } else{
                    printf("Excepted string (puts [s:STR])\n");
                }
            } else{
                printf("Too many arguments (puts [s:STR])\n");
            }
        } else if(cmd == "push"){
            if(arg_stack.size() == 0){
                printf("Excepted any (push [v:ANY])\n");
            } else if(arg_stack.size() == 1){
                if(arg_stack.top().first == ARG_VAR){
                    memory::var_type buffer = memory::get_var(arg_stack.top().second);
                    if(memory::get_var_error){
                        return;
                    }
                    memory::memory_stack.push(buffer);
                } else{
                    memory::memory_stack.push(memory::var_type((var_type)arg_stack.top().first, arg_stack.top().second));
                }
            } else{
                printf("Too many arguments (push [v:ANY])\n");
            }
        } else if(cmd == "pop"){
            if(arg_stack.size() == 0){
                if(memory::memory_stack.empty()){
                    printf("Memory stack is empty\n");
                } else{
                    memory::memory_stack.pop();
                }
            } else{
                printf("Too many arguments (pop [void])\n");
            }
        } else if(cmd == "get"){
            if(arg_stack.size() == 0){
                if(memory::memory_stack.empty()){
                    printf("Memory stack is empty\n");
                } else{
                    memory::_get = memory::memory_stack.top();
                }
            } else if(arg_stack.size() == 1){
                unsigned long long id = 0;
                if(arg_stack.top().first == ARG_NUM){
                    id = stoll(arg_stack.top().second);
                } else if(arg_stack.top().first == ARG_VAR){
                    memory::var_type v = memory::get_var(arg_stack.top().second);
                    if(v.first == defs::VAR_NUM){
                        id = stoll(v.second);
                    } else{
                        printf("Excepted number (get [void]) or (get [id:NUM])\n");
                        return;
                    }
                } else{
                    printf("Excepted number (get [void]) or (get [id:NUM])\n");
                    return;
                }

                if(memory::memory_stack.empty()){
                    printf("Memory stack is empty\n");
                } else{
                    memory::set_index(id, memory::memory_stack.top());
                }
            } else{
                printf("Too many arguments (get [void]) or (get [id:NUM])\n");
            }
        } else if(cmd == "index"){
            if(arg_stack.size() == 0){
                printf("Too few arguments (index [id:NUM]) or (index [id:NUM] [v:ANY])\n");
            } else if(arg_stack.size() == 1){
                if(arg_stack.top().first == ARG_NUM){
                    memory::_get = memory::get_var("$" + arg_stack.top().second);
                } else if(arg_stack.top().first == ARG_VAR){
                    memory::var_type id = memory::get_var(arg_stack.top().second);
                    if(memory::get_var_error){
                        return;
                    }

                    if(id.first == defs::VAR_NUM){
                        memory::_get = memory::get_var("$" + id.second);
                    } else{
                        printf("Excepted number (index [id:NUM]) or (index [id:NUM] [v:ANY])\n");
                    }
                } else{
                    printf("Excepted number (index [id:NUM]) or (index [id:NUM] [v:ANY])\n");
                }
            } else if(arg_stack.size() == 2){
                memory::var_type v;
                if(arg_stack.top().first == ARG_VAR){
                    v = memory::get_var(arg_stack.top().second);
                    if(memory::get_var_error){
                        return;
                    }
                } else{
                    v = memory::var_type((defs::var_type)arg_stack.top().first, arg_stack.top().second);
                }

                arg_stack.pop();

                unsigned long long id;
                if(arg_stack.top().first == ARG_NUM){
                    id = stoll(arg_stack.top().second);
                } else if(arg_stack.top().first == ARG_VAR){
                    memory::var_type i = memory::get_var(arg_stack.top().second);
                    if(memory::get_var_error){
                        return;
                    }

                    if(i.first == defs::VAR_NUM){
                        id = stoll(i.second);
                    } else{
                        printf("Excepted number (index [id:NUM]) or (index [id:NUM] [v:ANY])\n");
                        return;
                    }
                } else{
                    printf("Excepted number (index [id:NUM]) or (index [id:NUM] [v:ANY])\n");
                    return;
                }

                memory::set_index(id, v);
            } else{
                printf("Too many arguments (index [id:NUM]) or (index [id:NUM] [v:ANY])\n");
            }
        } else if(cmd == "pushi"){
            if(arg_stack.size() == 0){
                printf("Too few arguments (pushi [v:ANY])\n");
            } else if(arg_stack.size() == 1){
                if(arg_stack.top().first == ARG_VAR){
                    memory::index_memory.push_back(memory::get_var(arg_stack.top().second));
                    if(memory::get_var_error){
                        memory::index_memory.pop_back();
                    }
                } else{
                    memory::index_memory.push_back(memory::var_type((var_type)arg_stack.top().first, arg_stack.top().second));
                }
            } else{
                printf("Too many arguments (pushi [v:ANY])\n");
            }
        } else if(cmd == "popi"){
            if(arg_stack.size() == 0){
                if(memory::index_memory.empty()){
                    printf("There is no inited index\n");
                } else{
                    memory::index_memory.pop_back();
                }
            } else{
                printf("Too many arguments (popi [VOID])\n");
            }
        } else if(cmd == "sizei"){
            if(arg_stack.size() == 0){
                memory::_get = memory::var_type(VAR_NUM, to_string(memory::index_memory.size()));
            } else if(arg_stack.size() == 1){
                if(arg_stack.top().first == ARG_NUM){
                    memory::set_index(stoll(arg_stack.top().second), memory::var_type(VAR_NUM, to_string(memory::index_memory.size())));
                } else if(arg_stack.top().first == ARG_NUM){
                    memory::var_type id = memory::get_var(arg_stack.top().second);
                    if(memory::get_var_error){
                        return;
                    } else if(id.first == VAR_NUM){
                        memory::set_index(stoll(id.second), memory::var_type(VAR_NUM, to_string(memory::index_memory.size())));
                    } else{
                        printf("Excepted number (sizei [VOID]) or (sizei [id:NUM])\n");
                    }
                } else{
                    printf("Excepted number (sizei [VOID]) or (sizei [id:NUM])\n");
                }
            } else{
                printf("Too many arguments (sizei [VOID]) or (sizei [id:NUM])\n");
            }
        } else if(cmd == "swapi"){
            if(arg_stack.size() <= 1){
                printf("Too few arguments (sizei [id1:NUM] [id2:NUM])\n");
            } else if(arg_stack.size() == 2){
                unsigned long long id1;
                unsigned long long id2;

                if(arg_stack.top().first == ARG_NUM){
                    id2 = stoll(arg_stack.top().second);
                } else if(arg_stack.top().first == ARG_VAR){
                    memory::var_type i = memory::get_var(arg_stack.top().second);
                    if(memory::get_var_error){
                        return;
                    } else if(i.first == VAR_NUM){
                        id2 = stoll(i.second);
                    } else{
                        printf("Excepted number (swapi [id1:NUM] [id2:NUM])\n");
                        return;
                    }
                }   else{
                    printf("Excepted number (swapi [id1:NUM] [id2:NUM])\n");
                    return;
                }

                arg_stack.pop();

                if(arg_stack.top().first == ARG_NUM){
                    id1 = stoll(arg_stack.top().second);
                } else if(arg_stack.top().first == ARG_VAR){
                    memory::var_type i = memory::get_var(arg_stack.top().second);
                    if(memory::get_var_error){
                        return;
                    } else if(i.first == VAR_NUM){
                        id1 = stoll(i.second);
                    } else{
                        printf("Excepted number (swapi [id1:NUM] [id2:NUM])\n");
                        return;
                    }
                }   else{
                    printf("Excepted number (swapi [id1:NUM] [id2:NUM])\n");
                    return;
                }

                memory::var_type i1v = memory::get_var("$" + to_string(id1)); if(memory::get_var_error) return;
                memory::var_type i2v = memory::get_var("$" + to_string(id2)); if(memory::get_var_error) return;
                if(!memory::set_index(id1, i2v)) return;
                if(!memory::set_index(id2, i1v)) return;
            } else{
                printf("Too many arguments (sizei [id1:NUM] [id2:NUM])\n");
            }
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