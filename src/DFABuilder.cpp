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

#include <random>


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

DFABuilder& DFABuilder::generatedDFA(int number_of_states, int alphabet_size, const string& mode, int seed) {
    // Проверки
    if (number_of_states < 2) throw invalid_argument("number_of_states должно быть не меньше 2");
    if (alphabet_size < 1) throw invalid_argument("alphabet_size должен быть положительным");
    if (mode != "sparse" && mode != "full")
        throw invalid_argument("mode должен быть 'sparse' или 'full'");

    // Генератор случайных чисел с фиксированным seed
    mt19937 rng(seed);

    // 1. Создаём состояния и алфавит
    for (int i = 0; i < number_of_states; ++i)
        states.push_back("q" + to_string(i));

    for (int i = 0; i < alphabet_size; ++i)
        alphabet.push_back("a" + to_string(i));

    // 2. Инициализируем таблицу переходов (все внутренние карты пусты)

    for (const auto& state : states)
        transitions[state] = {};

    // 3. Строим цепочку по первому символу алфавита (гарантирует достижимость всех состояний)
    for (int i = 0; i < number_of_states - 1; ++i) {
        transitions[states[i]][alphabet[0]] = states[i + 1];
    }

    // 4. Вычисляем целевую плотность переходов
    int capacity = number_of_states * alphabet_size;
    if (mode == "sparse" && capacity % 2 != 0) {
        throw invalid_argument("Для sparse режима number_of_states*|Sigma| должно быть чётным");
    }
    int target_m = (mode == "full") ? capacity : capacity / 2;
    if (target_m < number_of_states - 1) {
        throw invalid_argument("Целевая плотность недостаточна для достижимости всех состояний");
    }

    // 5. Формируем список доступных ячеек (state, symbol), в которых ещё нет перехода
    vector<pair<string, string>> available_keys;
    for (const auto& state : states) {
        for (const auto& sym : alphabet) {
            auto it_state = transitions.find(state);
            if (it_state != transitions.end() && it_state->second.find(sym) == it_state->second.end()) {
                available_keys.emplace_back(state, sym);
            }
        }
    }

    // 6. Перемешиваем доступные ячейки
    shuffle(available_keys.begin(), available_keys.end(), rng);

    // 7. Добавляем недостающие переходы до target_m
    int current_count = number_of_states - 1;  // уже есть цепочка
    int to_add = target_m - current_count;
    uniform_int_distribution<> dist(0, number_of_states - 1);
    for (int i = 0; i < to_add && i < (int)available_keys.size(); ++i) {
        const auto& key = available_keys[i];
        string target_state = states[dist(rng)];
        transitions[key.first][key.second] = target_state;
    }

    // 8. Начальное и финальные состояния
    start_state = states[0];
    permitted_states = {states[number_of_states - 1]};  // только последнее состояние

    return *this;
}

DFA5 DFABuilder::build() const{
    return DFA5(start_state, states, permitted_states, alphabet, transitions, string_transition, start_state);
}


}
