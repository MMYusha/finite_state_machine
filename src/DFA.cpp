#include <func_DFA/DFA.hpp> // публичные include подключаем как системные

#include <iostream>

#include <unordered_map>
#include <fstream>



namespace func_DFA{
using std::string;
using std::vector;
using std::unordered_map;
using std::ofstream;
using std::find;
using std::cout;
using std::endl;


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
const string&           DFA5::getStartState()       const {return start_state;};   
const vector<string>&   DFA5::getStates()           const {return states;}; 
const vector<string>&   DFA5::getPermittedStates()  const {return permitted_states;};
const vector<string>&   DFA5::getAlphabet()         const {return alphabet;};
const vector<string>&   DFA5::getStringTransition() const {return string_transition;};
const string&           DFA5::getCurrentState()     const {return current_state;};
const unordered_map<string, unordered_map<string, string>>& DFA5::getTransitions() const {return transitions;};

void DFA5::resetCurrentState() {this->current_state = start_state;}; // возвращение ДКА в начальное состояние


void DFA5::print(){
    // Информация о ДКА
    cout << "\n===== Информация о ДКА =====" << endl;

    printStartState();
    printStates();
    printPermittedStates();
    printAlphabet();
    printTransitions();

    printStringTransition();
    printCurrentState();

    cout << endl;
}


void DFA5::printStartState(){
    cout << "Начальное состояние - " << start_state << endl;
}
void DFA5::printStates(){
    cout << "Набор состояний - ";
    for (auto cls : states) {   
        printf("%s ", cls.c_str());
    }
    printf("\n");
}
void DFA5::printPermittedStates(){
    cout << "Допустимые состояния - ";
    for (auto cls : permitted_states) {   
        printf("%s ", cls.c_str());
    }
    printf("\n");
}
void DFA5::printAlphabet(){
    cout << "Алфавит - " ;
    for (auto cls : alphabet) {   
        printf("%s ", cls.c_str());
    }
    printf("\n");
}
void DFA5::printTransitions(){
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
}
void DFA5::printStringTransition(){
    cout << "Строка перехода - ";
    auto flag = true;
    for (auto symbol : string_transition) {
        if (flag) {
            cout << symbol;
            flag = false;
        }
        else{
            cout << ", " << symbol;
        } 
    } 
    cout << endl;  
}
void DFA5::printCurrentState(){
    cout << "Текущее состояние - " << current_state << endl;
}


void DFA5::exportCSV(const string& filename) {
    // Открытие файла для записи
    ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Не удалось открыть файл для записи!");
    }

    // Чтобы при открытии CSV файла в Excel и т.д. нормально отображались русский буквы
    const unsigned char bom[] = {0xEF, 0xBB, 0xBF};
    file.write(reinterpret_cast<const char*>(bom), sizeof(bom));

    // Запись ДКА в файл
    exportStringTransitionCSV(file);// 1) Строка "переход;1;2;3;..."
    exportPermittedStatesCSV(file); // 2) Строка "допустимые состояния;A;B;C;..."
    exportStartStateCSV(file);      // 3) Строка "Начальное состояние;A;;;..."
    exportAlphabetCSV(file);        // 4) Строка "состояния/алфавит;1;2;3;..." - шапка таблицы переходов, она же алфавит
    exportTransitionsCSV(file);     // 5) Строки состояний и переходов: первый столбец - набор состояний; остальные столбцы - содержимое таблицы переходов
    
    printf("\n====== ДКА сохранен в %s ======\n",filename.c_str());
}


void DFA5::exportStringTransitionCSV(ofstream& file){
    // 1) Строка "переход;1;2;3;..."
    file << "Переход;";
    for (size_t i = 0; i < string_transition.size(); ++i) {
        if (i > 0) file << ";";
        file << string_transition[i];
    }
    file << "\n";
}
void DFA5::exportPermittedStatesCSV(ofstream& file){
    // 2) Строка "допустимые состояния;A;B;C;..."
    file << "Допустимые состояния;";
    for (size_t i = 0; i < permitted_states.size(); ++i) {
        if (i > 0) file << ";";
        file << permitted_states[i];
    }
    file << "\n";
}
void DFA5::exportStartStateCSV(ofstream& file){
    // 3) Строка "Начальное состояние;A;;;..."
    file << "Начальное состояние;" << start_state << ";;;\n";

}
void DFA5::exportAlphabetCSV(ofstream& file){
    // 4) Строка "состояния/алфавит;1;2;3;..." - шапка таблицы переходов, она же алфавит
    file << "состояния/алфавит;";
    for (size_t i = 0; i < alphabet.size(); ++i) {
        if (i > 0) file << ";";
        file << alphabet[i];
    }
    file << "\n";
}
void DFA5::exportTransitionsCSV(ofstream& file){
    // 5) Строки состояний и переходов: первый столбец - набор состояний; остальные столбцы - содержимое таблицы переходов 
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
}



}