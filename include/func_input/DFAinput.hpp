#ifndef func_minimization_DFAmin_HPP_
#define func_minimization_DFAmin_HPP_


#include <iostream>   // для std::cout
#include <vector> 
#include <unordered_map>    
#include <string>

using namespace std;

namespace func_input{
    struct Result {
        vector<string> string_transition;
        vector<string> permited_state;
        vector<string> alphabet;
        vector<string> states;
        unordered_map<string, unordered_map<string, string>> transitions;
    };

 Result DFAinput(string filename);

}

#endif // func_minimization_DFAmin_HPP_