#include <terminal/memory.hpp>

#include <cstdio>

using namespace std;

namespace memory{
    var_type _get = var_type(defs::VAR_INT, "0");
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
        } else if(name == "_true"){
            return var_type(defs::VAR_BOOL, "1");
        } else if(name == "_false"){
            return var_type(defs::VAR_BOOL, "0");
        } else if(name == "_int"){
            return var_type(defs::VAR_TYPE, "0");
        } else if(name == "_dot"){
            return var_type(defs::VAR_TYPE, "1");
        } else if(name == "_str"){
            return var_type(defs::VAR_TYPE, "2");
        } else if(name == "_bool"){
            return var_type(defs::VAR_TYPE, "3");
        } else if(name == "_type"){
            return var_type(defs::VAR_TYPE, "4");
        } else if(name == "_relation"){
            return var_type(defs::VAR_TYPE, "5");
        } else if(name == "_equal" || name == "="){
            return var_type(defs::VAR_RELATION, "0");
        } else if(name == "_bigger" || name == ">"){
            return var_type(defs::VAR_RELATION, "1");
        } else if(name == "_smaller" || name == "<"){
            return var_type(defs::VAR_RELATION, "2");
        } else if(name == "_notequal" || name == "!" || name == "!="){
            return var_type(defs::VAR_RELATION, "3");
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

    bool set_index(unsigned long long id, var_type v){
        if(index_memory.size() > id){
            index_memory[id] = v;
        } else if(index_memory.size() == id){
            index_memory.push_back(v);
        } else{
            printf("Index $%lli is not inited\n", id);
            return false;
        }
        return true;
    }
}