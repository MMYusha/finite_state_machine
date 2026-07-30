// заголовочный файл тестового фреймворка
#include <gtest/gtest.h>
#include <cstdio>  // для printf
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>

#include <func_DFA/DFA.hpp>

using namespace std;
using namespace func_DFA;

TEST(DFATests, CreateNewTransitionsTest1) {
    system("chcp 65001 > nul"); 
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
    
    // Ручное создание разбиения 
    vector<vector<string>> Partition = {
        {"C"},
        {"F"},
        {"B", "H"},
        {"G"},
        {"A", "E"}
    };
                                            
    // ============== ACT ===================
    // ACT - это само действие: то, что нужно проверить
    auto NewDfa = DFATestHelper::getNewDFAwithPartition(dfa, Partition);

    // ============== ASSERT ===================
    // ASSERT - проверка результатов: ASSERT определяет, пройден тест или нет
    
    // 1) Проверка новых состояний
    vector<string> TrueStates = {"C", "F", "B+H", "G", "A+E"};
    ASSERT_EQ(TrueStates, NewDfa.getStates());

    // 2) Проверка таблицы переходов (полное сравнение)
    unordered_map<string, unordered_map<string, string>> TrueTransitions;
    TrueTransitions["A+E"]["0"] = "B+H";
    TrueTransitions["A+E"]["1"] = "F";
    TrueTransitions["B+H"]["0"] = "G";
    TrueTransitions["B+H"]["1"] = "C";
    TrueTransitions["C"]["0"] = "A+E";
    TrueTransitions["C"]["1"] = "C";
    TrueTransitions["F"]["0"] = "C";
    TrueTransitions["F"]["1"] = "G";
    TrueTransitions["G"]["0"] = "G";
    TrueTransitions["G"]["1"] = "A+E";

    auto NewTransitions = NewDfa.getTransitions(); 
    ASSERT_EQ(TrueTransitions.size(), NewTransitions.size());
    for (const auto& [state, trans] : TrueTransitions) {
        auto it = NewTransitions.find(state);
        ASSERT_NE(it, NewTransitions.end());
        ASSERT_EQ(trans, it->second);
    }

    // 3) Проверка начального состояния
    ASSERT_EQ("A+E", NewDfa.getStartState());

    // 4) Проверка допустимых состояний
    vector<string> TruePermitedStates = {"C"};
    ASSERT_EQ(TruePermitedStates, NewDfa.getPermittedStates());
    
    // 5) Проверка алфавита
    vector<string> TrueAlphabet = alphabet; // не должен был измениться
    ASSERT_EQ(TrueAlphabet, NewDfa.getAlphabet());


    // 6) Проверка через функцию transit (по аналогии с тестами переходов)
    // Проверка, что автомат обрабатывает строки согласно ожидаемой логике
    
    // Строка "0101" - из A+E: 0->B+H, 1->C, 0->A+E, 1->F
    vector<string> input1 = {"0","1","0","1"};
    NewDfa.transitInput(input1);
    ASSERT_EQ("F", NewDfa.getCurrentState());
    NewDfa.resetCurrentState();

    // Строка "000" - A+E -0-> B+H -0-> G -0-> G
    vector<string> input2 = {"0","0","0"};
    NewDfa.transitInput(input2);
    ASSERT_EQ("G", NewDfa.getCurrentState());
    NewDfa.resetCurrentState();

    // Строка "111" - A+E -1-> F -1-> G -1-> A+E
    vector<string> input3 = {"1","1","1"};
    NewDfa.transitInput(input3);
    ASSERT_EQ("A+E", NewDfa.getCurrentState());
    NewDfa.resetCurrentState();

    // Пустая строка - должно вернуть начальное состояние
    vector<string> empty_input;
    NewDfa.transitInput(empty_input);
    ASSERT_EQ("A+E", NewDfa.getCurrentState());
    NewDfa.resetCurrentState();
}

