// заголовочный файл тестового фреймворка
#include <gtest/gtest.h>
#include <cstdio>  // для printf
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>

#include <func_transition/transit.hpp>
#include <func_minimization/DFAmin.hpp>
#include <func_input/DFAinput.hpp>

using namespace std;
using namespace func_transition;
using namespace func_minimization;
using namespace func_input;


TEST(DFATests, CreateNewTransitionsTest1) {
    // ARRANGE
    func_input::Result OldRes;
    OldRes.alphabet = {"0", "1"};
    OldRes.states = {"A", "B", "C", "D", "E", "F", "G", "H"};
    OldRes.permited_state = {"C"};
    OldRes.start_state = "A";

    OldRes.transitions["A"]["0"] = "B";
    OldRes.transitions["A"]["1"] = "F";
    OldRes.transitions["B"]["0"] = "G";
    OldRes.transitions["B"]["1"] = "C";
    OldRes.transitions["C"]["0"] = "A";
    OldRes.transitions["C"]["1"] = "C";
    OldRes.transitions["D"]["0"] = "C";
    OldRes.transitions["D"]["1"] = "G";
    OldRes.transitions["E"]["0"] = "H";
    OldRes.transitions["E"]["1"] = "F";
    OldRes.transitions["F"]["0"] = "C";
    OldRes.transitions["F"]["1"] = "G";
    OldRes.transitions["G"]["0"] = "G";
    OldRes.transitions["G"]["1"] = "E";
    OldRes.transitions["H"]["0"] = "G";
    OldRes.transitions["H"]["1"] = "C";

    std::vector<std::vector<std::string>> P = {
        {"C"},
        {"D", "F"},
        {"B", "H"},
        {"G"},
        {"A", "E"}
    };

    // ACT
    func_input::Result NewRes = func_minimization::CreateNewTransitions(OldRes, P);

    // ASSERT

    // 1) Проверка новых состояний
    std::vector<std::string> expectedStates = {"C", "D+F", "B+H", "G", "A+E"};
    ASSERT_EQ(expectedStates, NewRes.states);

    // 2) Проверка таблицы переходов (полное сравнение)
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> expectedTransitions;
    expectedTransitions["A+E"]["0"] = "B+H";
    expectedTransitions["A+E"]["1"] = "D+F";
    expectedTransitions["B+H"]["0"] = "G";
    expectedTransitions["B+H"]["1"] = "C";
    expectedTransitions["C"]["0"] = "A+E";
    expectedTransitions["C"]["1"] = "C";
    expectedTransitions["D+F"]["0"] = "C";
    expectedTransitions["D+F"]["1"] = "G";
    expectedTransitions["G"]["0"] = "G";
    expectedTransitions["G"]["1"] = "A+E";

    ASSERT_EQ(expectedTransitions.size(), NewRes.transitions.size());
    for (const auto& [state, trans] : expectedTransitions) {
        auto it = NewRes.transitions.find(state);
        ASSERT_NE(it, NewRes.transitions.end());
        ASSERT_EQ(trans, it->second);
    }

    // 3) Проверка начального и допустимых состояний
    ASSERT_EQ("A+E", NewRes.start_state);
    std::vector<std::string> expectedPermited = {"C"};
    ASSERT_EQ(expectedPermited, NewRes.permited_state);

    // 4) Проверка через функцию transit (по аналогии с тестами переходов)
    // Проверим, что автомат обрабатывает строки согласно ожидаемой логике
    
    // Строка "0101" - из A+E: 0->B+H, 1->C, 0->A+E, 1->D+F
    std::vector<std::string> input1 = {"0","1","0","1"};
    std::string final1 = func_transition::transit(input1, NewRes.start_state, NewRes.transitions);
    ASSERT_EQ("D+F", final1);

    // Строка "000" - A+E -0-> B+H -0-> G -0-> G
    std::vector<std::string> input2 = {"0","0","0"};
    std::string final2 = func_transition::transit(input2, NewRes.start_state, NewRes.transitions);
    ASSERT_EQ("G", final2);

    // Строка "111" - A+E -1-> D+F -1-> G -1-> A+E
    std::vector<std::string> input3 = {"1","1","1"};
    std::string final3 = func_transition::transit(input3, NewRes.start_state, NewRes.transitions);
    ASSERT_EQ("A+E", final3);

    // Пустая строка - должно вернуть начальное состояние
    std::vector<std::string> empty;
    std::string final_empty = func_transition::transit(empty, NewRes.start_state, NewRes.transitions);
    ASSERT_EQ("A+E", final_empty);
}