#include <func_DFA/DFA.hpp> // публичные include подключаем как системные
#include <iostream>   
#include <sstream>
#include <fstream>
#include <vector> 
#include <unordered_map>    
#include <string>

using namespace std;


func_DFA::DFA func_DFA::DFAinput(string filename) {
    DFA dfa;
    string token;
    string Line;

    ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл!\n";
        return dfa;
    }


    getline(file, Line);
    stringstream ss1(Line);
    bool first = true;
    while (getline(ss1, token,';')){
        if (token == "") break;
        if (!first) dfa.string_transition.push_back(token);
        first = false;
    }

    getline(file, Line);
    stringstream ss2(Line);
    first = true;
    while (getline(ss2, token,';')){
        if (token == "") break;
        if (!first) dfa.permited_state.push_back(token);
        first = false;
    }

    getline(file, Line);
    stringstream ss3(Line);
    first = true;
    while (getline(ss3, token,';')){
        if (!first){
            dfa.start_state = token;
            break;
        }
        first = false;
    }

    getline(file, Line);
    stringstream ss4(Line);
    first = true;
    while (getline(ss4, token,';')){
        if (token == "") break;
        if (!first) dfa.alphabet.push_back(token);
        first = false;
    }


    while (getline(file, Line)){
        stringstream ss(Line);
        string current_state;
        first = true;
        size_t count_alphabet = 0;
        while (getline(ss, token,';')){
            if (first) {
                current_state = token;
                dfa.states.push_back(token);
                first = false;
            }
            else{
                if (count_alphabet < dfa.alphabet.size()){
                    if (!token.empty()){
                        dfa.transitions[current_state][dfa.alphabet[count_alphabet]] = token;  
                    }
                    ++count_alphabet;
                }
                else break;
            }
        }
    }





    return dfa;
    }