#include <func_minimization/DFAmin.hpp> // публичные include подключаем как системные
#include <iostream>   // для std::cout
#include <vector> 
#include <unordered_map>
#include <unordered_set>   // для unordered_set

#include <queue>
#include <utility>   // для std::pair

using namespace std;


vector<vector<char>> func_minimization::DFAmin(
    const vector<char>& alphabet,
    const vector<char>& Q, 
    const vector<char>& F,
    const unordered_map<char, unordered_map<char, char>>& transitions) {
    
    // Создание хэш-таблицы обратных переходов
    unordered_map<char, unordered_map<char, vector<char>>> Inv;
    for (char state : Q){ // перебор всех состояний
        auto it = transitions.find(state);
        for (auto pair : it->second){ // перебор всех переходов из состояния
            char symbol = pair.first; 
            char next  = pair.second;
            Inv[next][symbol].push_back(state);
        }
    }


    //P←{F, Q∖F}
    vector<unordered_set<char>> P;
    //добавление в разбиение P допускающих состояний
    P.push_back(unordered_set<char>(F.begin(), F.end()));
    

    // добавление в разбиение P недопускающих состояний
    // Инициализация классов разбиения Class
    unordered_map<char, int> Class; // классы разбиения Class
    unordered_set<char> setF(F.begin(), F.end()); //хэш-таблица для быстрого поиска
    vector<char> nonF;
    for (char state : Q){
        if (setF.find(state) == setF.end()){
            nonF.push_back(state);
            Class[state] = 1;
        }
        else {
            Class[state] = 0;
        }
    }
    P.push_back(unordered_set<char>(nonF.begin(),nonF.end()));
    
    // Инициализация очереди Queue
    queue<pair<int,char>> Queue;
    for (char c : alphabet){ // alphabet = E
        Queue.push({0, c});
        Queue.push({1, c});
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
        for (char q : splitter){
            for (char r : Inv[q][a]){
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

        for (int q : splitter){
            for (char r : Inv[q][a]){
                int i = Class[r];
                int j = Twin[i];
                if (j != 0){

                }
            }
        }

    }

    // <-- Преобразование результата в vector<vector<char>> для возврата
    vector<vector<char>> result;
    result.reserve(P.size());
    for (auto cls : P) {
        result.push_back(vector<char>(cls.begin(), cls.end()));
    }

    return result;
}
