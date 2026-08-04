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

// Создание полей класса DFABuilder через CSV
DFABuilder& DFABuilder::withCSV(const string& filename){
    // Проверка открытия файла
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Не удалось открыть файл!");
    }

    // Чтение файла
    readStringTransition(file);     // запись string_transition
    readPermittedStates(file);      // запись permitted_states
    readStartState(file);           // запись start_state
    readAlphabet(file);             // запись alphabet
    readStatesAndTransitions(file); // запись states и transitions
    
    return *this;
};


// Создание поля string_transition через CSV
void DFABuilder::readStringTransition(ifstream& file){
    // Вспомогательные структуры для чтения
    string token;
    string Line;

    // Проверка чтения строки
    if (!getline(file, Line)) {
        throw std::runtime_error("Не удалось прочитать строку с переходом");
    }
    
    // Чтение строки и запись
    stringstream ss1(Line);
    bool first = true;
    while (getline(ss1, token,';')){
        if (token.empty()) break; // если токен пустой, то достигнут конец содержимого строки
        if (!first) string_transition.push_back(token); // пропуск заголовка строки в CSV
        first = false;
    }
}


// Создание поля permitted_states через CSV
void DFABuilder::readPermittedStates(ifstream& file){
    // Вспомогательные структуры для чтения
    string token;
    string Line;

    // Проверка чтения строки
    if (!getline(file, Line)) {
        throw std::runtime_error("Не удалось прочитать строку с допустимыми состояниями");
    }
    
    // Чтение строки и запись
    stringstream ss2(Line);
    bool first = true;
    while (getline(ss2, token,';')){
        if (token.empty()) break;   // если токен пустой, то достигнут конец содержимого строки
        if (!first) permitted_states.push_back(token); // пропуск заголовка строки в CSV
        first = false;
    }
}


// Создание поля start_state через CSV
void DFABuilder::readStartState(ifstream& file){
    // Вспомогательные структуры для чтения
    string token;
    string Line;

    // Проверка чтения строки
    if (!getline(file, Line)) {
        throw std::runtime_error("Не удалось прочитать строку с начальным состоянием");
    }
    
    // Чтение строки и запись
    stringstream ss3(Line);
    bool first = true;
    while (getline(ss3, token,';')){
        if (!first){ // пропуск заголовка строки в CSV
            start_state = token;
            break; 
            // Начальное состояние только одно -> выходим как было получено 1 значение
        }
        first = false;
    }
}


// Создание поля alphabet через CSV
void DFABuilder::readAlphabet(ifstream& file){
    // Вспомогательные структуры для чтения
    string token;
    string Line;

    // Проверка чтения строки
    if (!getline(file, Line)) {
        throw std::runtime_error("Не удалось прочитать строку с Алфавитом (шапка таблицы переходов)");
    }
    
    // Чтение строки и запись
    stringstream ss2(Line);
    bool first = true;
    while (getline(ss2, token,';')){
        if (token.empty()) break;   // если токен пустой, то достигнут конец содержимого строки
        if (!first) alphabet.push_back(token); // пропуск заголовка строки в CSV
        first = false;
    }
}


// Создание полей states и transitions через CSV
void DFABuilder::readStatesAndTransitions(ifstream& file){
    // Вспомогательные структуры для чтения
    string token;
    string Line;
   
    // Чтение строк таблицы переходов до конца файла
    // Первый столбец таблицы - вектор состояний
    while (getline(file, Line)){
        stringstream ss(Line);
        string current_state;
        bool first = true;
        size_t count_alphabet = 0;
        while (getline(ss, token,';')){ 
            // Чтение и запись вектора состояний
            if (first) {
                current_state = token;
                states.push_back(current_state); 
                first = false;
            }
            // Чтение и запись таблицы переходов 
            else{
                if (count_alphabet < alphabet.size()){ 
                    // если токен пустой, то перехода по символу алфавита под номером count_alphabet не существует
                    if (!token.empty()){ 
                        // запись перехода: "current_state --alphabet[count_alphabet]--> token"
                        transitions[current_state][alphabet[count_alphabet]] = token;  
                    }
                    ++count_alphabet; // чтение следующего символа алфавита
                }
                // если count_alphabet превышает размер алфавита, то достигнут конец содержимого строки
                else break;
            }
        }
    }
}
//=======================================================================



DFABuilder& DFABuilder::generatedDFA(int number_of_states, int alphabet_size, const string& mode, int seed) {
    // Проверки
    if (number_of_states < 2) 
        throw invalid_argument("number_of_states должно быть не меньше 2");
    if (alphabet_size < 1) 
        throw invalid_argument("alphabet_size должен быть положительным");
    if (mode != "sparse" && mode != "full")
        throw invalid_argument("mode должен быть 'sparse' или 'full'");

    // 1. Создаём состояния и алфавит
    generateStates(number_of_states);
    generateAlphabet(alphabet_size);

    // 2. Создаём таблицу переходов с достижимостью всех состояний
    generateTransitions();

    // 4. Вычисляем целевую плотность переходов
    int capacity = computeCapacity(mode);

    // 5. Формируем список доступных ячеек (state, symbol), в которых ещё нет перехода
    vector<pair<string, string>> available_keys = computeAvailableKeys(seed);

    // 6. Добавляем недостающие переходы до capacity
    addTransitionsToCapacity(available_keys, capacity, seed);

    // 7. Начальное и финальные состояния
    start_state = states[0];
    permitted_states = {states[number_of_states - 1]};  // только последнее состояние

    return *this;
}

void DFABuilder::generateStates(int number_of_states){
    for (int i = 0; i < number_of_states; ++i)
        states.push_back("q" + to_string(i));
}

void DFABuilder::generateAlphabet(int alphabet_size){
    for (int i = 0; i < alphabet_size; ++i)
        alphabet.push_back("q" + to_string(i));
}

void DFABuilder::generateTransitions(){
    // Инициализируем таблицу переходов (все внутренние переходы пусты)
    for (const auto& state : states) {
        transitions[state] = {};
    }
    
    // Строим цепочку по первому символу алфавита (гарантирует достижимость всех состояний)
    for (int i = 0; i < states.size() - 1; ++i) {
        transitions[states[i]][alphabet[0]] = states[i + 1];
    }
}


int DFABuilder::computeCapacity(const string& mode){
    int capacity = states.size() * alphabet.size();
    if (mode == "sparse" && capacity % 2 != 0) {
        throw invalid_argument("Для sparse режима number_of_states*|Sigma| должно быть чётным");
    }
    int target_m = (mode == "full") ? capacity : capacity / 2;
    if (target_m < states.size() - 1) {
        throw invalid_argument("Целевая плотность недостаточна для достижимости всех состояний");
    }
    return target_m;
}


vector<pair<string, string>> DFABuilder::computeAvailableKeys(int seed){
    vector<pair<string, string>> available_keys;
    for (const auto& state : states) {
        for (const auto& sym : alphabet) {
            auto it_state = transitions.find(state);
            if (it_state != transitions.end() && it_state->second.find(sym) == it_state->second.end()) {
                available_keys.emplace_back(state, sym);
            }
        }
    }

    // Перемешиваем доступные ячейки
    mt19937 rng(seed); // Генератор случайных чисел с фиксированным seed
    shuffle(available_keys.begin(), available_keys.end(), rng);

    return available_keys;
}


void DFABuilder::addTransitionsToCapacity(vector<pair<string, string>> available_keys, int capacity, int seed){
    mt19937 rng(seed); // Генератор случайных чисел с фиксированным seed
    int current_count = states.size() - 1;  // уже есть цепочка
    int to_add = capacity - current_count;
    uniform_int_distribution<> dist(0, states.size() - 1);
    for (int i = 0; i < to_add && i < (int)available_keys.size(); ++i) {
        const auto& key = available_keys[i];
        string target_state = states[dist(rng)];
        transitions[key.first][key.second] = target_state;
    }
}


DFABuilder& DFABuilder::withComponents( string start_state, 
                            vector<string> states, 
                            vector<string> permitted_states, 
                            vector<string> alphabet, 
                            unordered_map<string, unordered_map<string, string>> transitions){
    this->start_state = start_state;
    this->states = states;
    this->permitted_states = permitted_states;
    this->alphabet = alphabet;
    this->transitions = transitions;
    
    return *this;
    }

DFA5 DFABuilder::build() const{
    return DFA5(start_state, states, permitted_states, alphabet, transitions, string_transition, start_state);
}


}
