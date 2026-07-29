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

vector<vector<string>> DFA5::computePartition() const {
    // Алгоритм минимизации Хопкрофта О(alphabet*n*log2n)

    // ----- ПРОВЕРКИ ВХОДНЫХ ДАННЫХ -----
    // 1) Все состояния из states должны быть определены в transitions
    for (const string& state : states) {
        if (transitions.find(state) == transitions.end()) {
            throw invalid_argument("Состояние \"" + state + "\" не найдено в таблице переходов");
        }
    }

    // 2) Все допускающие состояния из permitted_states должны присутствовать в states
    for (const string& state : permitted_states) {
        if (find(states.begin(), states.end(), state) == states.end()) {
            throw invalid_argument("Допустимое состояние \"" + state + "\" не найдено в states");
        }
    }

    // 3) Все символы, используемые в переходах, должны принадлежать алфавиту
    unordered_set<string> alphabetSet(alphabet.begin(), alphabet.end());
    for (const auto& statePair : transitions) {
        for (const auto& symPair : statePair.second) {
            const string& symbol = symPair.first;
            if (alphabetSet.find(symbol) == alphabetSet.end()) {
                throw invalid_argument(
                    "Symbol \"" + symbol + "\" used in transitions but not in alphabet"
                );
            }
        }
    }

    // 4) Входные данные не пустые
    if (alphabet.empty()) {
        throw std::invalid_argument("Алфавит не может быть пустым");
    }
    if (states.empty()) {
        throw std::invalid_argument("Множество состояний не может быть пустым");
    }
    if (permitted_states.empty()) {
        throw std::invalid_argument("Множество допускающих состояний не может быть пустым");
    }
    if (transitions.empty()) {
        throw std::invalid_argument("Таблица переходов не может быть пустой");
    }
    // ----- КОНЕЦ ПРОВЕРОК -----


    // ----- УДАЛЕНИЕ НЕДОСТИЖИМЫХ СОСТОЯНИЙ -----
    // Создаём изменяемые копии
    vector<string> localStates = states;
    vector<string> local_permitted_states = permitted_states;
    unordered_map<string, unordered_map<string, string>> localTransitions = transitions;

    if (!localStates.empty()) {
        unordered_set<string> reachable;
        queue<string> bfs;
        bfs.push(start_state);
        reachable.insert(start_state);

        while (!bfs.empty()) {
            string cur = bfs.front();
            bfs.pop();
            auto it = localTransitions.find(cur);
            if (it != localTransitions.end()) {
                for (const auto& symPair : it->second) {
                    const string& next = symPair.second;
                    if (reachable.find(next) == reachable.end()) {
                        reachable.insert(next);
                        bfs.push(next);
                    }
                }
            }
        }

        // Формируем новые множества
        vector<string> newStates;
        unordered_map<string, unordered_map<string, string>> newTransitions;
        for (const string& state : localStates) {
            if (reachable.find(state) != reachable.end()) {
                newStates.push_back(state);
                auto it = localTransitions.find(state);
                if (it != localTransitions.end()) {
                    newTransitions[state] = it->second;
                }
            }
        }

        vector<string> new_permitted_states;
        for (const string& state : local_permitted_states) {
            if (reachable.find(state) != reachable.end()) {
                new_permitted_states.push_back(state);
            }
        }

        // Заменяем локальные данные
        localStates = move(newStates);
        local_permitted_states = move(new_permitted_states);
        localTransitions = move(newTransitions);

        if (localStates.empty()) {
            throw runtime_error("После удаления недостижимых состояний не осталось ни одного состояния");
        }
    }
    // ----- КОНЕЦ УДАЛЕНИЯ -----



    // Создание хэш-таблицы обратных переходов
    unordered_map<string, unordered_map<string, vector<string>>> Inv;
    for (string state : localStates){ // перебор всех состояний
        auto it = localTransitions.find(state);
        for (auto pair : it->second){ // перебор всех переходов из состояния
            string symbol = pair.first; 
            string next  = pair.second;
            Inv[next][symbol].push_back(state);
        }
    }


    //P←{permitted_states, Q∖F}
    vector<unordered_set<string>> Partition ;
    //добавление в разбиение Partition допускающих состояний
    Partition.push_back(unordered_set<string>(local_permitted_states.begin(), local_permitted_states.end()));
    
    // добавление в разбиение Partition недопускающих состояний
    // Инициализация классов разбиения Class
    unordered_map<string, int> Class; // классы разбиения Class
    unordered_set<string> setF(local_permitted_states.begin(), local_permitted_states.end()); //хэш-таблица для быстрого поиска
    vector<string> nonF;
    for (string state : localStates){
        if (setF.find(state) == setF.end()){
            nonF.push_back(state);
            Class[state] = 1;
        }
        else {
            Class[state] = 0;
        }
    }
    if (!nonF.empty()) {
        Partition.push_back(unordered_set<string>(nonF.begin(), nonF.end()));
    }
    
    // Инициализация очереди Queue
    queue<pair<int,string>> Queue;
    for (int idx = 0; idx < static_cast<int>(Partition.size()); ++idx) {
        for (const string& c : alphabet) {
            Queue.push({idx, c});
        }
    }

    // Инициализация классов разбиения Class

    // Основной цикл
    vector<int> Count(localStates.size(), 0);
    vector<int> Twin(localStates.size(), 0);
    vector<int> Involved;


    long long iterations = 0;
    const long long MAX_ITERATIONS = 10000000; 


    while (!Queue.empty()){
        ++iterations;
        //====================================================================
        // ----- ДОБАВЛЕНО: защита от бесконечного роста классов -----
        if (Partition.size() > localStates.size()) {
            throw std::runtime_error(
                "Количество классов превысило число состояний: " + 
                std::to_string(Partition.size()) + " > " + std::to_string(localStates.size())
            );
        }

        // ----- ДОБАВЛЕНО: периодический вывод прогресса -----
        if (iterations % 100000 == 0) {
            std::cout << "[DFAmin] Iteration " << iterations 
                      << ", classes: " << Partition.size() 
                      << ", queue: " << Queue.size() << std::endl;
        }

        // ----- ДОБАВЛЕНО: защита от бесконечного цикла -----
        if (iterations > MAX_ITERATIONS) {
            throw std::runtime_error(
                "Превышено максимальное число итераций (" + 
                std::to_string(MAX_ITERATIONS) + ")"
            );
        }
        //=======================================================================


        // Получение пары из очереди [индекс класса Сплиттера, символ алфавита]
        auto [C, a] = Queue.front();
        Queue.pop();
        auto splitter = Partition[C]; 
        
        // Заполнение Involved 
        Involved.clear();   
        for (string q : splitter){
            for (string r : Inv[q][a]){
                int i = Class[r];
                if (Count[i] == 0){
                    Involved.push_back(i);
                }
                ++Count[i];
            }
        }

        // Проверка возможности разбиения классов в Involved по сплиттеру С
        for (int i : Involved){
            if (Count[i] > 0 && Count[i] < static_cast<int>(Partition[i].size()) && Partition.size() < localStates.size()) {//------------------------
                Partition.push_back({});
                Twin[i] = static_cast<int>(Partition.size()) - 1;
            }
        }

        // Перемещение состояний
        for (string q : splitter){
            for (string r : Inv[q][a]){
                int i = Class[r];
                int j = Twin[i];
                if (j != 0){
                    // перенос r из Partition[i] в Partition[j]
                    Partition[i].erase(r); // удаление r из старого класса
                    Partition[j].insert(r); // добавление r в новый класс
                    Class[r] = j; // обновление индекса класса, в котором r
                }
            }
        }

        for (int i : Involved){
            int j = Twin[i];
            if (j != 0){
                if (Partition[j].size()<Partition[i].size()){ // парный класс должен быть меньшего размера
                    swap(Partition[i],Partition[j]);
                }
                for (const string& r : Partition[i]) { // -------------------------------------- Исправление
                    Class[r] = i;
                }
                for (auto r : Partition[j]){
                    Class[r] = j;
                }
                for (string c : alphabet){
                    Queue.push({j,c});
                }
            }
            Count[i] = 0;
            Twin[i] = 0;
        }
    }

    // Преобразование результата в vector<vector<char>> для возврата
    vector<vector<string>> resultPartition;
    resultPartition.reserve(Partition.size());
    for (auto cls : Partition) {
        resultPartition.push_back(vector<string>(cls.begin(), cls.end()));
    }
    for (auto& vec : resultPartition) {
        sort(vec.begin(), vec.end());
    }
    return resultPartition;
}


DFA5 DFA5::CreateNewTransitions(const vector<vector<string>>& Partition) const {

    // отображение "старое состояние -> имя класса" 
    unordered_map<string, string> stateToNewState;
    vector<string> newStates;

    for (auto cls : Partition) {
        // Формируем имя класса как сумму состояний через '+'
        string className;
        for (size_t i = 0; i < cls.size(); ++i) {
            if (i > 0) className += "+";
            className += cls[i];
        }
        newStates.push_back(className);
        for (string state : cls) {
            stateToNewState[state] = className;   // запоминаем соответствие
        }
    }

    // построение новых переходов 
    unordered_map<string, unordered_map<string, string>> newTransitions;

    for (auto cls : Partition) {
        string className = stateToNewState[cls[0]]; // имя текущего класса

        unordered_map<string, string> trans; // переходы для этого класса
        for (string sym : alphabet) {
            // Берем переход из любого состояния класса (возьмём первое)
            string oldTarget = transitions.at(cls[0]).at(sym);
            // Находим класс, в который попадаем
            string newTarget = stateToNewState[oldTarget];
            trans[sym] = newTarget;
        }
        newTransitions[className] = trans;
    }

    
    // Обновление начального состояния
    auto new_start_state = stateToNewState[start_state];
    
    // Обновление допустимых состояний
    vector<string> newPermitted;
    for (string st : permitted_states) {
        auto it = stateToNewState.find(st);
        if (it != stateToNewState.end()) {
            // добавляем только уникальные классы
            if (find(newPermitted.begin(), newPermitted.end(), it->second) == newPermitted.end())
                newPermitted.push_back(it->second);
        }
    }

    // Обновление текущего состояния
    auto new_current_state = stateToNewState[current_state];


    return DFA5(new_start_state, newStates, newPermitted, alphabet, newTransitions, string_transition, new_current_state);
}


void DFA5::minimize(){
    auto Partition = computePartition();
    auto minimizedDFA = CreateNewTransitions(Partition);
    *this = minimizedDFA; 
    cout << "\n========= Минимизация ========" << endl;
    print();
}


void DFA5::transit_string(const vector<string>& str) {
    cout << "\n===== Осуществление перехода ====="<< endl;
    cout << "Переход из состояния - " << current_state << endl;
    cout << "По строке - "; 
    auto flag = true;
    // Обработка каждого символа
    for (string ch : str) {   
        // Вывод строки перехода
        if (flag) {
            cout << ch;
            flag = false;
        }
        else{
            cout << ", " << ch;
        }
        
        // Ищем таблицу переходов из текущего состояния
        auto state_it = transitions.find(current_state);
        if (state_it == transitions.end()) {
            std::cout << "\n-----------Нет переходов из состояния " << current_state << "---------------\n";
            break; // автомат застрял
        }
        // Ищем переход по текущему символу
        // Внутренняя таблица – это unordered_map<string, string>
        auto sym_it = state_it->second.find(ch);
        if (sym_it == state_it->second.end()) {
            std::cout << "\n------------Нет перехода по символу '" << ch << "'-------------------\n";
            break; // переход не определён
        }

        // Выполняем переход
        current_state = sym_it->second;
        //std::cout << "\nПереход по '" << ch << "' → состояние " << current_state << "\n";
    }
    cout << "\nВ состояние - " << current_state << endl;
    cout << endl;

}


void DFA5::transit_Input(const vector<string>& input){
    transit_string(input);
}

void DFA5::transit_fromCSV(){
    transit_string(string_transition);
}



}
