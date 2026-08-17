#include <algorithm>  // для std::sort
#include <func_DFA/DFA.hpp>
#include <iterator>
#include <queue>  //для bfs
#include <stdexcept>
#include <utility>

namespace func_DFA {
using std::invalid_argument;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;

using std::all_of;  // Для проверок

// для фильтрации после bfs
using std::back_inserter;
using std::copy_if;

HopcroftMinimizer::HopcroftMinimizer(const DFA5& input_dfa) : dfa(input_dfa) {}

void HopcroftMinimizer::ValidateInput() {
    // Получение входных данных
    const auto& states = dfa.getStates();
    const auto& alphabet = dfa.getAlphabet();
    const auto& transitions = dfa.getTransitions();
    const auto& permitted_states = dfa.getPermittedStates();
    const auto& start_state = dfa.getStartState();

    unordered_set<string> states_set(dfa.getStates().begin(), dfa.getStates().end());
    unordered_set<string> alphabetSet(alphabet.begin(), alphabet.end());

    // 1) Все состояния из states должны быть определены в transitions
    if (!all_of(
            states.begin(), states.end(),  // диапазон проверки // [&] - все переменные по ссылке;
                                           // (const string& state) - параметр
            [&](const string& state) { return transitions.contains(state); })) {  // тело проверки
        throw invalid_argument("Не все состояния найдены в таблице переходов");   // тело if
    }

    // 2) Все допускающие состояния из permitted_states должны присутствовать в states
    if (!all_of(permitted_states.begin(), permitted_states.end(),
                [&](const string& state) { return states_set.contains(state); })) {
        throw invalid_argument("Не все допустимые состояния найдены в states");
    }

    // 3) Все символы, используемые в переходах, должны принадлежать алфавиту
    for (const auto& [state, trans] : transitions) {
        for (const auto& [symbol, next] : trans) {
            if (!alphabetSet.contains(symbol)) {
                throw invalid_argument("Символ \"" + symbol + "\" не указан в алфавите");
            }
        }
    }

    // 4) Начальное состояние start_state присутствует в states
    if (!states_set.contains(start_state)) {
        throw invalid_argument("Начальное состояние \"" + start_state + "\" не найдено в states");
    }

    // 5) Входные данные не пустые
    if (alphabet.empty()) {
        throw invalid_argument("Алфавит не может быть пустым");
    }
    if (states.empty()) {
        throw invalid_argument("Множество состояний не может быть пустым");
    }
    if (permitted_states.empty()) {
        throw invalid_argument("Множество допускающих состояний не может быть пустым");
    }
    if (transitions.empty()) {
        throw invalid_argument("Таблица переходов не может быть пустой");
    }

    // ----- КОНЕЦ ПРОВЕРОК -----
}

unordered_set<string> HopcroftMinimizer::findReachableStates() {
    // Выполняет BFS для поиска всех состояний, достижимых из начального
    unordered_set<string> reachableStates;
    std::queue<string> bfs;
    bfs.push(dfa.getStartState());
    reachableStates.insert(dfa.getStartState());

    while (!bfs.empty()) {
        string cur = bfs.front();
        bfs.pop();
        auto it = dfa.getTransitions().find(cur);
        if (it != dfa.getTransitions().end()) {
            for (const auto& symPair : it->second) {
                const string& next = symPair.second;
                if (reachableStates.insert(next).second) {
                    bfs.push(next);
                }
            }
        }
    }
    return reachableStates;
}

void HopcroftMinimizer::buildReachableStates(const unordered_set<string>& reachableStates,
                                             vector<string>& newStates) {
    // Формирует новые множества состояний, переходов и допускающих на основе достижимых состояний
    copy_if(dfa.getStates().begin(), dfa.getStates().end(), back_inserter(newStates),
            [&](const string& state) { return reachableStates.contains(state); });
}

void HopcroftMinimizer::buildReachableTransitions(
    const unordered_set<string>& reachableStates,
    unordered_map<string, unordered_map<string, string>>& newTransitions) {
    // Формирует новые множества состояний, переходов и допускающих на основе достижимых состояний
    for (const string& state : dfa.getStates()) {
        if (reachableStates.contains(state)) {
            auto it = dfa.getTransitions().find(state);
            if (it != dfa.getTransitions().end()) {
                newTransitions[state] = it->second;
            }
        }
    }
}

void HopcroftMinimizer::buildReachablePermittedStates(const unordered_set<string>& reachableStates,
                                                      vector<string>& newPermittedStates) {
    copy_if(dfa.getPermittedStates().begin(), dfa.getPermittedStates().end(),
            back_inserter(newPermittedStates),
            [&](const string& state) { return reachableStates.contains(state); });
}

void HopcroftMinimizer::removeUnreachableStates() {
    // ----- УДАЛЕНИЕ НЕДОСТИЖИМЫХ СОСТОЯНИЙ -----

    auto reachableStates = findReachableStates();  // Поиск всех состояний, достижимых из начального

    // Формируем новые множества
    vector<string> newStates;
    unordered_map<string, unordered_map<string, string>> newTransitions;
    vector<string> new_permitted_states;

    buildReachableStates(reachableStates, newStates);
    buildReachableTransitions(reachableStates, newTransitions);
    buildReachablePermittedStates(reachableStates, new_permitted_states);

    // Проверка, что есть хотя бы 1 достижимое состояние
    if (newStates.empty()) {
        throw std::runtime_error(
            "После удаления недостижимых состояний не осталось ни одного состояния");
    }

    // Обновление dfa - Присваивает полю dfa новый автомат, построенный из переданных данных
    dfa = DFA5(dfa.getStartState(),  // не меняется
               std::move(newStates), std::move(new_permitted_states),
               dfa.getAlphabet(),  // не меняется
               std::move(newTransitions),
               dfa.getStringTransition(),  // не меняется
               dfa.getCurrentState()       // не меняется
    );
}

void HopcroftMinimizer::CreateInvariantTransitions() {
    // Создание хэш-таблицы обратных переходов
    for (string state : dfa.getStates()) {  // перебор всех состояний
        auto it = dfa.getTransitions().find(state);
        for (auto pair : it->second) {  // перебор всех переходов из состояния
            string symbol = pair.first;
            string next = pair.second;
            invariant_transitions[next][symbol].push_back(state);
        }
    }
}

void HopcroftMinimizer::InitPartition() {
    // P <- {permitted_states, Q∖F}

    // добавление в разбиение Partition допускающих состояний
    unordered_set<string> permitted_states_set(
        dfa.getPermittedStates().begin(),
        dfa.getPermittedStates().end());  // хэш-таблица для быстрого поиска
    Partition.push_back(permitted_states_set);

    // добавление в разбиение Partition недопускающих состояний
    // unordered_map<string, int> StateToClass; // классы разбиения StateToClass
    // Формирование вектора недопускающих состояний non_permitted_states
    vector<string> non_permitted_states;
    copy_if(dfa.getStates().begin(), dfa.getStates().end(), back_inserter(non_permitted_states),
            [&](const string& state) { return !permitted_states_set.contains(state); });

    if (!non_permitted_states.empty()) {
        // добавление в разбиение Partition недопускающих состояний non_permitted_states
        Partition.push_back(
            unordered_set<string>(non_permitted_states.begin(), non_permitted_states.end()));
    }
}

void HopcroftMinimizer::InitClass() {
    // Инициализация классов разбиения StateToClass
    unordered_set<string> permitted_states_set(
        dfa.getPermittedStates().begin(),
        dfa.getPermittedStates().end());  // хэш-таблица для быстрого поиска
    for (string state : dfa.getStates()) {
        if (permitted_states_set.find(state) != permitted_states_set.end()) {
            StateToClass[state] = 0;  // все допускающие состояния записываем в класс 0
        } else {
            StateToClass[state] = 1;  // все остальные состояния записываем в класс 1
        }
    }
}

void HopcroftMinimizer::InitQueue() {
    for (int idx = 0; idx < static_cast<int>(Partition.size()); ++idx) {
        for (const string& c : dfa.getAlphabet()) {
            Queue.push({idx, c});
        }
    }
}

void HopcroftMinimizer::takeSplitter() {
    auto [Class_id, sym] = Queue.front();
    Queue.pop();
    splitter = Partition[Class_id];
    symbol = sym;
}

void HopcroftMinimizer::fillInvolved() {
    Involved.clear();
    for (const string& q : splitter) {
        for (const string& r : invariant_transitions[q][symbol]) {
            int i = StateToClass[r];
            if (Count[i] == 0) {
                Involved.push_back(i);
            }
            ++Count[i];
        }
    }
}

void HopcroftMinimizer::split() {
    for (int i : Involved) {
        if (Count[i] > 0 && Count[i] < static_cast<int>(Partition[i].size()) &&
            Partition.size() < dfa.getStates().size()) {  //------------------------
            Partition.push_back({});
            Twin[i] = static_cast<int>(Partition.size()) - 1;
        }
    }
}

void HopcroftMinimizer::moveStatesInPartition() {
    // Перемещение состояний
    for (string q : splitter) {
        for (string r : invariant_transitions[q][symbol]) {
            int i = StateToClass[r];
            int j = Twin[i];
            if (j != 0) {
                // перенос r из Partition[i] в Partition[j]
                auto node = Partition[i].extract(r);   // удаление r из старого класса
                Partition[j].insert(std::move(node));  // добавление r в новый класс
                StateToClass[r] = j;                   // обновление индекса класса, в котором r
            }
        }
    }
}

void HopcroftMinimizer::moveStatesInClass() {
    // Перемещение состояние в переменной StateToClass
    for (int i : Involved) {
        int j = Twin[i];
        if (j != 0) {
            if (Partition[j].size() <
                Partition[i].size()) {  // парный класс должен быть меньшего размера
                swap(Partition[i], Partition[j]);
            }
            for (const string& r :
                 Partition[i]) {  // -------------------------------------- Исправление
                StateToClass[r] = i;
            }
            for (const string& r : Partition[j]) {
                StateToClass[r] = j;
            }
            for (const string& c : dfa.getAlphabet()) {
                Queue.push({j, c});  // добавление в очередь
            }
        }
        Count[i] = 0;
        Twin[i] = 0;
    }
}

vector<vector<string>> HopcroftMinimizer::getResultPartition() {
    // Преобразование результата в vector<vector<string>> для возврата + сортировка состояний внутри
    // классов
    vector<vector<string>> resultPartition;
    resultPartition.reserve(Partition.size());
    for (auto cls : Partition) {  // Преобразование результата в vector<vector<string>>
        resultPartition.push_back(vector<string>(cls.begin(), cls.end()));
    }
    for (auto& vec : resultPartition) {  // сортировка состояний внутри классов
        std::sort(vec.begin(), vec.end());
    }
    return resultPartition;
}

vector<vector<string>> HopcroftMinimizer::computePartition() {
    // Алгоритм минимизации Хопкрофта О(alphabet*n*log2n)

    // ----- ПРОВЕРКИ ВХОДНЫХ ДАННЫХ -----
    ValidateInput();

    // ----- УДАЛЕНИЕ НЕДОСТИЖИМЫХ СОСТОЯНИЙ -----
    removeUnreachableStates();

    // ----- ПОДГОТОВКА  -----
    CreateInvariantTransitions();  // Создание хэш-таблицы обратных переходов
    InitPartition();  // Первичное разбиение: P←{permitted_states, Q∖F}
    InitClass();      // Инициализация классов разбиения StateToClass
    InitQueue();      // Инициализация очереди Queue

    // ----- ОСНОВНОЙ ЦИКЛ  -----
    Count.assign(dfa.getStates().size(), 0);
    Twin.assign(dfa.getStates().size(), 0);
    while (!Queue.empty()) {
        takeSplitter();  // Получение пары из очереди [индекс класса Сплиттера, символ алфавита]
        fillInvolved();  // Заполнение Involved
        split();         // Проверка возможности разбиения классов в Involved по сплиттеру
        moveStatesInPartition();  // Перемещение состояний в переменной Partition
        moveStatesInClass();      // Перемещение состояние в переменной StateToClass
    }

    // Преобразование результата в vector<vector<string>> для возврата + сортировка состояний внутри
    // классов
    auto resultPartition = getResultPartition();
    return resultPartition;
}

vector<string> DFA5::CreateNewStates(const vector<vector<string>>& Partition) const {
    vector<string> newStates;
    for (auto cls : Partition) {
        // Формируем имя нового состояния (класса) как сумму состояний в классе через '+'
        auto stateName = CreateStateName(cls);
        newStates.push_back(stateName);
    }
    return newStates;
}

string DFA5::CreateStateName(const vector<string>& cls) const {
    // Формируем имя нового состояния (класса) как сумму состояний в классе через '+'
    string stateName;
    for (size_t i = 0; i < cls.size(); ++i) {
        if (i > 0) stateName += "+";
        stateName += cls[i];
    }
    return stateName;
}

unordered_map<string, string> DFA5::CreateStateToNewState(
    const vector<vector<string>>& Partition) const {
    // отображение "старое состояние -> имя нового состояния (класса)"
    unordered_map<string, string> stateToNewState;
    for (auto cls : Partition) {
        for (string state : cls) {
            auto stateName = CreateStateName(cls);
            stateToNewState[state] = stateName;  // запоминаем соответствие
        }
    }
    return stateToNewState;
}

unordered_map<string, unordered_map<string, string>> DFA5::CreateNewTransitions(
    const vector<vector<string>>& Partition,
    const unordered_map<string, string>& stateToNewState) const {
    unordered_map<string, unordered_map<string, string>> newTransitions;
    for (auto cls : Partition) {
        string className = stateToNewState.at(cls.at(0));  // имя текущего класса

        unordered_map<string, string> trans;  // переходы для этого класса
        for (string sym : alphabet) {
            // Берем переход из любого состояния класса (возьмём первое)
            string oldTarget = transitions.at(cls.at(0)).at(sym);
            // Находим класс, в который попадаем
            string newTarget = stateToNewState.at(oldTarget);
            trans[sym] = newTarget;
        }
        newTransitions[className] = trans;
    }
    return newTransitions;
}

vector<string> DFA5::CreateNewPermittedStates(
    const unordered_map<string, string>& stateToNewState) const {
    vector<string> newPermittedStates;
    unordered_set<string> newPermittedStates_set;  // контейнер
    for (string state : permitted_states) {
        auto it = stateToNewState.find(state);
        if (it != stateToNewState.end()) {
            // добавляем только уникальные классы
            newPermittedStates_set.insert(it->second);  // дубликат игнорируется
        }
    }
    newPermittedStates.assign(newPermittedStates_set.begin(),
                              newPermittedStates_set.end());  // контейнер -> вектор
    return newPermittedStates;
}

DFA5 DFA5::CreateNewDFAwithPartition(const vector<vector<string>>& Partition) const {
    // Создание нового ДКА по классам разбиения

    auto newStates = CreateNewStates(
        Partition);  // Формируем имя состояния как сумму состояний в классе через '+'

    auto stateToNewState = CreateStateToNewState(
        Partition);  // отображение "старое состояние -> имя нового состояния (класса)"

    auto newTransitions =
        CreateNewTransitions(Partition, stateToNewState);  // построение новых переходов

    auto new_start_state = stateToNewState[start_state];  // Обновление начального состояния

    auto newPermittedStates =
        CreateNewPermittedStates(stateToNewState);  // Обновление допустимых состояний

    auto new_current_state = stateToNewState[current_state];  // Обновление текущего состояния

    return DFA5(new_start_state, newStates, newPermittedStates, alphabet, newTransitions,
                string_transition, new_current_state);
}

void DFA5::minimize() {
    HopcroftMinimizer minimizer(*this);
    auto Partition = minimizer.computePartition();
    auto minimizedDFA = CreateNewDFAwithPartition(Partition);
    *this = minimizedDFA;
}

}  // namespace func_DFA
