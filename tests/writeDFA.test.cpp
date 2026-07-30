#include <gtest/gtest.h>
#include <func_DFA/DFA.hpp>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;
using namespace func_DFA;

TEST(DFATests, WriteDFATest1) {
        // ================ ARRANGE ===============
    // ARRANGE: создаём эталонный объект DFA с известными данными
    vector<string> TrueStringTransition = {"0", "1", "0", "1"};
    vector<string> TruePermittedState = {"C"};
    string TrueStartState = "A";
    vector<string> TrueAlphabet = {"0", "1"};
    vector<string> TrueStates = {"A", "B", "C", "D", "E", "F", "G", "H"};
    unordered_map<string, unordered_map<string, string>> TrueTransitions;
    TrueTransitions["A"]["0"] = "B";
    TrueTransitions["A"]["1"] = "F";
    TrueTransitions["B"]["0"] = "G";
    TrueTransitions["B"]["1"] = "C";
    TrueTransitions["C"]["0"] = "A";
    TrueTransitions["C"]["1"] = "C";
    TrueTransitions["D"]["0"] = "C";
    TrueTransitions["D"]["1"] = "G";
    TrueTransitions["E"]["0"] = "H";
    TrueTransitions["E"]["1"] = "F";
    TrueTransitions["F"]["0"] = "C";
    TrueTransitions["F"]["1"] = "G";
    TrueTransitions["G"]["0"] = "G";
    TrueTransitions["G"]["1"] = "E";
    TrueTransitions["H"]["0"] = "G";
    TrueTransitions["H"]["1"] = "C";

    string filename = "test_write.csv";


    // ============== ACT ===================
    // ACT: создаем и записываем объект в файл
    // Создание ДКА
    auto TrueDFA = DFABuilder{}.withComponents(  TrueStartState,
                                            TrueStates,
                                            TruePermittedState,
                                            TrueAlphabet,
                                            TrueTransitions     ).build();
    // Запись ДКА в файлЫ                                        
    TrueDFA.exportCSV(filename);

    // Чтение файла обратно
    auto readBackDFA = DFABuilder{}.withCSV(filename).build();

    // Очистка: удаляем временный файл
    remove(filename.c_str());


    // ============== ASSERT ===================
    // ASSERT: сравниваем все поля
    // 1) string_transition
    ASSERT_EQ(TrueDFA.getStringTransition(), readBackDFA.getStringTransition());

    // 2) permited_state
    ASSERT_EQ(TrueDFA.getPermittedStates(), readBackDFA.getPermittedStates());

    // 3) start_state
    ASSERT_EQ(TrueDFA.getStartState(), readBackDFA.getStartState());

    // 4) alphabet
    ASSERT_EQ(TrueDFA.getAlphabet(), readBackDFA.getAlphabet());

    // 5) states
    ASSERT_EQ(TrueDFA.getStates(), readBackDFA.getStates());

    // 6) transitions – сравниваем поэлементно
    auto transitions = readBackDFA.getTransitions();
    ASSERT_EQ(TrueTransitions.size(), transitions.size());
    for (const auto& [state, trans] : TrueTransitions) {
        auto it = transitions.find(state);
        ASSERT_NE(it, transitions.end()) << "Отсутствует состояние " << state;
        ASSERT_EQ(trans, it->second) << "Неверные переходы для состояния " << state;
    }
}