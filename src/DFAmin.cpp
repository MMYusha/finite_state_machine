#include <func_minimization/DFAmin.hpp> // публичные include подключаем как системные
#include <iostream>   // для std::cout
#include <vector> 
#include <unordered_map>
#include <unordered_set>   // для unordered_set

#include <queue>
#include <utility>   // для std::pair
#include <algorithm>   // для std::sort
using namespace std;


vector<vector<string>> func_minimization::DFAmin(
    const vector<string>& alphabet,
    const vector<string>& Q, 
    const vector<string>& F,
    const unordered_map<string, unordered_map<string, string>>& transitions) {
    


    // ----- ПРОВЕРКИ ВХОДНЫХ ДАННЫХ -----
    // 1) Все состояния из Q должны быть определены в transitions
    for (const string& state : Q) {
        if (transitions.find(state) == transitions.end()) {
            throw invalid_argument("Состояние \"" + state + "\" не найдено в таблице переходов");
        }
    }

    // 2) Все допускающие состояния из F должны присутствовать в Q
    for (const string& state : F) {
        if (find(Q.begin(), Q.end(), state) == Q.end()) {
            throw invalid_argument("Допустимое состояние \"" + state + "\" не найдено в Q");
        }
    }
    // ----- КОНЕЦ ПРОВЕРОК -----


    // Создание хэш-таблицы обратных переходов
    unordered_map<string, unordered_map<string, vector<string>>> Inv;
    for (string state : Q){ // перебор всех состояний
        auto it = transitions.find(state);
        for (auto pair : it->second){ // перебор всех переходов из состояния
            string symbol = pair.first; 
            string next  = pair.second;
            Inv[next][symbol].push_back(state);
        }
    }


    //P←{F, Q∖F}
    vector<unordered_set<string>> P ;
    //добавление в разбиение P допускающих состояний
    P.push_back(unordered_set<string>(F.begin(), F.end()));
    
    // добавление в разбиение P недопускающих состояний
    // Инициализация классов разбиения Class
    unordered_map<string, int> Class; // классы разбиения Class
    unordered_set<string> setF(F.begin(), F.end()); //хэш-таблица для быстрого поиска
    vector<string> nonF;
    for (string state : Q){
        if (setF.find(state) == setF.end()){
            nonF.push_back(state);
            Class[state] = 1;
        }
        else {
            Class[state] = 0;
        }
    }
    if (!nonF.empty()) {
        P.push_back(unordered_set<string>(nonF.begin(), nonF.end()));
    }
    
    // Инициализация очереди Queue
    queue<pair<int,string>> Queue;
    for (int idx = 0; idx < static_cast<int>(P.size()); ++idx) {
        for (const string& c : alphabet) {
            Queue.push({idx, c});
        }
    }

    // Инициализация классов разбиения Class

    // Основной цикл
    vector<int> Count(Q.size(), 0);
    vector<int> Twin(Q.size(), 0);
    vector<int> Involved;
    while (!Queue.empty()){
        // Получение пары из очереди [индекс класса Сплиттера, символ алфавита]
        auto [C, a] = Queue.front();
        Queue.pop();
        auto splitter = P[C]; 
        
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
            if (Count[i] < static_cast<int>(P[i].size())){
                P.push_back({}); // создадим пустой класс в разбиении P
                Twin[i] = static_cast<int>(P.size()) - 1; // индекс нового класса под разбиение
            }
        }

        // Перемещение состояний
        for (string q : splitter){
            for (string r : Inv[q][a]){
                int i = Class[r];
                int j = Twin[i];
                if (j != 0){
                    // перенос r из P[i] в P[j]
                    P[i].erase(r); // удаление r из старого класса
                    P[j].insert(r); // добавление r в новый класс
                    Class[r] = j; // обновление индекса класса, в котором r
                }
            }
        }

        for (int i : Involved){
            int j = Twin[i];
            if (j != 0){
                if (P[j].size()<P[i].size()){ // парный класс должен быть меньшего размера
                    swap(P[i],P[j]);
                }
                for (auto r : P[j]){
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
    vector<vector<string>> result;
    result.reserve(P.size());
    for (auto cls : P) {
        result.push_back(vector<string>(cls.begin(), cls.end()));
    }
    for (auto& vec : result) {
        sort(vec.begin(), vec.end());
    }
    return result;
}
