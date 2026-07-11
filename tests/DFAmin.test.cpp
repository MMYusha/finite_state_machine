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

TEST(DFATests, DFAminTest2_AllEquivalent) {
    // ARRANGE
    using State = std::string;
    using Symbol = std::string;

    std::unordered_map<State, std::unordered_map<Symbol, State>> transitions;
    transitions["A"]["0"] = "A";
    transitions["A"]["1"] = "A";
    transitions["B"]["0"] = "B";
    transitions["B"]["1"] = "B";

    std::vector<std::string> Q = {"A", "B"};
    std::vector<std::string> E = {"0", "1"};
    std::vector<std::string> F = {"A", "B"};   // оба допускающие

    // ACT
    auto P = func_minimization::DFAmin(E, Q, F, transitions);

    // ASSERT: все состояния эквивалентны – один класс
    std::vector<std::vector<std::string>> expected = {{"A", "B"}};
    ASSERT_EQ(expected, P);
}

TEST(DFATests, DFAminTest3_AlreadyMinimal) {
    // ARRANGE
    using State = std::string;
    using Symbol = std::string;

    std::unordered_map<State, std::unordered_map<Symbol, State>> transitions;
    transitions["S"]["0"]  = "S0";
    transitions["S"]["1"]  = "S";
    transitions["S0"]["0"] = "S0";
    transitions["S0"]["1"] = "S01";
    transitions["S01"]["0"]= "S0";
    transitions["S01"]["1"]= "S";

    std::vector<std::string> Q = {"S", "S0", "S01"};
    std::vector<std::string> E = {"0", "1"};
    std::vector<std::string> F = {"S01"};

    // ACT
    auto P = func_minimization::DFAmin(E, Q, F, transitions);

    // ASSERT: каждый класс содержит ровно одно состояние
    std::vector<std::vector<std::string>> expected = {
        {"S01"}, {"S0"}, {"S"}   // порядок зависит от алгоритма, здесь ожидаем такой же, как в исходном разбиении
    };

    std::vector<std::vector<std::string>> expected_ordered = {{"S01"}, {"S"}, {"S0"}};
    ASSERT_EQ(expected_ordered, P);
}

TEST(DFATests, DFAminTest4_TwoEquivalent) {
    // ARRANGE
    using State = std::string;
    using Symbol = std::string;

    std::unordered_map<State, std::unordered_map<Symbol, State>> transitions;
    transitions["A"]["0"] = "C";
    transitions["A"]["1"] = "B";
    transitions["B"]["0"] = "C";
    transitions["B"]["1"] = "B";
    transitions["C"]["0"] = "C";
    transitions["C"]["1"] = "C";

    std::vector<std::string> Q = {"A", "B", "C"};
    std::vector<std::string> E = {"0", "1"};
    std::vector<std::string> F = {"C"};   // только C допускающее

    // ACT
    auto P = func_minimization::DFAmin(E, Q, F, transitions);

    // ASSERT: A и B эквивалентны, C отдельно
    // Начальное разбиение: { {C}, {A, B} }
    // Проверяем сплиттер {C} по '0' или '1' – из A и B по 0 оба в C, по 1 оба в B (не в C), так что класс {A,B} не разбивается.
    // Итоговые классы: {C}, {A,B}.
    std::vector<std::vector<std::string>> expected = {{"C"}, {"A", "B"}};
    ASSERT_EQ(expected, P);
}

TEST(ErorDFATests, ErrorDFAminTest1) {
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

    vector<string> Q = {"A", "B", "C", "D", "E", "F", "G", "K"}; // множество состояний -------------
    vector<string> E = {"0","1"};
    vector<string> F = {"C"};           // множество допустимых состояний

    // ACT - это само действие; именно то, что нам нужно проверить
    // ASSERT - проверка результатов; именно ASSERT определяет, пройден тест или нет
    EXPECT_THROW(DFAmin(E, Q, F, transitions), invalid_argument);  
}


// если материал выше легко усвоили, почитайте также про TEST_F (test fixtures) и setup/teardown
// в любом случае, вам поможет gtest primer 