#include <gtest/gtest.h>
#include <func_DFA/DFA.hpp>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;
using namespace func_DFA;

TEST(DFATests, WriteDFATest1) {
    // ARRANGE: создаём эталонный объект DFA с известными данными
    DFA original;
    original.string_transition = {"0", "1", "0", "1"};
    original.permited_state = {"C"};
    original.start_state = "A";
    original.alphabet = {"0", "1"};
    original.states = {"A", "B", "C", "D", "E", "F", "G", "H"};

    // Заполняем переходы
    original.transitions["A"]["0"] = "B";
    original.transitions["A"]["1"] = "F";
    original.transitions["B"]["0"] = "G";
    original.transitions["B"]["1"] = "C";
    original.transitions["C"]["0"] = "A";
    original.transitions["C"]["1"] = "C";
    original.transitions["D"]["0"] = "C";
    original.transitions["D"]["1"] = "G";
    original.transitions["E"]["0"] = "H";
    original.transitions["E"]["1"] = "F";
    original.transitions["F"]["0"] = "C";
    original.transitions["F"]["1"] = "G";
    original.transitions["G"]["0"] = "G";
    original.transitions["G"]["1"] = "E";
    original.transitions["H"]["0"] = "G";
    original.transitions["H"]["1"] = "C";

    string filename = "test_write.csv";

    // ACT: записываем объект в файл
    writeDFA(filename, original);

    // Читаем файл обратно
    DFA readBack = DFAinput(filename);

    // ASSERT: сравниваем все поля
    // 1) string_transition
    ASSERT_EQ(original.string_transition, readBack.string_transition);

    // 2) permited_state
    ASSERT_EQ(original.permited_state, readBack.permited_state);

    // 3) start_state
    ASSERT_EQ(original.start_state, readBack.start_state);

    // 4) alphabet
    ASSERT_EQ(original.alphabet, readBack.alphabet);

    // 5) states
    ASSERT_EQ(original.states, readBack.states);

    // 6) transitions – сравниваем поэлементно
    ASSERT_EQ(original.transitions.size(), readBack.transitions.size());
    for (const auto& [state, trans] : original.transitions) {
        auto it = readBack.transitions.find(state);
        ASSERT_NE(it, readBack.transitions.end()) << "Отсутствует состояние " << state;
        ASSERT_EQ(trans, it->second) << "Неверные переходы для состояния " << state;
    }

    // Очистка: удаляем временный файл
    remove(filename.c_str());
}