#include <gtest/gtest.h>

#include <fstream>
#include <func_DFA/DFA.hpp>
#include <string>
#include <unordered_map>
#include <vector>


using namespace std;
using namespace func_DFA;

TEST(DFATests, DFAinputTest1) {
    // ================ ARRANGE ===============
    // ARRANGE: создаём временный CSV-файл с тестовыми данными
    string filename = "test_dfa_input.csv";
    ofstream file(filename);
    ASSERT_TRUE(file.is_open()) << "Не удалось создать временный файл";

    // Записываем содержимое в формате, ожидаемом DFAinput
    file << "переход;0;1;0;1\n";
    file << "допустимые состояния;C;;;\n";
    file << "Начальное состояние;A;;;\n";
    file << "состояния/алфавит;0;1;;\n";
    file << "A;B;F;;\n";
    file << "B;G;C;;\n";
    file << "C;A;C;;\n";
    file << "D;C;G;;\n";
    file << "E;H;F;;\n";
    file << "F;C;G;;\n";
    file << "G;G;E;;\n";
    file << "H;G;C;;\n";
    file.close();

    // ============== ACT ===================
    // ACT: вызываем функцию чтения
    auto dfa = DFABuilder{}.withCSV(filename).build();
    remove(filename.c_str());  // Очистка: удаляем временный файл

    // ============== ASSERT ===================
    // ASSERT: проверяем каждое поле структуры DFA

    // 1) string_transition – цифры из первой строки (все, кроме первого токена)
    vector<string> TrueStringTransition = {"0", "1", "0", "1"};
    ASSERT_EQ(TrueStringTransition, dfa.getStringTransition());

    // 2) permited_state – допустимые состояния (второй токен второй строки)
    vector<string> TruePermitedStates = {"C"};
    ASSERT_EQ(TruePermitedStates, dfa.getPermittedStates());

    // 3) start_state – начальное состояние (второй токен третьей строки)
    ASSERT_EQ("A", dfa.getStartState());

    // 4) alphabet – символы алфавита (со второй ячейки четвёртой строки)
    vector<string> TrueAlphabet = {"0", "1"};
    ASSERT_EQ(TrueAlphabet, dfa.getAlphabet());

    // 5) states – все состояния (первые токены строк, начиная с пятой)
    vector<string> TrueStates = {"A", "B", "C", "D", "E", "F", "G", "H"};
    ASSERT_EQ(TrueStates, dfa.getStates());

    // 6) transitions – таблица переходов
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

    // Сравниваем размеры и каждый переход
    auto transitions = dfa.getTransitions();
    ASSERT_EQ(TrueTransitions.size(), transitions.size());
    for (const auto& [state, trans] : TrueTransitions) {
        auto it = transitions.find(state);
        ASSERT_NE(it, transitions.end()) << "Отсутствует состояние " << state;
        ASSERT_EQ(trans, it->second) << "Неверные переходы для состояния " << state;
    }
}

TEST(DFATests, DFAinputTest2_ParityOfOnes) {
    // ================ ARRANGE ===============
    // ARRANGE: создаём временный CSV-файл с тестовыми данными
    string filename = "test_dfa_parity.csv";
    ofstream file(filename);
    ASSERT_TRUE(file.is_open()) << "Не удалось создать временный файл";

    // Записываем содержимое в формате, ожидаемом DFAinput
    file << "переход;0;1;;\n";                 // пример входной строки (пустая)
    file << "допустимые состояния;Even;;;\n";  // допускающее состояние
    file << "Начальное состояние;Even;;;\n";
    file << "состояния/алфавит;0;1;;\n";  // алфавит
    file << "Even;Even;Odd;;\n";          // переходы из Even
    file << "Odd;Odd;Even;;\n";           // переходы из Odd
    file.close();

    // ============== ACT ===================
    // ACT: вызываем функцию чтения
    auto dfa = DFABuilder{}.withCSV(filename).build();
    remove(filename.c_str());  // Очистка: удаляем временный файл

    // ============== ASSERT ===================
    // ASSERT: проверяем каждое поле структуры DFA

    // 1) string_transition – цифры из первой строки (все, кроме первого токена)
    vector<string> TrueStringTransition = {"0", "1"};
    ASSERT_EQ(TrueStringTransition, dfa.getStringTransition());

    // 2) permited_state – допустимые состояния (второй токен второй строки)
    vector<string> TruePermitedStates = {"Even"};
    ASSERT_EQ(TruePermitedStates, dfa.getPermittedStates());

    // 3) start_state – начальное состояние (второй токен третьей строки)
    ASSERT_EQ("Even", dfa.getStartState());

    // 4) alphabet – символы алфавита (со второй ячейки четвёртой строки)
    vector<string> TrueAlphabet = {"0", "1"};
    ASSERT_EQ(TrueAlphabet, dfa.getAlphabet());

    // 5) states – все состояния (первые токены строк, начиная с пятой)
    vector<string> TrueStates = {"Even", "Odd"};
    ASSERT_EQ(TrueStates, dfa.getStates());

    // 6) transitions – таблица переходов
    unordered_map<string, unordered_map<string, string>> TrueTransitions;
    TrueTransitions["Even"]["0"] = "Even";
    TrueTransitions["Even"]["1"] = "Odd";
    TrueTransitions["Odd"]["0"] = "Odd";
    TrueTransitions["Odd"]["1"] = "Even";

    // Сравниваем размеры и каждый переход
    auto transitions = dfa.getTransitions();
    ASSERT_EQ(TrueTransitions.size(), transitions.size());
    for (const auto& [state, trans] : TrueTransitions) {
        auto it = transitions.find(state);
        ASSERT_NE(it, transitions.end()) << "Отсутствует состояние " << state;
        ASSERT_EQ(trans, it->second) << "Неверные переходы для состояния " << state;
    }
}

TEST(DFATests, DFAinputTest3_EndsWith01) {
    // ================ ARRANGE ===============
    // ARRANGE: создаём временный CSV-файл с тестовыми данными
    string filename = "test_dfa_ends01.csv";
    ofstream file(filename);
    ASSERT_TRUE(file.is_open()) << "Не удалось создать временный файл";

    // Записываем содержимое в формате, ожидаемом DFAinput
    file << "переход;0;1;0;1\n";  // пример строки "01"
    file << "допустимые состояния;S01;;;\n";
    file << "Начальное состояние;S;;;\n";
    file << "состояния/алфавит;0;1;;\n";
    file << "S;S0;S;;\n";     // S по 0 -> S0, по 1 -> S
    file << "S0;S0;S01;;\n";  // S0 по 0 -> S0, по 1 -> S01
    file << "S01;S0;S;;\n";   // S01 по 0 -> S0, по 1 -> S
    file.close();

    // ============== ACT ===================
    // ACT: вызываем функцию чтения
    auto dfa = DFABuilder{}.withCSV(filename).build();
    remove(filename.c_str());  // Очистка: удаляем временный файл

    // ============== ASSERT ===================
    // ASSERT: проверяем каждое поле структуры DFA

    // 1) string_transition – цифры из первой строки (все, кроме первого токена)
    vector<string> TrueStringTransition = {"0", "1", "0", "1"};
    ASSERT_EQ(TrueStringTransition, dfa.getStringTransition());

    // 2) permited_state – допустимые состояния (второй токен второй строки)
    vector<string> TruePermitedStates = {"S01"};
    ASSERT_EQ(TruePermitedStates, dfa.getPermittedStates());

    // 3) start_state – начальное состояние (второй токен третьей строки)
    ASSERT_EQ("S", dfa.getStartState());

    // 4) alphabet – символы алфавита (со второй ячейки четвёртой строки)
    vector<string> TrueAlphabet = {"0", "1"};
    ASSERT_EQ(TrueAlphabet, dfa.getAlphabet());

    // 5) states – все состояния (первые токены строк, начиная с пятой)
    vector<string> TrueStates = {"S", "S0", "S01"};
    ASSERT_EQ(TrueStates, dfa.getStates());

    // 6) transitions – таблица переходов
    unordered_map<string, unordered_map<string, string>> TrueTransitions;
    TrueTransitions["S"]["0"] = "S0";
    TrueTransitions["S"]["1"] = "S";
    TrueTransitions["S0"]["0"] = "S0";
    TrueTransitions["S0"]["1"] = "S01";
    TrueTransitions["S01"]["0"] = "S0";
    TrueTransitions["S01"]["1"] = "S";

    // Сравниваем размеры и каждый переход
    auto transitions = dfa.getTransitions();
    ASSERT_EQ(TrueTransitions.size(), transitions.size());
    for (const auto& [state, trans] : TrueTransitions) {
        auto it = transitions.find(state);
        ASSERT_NE(it, transitions.end()) << "Отсутствует состояние " << state;
        ASSERT_EQ(trans, it->second) << "Неверные переходы для состояния " << state;
    }
}

TEST(DFATests, DFAinputTest4_SingleState) {
    // ================ ARRANGE ===============
    // ARRANGE: создаём временный CSV-файл с тестовыми данными
    string filename = "test_dfa_single.csv";
    ofstream file(filename);
    ASSERT_TRUE(file.is_open()) << "Не удалось создать временный файл";

    // Записываем содержимое в формате, ожидаемом DFAinput
    file << "переход;0;1;;;;\n";  // пример строки "01"
    file << "допустимые состояния;A;;;\n";
    file << "Начальное состояние;A;;;;;;\n";
    file << "состояния/алфавит;0;1;;;;;\n";
    file << "A;A;A;;;;\n";  // петли по 0 и 1
    file.close();

    // ============== ACT ===================
    // ACT: вызываем функцию чтения
    auto dfa = DFABuilder{}.withCSV(filename).build();
    remove(filename.c_str());  // Очистка: удаляем временный файл

    // ============== ASSERT ===================
    // ASSERT: проверяем каждое поле структуры DFA

    // 1) string_transition – цифры из первой строки (все, кроме первого токена)
    vector<string> TrueStringTransition = {"0", "1"};
    ASSERT_EQ(TrueStringTransition, dfa.getStringTransition());

    // 2) permited_state – допустимые состояния (второй токен второй строки)
    vector<string> TruePermitedStates = {"A"};
    ASSERT_EQ(TruePermitedStates, dfa.getPermittedStates());

    // 3) start_state – начальное состояние (второй токен третьей строки)
    ASSERT_EQ("A", dfa.getStartState());

    // 4) alphabet – символы алфавита (со второй ячейки четвёртой строки)
    vector<string> TrueAlphabet = {"0", "1"};
    ASSERT_EQ(TrueAlphabet, dfa.getAlphabet());

    // 5) states – все состояния (первые токены строк, начиная с пятой)
    vector<string> TrueStates = {"A"};
    ASSERT_EQ(TrueStates, dfa.getStates());

    // 6) transitions – таблица переходов
    unordered_map<string, unordered_map<string, string>> TrueTransitions;
    TrueTransitions["A"]["0"] = "A";
    TrueTransitions["A"]["1"] = "A";

    // Сравниваем размеры и каждый переход
    auto transitions = dfa.getTransitions();
    ASSERT_EQ(TrueTransitions.size(), transitions.size());
    for (const auto& [state, trans] : TrueTransitions) {
        auto it = transitions.find(state);
        ASSERT_NE(it, transitions.end()) << "Отсутствует состояние " << state;
        ASSERT_EQ(trans, it->second) << "Неверные переходы для состояния " << state;
    }
}

TEST(DFAinputTest_NotAllAlphabetUse, NotAllAlphabetUse) {
    // ================ ARRANGE ===============
    // ARRANGE: создаём временный CSV-файл с тестовыми данными
    string filename = "test_dfa_NotAllAlphabetUse.csv";
    ofstream file(filename);
    ASSERT_TRUE(file.is_open()) << "Не удалось создать временный файл";

    // Записываем содержимое в формате, ожидаемом DFAinput
    file << "переход;0;1;0;1\n";
    file << "допустимые состояния;C;;;\n";
    file << "Начальное состояние;A;;;\n";
    file << "состояния/алфавит;0;1;;\n";
    file << "A;B;F;;\n";
    file << "B;G;C;;\n";
    file << "C;A;C;;\n";
    file << "D;;G;;\n";
    file << "E;H;F;;\n";
    file << "F;;G;;\n";
    file << "G;;E;;\n";
    file << "H;G;C;;\n";
    file.close();

    // ============== ACT ===================
    // ACT: вызываем функцию чтения
    auto dfa = DFABuilder{}.withCSV(filename).build();
    remove(filename.c_str());  // Очистка: удаляем временный файл

    // ============== ASSERT ===================
    // ASSERT: проверяем каждое поле структуры DFA

    // 1) string_transition – цифры из первой строки (все, кроме первого токена)
    vector<string> TrueStringTransition = {"0", "1", "0", "1"};
    ASSERT_EQ(TrueStringTransition, dfa.getStringTransition());

    // 2) permited_state – допустимые состояния (второй токен второй строки)
    vector<string> TruePermitedStates = {"C"};
    ASSERT_EQ(TruePermitedStates, dfa.getPermittedStates());

    // 3) start_state – начальное состояние (второй токен третьей строки)
    ASSERT_EQ("A", dfa.getStartState());

    // 4) alphabet – символы алфавита (со второй ячейки четвёртой строки)
    vector<string> TrueAlphabet = {"0", "1"};
    ASSERT_EQ(TrueAlphabet, dfa.getAlphabet());

    // 5) states – все состояния (первые токены строк, начиная с пятой)
    vector<string> TrueStates = {"A", "B", "C", "D", "E", "F", "G", "H"};
    ASSERT_EQ(TrueStates, dfa.getStates());

    // 6) transitions – таблица переходов
    unordered_map<string, unordered_map<string, string>> TrueTransitions;
    TrueTransitions["A"]["0"] = "B";
    TrueTransitions["A"]["1"] = "F";
    TrueTransitions["B"]["0"] = "G";
    TrueTransitions["B"]["1"] = "C";
    TrueTransitions["C"]["0"] = "A";
    TrueTransitions["C"]["1"] = "C";
    // TrueTransitions["D"]["0"] = "C";
    TrueTransitions["D"]["1"] = "G";
    TrueTransitions["E"]["0"] = "H";
    TrueTransitions["E"]["1"] = "F";
    // TrueTransitions["F"]["0"] = "C";
    TrueTransitions["F"]["1"] = "G";
    // TrueTransitions["G"]["0"] = "G";
    TrueTransitions["G"]["1"] = "E";
    TrueTransitions["H"]["0"] = "G";
    TrueTransitions["H"]["1"] = "C";

    // Сравниваем размеры и каждый переход
    auto transitions = dfa.getTransitions();
    ASSERT_EQ(TrueTransitions.size(), transitions.size());
    for (const auto& [state, trans] : TrueTransitions) {
        auto it = transitions.find(state);
        ASSERT_NE(it, transitions.end()) << "Отсутствует состояние " << state;
        ASSERT_EQ(trans, it->second) << "Неверные переходы для состояния " << state;
    }
}