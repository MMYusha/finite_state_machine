// заголовочный файл тестового фреймворка
#include <gtest/gtest.h>
#include <cstdio>  // для printf
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm> // для функции сортировки

#include <func_DFA/DFA.hpp>
#include <func_DFA/DFAmin.hpp>
#include <func_DFA/DFAinput.hpp>

using namespace std;
using namespace func_DFA;
using namespace func_minimization;
using namespace func_input;


void sortClasses(std::vector<std::vector<std::string>>& v) { 
    for (auto& cls : v) {
        std::sort(cls.begin(), cls.end());
    }
    std::sort(v.begin(), v.end());
}


TEST(DFATests, DFAminTest1_RealDFA_1) {
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
    vector<vector<string>> Ptrue = {{"C"},{"F"},{"B","H"},{"G"},{"A","E"}};
    auto P_sorted = P;                 // копируем фактический результат
    auto expected_sorted = Ptrue;      // копируем ожидаемый
    sortClasses(P_sorted);
    sortClasses(expected_sorted);
    ASSERT_EQ(expected_sorted, P_sorted);    
}


TEST(DFATests, DFAminTest1_RealDFA_2) {
    // хороший тест делится на три части - Arrange-Act-Assert (либо Given-When-Then, называйте как нравится)
    
    // ARRANGE - это подготовка почвы; здесь нужные объявления и операции для создания ситуации, которую хотим проверить
    using State = string;
    using Symbol = string;

    // Вложенная хеш-таблица переходов:
    // состояние -> (символ -> следующее состояние)
    std::unordered_map<State, std::unordered_map<Symbol, State>> transitions;

    // Заполнение переходов согласно таблице
    transitions["0"]["a"] = "1";
    transitions["0"]["b"] = "2";

    transitions["1"]["a"] = "7";
    transitions["1"]["b"] = "3";

    transitions["2"]["a"] = "5";
    transitions["2"]["b"] = "0";

    transitions["3"]["a"] = "3";
    transitions["3"]["b"] = "4";

    transitions["4"]["a"] = "2";
    transitions["4"]["b"] = "6";

    transitions["5"]["a"] = "5";
    transitions["5"]["b"] = "6";

    transitions["6"]["a"] = "3";
    transitions["6"]["b"] = "4";

    transitions["7"]["a"] = "5";
    transitions["7"]["b"] = "3";


    string start = "0";                 // начальное состояние
    string current = start;

    vector<string> Q = {"0", "1", "2", "3", "4", "5", "6", "7"}; // множество состояний
    vector<string> E = {"a","b"};
    vector<string> F = {"3", "4", "6",};           // множество допустимых состояний

    // ACT - это само действие; именно то, что нам нужно проверить
    vector<vector<string>> P = DFAmin(E,Q,F,transitions);

    
    // ASSERT - проверка результатов; именно ASSERT определяет, пройден тест или нет
    vector<vector<string>> Ptrue = {{"0","2"},{"1","5","7"},{"3","6"},{"4"}};
    auto P_sorted = P;                 // копируем фактический результат
    auto expected_sorted = Ptrue;      // копируем ожидаемый
    sortClasses(P_sorted);
    sortClasses(expected_sorted);
    ASSERT_EQ(expected_sorted, P_sorted);    
}


TEST(DFATests, DFAminTest1_RealDFA_3) {
    // хороший тест делится на три части - Arrange-Act-Assert (либо Given-When-Then, называйте как нравится)
    
    // ARRANGE - это подготовка почвы; здесь нужные объявления и операции для создания ситуации, которую хотим проверить
    using State = string;
    using Symbol = string;

    // Вложенная хеш-таблица переходов:
    // состояние -> (символ -> следующее состояние)
    std::unordered_map<State, std::unordered_map<Symbol, State>> transitions;

    // Заполнение переходов согласно таблице
    transitions["q0"]["a"] = "q2";
    transitions["q0"]["b"] = "q1";

    transitions["q1"]["a"] = "q3";
    transitions["q1"]["b"] = "q0";

    transitions["q2"]["a"] = "q4";
    transitions["q2"]["b"] = "q3";

    transitions["q3"]["a"] = "q4";
    transitions["q3"]["b"] = "q3";

    transitions["q4"]["a"] = "q1";
    transitions["q4"]["b"] = "q4";

    transitions["q5"]["a"] = "q2";
    transitions["q5"]["b"] = "q5";

    vector<string> alphabet = {"a", "b"};
    vector<string> Q = {"q0", "q1", "q2", "q3", "q4", "q5"}; // q5 недостижимое состояние 
    vector<string> F = {"q0", "q1"};   // допускающее

    // ACT
    auto P = DFAmin(alphabet, Q, F, transitions);

    // ASSERT: все состояния различны → 4 класса по одному состоянию
    vector<vector<string>> Ptrue = {
        {"q0", "q1"},
        {"q2", "q3"},
        {"q4"},
    };

    auto P_sorted = P;                 // копируем фактический результат
    auto expected_sorted = Ptrue;      // копируем ожидаемый
    sortClasses(P_sorted);
    sortClasses(expected_sorted);
    ASSERT_EQ(expected_sorted, P_sorted);    
}

TEST(DFATests, DFAminTest1_UnreachableStates) {
    // хороший тест делится на три части - Arrange-Act-Assert (либо Given-When-Then, называйте как нравится)
    
    // ARRANGE - это подготовка почвы; здесь нужные объявления и операции для создания ситуации, которую хотим проверить
    using State = string;
    using Symbol = string;

    // Вложенная хеш-таблица переходов:
    // состояние -> (символ -> следующее состояние)
    std::unordered_map<State, std::unordered_map<Symbol, State>> transitions;

    // Заполнение переходов согласно таблице
    transitions["A"]["0"] = "B";
    transitions["A"]["1"] = "C";

    //transitions["B"]["0"] = "B";
    transitions["B"]["1"] = "D";

    transitions["C"]["0"] = "D";
    transitions["C"]["1"] = "E";

    //transitions["D"]["0"] = "D";
    transitions["D"]["1"] = "E";

    transitions["E"]["0"] = "B";
    transitions["E"]["1"] = "D";

    transitions["F"]["0"] = "G";
    transitions["F"]["1"] = "E";

    transitions["G"]["0"] = "F";
    transitions["G"]["1"] = "D";


    vector<string> alphabet = {"0", "1"};
    vector<string> Q = {"A", "B", "C", "D", "E", "F", "G"};
    vector<string> F = {"E", "D"};   

    // ACT
    auto P = DFAmin(alphabet, Q, F, transitions);

    // ASSERT
    vector<vector<string>> Ptrue =  { { "A" }, { "B" }, { "C" }, { "D" }, { "E" } }; // F и G недостижимы

    auto P_sorted = P;                 // копируем фактический результат
    auto expected_sorted = Ptrue;      // копируем ожидаемый
    sortClasses(P_sorted);
    sortClasses(expected_sorted);
    ASSERT_EQ(expected_sorted, P_sorted);    
}


TEST(DFATests, DFAminTest2_Idempotency) {
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
    
    // Инициализация ДКА
    Result DFA;
    //DFA.start_state = ;
    DFA.permited_state = F;
    //DFA.string_transition = ; // пример входной строки из файла
    DFA.alphabet = E;
    DFA.states = Q;
    DFA.transitions = transitions;

    // Минимизация (Разбиение на классы)
    vector<vector<string>> P = DFAmin(E,Q,F,transitions);

    // Получение описания нового автомата 
    Result newDFA = CreateNewTransitions(DFA, P);

    // Инициализация нового ДКА
    vector<string> newF = newDFA.permited_state;
    vector<string> newE = newDFA.alphabet;
    vector<string> newQ = newDFA.states;
    std::unordered_map<State, std::unordered_map<Symbol, State>> newTransitions = newDFA.transitions;

    // Повторное разбиение
    vector<vector<string>> newP = DFAmin(newE,newQ,newF,newTransitions); // 

    
    // ASSERT - проверка результатов; именно ASSERT определяет, пройден тест или нет
    vector<vector<string>> Ptrue =  { { "A+E" }, { "B+H" }, { "C" }, { "F" }, { "G" } };
    auto P_sorted = newP;                 // копируем фактический результат
    auto expected_sorted = Ptrue;      // копируем ожидаемый
    sortClasses(P_sorted);
    sortClasses(expected_sorted);
    ASSERT_EQ(expected_sorted, P_sorted);    
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

TEST(ErorDFATests, ErrorDFAminTest1_WrongStates) {
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
TEST(ErorDFATests, ErrorDFAminTest2_WrongPermitedStates) {
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

    vector<string> Q = {"A", "B", "C", "D", "E", "F", "G", "H"}; // множество состояний -------------
    vector<string> E = {"0","1"};
    vector<string> F = {"K"};           // множество допустимых состояний

    // ACT - это само действие; именно то, что нам нужно проверить
    // ASSERT - проверка результатов; именно ASSERT определяет, пройден тест или нет
    EXPECT_THROW(DFAmin(E, Q, F, transitions), invalid_argument);  
}

TEST(ErorDFATests, ErrorDFAminTest3_WrongAlphabet) {
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
    transitions["G"]["K"] = "E";

    transitions["H"]["0"] = "G";
    transitions["H"]["1"] = "C";

    string start = "A";                 // начальное состояние 
    string current = start;

    vector<string> Q = {"A", "B", "C", "D", "E", "F", "G", "H"}; // множество состояний -------------
    vector<string> E = {"0","1"};
    vector<string> F = {"C"};           // множество допустимых состояний

    // ACT - это само действие; именно то, что нам нужно проверить
    // ASSERT - проверка результатов; именно ASSERT определяет, пройден тест или нет
    EXPECT_THROW(DFAmin(E, Q, F, transitions), invalid_argument);  
}

TEST(DFAminTest_NotAllAlphabetUse, NotAllAlphabetUse) {
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

    //transitions["D"]["0"] = "C";
    transitions["D"]["1"] = "G";

    transitions["E"]["0"] = "H";
    transitions["E"]["1"] = "F";

    //transitions["F"]["0"] = "C";
    transitions["F"]["1"] = "G";

    //transitions["G"]["0"] = "G";
    transitions["G"]["1"] = "E";

    transitions["H"]["0"] = "G";
    transitions["H"]["1"] = "C";

    string start = "A";                 // начальное состояние 
    string current = start;

    vector<string> Q = {"A", "B", "C", "D", "E", "F", "G", "H"}; // множество состояний -------------
    vector<string> E = {"0","1"};
    vector<string> F = {"C"};           // множество допустимых состояний

    // ACT - это само действие; именно то, что нам нужно проверить
    vector<vector<string>> P = DFAmin(E,Q,F,transitions);

    
    // ASSERT - проверка результатов; именно ASSERT определяет, пройден тест или нет
    vector<vector<string>> Ptrue = {{"C"},{"F"},{"B","H"},{"G"},{"A","E"}};
    auto P_sorted = P;                 // копируем фактический результат
    auto expected_sorted = Ptrue;      // копируем ожидаемый
    sortClasses(P_sorted);
    sortClasses(expected_sorted);
    ASSERT_EQ(expected_sorted, P_sorted);  
}

// если материал выше легко усвоили, почитайте также про TEST_F (test fixtures) и setup/teardown
// в любом случае, вам поможет gtest primer 