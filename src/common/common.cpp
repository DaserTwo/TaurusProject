#include <common/common.hpp>

using namespace std;

namespace common{
    bool is_num(char c){
        return ((c >= '1' && c <= '9') || c == '0');
    }

    string str_to_lower(string s){
        string ret = "";
        for(char c : s){
            ret += tolower(c);
        }
        return ret;
    }

    string clear_str_format(string s){
        string ret = "";
        string buffer = s.substr(1, s.length() - 2);
        bool slash = false;
        for(char c : buffer){
            if(slash){
                slash = false;
                switch(c){
                    case 'n':
                        ret += '\n';
                        break;
                    case 't':
                        ret += '\t';
                        break;
                    case '0':
                        return ret;
                    default:
                        ret += c;
                }
            } else if(c == '\\'){
                slash = true;
            } else{
                ret += c;
            }
        }
        return ret;
    }
}