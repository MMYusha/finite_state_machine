// заголовочный файл тестового фреймворка
#include <gtest/gtest.h>
#include <cstdio>  // для printf
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm> // для функции сортировки

#include <func_DFA/DFA.hpp>

using namespace std;
using namespace func_DFA;


void sortClasses(vector<vector<string>>& v) { 
    for (auto& cls : v) {
        sort(cls.begin(), cls.end());
    }
    sort(v.begin(), v.end());
}


TEST(DFATests, DFAminTest1_RealDFA_1) {

    // ================ ARRANGE ===============
    // ARRANGE - это подготовка почвы: здесь нужные объявления и операции для создания ситуации, которую хотим проверить
    string start = "A";                                                 // начальное состояние
    vector<string> states = {"A", "B", "C", "D", "E", "F", "G", "H"};   // множество состояний
    vector<string> alphabet = {"0","1"};                                // алфавит
    vector<string> Permitted_states = {"C"};                            // множество допустимых состояний
    unordered_map<string, unordered_map<string, string>> transitions;   // Вложенная хеш-таблица переходов: состояние -> (символ -> следующее состояние)
    transitions["A"]["0"] = "B";
    transitions["A"]["1"] = "F";
    transitions["B"]["0"] = "G";
    transitions["B"]["1"] = "C";
    transitions["C"]["0"] = "A";
    transitions["C"]["1"] = "C";
    transitions["D"]["0"] = "C";
    transitions["D"]["1"] = "G";
    transitions["E"]["0"] = "H";
    transitions["E"]["1"] = "F";
    transitions["F"]["0"] = "C";
    transitions["F"]["1"] = "G";
    transitions["G"]["0"] = "G";
    transitions["G"]["1"] = "E";
    transitions["H"]["0"] = "G";
    transitions["H"]["1"] = "C";

    // Создание ДКА
    auto dfa = DFABuilder{}.withComponents( start,
                                            states,
                                            Permitted_states,
                                            alphabet,
                                            transitions).build();
                                            

    // ============== ACT ===================
    // ACT - это само действие: то, что нужно проверить
    auto Partition = DFATestHelper::getPartition(dfa);


    // ============== ASSERT ===================
    // ASSERT - проверка результатов: ASSERT определяет, пройден тест или нет
    vector<vector<string>> PartitionTrue = {{"C"},{"F"},{"B","H"},{"G"},{"A","E"}};
    auto Partition_sorted = Partition;                 // копируем фактический результат
    auto PartitionTrue_sorted = PartitionTrue;      // копируем ожидаемый
    sortClasses(Partition_sorted);
    sortClasses(PartitionTrue_sorted);
    ASSERT_EQ(PartitionTrue_sorted, Partition_sorted);    
}

