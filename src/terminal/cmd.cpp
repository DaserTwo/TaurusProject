#include <terminal/cmd.hpp>

#include <common/common.hpp>

#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;
using namespace defs;

namespace cmd{
    bool _prot_err = false;
    void std_cmd_call(stack<pair<arg_type, string>>* arg_stack, size_t argc, cmd_error::Error* err, memory::var_type(*cmd_prot)(stack<pair<arg_type, string>>* arg_stack), bool* cmd_prot_err, int arg_prot_c){
        if(argc < arg_prot_c){
            err->too_few_arg();
        } else if(argc == arg_prot_c){
            memory::var_type g = cmd_prot(arg_stack);
            if(*cmd_prot_err) return;

            memory::_get = g;
        } else if(argc == arg_prot_c + 1){
            unsigned long long id;
            
            memory::var_type g = cmd_prot(arg_stack);
            if(*cmd_prot_err) return;

            if(arg_prot_c != 0){
                arg_stack->pop();
            }

            if(arg_stack->top().first == ARG_INT){
                id = stoll(arg_stack->top().second);
            } else if(arg_stack->top().first == ARG_VAR){
                memory::var_type v = memory::get_var(arg_stack->top().second);
                if(memory::get_var_error) return;
                else if(v.first == VAR_INT){
                    id = stoll(v.second);
                } else{
                    err->bad_arg();
                    return;
                }
            } else{
                err->bad_arg();
                return;
            }
            
            memory::set_index(id, g);
        } else{
            err->too_many_arg();
        }
    }

    void std_cmd_call_std_err(stack<pair<arg_type, string>>* arg_stack, size_t argc, cmd_error::Error* err, memory::var_type(*cmd_prot)(stack<pair<arg_type, string>>* arg_stack), int arg_prot_c){
        _prot_err = false;
        std_cmd_call(arg_stack, argc, err, cmd_prot, &_prot_err, arg_prot_c);
    }

    cmd_error::Error exit_err("(exit {code:INT})");
    void cmd_exit(stack<pair<arg_type, string>>* arg_stack, size_t argc){
        if(argc == 0){
            exit(0);
        } else if(argc == 1){
            if(arg_stack->top().first == ARG_INT){
                int code = stoi(arg_stack->top().second);
                exit(code);
            } else if(arg_stack->top().first == ARG_VAR){
                memory::var_type buffer = memory::get_var(arg_stack->top().second);
                if(memory::get_var_error){
                    return;
                } else if(buffer.first != VAR_INT){
                    exit_err.exc_int();
                    return;
                }
                int code = stoi(buffer.second);
                exit(code);
            } else{
                exit_err.exc_int();
            }
        } else{
            exit_err.too_many_arg();
        }
    }

    cmd_error::Error push_err("(push [v:ANY])");
    void cmd_push(stack<pair<arg_type, string>>* arg_stack, size_t argc){
        if(argc == 0){
            push_err.too_few_arg();
        } else if(argc == 1){
            if(arg_stack->top().first == ARG_VAR){
                memory::var_type buffer = memory::get_var(arg_stack->top().second);
                if(memory::get_var_error){
                    return;
                }
                memory::memory_stack.push(buffer);
            } else{
                memory::memory_stack.push(memory::var_type((var_type)arg_stack->top().first, arg_stack->top().second));
            }
        } else{
            push_err.too_many_arg();
        }
    }

    cmd_error::Error pop_err("(pop [VOID])");
    void cmd_pop(stack<pair<arg_type, string>>* arg_stack, size_t argc){
        if(argc == 0){
            if(memory::memory_stack.empty()){
                printf("Memory stack is empty\n");
            } else{
                memory::memory_stack.pop();
            }
        } else{
            pop_err.too_many_arg();
        }
    }

    cmd_error::Error stack_err("(stack {id:INT})");
    void cmd_stack(stack<pair<arg_type, string>>* arg_stack, size_t argc){
        if(argc == 0){
            if(memory::memory_stack.empty()){
                printf("Memory stack is empty\n");
            } else{
                memory::_get = memory::memory_stack.top();
            }
        } else if(argc == 1){
            unsigned long long id = 0;
            if(arg_stack->top().first == ARG_INT){
                id = stoll(arg_stack->top().second);
            } else if(arg_stack->top().first == ARG_VAR){
                memory::var_type v = memory::get_var(arg_stack->top().second);
                if(v.first == VAR_INT){
                    id = stoll(v.second);
                } else{
                    stack_err.exc_int();
                    return;
                }
            } else{
                stack_err.exc_int();
                return;
            }

            if(memory::memory_stack.empty()){
                printf("Memory stack is empty\n");
            } else{
                memory::set_index(id, memory::memory_stack.top());
            }
        } else{
            stack_err.too_many_arg();
        }
    }

    cmd_error::Error size_err("(size {id:INT})");
    void cmd_size(stack<pair<arg_type, string>>* arg_stack, size_t argc){
        if(argc == 0){
            memory::_get = memory::var_type(VAR_INT, to_string(memory::memory_stack.size()));
        } else if(argc == 1){
            unsigned long long id = 0;
            if(arg_stack->top().first == ARG_INT){
                id = stoll(arg_stack->top().second);
            } else if(arg_stack->top().first == ARG_VAR){
                memory::var_type v = memory::get_var(arg_stack->top().second);
                if(v.first == VAR_INT){
                    id = stoll(v.second);
                } else{
                    size_err.exc_int();
                    return;
                }
            } else{
                size_err.exc_int();
                return;
            }

            memory::set_index(id, memory::var_type(VAR_INT, to_string(memory::memory_stack.size())));
        } else{
            size_err.too_many_arg();
        }
    }

    cmd_error::Error ipush_err("(ipush [v:ANY])");
    void cmd_ipush(stack<pair<arg_type, string>>* arg_stack, size_t argc){
        if(argc == 0){
            ipush_err.too_few_arg();
        } else if(argc == 1){
            if(arg_stack->top().first == ARG_VAR){
                memory::index_memory.push_back(memory::get_var(arg_stack->top().second));
                if(memory::get_var_error){
                    memory::index_memory.pop_back();
                }
            } else{
                memory::index_memory.push_back(memory::var_type((var_type)arg_stack->top().first, arg_stack->top().second));
            }
        } else{
            ipush_err.too_many_arg();
        }
    }

    cmd_error::Error ipop_err("(ipop [VOID])");
    void cmd_ipop(stack<pair<arg_type, string>>* arg_stack, size_t argc){
        if(argc == 0){
            if(memory::index_memory.empty()){
                printf("There is no inited index\n");
            } else{
                memory::index_memory.pop_back();
            }
        } else{
            ipop_err.too_many_arg();
        }
    }

    cmd_error::Error index_err("(index [id:INT] {v:ANY})");
    void cmd_index(stack<pair<arg_type, string>>* arg_stack, size_t argc){
        if(argc == 0){
            index_err.too_few_arg();
        } else if(argc == 1){
            if(arg_stack->top().first == ARG_INT){
                memory::_get = memory::get_var("$" + arg_stack->top().second);
            } else if(arg_stack->top().first == ARG_VAR){
                memory::var_type id = memory::get_var(arg_stack->top().second);
                if(memory::get_var_error){
                    return;
                }

                if(id.first == VAR_INT){
                    memory::_get = memory::get_var("$" + id.second);
                } else{
                    index_err.exc_int();
                }
            } else{
                index_err.exc_int();
            }
        } else if(argc == 2){
            memory::var_type v;
            if(arg_stack->top().first == ARG_VAR){
                v = memory::get_var(arg_stack->top().second);
                if(memory::get_var_error){
                    return;
                }
            } else{
                v = memory::var_type((var_type)arg_stack->top().first, arg_stack->top().second);
            }

            arg_stack->pop();

            unsigned long long id;
            if(arg_stack->top().first == ARG_INT){
                id = stoll(arg_stack->top().second);
            } else if(arg_stack->top().first == ARG_VAR){
                memory::var_type i = memory::get_var(arg_stack->top().second);
                if(memory::get_var_error){
                    return;
                }

                if(i.first == VAR_INT){
                    id = stoll(i.second);
                } else{
                    index_err.exc_int();
                    return;
                }
            } else{
                index_err.exc_int();
                return;
            }
            memory::set_index(id, v);
        } else{
            index_err.too_many_arg();
        }
    }

    cmd_error::Error isize_err("(isize {id:INT})");
    void cmd_isize(stack<pair<arg_type, string>>* arg_stack, size_t argc){
        if(argc == 0){
            memory::_get = memory::var_type(VAR_INT, to_string(memory::index_memory.size()));
        } else if(argc == 1){
            if(arg_stack->top().first == ARG_INT){
                memory::set_index(stoll(arg_stack->top().second), memory::var_type(VAR_INT, to_string(memory::index_memory.size())));
            } else if(arg_stack->top().first == ARG_INT){
                memory::var_type id = memory::get_var(arg_stack->top().second);
                if(memory::get_var_error){
                    return;
                } else if(id.first == VAR_INT){
                    memory::set_index(stoll(id.second), memory::var_type(VAR_INT, to_string(memory::index_memory.size())));
                } else{
                    isize_err.exc_int();
                }
            } else{
                isize_err.exc_int();
            }
        } else{
            isize_err.too_many_arg();
        }
    }

    cmd_error::Error iswap_err("(iswap [id1:INT] [id2:INT])");
    void cmd_iswap(stack<pair<arg_type, string>>* arg_stack, size_t argc){
        if(argc <= 1){
            iswap_err.too_few_arg();
        } else if(argc == 2){
            unsigned long long id1;
            unsigned long long id2;

            if(arg_stack->top().first == ARG_INT){
                id2 = stoll(arg_stack->top().second);
            } else if(arg_stack->top().first == ARG_VAR){
                memory::var_type i = memory::get_var(arg_stack->top().second);
                if(memory::get_var_error){
                    return;
                } else if(i.first == VAR_INT){
                    id2 = stoll(i.second);
                } else{
                    iswap_err.exc_int();
                    return;
                }
            } else{
                iswap_err.exc_int();
                return;
            }

            arg_stack->pop();

            if(arg_stack->top().first == ARG_INT){
                id1 = stoll(arg_stack->top().second);
            } else if(arg_stack->top().first == ARG_VAR){
                memory::var_type i = memory::get_var(arg_stack->top().second);
                if(memory::get_var_error){
                    return;
                } else if(i.first == VAR_INT){
                    id1 = stoll(i.second);
                } else{
                    iswap_err.exc_int();
                    return;
                }
            } else{
                iswap_err.exc_int();
                return;
            }

            memory::var_type i1v = memory::get_var("$" + to_string(id1)); if(memory::get_var_error) return;
            memory::var_type i2v = memory::get_var("$" + to_string(id2)); if(memory::get_var_error) return;

            if(!memory::set_index(id1, i2v)) return;
            if(!memory::set_index(id2, i1v)) return;
        } else{
            iswap_err.too_many_arg();
        }
    }

    cmd_error::Error add_err("(add {id:INT} [v1:REL] [v2:REL]) or (add {id:INT} [v1:STR] [v2:ANY])");
    bool add_prot_err = false;
    memory::var_type add_prot(stack<pair<arg_type, string>>* arg_stack){
        add_prot_err = false;
        memory::var_type v1;
        memory::var_type v2;

        if(arg_stack->top().first == ARG_VAR){
            v2 = memory::get_var(arg_stack->top().second);
            if(memory::get_var_error){
                add_prot_err = true;
                return memory::var_type();
            }
        } else{
            v2 = memory::var_type((var_type)arg_stack->top().first, arg_stack->top().second);
        }

        arg_stack->pop();
        if(arg_stack->top().first == ARG_VAR){
            v1 = memory::get_var(arg_stack->top().second);
            if(memory::get_var_error){
                add_prot_err = true;
                return memory::var_type();
            }
        } else{
            v1 = memory::var_type((var_type)arg_stack->top().first, arg_stack->top().second);
        }

        if(v1.first == VAR_STR){
            string av = v1.second;
            string bv = v2.second;
            if(v2.first == VAR_BOOL)
                bv = (v2.second == "0")?"false":"true";
            else if(v2.first == VAR_TYPE)
                bv = (v2.second == "0")?"INT":((v2.second == "1")?"DOT":((v2.second == "2")?"STR":((v2.second == "3")?"BOOL":((v2.second == "4")?"TYPE":"ANY"))));
            else if(v2.first == VAR_RELATION)
                bv = (v2.second == "0")?"equal":((v2.second == "1")?"bigger":((v2.second == "2")?"smaller":"not equal"));
            return memory::var_type(VAR_STR, av + bv);
        } else if(v1.first == VAR_INT){
            if(v2.first == VAR_INT){
                return memory::var_type(VAR_INT, to_string(stoll(v1.second) + stoll(v2.second)));
            } else if(v2.first == VAR_DOT){
                return memory::var_type(VAR_DOT, to_string(stoll(v1.second) + stold(v2.second)));
            } else{
                add_err.bad_arg();
            }
        } else if(v1.first == VAR_DOT){
            if(v2.first == VAR_INT || v2.first == VAR_DOT){
                return memory::var_type(VAR_DOT, to_string(stold(v1.second) + stold(v2.second)));
            } else{
                add_err.bad_arg();
            }
        } else{
            add_err.bad_arg();
        }

        add_prot_err = true;
        return memory::var_type();
    }

    cmd_error::Error sub_err("(sub {id:INT} [v1:REL] [v2:REL]) or (sub {id:INT} [v1:STR] [begin:INT] {end:INT})");
    bool sub_prot_err = false;
    memory::var_type sub_prot(stack<pair<arg_type, string>>* arg_stack){
        sub_prot_err = false;

        memory::var_type v1;
        memory::var_type v2;

        if(arg_stack->top().first == ARG_VAR){
            v2 = memory::get_var(arg_stack->top().second);
            if(memory::get_var_error){
                sub_prot_err = true;
                return memory::var_type();
            }
        } else{
            v2 = memory::var_type((var_type)arg_stack->top().first, arg_stack->top().second);
        }

        arg_stack->pop();
        if(arg_stack->top().first == ARG_VAR){
            v1 = memory::get_var(arg_stack->top().second);
            if(memory::get_var_error){
                sub_prot_err = true;
                return memory::var_type();
            }
        } else{
            v1 = memory::var_type((var_type)arg_stack->top().first, arg_stack->top().second);
        }

        if(v1.first == VAR_STR){
            string str = v1.second;
            if(v2.first == VAR_INT){
                unsigned long long begin = stoll(v2.second);
                if(begin >= str.length()){
                    printf("Begin index cannot be biger than string length\n");
                } else{
                    return memory::var_type(VAR_STR, str.substr(begin));
                }
            } else{
                sub_err.exc_int();
            }
        } else if(v1.first == VAR_INT){
            if(v2.first == VAR_INT){
                return memory::var_type(VAR_INT, to_string(stoll(v1.second) - stoll(v2.second)));
            } else if(v2.first == VAR_DOT){
                return memory::var_type(VAR_DOT, to_string(stoll(v1.second) - stold(v2.second)));
            } else{
                sub_err.bad_arg();
            }
        } else if(v1.first == VAR_DOT){
            if(v2.first == VAR_INT || v2.first == VAR_DOT){
                return memory::var_type(VAR_DOT, to_string(stold(v1.second) - stold(v2.second)));
            } else{
                sub_err.bad_arg();
            }
        } else{
            sub_err.bad_arg();
        }
        sub_prot_err = true;
        return memory::var_type();
    }

    void cmd_sub(stack<pair<arg_type, string>>* arg_stack, size_t argc){
        if(argc <= 1){
            sub_err.too_few_arg();
        } else if(argc == 2){
            memory::var_type g = sub_prot(arg_stack);
            if(sub_prot_err) return;
            else memory::_get = g;
        } else if(argc == 3){
            memory::var_type v1;
            memory::var_type v2;
            memory::var_type v3;

            if(arg_stack->top().first == ARG_VAR){
                v3 = memory::get_var(arg_stack->top().second);
                if(memory::get_var_error) return;
            } else{
                v3 = memory::var_type((var_type)arg_stack->top().first, arg_stack->top().second);
            }

            memory::var_type g = sub_prot(arg_stack);
            if(arg_stack->top().first == ARG_VAR){
                v2 = memory::get_var(arg_stack->top().second);
                if(memory::get_var_error) return;
            } else{
                v2 = memory::var_type((var_type)arg_stack->top().first, arg_stack->top().second);
            }

            arg_stack->pop();
            if(arg_stack->top().first == ARG_VAR){
                v1 = memory::get_var(arg_stack->top().second);
                if(memory::get_var_error) return;
            } else{
                v1 = memory::var_type((var_type)arg_stack->top().first, arg_stack->top().second);
            }

            if(v1.first == VAR_STR){
                string str = v1.second;
                if(v2.first == VAR_INT){
                    if(v3.first == VAR_INT){
                        unsigned long long begin = stoll(v2.second);
                        unsigned long long end = stoll(v3.second);
                        if(begin >= str.length()){
                            printf("Begin index cannot be biger than string length\n");
                        } else if(end >= str.length()){
                            printf("End index cannot be biger than string length\n");
                        } else{
                            memory::_get = memory::var_type(VAR_STR, str.substr(begin, end));
                        }
                    } else{
                        sub_err.exc_int();
                    }
                } else{
                    sub_err.exc_int();
                }
            } else if(v1.first == VAR_INT){
                if(sub_prot_err) return;
                unsigned long long id = stoll(v1.second);
                memory::set_index(id, g);
            } else{
                sub_err.bad_arg();
            }
        } else if(argc == 4){
            unsigned long long id = 0;
            memory::var_type str;
            memory::var_type begin;
            memory::var_type end;
            
            if(arg_stack->top().first == ARG_VAR){
                end = memory::get_var(arg_stack->top().second);
                if(memory::get_var_error) return;
            } else{
                end = memory::var_type((var_type)arg_stack->top().first, arg_stack->top().second);
            }
            arg_stack->pop();
            if(arg_stack->top().first == ARG_VAR){
                begin = memory::get_var(arg_stack->top().second);
                if(memory::get_var_error) return;
            } else{
                begin = memory::var_type((var_type)arg_stack->top().first, arg_stack->top().second);
            }
            arg_stack->pop();
            if(arg_stack->top().first == ARG_VAR){
                str = memory::get_var(arg_stack->top().second);
                if(memory::get_var_error) return;
            } else{
                str = memory::var_type((var_type)arg_stack->top().first, arg_stack->top().second);
            }
            arg_stack->pop();
            if(arg_stack->top().first == ARG_INT){
                id = stoll(arg_stack->top().second);
            } else if(arg_stack->top().first == ARG_VAR){
                memory::var_type v = memory::get_var(arg_stack->top().second);
                if(memory::get_var_error) return;
                else if(v.first == VAR_INT){
                    id = stoll(v.second);
                } else{
                    sub_err.bad_arg();
                    return;
                }
            } else{
                sub_err.bad_arg();
                return;
            }
            if(str.first == VAR_STR){
                string s = str.second;
                if(begin.first == VAR_INT && end.first == VAR_INT){
                    unsigned long long b = stoll(begin.second);
                    unsigned long long e = stoll(end.second);
                    if(b >= s.length()){
                        printf("Begin index cannot be biger than string length\n");
                    } else if(e >= s.length()){
                        printf("End index cannot be biger than string length\n");
                    } else{
                        memory::set_index(id, memory::var_type(VAR_STR, s.substr(b, e)));
                    }
                } else{
                    sub_err.exc_int();
                }
            } else{
                sub_err.exc_str();
            }
        } else{
            sub_err.too_many_arg();
        }
    }

    cmd_error::Error mul_err("(mul {id:INT} [v1:REL] [v2:REL]) or (mul {id:INT} [str:STR] [times:INT])");
    bool mul_prot_err = false;
    memory::var_type mul_prot(stack<pair<arg_type, string>>* arg_stack){
        mul_prot_err = false;

        memory::var_type v1;
        memory::var_type v2;

        if(arg_stack->top().first == ARG_VAR){
            v2 = memory::get_var(arg_stack->top().second);
            if(memory::get_var_error){
                mul_prot_err = true;
                return memory::var_type();
            }
        } else{
            v2 = memory::var_type((var_type)arg_stack->top().first, arg_stack->top().second);
        }

        arg_stack->pop();
        if(arg_stack->top().first == ARG_VAR){
            v1 = memory::get_var(arg_stack->top().second);
            if(memory::get_var_error){
                mul_prot_err = true;
                return memory::var_type();
            }
        } else{
            v1 = memory::var_type((var_type)arg_stack->top().first, arg_stack->top().second);
        }

        if(v1.first == VAR_STR){
            if(v2.first == VAR_INT){
                string get = "";
                long long times = stoll(v2.second);
                while(times > 0){
                    get += v1.second;
                    times--;
                }
                return memory::var_type(VAR_STR, get + "");
            } else{
                mul_err.exc_int();
            }
        } else if(v1.first == VAR_INT){
            if(v2.first == VAR_INT){
                return memory::var_type(VAR_INT, to_string(stoll(v1.second) * stoll(v2.second)));
            } else if(v2.first == VAR_DOT){
                return memory::var_type(VAR_DOT, to_string(stoll(v1.second) * stold(v2.second)));
            } else{
                mul_err.bad_arg();
            }
        } else if(v1.first == VAR_DOT){
            if(v2.first == VAR_INT || v2.first == VAR_DOT){
                return memory::var_type(VAR_DOT, to_string(stoll(v1.second) * stoll(v2.second)));
            } else{
                mul_err.bad_arg();
            }
        } else{
            mul_err.bad_arg();
        }
        
        mul_prot_err = true;
        return memory::var_type();
    }

    cmd_error::Error div_err("(div {id:INT} [v1:REL] [v2:REL])");
    bool div_prot_err = false;
    memory::var_type div_prot(stack<pair<arg_type, string>>* arg_stack){
        div_prot_err = false;

        memory::var_type v1;
        memory::var_type v2;
        if(arg_stack->top().first == ARG_VAR){
            v2 = memory::get_var(arg_stack->top().second);
            if(memory::get_var_error){
                div_prot_err = true;
                return memory::var_type();
            }
        } else{
            v2 = memory::var_type((var_type)arg_stack->top().first, arg_stack->top().second);
        }

        arg_stack->pop();
        if(arg_stack->top().first == ARG_VAR){
            v1 = memory::get_var(arg_stack->top().second);
            if(memory::get_var_error){
                div_prot_err = true;
                return memory::var_type();
            }
        } else{
            v1 = memory::var_type((var_type)arg_stack->top().first, arg_stack->top().second);
        }

        if(v1.first == VAR_INT){
            if(v2.first == VAR_INT){
                return memory::var_type(VAR_INT, to_string(stoll(v1.second) / stoll(v2.second)));
            } else if(v2.first == VAR_DOT){
                return memory::var_type(VAR_DOT, to_string(stoll(v1.second) / stold(v2.second)));
            } else{
                div_err.bad_arg();
            }
        } else if(v1.first == VAR_DOT){
            if(v2.first == VAR_INT || v2.first == VAR_DOT){
                return memory::var_type(VAR_DOT, to_string(stoll(v1.second) / stoll(v2.second)));
            } else{
                div_err.bad_arg();
            }
        } else{
            div_err.bad_arg();
        }
        
        div_prot_err = true;
        return memory::var_type();
    }

    cmd_error::Error mod_err("(mod {id:INT} [v1:INT] [v2:INT])");
    bool mod_prot_err = false;
    memory::var_type mod_prot(stack<pair<arg_type, string>>* arg_stack){
        mod_prot_err = false;

        memory::var_type v1;
        memory::var_type v2;

        if(arg_stack->top().first == ARG_VAR){
            v2 = memory::get_var(arg_stack->top().second);
            if(memory::get_var_error){
                mod_prot_err = true;
                return memory::var_type();
            }
        } else{
            v2 = memory::var_type((var_type)arg_stack->top().first, arg_stack->top().second);
        }

        arg_stack->pop();
        if(arg_stack->top().first == ARG_VAR){
            v1 = memory::get_var(arg_stack->top().second);
            if(memory::get_var_error){
                mod_prot_err = true;
                return memory::var_type();
            }
        } else{
            v1 = memory::var_type((var_type)arg_stack->top().first, arg_stack->top().second);
        }

        if(v1.first == VAR_INT){
            if(v2.first == VAR_INT){
                return memory::var_type(VAR_INT, to_string(stoll(v1.second) % stoll(v2.second)));
            } else{
                mod_err.bad_arg();
            }
        } else{
            mod_err.bad_arg();
        }

        mod_prot_err = true;
        return memory::var_type();
    }

    cmd_error::Error typeof_err("(typeof {id:INT} [v:ANY])");
    bool typeof_prot_err = false;
    memory::var_type typeof_prot(stack<pair<arg_type, string>>* arg_stack){
        typeof_prot_err = false;

        memory::var_type v;

        if(arg_stack->top().first == ARG_VAR){
            v = memory::get_var(arg_stack->top().second);
            if(memory::get_var_error){
                typeof_prot_err = true;
                return memory::var_type();
            }
        } else{
            v = memory::var_type((var_type)arg_stack->top().first, arg_stack->top().second);
        }
        
        switch (v.first){
            case VAR_INT:
                return memory::var_type(VAR_TYPE, "0");
            case VAR_DOT:
                return memory::var_type(VAR_TYPE, "1");
            case VAR_STR:
                return memory::var_type(VAR_TYPE, "2");
            case VAR_BOOL:
                return memory::var_type(VAR_TYPE, "3");
            case VAR_TYPE:
                return memory::var_type(VAR_TYPE, "4");
            case VAR_RELATION:
                return memory::var_type(VAR_TYPE, "5");
            default:
                printf("An error\n");
                break;
        }

        typeof_prot_err = true;
        return memory::var_type();
    }

    cmd_error::Error at_err("(at {id:INT} [s:STR] [index:INT])");
    bool at_prot_err = false;
    memory::var_type at_prot(stack<pair<arg_type, string>>* arg_stack){
        at_prot_err = false;

        unsigned long long index = 0;
        string s = "";

        if(arg_stack->top().first == ARG_INT){
            index = stoll(arg_stack->top().second);
        } else if(arg_stack->top().first == ARG_VAR){
            memory::var_type x = memory::get_var(arg_stack->top().second);
            if(memory::get_var_error){
                at_prot_err = true;
                return memory::var_type();
            }

            if(x.first == VAR_INT){
                index = stoll(x.second);
            } else{
                at_err.exc_int();
                at_prot_err = true;
                return memory::var_type();
            }
        } else{
            at_err.exc_int();
            at_prot_err = true;
            return memory::var_type();
        }

        arg_stack->pop();

        if(arg_stack->top().first == ARG_STR){
            s = arg_stack->top().second;
        } else if(arg_stack->top().first == ARG_VAR){
            memory::var_type x = memory::get_var(arg_stack->top().second);
            if(memory::get_var_error){
                at_prot_err = true;
                return memory::var_type();
            }

            if(x.first == VAR_STR){
                s = x.second;
            } else{
                at_err.exc_int();
                at_prot_err = true;
                return memory::var_type();
            }
        } else{
            at_err.exc_int();
            at_prot_err = true;
            return memory::var_type();
        }

        if(index >= s.length()){
            printf("Index cannot be bigger than s's length\n");
        } else{
            return memory::var_type(VAR_STR, string() + s[index]);
        }

        at_prot_err = true;
        return memory::var_type();
    }

    cmd_error::Error rel_err("(rel {id:INT} [v:ANY])");
    bool rel_prot_err = false;
    memory::var_type rel_prot(stack<pair<arg_type, string>>* arg_stack){
        rel_prot_err = false;

        memory::var_type v;
        if(arg_stack->top().first == ARG_VAR){
            v = memory::get_var(arg_stack->top().second);
            if(memory::get_var_error){
                rel_prot_err = true;
                return memory::var_type();
            }
        } else{
            v = memory::var_type((var_type)arg_stack->top().first, arg_stack->top().second);
        }

        switch(v.first){
            case VAR_INT:
            case VAR_DOT:
                return v;
            case VAR_STR:
                try{
                    return memory::var_type(VAR_DOT, to_string(stold(v.second)));
                } catch(const exception& e){
                    printf("Conversation error\nWhat: %s\n", e.what());
                    rel_prot_err = true;
                    return memory::var_type();
                }
            case VAR_BOOL:
            case VAR_TYPE:
            case VAR_RELATION:
                return memory::var_type(VAR_INT, v.second);
            default:
                printf("An error\n");
        }

        rel_prot_err = true;
        return memory::var_type();
    }

    cmd_error::Error str_err("(str {id:INT} [v:ANY])");
    bool str_prot_err = false;
    memory::var_type str_prot(stack<pair<arg_type, string>>* arg_stack){
        str_prot_err = false;

        memory::var_type v;
        if(arg_stack->top().first == ARG_VAR){
            v = memory::get_var(arg_stack->top().second);
            if(memory::get_var_error){
                str_prot_err = true;
                return memory::var_type();
            }
        } else{
            v = memory::var_type((var_type)arg_stack->top().first, arg_stack->top().second);
        }

        switch(v.first){
            case VAR_INT:
            case VAR_DOT:
            case VAR_STR:
                return memory::var_type(VAR_STR, v.second);
            case VAR_BOOL:
                return memory::var_type(VAR_STR, ((v.second == "0")?"false":"true"));
            case VAR_TYPE:
                return memory::var_type(VAR_STR, ((v.second == "0")?"INT":((v.second == "1")?"DOT":((v.second == "2")?"STR":((v.second == "3")?"BOOL":((v.second == "4")?"TYPE":((v.second == "5")?"RELATION":"ANY")))))));
            case VAR_RELATION:
                return memory::var_type(VAR_STR, ((v.second == "0")?"equal":((v.second == "1")?"bigger":((v.second == "2")?"smaller":"not equal"))));
            default:
                printf("An error\n");
        }

        str_prot_err = true;
        return memory::var_type();
    }

    cmd_error::Error ceil_err("(ceil {id:INT} [v:DOT])");
    bool ceil_prot_err = false;
    memory::var_type ceil_prot(stack<pair<arg_type, string>>* arg_stack){
        ceil_prot_err = false;

        memory::var_type v;
        if(arg_stack->top().first == ARG_VAR){
            v = memory::get_var(arg_stack->top().second);
            if(memory::get_var_error){
                ceil_prot_err = true;
                return memory::var_type();
            }
        } else{
            v = memory::var_type((var_type)arg_stack->top().first, arg_stack->top().second);
        }

        if(v.first == VAR_DOT){
            return memory::var_type(VAR_INT, to_string(ceil(stold(v.second))));
        } else{
            ceil_err.exc_dot();
        }

        ceil_prot_err = true;
        return memory::var_type();
    }

    cmd_error::Error floor_err("(floor {id:INT} [v:DOT])");
    bool floor_prot_err = false;
    memory::var_type floor_prot(stack<pair<arg_type, string>>* arg_stack){
        floor_prot_err = false;

        memory::var_type v;
        if(arg_stack->top().first == ARG_VAR){
            v = memory::get_var(arg_stack->top().second);
            if(memory::get_var_error){
                floor_prot_err = true;
                return memory::var_type();
            }
        } else{
            v = memory::var_type((var_type)arg_stack->top().first, arg_stack->top().second);
        }

        if(v.first == VAR_DOT){
            return memory::var_type(VAR_INT, to_string(floor(stold(v.second))));
        } else{
            floor_err.exc_dot();
        }

        floor_prot_err = true;
        return memory::var_type();
    }

    cmd_error::Error round_err("(round {id:INT} [v:DOT])");
    bool round_prot_err = false;
    memory::var_type round_prot(stack<pair<arg_type, string>>* arg_stack){
        round_prot_err = false;

        memory::var_type v;
        if(arg_stack->top().first == ARG_VAR){
            v = memory::get_var(arg_stack->top().second);
            if(memory::get_var_error){
                round_prot_err = true;
                return memory::var_type();
            }
        } else{
            v = memory::var_type((var_type)arg_stack->top().first, arg_stack->top().second);
        }

        if(v.first == VAR_DOT){
            return memory::var_type(VAR_INT, to_string(llround(stold(v.second))));
        } else{
            round_err.exc_dot();
        }

        round_prot_err = true;
        return memory::var_type();
    }

    cmd_error::Error process_err("(process [cmd:STR])");
    string process_output = "";
    void cmd_process(stack<pair<arg_type, string>>* arg_stack, size_t argc){
        process_output = "";

        if(argc == 0){
            process_err.too_few_arg();
        } else if(argc == 1){
            if(arg_stack->top().first == ARG_STR){
                process_output = arg_stack->top().second;
            } else if(arg_stack->top().first == ARG_VAR){
                process_output = memory::get_var(arg_stack->top().second).second;
                if(memory::get_var_error) return;
            } else{
                process_err.exc_str();
                return;
            }
            process_output = process_output;
        } else{
            process_err.too_many_arg();
        }
    }

    cmd_error::Error is_err("(is {id:INT} [v1:ANY] [q:RELATION] [v2:ANY])");
    bool is_prot_err = false;
    memory::var_type is_prot(stack<pair<arg_type, string>>* arg_stack){
        is_prot_err = false;

        memory::var_type v1;
        memory::var_type v2;
        memory::var_type q;

        if(arg_stack->top().first == ARG_VAR){
            v2 = memory::get_var(arg_stack->top().second);
            if(memory::get_var_error){
                is_prot_err = true;
                return memory::var_type();
            }
        } else{
            v2 = memory::var_type((var_type)arg_stack->top().first, arg_stack->top().second);
        }

        arg_stack->pop();
        if(arg_stack->top().first == ARG_VAR){
            q = memory::get_var(arg_stack->top().second);
            if(memory::get_var_error){
                is_prot_err = true;
                return memory::var_type();
            } else if(q.first != VAR_RELATION){
                is_err.exc_relation();
                is_prot_err = true;
                return memory::var_type();
            }
        } else{
            is_err.exc_relation();
            is_prot_err = true;
            return memory::var_type();
        }

        arg_stack->pop();
        if(arg_stack->top().first == ARG_VAR){
            v1 = memory::get_var(arg_stack->top().second);
            if(memory::get_var_error){
                is_prot_err = true;
                return memory::var_type();
            }
        } else{
            v1 = memory::var_type((var_type)arg_stack->top().first, arg_stack->top().second);
        }

        if(q.second == "0"){
            if(v1.first == v2.first){
                return memory::var_type(VAR_BOOL, (v1.second == v2.second)?"1":"0");
            } else{
                return memory::var_type(VAR_BOOL, "0");
            }
        } else if(q.second == "1"){
            if((v1.first == VAR_INT || v1.first == VAR_DOT) && (v2.first == VAR_INT || v2.first == VAR_DOT)){
                if(v1.first == VAR_INT && v2.first == VAR_INT){
                    return memory::var_type(VAR_BOOL, (stoll(v1.second) > stoll(v2.second))?"1":"0");
                } else{
                    return memory::var_type(VAR_BOOL, (stold(v1.second) > stold(v2.second))?"1":"0");
                }
            } else{
                return memory::var_type(VAR_BOOL, "0");
            }
        } else if(q.second == "2"){
            if((v1.first == VAR_INT || v1.first == VAR_DOT) && (v2.first == VAR_INT || v2.first == VAR_DOT)){
                if(v1.first == VAR_INT && v2.first == VAR_INT){
                    return memory::var_type(VAR_BOOL, (stoll(v1.second) < stoll(v2.second))?"1":"0");
                } else{
                    return memory::var_type(VAR_BOOL, (stold(v1.second) < stold(v2.second))?"1":"0");
                }
            } else{
                return memory::var_type(VAR_BOOL, "0");
            }
        } else{
            is_err.bad_arg();
        }

        is_prot_err = true;
        return memory::var_type();
    }

    cmd_error::Error cmp_err("(cmp {id:INT} [v1:ANY] [v2:ANY])");
    bool cmp_prot_err = false;
    memory::var_type cmp_prot(stack<pair<arg_type, string>>* arg_stack){
        cmp_prot_err = false;

        memory::var_type v1;
        memory::var_type v2;

        if(arg_stack->top().first == ARG_VAR){
            v2 = memory::get_var(arg_stack->top().second);
            if(memory::get_var_error){
                cmp_prot_err = true;
                return memory::var_type();
            }
        } else{
            v2 = memory::var_type((var_type)arg_stack->top().first, arg_stack->top().second);
        }

        arg_stack->pop();
        if(arg_stack->top().first == ARG_VAR){
            v1 = memory::get_var(arg_stack->top().second);
            if(memory::get_var_error){
                cmp_prot_err = true;
                return memory::var_type();
            }
        } else{
            v1 = memory::var_type((var_type)arg_stack->top().first, arg_stack->top().second);
        }

        if((v1.first == VAR_INT || v1.first == VAR_DOT) && (v2.first == VAR_INT || v2.first == VAR_DOT)){
            return memory::var_type(VAR_RELATION, ((stold(v1.second) == stold(v2.second))?"0":((stold(v1.second) > stold(v2.second))?"1":"2")));
        } else if(v1.first == v2.first){
            if(v1.second == v2.second){
                return memory::var_type(VAR_RELATION, "0");
            } else{
                return memory::var_type(VAR_RELATION, "3");
            }
        } else{
            return memory::var_type(VAR_RELATION, "3");
        }

        cmp_prot_err = true;
        return memory::var_type();
    }

    cmd_error::Error if_err("(if [x:BOOL] [then:STR] {else:STR})");
    string if_output = "";
    void cmd_if(stack<pair<arg_type, string>>* arg_stack, size_t argc){
        if_output = "";

        if(argc <= 1){
            if_err.too_few_arg();
        } else if(argc == 2){
            bool x;
            string then = "";

            if(arg_stack->top().first == ARG_STR){
                then = arg_stack->top().second;
            } else if(arg_stack->top().first == ARG_VAR){
                memory::var_type g = memory::get_var(arg_stack->top().second);
                if(memory::get_var_error) return;
                else if(g.first != VAR_STR){
                    if_err.exc_str();
                    return;
                }

                then = g.second;
            } else{
                if_err.exc_str();
                return;
            }

            arg_stack->pop();
            if(arg_stack->top().first == ARG_VAR){
                memory::var_type g = memory::get_var(arg_stack->top().second);
                if(memory::get_var_error) return;
                else if(g.first != VAR_BOOL){
                    if_err.exc_bool();
                    return;
                }

                x = !(g.second == "0");
            } else{
                if_err.exc_bool();
                return;
            }

            if(x) if_output = then;
        } else if(argc == 3){
            bool x;
            string then = "";
            string el = "";

            if(arg_stack->top().first == ARG_STR){
                el = arg_stack->top().second;
            } else if(arg_stack->top().first == ARG_VAR){
                memory::var_type g = memory::get_var(arg_stack->top().second);
                if(memory::get_var_error) return;
                else if(g.first != VAR_STR){
                    if_err.exc_str();
                    return;
                }

                el = g.second;
            } else{
                if_err.exc_str();
                return;
            }

            arg_stack->pop();
            if(arg_stack->top().first == ARG_STR){
                then = arg_stack->top().second;
            } else if(arg_stack->top().first == ARG_VAR){
                memory::var_type g = memory::get_var(arg_stack->top().second);
                if(memory::get_var_error) return;
                else if(g.first != VAR_STR){
                    if_err.exc_str();
                    return;
                }

                then = g.second;
            } else{
                if_err.exc_str();
                return;
            }

            arg_stack->pop();
            if(arg_stack->top().first == ARG_VAR){
                memory::var_type g = memory::get_var(arg_stack->top().second);
                if(memory::get_var_error) return;
                else if(g.first != VAR_BOOL){
                    if_err.exc_bool();
                    return;
                }

                x = !(g.second == "0");
            } else{
                if_err.exc_bool();
                return;
            }

            if(x) if_output = then;
            else if_output = el;
        } else{
            if_err.too_many_arg();
        }
    }

    cmd_error::Error put_err("(put [v:ANY])");
    void cmd_put(stack<pair<arg_type, string>>* arg_stack, size_t argc){
        if(argc == 0){
            put_err.too_few_arg();
        } else if(argc == 1){
            string v = "";
            if(arg_stack->top().first == ARG_VAR){
                v = memory::get_var(arg_stack->top().second).second;
                if(memory::get_var_error) return;
            } else{
                v = arg_stack->top().second;
            }
            printf("%s", v.c_str());
        } else{
            put_err.too_many_arg();
        }
    }

    cmd_error::Error putl_err("(putl [v:ANY])");
    void cmd_putl(stack<pair<arg_type, string>>* arg_stack, size_t argc){
        if(argc == 0){
            put_err.too_few_arg();
        } else if(argc == 1){
            string v = "";
            if(arg_stack->top().first == ARG_VAR){
                v = memory::get_var(arg_stack->top().second).second;
                if(memory::get_var_error) return;
            } else{
                v = arg_stack->top().second;
            }
            printf("%s\n", v.c_str());
        } else{
            put_err.too_many_arg();
        }
    }

    cmd_error::Error get_err("(get {id:INT})");
    memory::var_type get_prot(stack<pair<arg_type, string>>* arg_stack){
        string val = "";
        char buf = (char)getc(stdin);
        
        while(buf == ' ' || buf == '\t' || buf == '\n'){
            buf = (char)getc(stdin);
        }

        while(buf != ' ' && buf != '\t' && buf != '\n'){
            val += buf;
            buf = (char)getc(stdin);
        }
        return memory::var_type(VAR_STR, val);
    }

    cmd_error::Error getl_err("(getl {id:INT})");
    memory::var_type getl_prot(stack<pair<arg_type, string>>* arg_stack){
        string i = "";
        getline(cin, i);
        return memory::var_type(VAR_STR, i);
    }

    cmd_error::Error fput_err("(fput [path:STR] [v:ANY])");
    void cmd_fput(stack<pair<arg_type, string>>* arg_stack, size_t argc){
        if(argc <= 1){
            fput_err.too_few_arg();
        } else if(argc == 2){
            string v = "";
            string path = "";

            if(arg_stack->top().first == ARG_VAR){
                v = memory::get_var(arg_stack->top().second).second;
                if(memory::get_var_error) return;
            } else{
                v = arg_stack->top().second;
            }

            arg_stack->pop();
            if(arg_stack->top().first == ARG_VAR){
                auto p = memory::get_var(arg_stack->top().second);
                if(memory::get_var_error) return;
                else if(p.first != VAR_STR){
                    fput_err.exc_str();
                    return;
                }
                path = p.second;
            } else if(arg_stack->top().first == ARG_STR){
                path = arg_stack->top().second;
            } else{
                fput_err.exc_str();
                return;
            }

            fstream file;
            file.open(path, ios::out | ios::app);
            if(!file.good()){
                printf("Failed to open file (\'%s\')\n", path.c_str());
                return;
            }

            file << v;
            file.close();
        } else{
            fput_err.too_many_arg();
        }
    }

    cmd_error::Error fputl_err("(fputl [path:STR] [v:ANY])");
    void cmd_fputl(stack<pair<arg_type, string>>* arg_stack, size_t argc){
        if(argc <= 1){
            fputl_err.too_few_arg();
        } else if(argc == 2){
            string v = "";
            string path = "";

            if(arg_stack->top().first == ARG_VAR){
                v = memory::get_var(arg_stack->top().second).second;
                if(memory::get_var_error) return;
            } else{
                v = arg_stack->top().second;
            }

            arg_stack->pop();
            if(arg_stack->top().first == ARG_VAR){
                auto p = memory::get_var(arg_stack->top().second);
                if(memory::get_var_error) return;
                else if(p.first != VAR_STR){
                    fputl_err.exc_str();
                    return;
                }
                path = p.second;
            } else if(arg_stack->top().first == ARG_STR){
                path = arg_stack->top().second;
            } else{
                fputl_err.exc_str();
                return;
            }

            fstream file;
            file.open(path, ios::out | ios::app);
            if(!file.good()){
                printf("Failed to open file (\'%s\')\n", path.c_str());
                return;
            }

            file << v << endl;
            file.close();
        } else{
            fputl_err.too_many_arg();
        }
    }

    cmd_error::Error fwrite_err("(fwrite [path:STR] [v:ANY])");
    void cmd_fwrite(stack<pair<arg_type, string>>* arg_stack, size_t argc){
        if(argc <= 1){
            fwrite_err.too_few_arg();
        } else if(argc == 2){
            string v = "";
            string path = "";

            if(arg_stack->top().first == ARG_VAR){
                v = memory::get_var(arg_stack->top().second).second;
                if(memory::get_var_error) return;
            } else{
                v = arg_stack->top().second;
            }

            arg_stack->pop();
            if(arg_stack->top().first == ARG_VAR){
                auto p = memory::get_var(arg_stack->top().second);
                if(memory::get_var_error) return;
                else if(p.first != VAR_STR){
                    fwrite_err.exc_str();
                    return;
                }
                path = p.second;
            } else if(arg_stack->top().first == ARG_STR){
                path = arg_stack->top().second;
            } else{
                fwrite_err.exc_str();
                return;
            }

            fstream file;
            file.open(path, ios::out);
            if(!file.good()){
                printf("Failed to open file (\'%s\')\n", path.c_str());
                return;
            }

            file << v;
            file.close();
        } else{
            fwrite_err.too_many_arg();
        }
    }

    cmd_error::Error fwritel_err("fwritel [path:STR] [v:ANY])");
    void cmd_fwritel(stack<pair<arg_type, string>>* arg_stack, size_t argc){
        if(argc <= 1){
            fwritel_err.too_few_arg();
        } else if(argc == 2){
            string v = "";
            string path = "";

            if(arg_stack->top().first == ARG_VAR){
                v = memory::get_var(arg_stack->top().second).second;
                if(memory::get_var_error) return;
            } else{
                v = arg_stack->top().second;
            }

            arg_stack->pop();
            if(arg_stack->top().first == ARG_VAR){
                auto p = memory::get_var(arg_stack->top().second);
                if(memory::get_var_error) return;
                else if(p.first != VAR_STR){
                    fwritel_err.exc_str();
                    return;
                }
                path = p.second;
            } else if(arg_stack->top().first == ARG_STR){
                path = arg_stack->top().second;
            } else{
                fwritel_err.exc_str();
                return;
            }

            fstream file;
            file.open(path, ios::out);
            if(!file.good()){
                printf("Failed to open file (\'%s\')\n", path.c_str());
                return;
            }

            file << v << endl;
            file.close();
        } else{
            fwritel_err.too_many_arg();
        }
    }

    cmd_error::Error fread_err("(fread {id:INT} [path:STR])");
    memory::var_type fread_prot(stack<pair<arg_type, string>>* arg_stack){
        string path = "";

        if(arg_stack->top().first == ARG_VAR){
            auto p = memory::get_var(arg_stack->top().second);
            if(memory::get_var_error){
                _prot_err = true;
                return memory::var_type();
            } else if(p.first != VAR_STR){
                fread_err.exc_str();
                _prot_err = true;
                return memory::var_type();
            }
            path = p.second;
        } else if(arg_stack->top().first == ARG_STR){
            path = arg_stack->top().second;
        } else{
            fread_err.exc_str();
            _prot_err = true;
            return memory::var_type();
        }

        string val = "";

        fstream file;
        file.open(path, ios::in);
        if(!file.good()){
            printf("Failed to open file (\'%s\')\n", path.c_str());
            _prot_err = true;
            return memory::var_type();
        }

        string line = "";
        while(getline(file, line)){
            val += "\n" + line;
        }

        file.close();

        if(!val.empty()){
            val = val.substr(1);
        }

        return memory::var_type(VAR_STR, val);
    }
}