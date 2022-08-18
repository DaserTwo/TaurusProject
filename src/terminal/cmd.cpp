#include <terminal/cmd.hpp>

#include <common/common.hpp>

#include <cmath>

using namespace std;
using namespace defs;

namespace cmd{
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

    cmd_error::Error putn_err("(putn [v:REL])");
    void cmd_putn(stack<pair<arg_type, string>>* arg_stack, size_t argc){
        if(argc == 0){
            putn_err.too_few_arg();
        } else if(argc == 1){
            memory::var_type buffer;
            if(arg_stack->top().first == ARG_VAR){
                buffer = memory::get_var(arg_stack->top().second);
                if(memory::get_var_error){
                    return;
                }
            } else{
                buffer = memory::var_type((var_type)arg_stack->top().first, arg_stack->top().second);
            }

            if(buffer.first == VAR_INT || VAR_BOOL || VAR_TYPE){
                long long n = stoll(buffer.second);
                printf("%lli", n);
            } else if(buffer.first == VAR_DOT){
                long double n = stold(buffer.second);
                printf("%Lf", n);
            } else{
                putn_err.exc_rel();
            }
        } else{
            putn_err.too_many_arg();
        }
    }

    cmd_error::Error puts_err("(puts [s:STR])");
    void cmd_puts(stack<pair<arg_type, string>>* arg_stack, size_t argc){
        if(argc == 0){
            puts_err.too_few_arg();
        } else if(argc == 1){
            if(arg_stack->top().first == ARG_STR){
                string s = common::clear_str_format(arg_stack->top().second);
                printf("%s", s.c_str());
            } else if(arg_stack->top().first == ARG_VAR){
                memory::var_type buffer = memory::get_var(arg_stack->top().second);
                if(memory::get_var_error){
                    return;
                } else if(buffer.first == VAR_BOOL){
                    printf("%s", (buffer.second == "0")?"false":"true");
                } else if(buffer.first == VAR_TYPE){
                    printf("%s", (buffer.second == "0")?"INT":((buffer.second == "1")?"DOT":((buffer.second == "2")?"STR":((buffer.second == "3")?"BOOL":((buffer.second == "4")?"TYPE":"ANY")))));
                } else if(buffer.first != VAR_STR){
                    puts_err.exc_str();
                    return;
                } 
                string s = common::clear_str_format(buffer.second);
                printf("%s", s.c_str());
            } else{
                puts_err.exc_str();
            }
        } else{
            puts_err.too_many_arg();
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
            string av = v1.second.substr(0, v1.second.length() - 1);
            string bv = v2.second;
            if(v2.first == VAR_BOOL)
                bv = (v2.second == "0")?"false":"true";
            else if(v2.first == VAR_STR)
                bv = v2.second.substr(1, v2.second.length() - 2);
            else if(v2.first == VAR_TYPE)
                bv = (v2.second == "0")?"INT":((v2.second == "1")?"DOT":((v2.second == "2")?"STR":((v2.second == "3")?"BOOL":((v2.second == "4")?"TYPE":"ANY"))));
            return memory::var_type(VAR_STR, av + bv + "'");
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

    void cmd_add(stack<pair<arg_type, string>>* arg_stack, size_t argc){
        if(argc <= 1){
            add_err.too_few_arg();
        } else if(argc == 2){
            memory::var_type g = add_prot(arg_stack);
            if(add_prot_err) return;
            else memory::_get = g;
        } else if(argc == 3){
            unsigned long long id;

            memory::var_type g = add_prot(arg_stack);
            if(add_prot_err) return;

            arg_stack->pop();
            if(arg_stack->top().first == ARG_INT){
                id = stoll(arg_stack->top().second);
            } else if(arg_stack->top().first == ARG_VAR){
                memory::var_type v = memory::get_var(arg_stack->top().second);
                if(memory::get_var_error) return;
                else if(v.first == VAR_INT){
                    id = stoll(v.second);
                } else{
                    add_err.bad_arg();
                    return;
                }
            } else{
                add_err.bad_arg();
                return;
            }

            memory::set_index(id, g);
        } else{
            add_err.too_many_arg();
        }
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
            string str = v1.second.substr(1, v1.second.length() - 2);
            if(v2.first == VAR_INT){
                unsigned long long begin = stoll(v2.second);
                if(begin >= str.length()){
                    printf("Begin index cannot be biger than string length\n");
                } else{
                    return memory::var_type(VAR_STR, "'" + str.substr(begin) + "'");
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
                string str = v1.second.substr(1, v1.second.length() - 2);
                if(v2.first == VAR_INT){
                    if(v3.first == VAR_INT){
                        unsigned long long begin = stoll(v2.second);
                        unsigned long long end = stoll(v3.second);
                        if(begin >= str.length()){
                            printf("Begin index cannot be biger than string length\n");
                        } else if(end >= str.length()){
                            printf("End index cannot be biger than string length\n");
                        } else{
                            memory::_get = memory::var_type(VAR_STR, "'" + str.substr(begin, end) + "'");
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
                string s = str.second.substr(1, str.second.length() - 2);
                if(begin.first == VAR_INT && end.first == VAR_INT){
                    unsigned long long b = stoll(begin.second);
                    unsigned long long e = stoll(end.second);
                    if(b >= s.length()){
                        printf("Begin index cannot be biger than string length\n");
                    } else if(e >= s.length()){
                        printf("End index cannot be biger than string length\n");
                    } else{
                        memory::set_index(id, memory::var_type(VAR_STR, "'" + s.substr(b, e) + "'"));
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
                string get = "'";
                long long times = stoll(v2.second);
                while(times > 0){
                    get += v1.second.substr(1, v1.second.length() - 2);
                    times--;
                }
                return memory::var_type(VAR_STR, get + "'");
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

    void cmd_mul(stack<pair<arg_type, string>>* arg_stack, size_t argc){
        if(argc <= 1){
            mul_err.too_few_arg();
        } else if(argc == 2){
            memory::var_type g = mul_prot(arg_stack);
            if(mul_prot_err) return;

            memory::_get = g;
        } else if(argc == 3){
            unsigned long long id;

            memory::var_type g = mul_prot(arg_stack);
            if(mul_prot_err) return;

            arg_stack->pop();
            if(arg_stack->top().first == ARG_INT){
                id = stoll(arg_stack->top().second);
            } else if(arg_stack->top().first == ARG_VAR){
                memory::var_type v = memory::get_var(arg_stack->top().second);
                if(memory::get_var_error) return;
                else if(v.first == VAR_INT){
                    id = stoll(v.second);
                } else{
                    mul_err.bad_arg();
                    return;
                }
            } else{
                mul_err.bad_arg();
                return;
            }

            memory::set_index(id, g);
        } else{
            mul_err.too_many_arg();
        }
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

    void cmd_div(stack<pair<arg_type, string>>* arg_stack, size_t argc){
        if(argc <= 1){
            div_err.too_few_arg();
        } else if(argc == 2){
            memory::var_type g = div_prot(arg_stack);
            if(div_prot_err) return;

            memory::_get = g;
        } else if(argc == 3){
            unsigned long long id;

            memory::var_type g = div_prot(arg_stack);
            if(div_prot_err) return;

            arg_stack->pop();
            if(arg_stack->top().first == ARG_INT){
                id = stoll(arg_stack->top().second);
            } else if(arg_stack->top().first == ARG_VAR){
                memory::var_type v = memory::get_var(arg_stack->top().second);
                if(memory::get_var_error) return;
                else if(v.first == VAR_INT){
                    id = stoll(v.second);
                } else{
                    div_err.bad_arg();
                    return;
                }
            } else{
                div_err.bad_arg();
                return;
            }

            memory::set_index(id, g);
        } else{
            div_err.too_many_arg();
        }
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
                memory::_get = memory::var_type(VAR_INT, to_string(stoll(v1.second) % stoll(v2.second)));
            } else{
                mod_err.bad_arg();
            }
        } else{
            mod_err.bad_arg();
        }

        mod_prot_err = true;
        return memory::var_type();
    }

    void cmd_mod(stack<pair<arg_type, string>>* arg_stack, size_t argc){
        if(argc <= 1){
            mod_err.too_few_arg();
        } else if(argc == 2){
            memory::var_type g = mod_prot(arg_stack);
            if(mod_prot_err) return;

            memory::_get = g;
        } else if(argc == 3){
            unsigned long long id;
            
            memory::var_type g = mod_prot(arg_stack);
            if(mod_prot_err) return;

            arg_stack->pop();
            if(arg_stack->top().first == ARG_INT){
                id = stoll(arg_stack->top().second);
            } else if(arg_stack->top().first == ARG_VAR){
                memory::var_type v = memory::get_var(arg_stack->top().second);
                if(memory::get_var_error) return;
                else if(v.first == VAR_INT){
                    id = stoll(v.second);
                } else{
                    mod_err.bad_arg();
                    return;
                }
            } else{
                mod_err.bad_arg();
                return;
            }
            
            memory::set_index(id, g);
        } else{
            mod_err.too_many_arg();
        }
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
            default:
                printf("An error\n");
                break;
        }

        typeof_prot_err = true;
        return memory::var_type();
    }

    void cmd_typeof(stack<pair<arg_type, string>>* arg_stack, size_t argc){
        if(argc  == 0){
            typeof_err.too_few_arg();
        } else if(argc == 1){
            memory::var_type g = typeof_prot(arg_stack);
            if(typeof_prot_err) return;

            memory::_get = g;
        } else if(argc == 2){
            unsigned long long id;
            
            memory::var_type g = typeof_prot(arg_stack);
            if(typeof_prot_err) return;

            arg_stack->pop();
            if(arg_stack->top().first == ARG_INT){
                id = stoll(arg_stack->top().second);
            } else if(arg_stack->top().first == ARG_VAR){
                memory::var_type v = memory::get_var(arg_stack->top().second);
                if(memory::get_var_error) return;
                else if(v.first == VAR_INT){
                    id = stoll(v.second);
                } else{
                    typeof_err.bad_arg();
                    return;
                }
            } else{
                typeof_err.bad_arg();
                return;
            }
            
            memory::set_index(id, g);
        } else{
            typeof_err.too_many_arg();
        }
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
            s = common::clear_str_format(arg_stack->top().second);
        } else if(arg_stack->top().first == ARG_VAR){
            memory::var_type x = memory::get_var(arg_stack->top().second);
            if(memory::get_var_error){
                at_prot_err = true;
                return memory::var_type();
            }

            if(x.first == VAR_STR){
                s = common::clear_str_format(x.second);
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
            return memory::var_type(VAR_STR, string("\'") + s[index] + "\'");
        }

        at_prot_err = true;
        return memory::var_type();
    }

    void cmd_at(stack<pair<arg_type, string>>* arg_stack, size_t argc){
        if(argc <= 1){
            at_err.too_few_arg();
        } else if(argc == 2){
            memory::var_type g = at_prot(arg_stack);
            if(at_prot_err) return;

            memory::_get = g;
        } else if(argc == 3){
            unsigned long long id;
            
            memory::var_type g = at_prot(arg_stack);
            if(at_prot_err) return;

            arg_stack->pop();
            if(arg_stack->top().first == ARG_INT){
                id = stoll(arg_stack->top().second);
            } else if(arg_stack->top().first == ARG_VAR){
                memory::var_type v = memory::get_var(arg_stack->top().second);
                if(memory::get_var_error) return;
                else if(v.first == VAR_INT){
                    id = stoll(v.second);
                } else{
                    at_err.bad_arg();
                    return;
                }
            } else{
                at_err.bad_arg();
                return;
            }
            
            memory::set_index(id, g);
        } else{
            at_err.too_many_arg();
        }
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
                    return memory::var_type(VAR_DOT, to_string(stold(common::clear_str_format(v.second))));
                } catch(const exception& e){
                    printf("Conversation error\nWhat: %s\n", e.what());
                    rel_prot_err = true;
                    return memory::var_type();
                }
            case VAR_BOOL:
            case VAR_TYPE:
                return memory::var_type(VAR_INT, v.second);
            default:
                printf("An error\n");
        }

        rel_prot_err = true;
        return memory::var_type();
    }

    void cmd_rel(stack<pair<arg_type, string>>* arg_stack, size_t argc){
        if(argc == 0){
            rel_err.too_few_arg();
        } else if(argc == 1){
            memory::var_type g = rel_prot(arg_stack);
            if(rel_prot_err) return;

            memory::_get = g;
        } else if(argc == 2){
            unsigned long long id;
            
            memory::var_type g = rel_prot(arg_stack);
            if(rel_prot_err) return;

            arg_stack->pop();
            if(arg_stack->top().first == ARG_INT){
                id = stoll(arg_stack->top().second);
            } else if(arg_stack->top().first == ARG_VAR){
                memory::var_type v = memory::get_var(arg_stack->top().second);
                if(memory::get_var_error) return;
                else if(v.first == VAR_INT){
                    id = stoll(v.second);
                } else{
                    rel_err.bad_arg();
                    return;
                }
            } else{
                rel_err.bad_arg();
                return;
            }
            
            memory::set_index(id, g);
        } else{
            rel_err.too_many_arg();
        }
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
                return memory::var_type(VAR_STR, string("\'") + v.second + "\'");
            case VAR_STR:
                return memory::var_type(VAR_STR, v.second);
            case VAR_BOOL:
                return memory::var_type(VAR_STR, string("\'") + ((v.second == "0")?"false":"true") + "\'");
            case VAR_TYPE:
                return memory::var_type(VAR_STR, string("\'") + ((v.second == "0")?"INT":((v.second == "1")?"DOT":((v.second == "2")?"STR":((v.second == "3")?"BOOL":((v.second == "4")?"TYPE":"ANY"))))) + "\'");
            default:
                printf("An error\n");
        }

        str_prot_err = true;
        return memory::var_type();
    }

    void cmd_str(stack<pair<arg_type, string>>* arg_stack, size_t argc){
        if(argc == 0){
            str_err.too_few_arg();
        } else if(argc == 1){
            memory::var_type g = str_prot(arg_stack);
            if(str_prot_err) return;

            memory::_get = g;
        } else if(argc == 2){
            unsigned long long id;
            
            memory::var_type g = str_prot(arg_stack);
            if(str_prot_err) return;

            arg_stack->pop();
            if(arg_stack->top().first == ARG_INT){
                id = stoll(arg_stack->top().second);
            } else if(arg_stack->top().first == ARG_VAR){
                memory::var_type v = memory::get_var(arg_stack->top().second);
                if(memory::get_var_error) return;
                else if(v.first == VAR_INT){
                    id = stoll(v.second);
                } else{
                    str_err.bad_arg();
                    return;
                }
            } else{
                str_err.bad_arg();
                return;
            }
            
            memory::set_index(id, g);
        } else{
            str_err.too_many_arg();
        }
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

    void cmd_ceil(stack<pair<arg_type, string>>* arg_stack, size_t argc){
        if(argc == 0){
            ceil_err.too_few_arg();
        } else if(argc == 1){
            memory::var_type g = ceil_prot(arg_stack);
            if(ceil_prot_err) return;

            memory::_get = g;
        } else if(argc == 2){
            unsigned long long id;
            
            memory::var_type g = ceil_prot(arg_stack);
            if(ceil_prot_err) return;

            arg_stack->pop();
            if(arg_stack->top().first == ARG_INT){
                id = stoll(arg_stack->top().second);
            } else if(arg_stack->top().first == ARG_VAR){
                memory::var_type v = memory::get_var(arg_stack->top().second);
                if(memory::get_var_error) return;
                else if(v.first == VAR_INT){
                    id = stoll(v.second);
                } else{
                    ceil_err.bad_arg();
                    return;
                }
            } else{
                ceil_err.bad_arg();
                return;
            }
            
            memory::set_index(id, g);
        } else{
            ceil_err.too_many_arg();
        }
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

    void cmd_floor(stack<pair<arg_type, string>>* arg_stack, size_t argc){
        if(argc == 0){
            floor_err.too_few_arg();
        } else if(argc == 1){
            memory::var_type g = floor_prot(arg_stack);
            if(floor_prot_err) return;

            memory::_get = g;
        } else if(argc == 2){
            unsigned long long id;
            
            memory::var_type g = floor_prot(arg_stack);
            if(floor_prot_err) return;

            arg_stack->pop();
            if(arg_stack->top().first == ARG_INT){
                id = stoll(arg_stack->top().second);
            } else if(arg_stack->top().first == ARG_VAR){
                memory::var_type v = memory::get_var(arg_stack->top().second);
                if(memory::get_var_error) return;
                else if(v.first == VAR_INT){
                    id = stoll(v.second);
                } else{
                    floor_err.bad_arg();
                    return;
                }
            } else{
                floor_err.bad_arg();
                return;
            }
            
            memory::set_index(id, g);
        } else{
            floor_err.too_many_arg();
        }
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

    void cmd_round(stack<pair<arg_type, string>>* arg_stack, size_t argc){
        if(argc == 0){
            round_err.too_few_arg();
        } else if(argc == 1){
            memory::var_type g = round_prot(arg_stack);
            if(round_prot_err) return;

            memory::_get = g;
        } else if(argc == 2){
            unsigned long long id;
            
            memory::var_type g = round_prot(arg_stack);
            if(round_prot_err) return;

            arg_stack->pop();
            if(arg_stack->top().first == ARG_INT){
                id = stoll(arg_stack->top().second);
            } else if(arg_stack->top().first == ARG_VAR){
                memory::var_type v = memory::get_var(arg_stack->top().second);
                if(memory::get_var_error) return;
                else if(v.first == VAR_INT){
                    id = stoll(v.second);
                } else{
                    round_err.bad_arg();
                    return;
                }
            } else{
                round_err.bad_arg();
                return;
            }
            
            memory::set_index(id, g);
        } else{
            round_err.too_many_arg();
        }
    }
}