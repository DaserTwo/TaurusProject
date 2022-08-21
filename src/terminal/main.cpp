#include <iostream>
#include <fstream>
#include <vector>

#include <terminal/process.hpp>

using namespace std;

vector<string> args;
int main(int argc, char** argv){
    string input = "";

    if(argc > 1){
        for(int i = 2; i < argc; i++)
            args.push_back(argv[i]);

        string inputPath = argv[1];
        bool debbug_log = false;
        bool quiet = false;
        for(auto arg : args){
            if(arg == "-d"){
                debbug_log = true;
            } else if(arg == "-q"){
                quiet = true;
            } else{
                cout << "Unexcepted argument \"" << arg << "\"" << endl << "Terminated..." << endl;
                exit(-1);
            }
        }

        fstream file;
        file.open(inputPath, ios::in);

        if(!file.good()){
            cout << "Failed to open input file \"" << inputPath << "\"" << endl << "Terminated..." << endl;
            exit(-2);
        }

        while(getline(file, input)){
            if(debbug_log){
                if(quiet) cout << input << endl;
                else cout << "Debbug log: \"" << input << "\"" << endl;
            }

            process::processInput(input);
        }

        file.close();
    }

    while(true){
        getline(cin, input);
        process::processInput(input);
    }
    return 0;
}