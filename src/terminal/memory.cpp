#include <terminal/memory.hpp>

#include <cstdio>

using namespace std;

namespace memory{
    var_type _get = var_type(defs::VAR_NUM, "0");
    stack<var_type> memory_stack;
    vector<var_type> index_memory;

    bool get_var_error = false;
    var_type get_var(string name){
        //TODO!!!
        get_var_error = false;
        if(name == "_get"){
            return _get;
        } else if(name == "_top"){
            if(memory_stack.empty()){
                printf("Memory stack is empty\n");
                get_var_error = true;
                return var_type();
            }
            return memory_stack.top();
        } else if(name[0] == '$'){
            string index = name.substr(1);
            if(index.empty()){
                return _get;
            }

            unsigned long long i = stoi(index);
            if(index_memory.size() <= i){
                printf("Index $%Li is not inited\n", i);
                get_var_error = true;
                return var_type();
            }
            return index_memory[i];
        } else{
            printf("Unknown varible \'%s\'\n", name.c_str());
            get_var_error = true;
            return var_type();
        }
    }
}