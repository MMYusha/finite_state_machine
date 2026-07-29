#include <func_DFA/DFA.hpp> // публичные include подключаем как системные
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>   // для unordered_set
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

#include <queue>
#include <utility>   // для std::pair
#include <algorithm>   // для std::sort


using namespace std;

namespace func_DFA{

DFABuilder& DFABuilder::withCSV(const string& filename){
    // Вспомогательные структуры для чтения
    string token;
    string Line;

    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Не удалось открыть файл!");
    }


    getline(file, Line);
    stringstream ss1(Line);
    bool first = true;
    while (getline(ss1, token,';')){
        if (token == "") break;
        if (!first) string_transition.push_back(token);
        first = false;
    }

    getline(file, Line);
    stringstream ss2(Line);
    first = true;
    while (getline(ss2, token,';')){
        if (token == "") break;
        if (!first) permitted_states.push_back(token);
        first = false;
    }

    getline(file, Line);
    stringstream ss3(Line);
    first = true;
    while (getline(ss3, token,';')){
        if (!first){
            start_state = token;
            break;
        }
        first = false;
    }

    getline(file, Line);
    stringstream ss4(Line);
    first = true;
    while (getline(ss4, token,';')){
        if (token == "") break;
        if (!first) alphabet.push_back(token);
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
                states.push_back(token);
                first = false;
            }
            else{
                if (count_alphabet < alphabet.size()){
                    if (!token.empty()){
                        transitions[current_state][alphabet[count_alphabet]] = token;  
                    }
                    ++count_alphabet;
                }
                else break;
            }
        }
    }
    
    return *this;
};


DFA5 DFABuilder::build() const{
    return DFA5(start_state, states, permitted_states, alphabet, transitions, string_transition, start_state);
}


}
