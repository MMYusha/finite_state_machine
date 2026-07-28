#include <func_DFA/DFA.hpp> // публичные include подключаем как системные
#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>


using namespace std;

namespace func_DFA{
DFA5::DFA5(
    const string& start,
    const vector<string>& st,
    const vector<string>& perm,
    const vector<string>& alph,
    const unordered_map<string, unordered_map<string, string>>& trans,
    const vector<string>& str_transition
) : start_state(start), states(st), permitted_states(perm),
    alphabet(alph), transitions(trans), string_transition(str_transition) {}

DFA5 DFA5::fromCSV(const string& filename){
    // Вспомогательные структуры для чтения
    string token;
    string Line;

    // Структуры для чтения
    vector<string> string_transition;
    vector<string> permitted_states;
    string start_state;
    vector<string> alphabet;
    vector<string> states;
    unordered_map<string, unordered_map<string, string>> transitions;

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

    return DFA5(start_state, states, permitted_states, alphabet, transitions, string_transition);
};

void DFA5::print(){
    // Информация о ДКА из файла
    cout << "\n===== Информация о ДКА =====" << endl;

    cout << "Начальное состояние - " << start_state << endl;

    cout << "Набор состояний - ";
    for (auto cls : states) {   
        printf("%s ", cls.c_str());
    }
    printf("\n");

    cout << "Допустимые состояния - ";
    for (auto cls : permitted_states) {   
        printf("%s ", cls.c_str());
    }
    printf("\n");

    cout << "Алфавит - " ;
    for (auto cls : alphabet) {   
        printf("%s ", cls.c_str());
    }
    printf("\n");

    cout << "Таблица переходов:" << endl;
    for (const auto& state : states) {
        auto it = transitions.find(state);
        if (it != transitions.end()) {
            for (const auto& symbol : alphabet) {
                auto symIt = it->second.find(symbol);
                if (symIt != it->second.end()) {
                    cout << state << " --" << symbol << "--> " << symIt->second << endl;
                } else {
                    cout << state << " --" << symbol << "--> (undefined)" << endl;
                }
            }
        } else {
            cout << state << " Нет таблицы переходов" << endl;
        }
    }

    cout << "Строка перехода - ";
    for (auto cls : string_transition) {   
        printf("%s ", cls.c_str());
    }
    printf("\n");


}

void DFA5::exportCSV(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Не удалось открыть файл для записи!");
    }
    const unsigned char bom[] = {0xEF, 0xBB, 0xBF};
    file.write(reinterpret_cast<const char*>(bom), sizeof(bom));

    // 1) Строка "переход;1;2;3;..."
    file << "Переход;";
    for (size_t i = 0; i < string_transition.size(); ++i) {
        if (i > 0) file << ";";
        file << string_transition[i];
    }
    file << "\n";

    // 2) Строка "допустимые состояния;A;B;C;..."
    file << "Допустимые состояния;";
    for (size_t i = 0; i < permitted_states.size(); ++i) {
        if (i > 0) file << ";";
        file << permitted_states[i];
    }
    file << "\n";

    // 3) Строка "Начальное состояние;A;;;..."
    file << "Начальное состояние;" << start_state << ";;;\n";

    // 4) Строка "состояния/алфавит;1;2;3;..."
    file << "состояния/алфавит;";
    for (size_t i = 0; i < alphabet.size(); ++i) {
        if (i > 0) file << ";";
        file << alphabet[i];
    }
    file << "\n";

    // 5) Строки состояний и переходов
    for (const auto& state : states) {
        file << state << ";";

        // Переходы по каждому символу алфавита
        auto itState = transitions.find(state);
        if (itState != transitions.end()) {
            const auto& transMap = itState->second;
            for (size_t i = 0; i < alphabet.size(); ++i) {
                if (i > 0) file << ";";
                auto itSym = transMap.find(alphabet[i]);
                if (itSym != transMap.end()) {
                    file << itSym->second;
                }
                // если перехода нет – пустое поле
            }
        } else {
            // если состояние отсутствует в таблице переходов – пустые поля
            for (size_t i = 0; i < alphabet.size(); ++i) {
                if (i > 0) file << ";";
                // пусто
            }
        }
        // Добавляем два пустых поля (;;) для соответствия формату
        file << ";;\n";
    }
    printf("\nДКА сохранен в %s\n\n",filename.c_str());
}

}
