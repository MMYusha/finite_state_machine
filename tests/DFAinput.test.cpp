#include <gtest/gtest.h>
#include <func_input/DFAinput.hpp>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;
using namespace func_input;

TEST(DFATests, DFAinputTest1) {
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

    // ACT: вызываем функцию чтения
    Result res = DFAinput(filename);

    // ASSERT: проверяем каждое поле структуры Result

    // 1) string_transition – цифры из первой строки (все, кроме первого токена)
    vector<string> expectedStringTransition = {"0", "1", "0", "1"};
    ASSERT_EQ(expectedStringTransition, res.string_transition);

    // 2) permited_state – допустимые состояния (второй токен второй строки)
    vector<string> expectedPermited = {"C"};
    ASSERT_EQ(expectedPermited, res.permited_state);

    // 3) start_state – начальное состояние (второй токен третьей строки)
    ASSERT_EQ("A", res.start_state);

    // 4) alphabet – символы алфавита (со второй ячейки четвёртой строки)
    vector<string> expectedAlphabet = {"0", "1"};
    ASSERT_EQ(expectedAlphabet, res.alphabet);

    // 5) states – все состояния (первые токены строк, начиная с пятой)
    vector<string> expectedStates = {"A", "B", "C", "D", "E", "F", "G", "H"};
    ASSERT_EQ(expectedStates, res.states);

    // 6) transitions – таблица переходов
    unordered_map<string, unordered_map<string, string>> expectedTransitions;
    expectedTransitions["A"]["0"] = "B";
    expectedTransitions["A"]["1"] = "F";
    expectedTransitions["B"]["0"] = "G";
    expectedTransitions["B"]["1"] = "C";
    expectedTransitions["C"]["0"] = "A";
    expectedTransitions["C"]["1"] = "C";
    expectedTransitions["D"]["0"] = "C";
    expectedTransitions["D"]["1"] = "G";
    expectedTransitions["E"]["0"] = "H";
    expectedTransitions["E"]["1"] = "F";
    expectedTransitions["F"]["0"] = "C";
    expectedTransitions["F"]["1"] = "G";
    expectedTransitions["G"]["0"] = "G";
    expectedTransitions["G"]["1"] = "E";
    expectedTransitions["H"]["0"] = "G";
    expectedTransitions["H"]["1"] = "C";

    // Сравниваем размеры и каждый переход
    ASSERT_EQ(expectedTransitions.size(), res.transitions.size());
    for (const auto& [state, trans] : expectedTransitions) {
        auto it = res.transitions.find(state);
        ASSERT_NE(it, res.transitions.end()) << "Отсутствует состояние " << state;
        ASSERT_EQ(trans, it->second) << "Неверные переходы для состояния " << state;
    }

    // Очистка: удаляем временный файл
    remove(filename.c_str());
}