// заголовочный файл тестового фреймворка
#include <gtest/gtest.h>
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <func_transition/transit.hpp>
#include <func_input/DFAinput.hpp>

using namespace func_transition;
using namespace func_input;
using namespace std;


TEST(DFATests, TransitTest) {
    // хороший тест делится на три части - Arrange-Act-Assert (либо Given-When-Then, называйте как нравится)
    
     system("chcp 65001 > nul"); 
    // Типы для читаемости
    using State = string;
    using Symbol = string;

    // Чтение ДКА из csv файла
    string filename = "input.csv";
    Result res;
    res = DFAinput(filename);

    // Инициализация ДКА
    State current = res.start_state;
    vector<string> F = res.permited_state;
    vector<string> input = res.string_transition; // пример входной строки из файла
    vector<string> E = res.alphabet;
    vector<string> Q = res.states;
    unordered_map<string, unordered_map<string, string>> transitions = res.transitions;


    // ACT - это само действие; именно то, что нам нужно проверить
    vector<string> input1 = {"0","1","0","1"}; // пример входной строки
    State final_state1 = func_transition::transit(input1, current, transitions);

    vector<string> input2 = {"0","0","0","1","0","0","1","0","1","1","1","0","1","1"}; // "00010010111011" пример входной строки A 0B 0G 0G 1E 0H 0G 1E 0H 1C 1C 1C 0A 1F 1G
    State final_state2 = func_transition::transit(input2, current, transitions);

    vector<string> input3 = {"0"}; // "0"
    State final_state3 = func_transition::transit(input3, current, transitions);

    vector<string> input4 = {"0","1","E","0","1"}; // "01E01"
    State final_state4 = func_transition::transit(input4, current, transitions);

    vector<string> input5 = {}; // пустая строка
    State final_state5 = func_transition::transit(input5, current, transitions);

    // ASSERT - проверка результатов; именно ASSERT определяет, пройден тест или нет
    ASSERT_EQ("F", final_state1);
    ASSERT_EQ("G", final_state2);
    ASSERT_EQ("B", final_state3);
    ASSERT_EQ("C", final_state4);
    ASSERT_EQ("A", final_state5);
}


// если материал выше легко усвоили, почитайте также про TEST_F (test fixtures) и setup/teardown
// в любом случае, вам поможет gtest primer 