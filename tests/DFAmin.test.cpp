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

TEST(DFATests, DFAminTest1) {
    // хороший тест делится на три части - Arrange-Act-Assert (либо Given-When-Then, называйте как нравится)
    
    // ARRANGE - это подготовка почвы; здесь нужные объявления и операции для создания ситуации, которую хотим проверить
    using State = string;
    using Symbol = string;

    // Вложенная хеш-таблица переходов:
    // состояние -> (символ -> следующее состояние)
    std::unordered_map<State, std::unordered_map<Symbol, State>> transitions;

    // Заполнение переходов согласно таблице
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

    string start = "A";                 // начальное состояние
    string current = start;

    vector<string> Q = {"A", "B", "C", "D", "E", "F", "G", "H"}; // множество состояний
    vector<string> E = {"0","1"};
    vector<string> F = {"C"};           // множество допустимых состояний

    // ACT - это само действие; именно то, что нам нужно проверить
    vector<vector<string>> P = DFAmin(E,Q,F,transitions);

    
    // ASSERT - проверка результатов; именно ASSERT определяет, пройден тест или нет
    vector<vector<string>> Ptrue = {{"C"},{"D","F"},{"B","H"},{"G"},{"A","E"}};
    ASSERT_EQ(Ptrue, P);    
}




// если материал выше легко усвоили, почитайте также про TEST_F (test fixtures) и setup/teardown
// в любом случае, вам поможет gtest primer 