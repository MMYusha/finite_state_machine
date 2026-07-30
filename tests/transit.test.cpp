// заголовочный файл тестового фреймворка
#include <gtest/gtest.h>
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <func_DFA/DFA.hpp>

using namespace func_DFA;
using namespace std;

TEST(DFATests, TransitTest1) {
    system("chcp 65001 > nul"); 
    // ================ ARRANGE ===============
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
    dfa.transitInput({"0","1","0","1"});
    auto FinalState1 = dfa.getCurrentState();
    dfa.resetCurrentState();

    dfa.transitInput({"0","0","0","1","0","0","1","0","1","1","1","0","1","1"});
    auto FinalState2 = dfa.getCurrentState();
    dfa.resetCurrentState();

    dfa.transitInput({"0"});
    auto FinalState3 = dfa.getCurrentState();
    dfa.resetCurrentState();

    dfa.transitInput({"0","1","E","0","1"});
    auto FinalState4 = dfa.getCurrentState();
    dfa.resetCurrentState();

    dfa.transitInput({});
    auto FinalState5 = dfa.getCurrentState();
    dfa.resetCurrentState();


    // ============== ASSERT ===================
    ASSERT_EQ("F", FinalState1);
    ASSERT_EQ("G", FinalState2);
    ASSERT_EQ("B", FinalState3);
    ASSERT_EQ("C", FinalState4);
    ASSERT_EQ("A", FinalState5);
}


TEST(DFATests, TransitTest2) {
    system("chcp 65001 > nul"); 
    // ================ ARRANGE ===============
    string start = "Even";                                                 // начальное состояние
    vector<string> states = {"Even", "Odd"};                             // множество состояний
    vector<string> alphabet = {"0","1"};                                // алфавит
    vector<string> Permitted_states = {"Even"};                            // множество допустимых состояний
    unordered_map<string, unordered_map<string, string>> transitions;   // Вложенная хеш-таблица переходов: состояние -> (символ -> следующее состояние)
    transitions["Even"]["0"] = "Even";
    transitions["Even"]["1"] = "Odd";
    transitions["Odd"]["0"]  = "Odd";
    transitions["Odd"]["1"]  = "Even";

    // Создание ДКА
    auto dfa = DFABuilder{}.withComponents( start,
                                            states,
                                            Permitted_states,
                                            alphabet,
                                            transitions).build();
                                       
                                            
    // ============== ACT ===================
    dfa.transitInput({"0", "1", "0", "1"});
    auto FinalState1 = dfa.getCurrentState();
    dfa.resetCurrentState();

    dfa.transitInput({"1", "0", "1"});
    auto FinalState2 = dfa.getCurrentState();
    dfa.resetCurrentState();

    dfa.transitInput({"1", "1", "1"});
    auto FinalState3 = dfa.getCurrentState();
    dfa.resetCurrentState();

    dfa.transitInput({});
    auto FinalState4 = dfa.getCurrentState();
    dfa.resetCurrentState();


    // ============== ASSERT ===================
    ASSERT_EQ("Even", FinalState1);
    ASSERT_EQ("Even", FinalState2);
    ASSERT_EQ("Odd",  FinalState3);
    ASSERT_EQ("Even", FinalState4);
}



TEST(DFATests, TransitTest3) {
    system("chcp 65001 > nul"); 
    // ================ ARRANGE ===============
    string start = "S";                                                 // начальное состояние
    vector<string> states = {"S", "S0", "S01"};                             // множество состояний
    vector<string> alphabet = {"0","1"};                                // алфавит
    vector<string> Permitted_states = {"S01"};                            // множество допустимых состояний
    unordered_map<string, unordered_map<string, string>> transitions;   // Вложенная хеш-таблица переходов: состояние -> (символ -> следующее состояние)
    transitions["S"]["0"]  = "S0";
    transitions["S"]["1"]  = "S";
    transitions["S0"]["0"] = "S0";
    transitions["S0"]["1"] = "S01";
    transitions["S01"]["0"]= "S0";
    transitions["S01"]["1"]= "S";

    // Создание ДКА
    auto dfa = DFABuilder{}.withComponents( start,
                                            states,
                                            Permitted_states,
                                            alphabet,
                                            transitions).build();
                                       
                                            
    // ============== ACT ===================
    dfa.transitInput({"0", "1"});
    auto FinalState1 = dfa.getCurrentState();
    dfa.resetCurrentState();

    dfa.transitInput({"1", "0", "1"});
    auto FinalState2 = dfa.getCurrentState();
    dfa.resetCurrentState();

    dfa.transitInput({"0", "0", "1", "0"});
    auto FinalState3 = dfa.getCurrentState();
    dfa.resetCurrentState();

    dfa.transitInput({"1", "1"});
    auto FinalState4 = dfa.getCurrentState();
    dfa.resetCurrentState();

    dfa.transitInput({});
    auto FinalState5 = dfa.getCurrentState();
    dfa.resetCurrentState();


    // ============== ASSERT ===================
    ASSERT_EQ("S01", FinalState1);
    ASSERT_EQ("S01", FinalState2);
    ASSERT_EQ("S0",  FinalState3);
    ASSERT_EQ("S", FinalState4);
    ASSERT_EQ("S", FinalState5);
}