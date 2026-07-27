// заголовочный файл тестового фреймворка
#include <gtest/gtest.h>
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <func_DFA/DFA.hpp>
#include <func_DFA/DFAinput.hpp>

using namespace func_DFA;
using namespace func_input;
using namespace std;


TEST(DFATests, TransitTest1) {
    // хороший тест делится на три части - Arrange-Act-Assert (либо Given-When-Then, называйте как нравится)
    
     system("chcp 65001 > nul"); 
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
    vector<string> input1 = {"0","1","0","1"}; // пример входной строки
    State final_state1 = transit(input1, current, transitions);

    vector<string> input2 = {"0","0","0","1","0","0","1","0","1","1","1","0","1","1"}; // "00010010111011" пример входной строки A 0B 0G 0G 1E 0H 0G 1E 0H 1C 1C 1C 0A 1F 1G
    State final_state2 = transit(input2, current, transitions);

    vector<string> input3 = {"0"}; // "0"
    State final_state3 = transit(input3, current, transitions);

    vector<string> input4 = {"0","1","E","0","1"}; // "01E01"
    State final_state4 = transit(input4, current, transitions);

    vector<string> input5 = {}; // пустая строка
    State final_state5 = transit(input5, current, transitions);

    // ASSERT - проверка результатов; именно ASSERT определяет, пройден тест или нет
    ASSERT_EQ("F", final_state1);
    ASSERT_EQ("G", final_state2);
    ASSERT_EQ("B", final_state3);
    ASSERT_EQ("C", final_state4);
    ASSERT_EQ("A", final_state5);
}
TEST(DFATests, TransitTest2) {
    system("chcp 65001 > nul");

    // ARRANGE: автомат с двумя состояниями: "Even" и "Odd"
    using State = std::string;
    using Symbol = std::string;

    std::unordered_map<State, std::unordered_map<Symbol, State>> transitions;
    transitions["Even"]["0"] = "Even";
    transitions["Even"]["1"] = "Odd";
    transitions["Odd"]["0"]  = "Odd";
    transitions["Odd"]["1"]  = "Even";

    State start = "Even";
    std::vector<std::string> Q = {"Even", "Odd"};
    std::vector<std::string> E = {"0", "1"};
    std::vector<std::string> F = {"Even"};  // допускающее – чётное число единиц

    // ACT: подаём строки с разным числом единиц
    State res1 = transit({"0", "1", "0", "1"}, start, transitions); // две единицы -> Even
    State res2 = transit({"1", "0", "1"}, start, transitions);      // две единицы -> Even
    State res3 = transit({"1", "1", "1"}, start, transitions);      // три единицы -> Odd
    State res4 = transit({}, start, transitions);                   // пустая строка -> Even

    // ASSERT
    ASSERT_EQ("Even", res1);
    ASSERT_EQ("Even", res2);
    ASSERT_EQ("Odd",  res3);
    ASSERT_EQ("Even", res4);
}

TEST(DFATests, TransitTest3) {
    system("chcp 65001 > nul");

    // ARRANGE: состояния: S (начальное, ничего не накоплено),
    // S0 (последний символ – 0), S01 (последние два – 01 – допускающее)
    using State = std::string;
    using Symbol = std::string;

    std::unordered_map<State, std::unordered_map<Symbol, State>> transitions;
    transitions["S"]["0"]  = "S0";
    transitions["S"]["1"]  = "S";
    transitions["S0"]["0"] = "S0";
    transitions["S0"]["1"] = "S01";
    transitions["S01"]["0"]= "S0";
    transitions["S01"]["1"]= "S";

    State start = "S";
    std::vector<std::string> Q = {"S", "S0", "S01"};
    std::vector<std::string> E = {"0", "1"};
    std::vector<std::string> F = {"S01"};

    // ACT
    State res1 = transit({"0", "1"}, start, transitions);           // "01" -> S01
    State res2 = transit({"1", "0", "1"}, start, transitions);      // "101" -> S01
    State res3 = transit({"0", "0", "1", "0"}, start, transitions); // "0010" -> S0 (не допускается)
    State res4 = transit({"1", "1"}, start, transitions);           // "11" -> S (не допускается)
    State res5 = transit({}, start, transitions);                   // пустая -> S (не допускается)

    // ASSERT
    ASSERT_EQ("S01", res1);
    ASSERT_EQ("S01", res2);
    ASSERT_EQ("S0",  res3);
    ASSERT_EQ("S",   res4);
    ASSERT_EQ("S",   res5);
}


// если материал выше легко усвоили, почитайте также про TEST_F (test fixtures) и setup/teardown
// в любом случае, вам поможет gtest primer 