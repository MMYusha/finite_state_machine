#include <func_input/DFAinput.hpp> // публичные include подключаем как системные
#include <iostream>   
#include <sstream>
#include <fstream>
#include <vector> 
#include <unordered_map>    
#include <string>

using namespace std;


func_input::Result func_input::DFAinput(string filename) {
    Result res;
    ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл!\n";
        return res;
    }

    string token;

    string firstLine;
    getline(file, firstLine);
    stringstream ss1(firstLine);
    bool first = true;
    while (getline(ss1, token,';')){
        if (token == "") break;
        if (!first) res.string_transition.push_back(token);
        first = false;
    }

    string secondLine;
    getline(file, secondLine);
    stringstream ss2(secondLine);
    first = true;
    while (getline(ss2, token,';')){
        if (token == "") break;
        if (!first) res.permited_state.push_back(token);
        first = false;
    }

    string ThirdLine;
    getline(file, ThirdLine);
    stringstream ss3(ThirdLine);
    first = true;
    while (getline(ss3, token,';')){
        if (!first){
            res.start_state = token;
            break;
        }
        first = false;
    }





    return res;
    }