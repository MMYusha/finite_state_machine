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

DFA5::DFA5(
    const string& start,
    const vector<string>& st,
    const vector<string>& perm,
    const vector<string>& alph,
    const unordered_map<string, unordered_map<string, string>>& trans,
    const vector<string>& str_transition,
    const string current
) : start_state(start), states(st), permitted_states(perm),
    alphabet(alph), transitions(trans), string_transition(str_transition), current_state(current) {}

//Геттеры
const string& DFA5::getStartState() const {return start_state;};   
const vector<string>& DFA5::getStates() const {return states;}; 
const vector<string>& DFA5::getPermittedStates() const {return permitted_states;};
const vector<string>& DFA5::getAlphabet() const {return alphabet;};
const unordered_map<string, unordered_map<string, string>>& DFA5::getTransitions() const {return transitions;};
const vector<string>& DFA5::getStringTransition() const {return string_transition;};
const string& DFA5::getCurrentState() const {return current_state;};

void DFA5::resetCurrentState() {this->current_state = start_state;};

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

    cout << "Текущее состояние - " << current_state << endl;
    cout << endl;


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
    printf("\n====== ДКА сохранен в %s ======\n",filename.c_str());
}

}