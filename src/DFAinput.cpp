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
    stringstream ss(firstLine);
    bool first = true;
    while (getline(ss, token,';')){
        if (!first) res.string_transition.push_back(token);
        first = false;
    }




    return res;
    }