#include <iostream>

#include <terminal/process.hpp>

using namespace std;

int main(int argc, char** argv){
    string input = "";
    while(true){
        getline(cin, input);
        process::processInput(input);
    }
    return 0;
}