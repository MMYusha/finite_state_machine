// заголовочный файл тестового фреймворка
#include <gtest/gtest.h>

#include <algorithm>  // для функции сортировки
#include <cstdio>     // для printf
#include <func_DFA/DFA.hpp>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;
using namespace func_DFA;

void sortClasses(vector<vector<string>>& v) {
    for (auto& cls : v) {
        sort(cls.begin(), cls.end());
    }
    sort(v.begin(), v.end());
}

TEST(DFATests, DFAminTest1_RealDFA_1) {
    // ================ ARRANGE ===============
    // ARRANGE - это подготовка почвы: здесь нужные объявления и операции для создания ситуации,
    // которую хотим проверить
    string start = "A";                                                // начальное состояние
    vector<string> states = {"A", "B", "C", "D", "E", "F", "G", "H"};  // множество состояний
    vector<string> alphabet = {"0", "1"};                              // алфавит
    vector<string> Permitted_states = {"C"};  // множество допустимых состояний
    unordered_map<string, unordered_map<string, string>>
        transitions;  // Вложенная хеш-таблица переходов: состояние -> (символ -> следующее
                      // состояние)
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
    auto dfa =
        DFABuilder{}.withComponents(start, states, Permitted_states, alphabet, transitions).build();

    // ============== ACT ===================
    // ACT - это само действие: то, что нужно проверить
    auto Partition = DFATestHelper::getPartition(dfa);

    // ============== ASSERT ===================
    // ASSERT - проверка результатов: ASSERT определяет, пройден тест или нет
    vector<vector<string>> PartitionTrue = {{"C"}, {"F"}, {"B", "H"}, {"G"}, {"A", "E"}};
    auto Partition_sorted = Partition;          // копируем фактический результат
    auto PartitionTrue_sorted = PartitionTrue;  // копируем ожидаемый
    sortClasses(Partition_sorted);
    sortClasses(PartitionTrue_sorted);
    ASSERT_EQ(PartitionTrue_sorted, Partition_sorted);
}

TEST(DFATests, DFAminTest1_RealDFA_2) {
    // ================ ARRANGE ===============
    // ARRANGE - это подготовка почвы: здесь нужные объявления и операции для создания ситуации,
    // которую хотим проверить
    string start = "0";                                                // начальное состояние
    vector<string> states = {"0", "1", "2", "3", "4", "5", "6", "7"};  // множество состояний
    vector<string> alphabet = {"a", "b"};                              // алфавит
    vector<string> Permitted_states = {"3", "4", "6"};  // множество допустимых состояний
    unordered_map<string, unordered_map<string, string>>
        transitions;  // Вложенная хеш-таблица переходов: состояние -> (символ -> следующее
                      // состояние)
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

    // Создание ДКА
    auto dfa =
        DFABuilder{}.withComponents(start, states, Permitted_states, alphabet, transitions).build();

    // ============== ACT ===================
    // ACT - это само действие: то, что нужно проверить
    auto Partition = DFATestHelper::getPartition(dfa);

    // ============== ASSERT ===================
    // ASSERT - проверка результатов: ASSERT определяет, пройден тест или нет
    vector<vector<string>> PartitionTrue = {{"0", "2"}, {"1", "5", "7"}, {"3", "6"}, {"4"}};
    auto Partition_sorted = Partition;          // копируем фактический результат
    auto PartitionTrue_sorted = PartitionTrue;  // копируем ожидаемый
    sortClasses(Partition_sorted);
    sortClasses(PartitionTrue_sorted);
    ASSERT_EQ(PartitionTrue_sorted, Partition_sorted);
}

TEST(DFATests, DFAminTest1_RealDFA_3) {
    // ================ ARRANGE ===============
    // ARRANGE - это подготовка почвы: здесь нужные объявления и операции для создания ситуации,
    // которую хотим проверить
    string start = "q0";                                           // начальное состояние
    vector<string> states = {"q0", "q1", "q2", "q3", "q4", "q5"};  // множество состояний
    vector<string> alphabet = {"a", "b"};                          // алфавит
    vector<string> Permitted_states = {"q0", "q1"};                // множество допустимых состояний
    unordered_map<string, unordered_map<string, string>>
        transitions;  // Вложенная хеш-таблица переходов: состояние -> (символ -> следующее
                      // состояние)
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

    // Создание ДКА
    auto dfa =
        DFABuilder{}.withComponents(start, states, Permitted_states, alphabet, transitions).build();

    // ============== ACT ===================
    // ACT - это само действие: то, что нужно проверить
    auto Partition = DFATestHelper::getPartition(dfa);

    // ============== ASSERT ===================
    // ASSERT - проверка результатов: ASSERT определяет, пройден тест или нет
    vector<vector<string>> PartitionTrue = {{"q0", "q1"}, {"q2", "q3"}, {"q4"}};
    auto Partition_sorted = Partition;          // копируем фактический результат
    auto PartitionTrue_sorted = PartitionTrue;  // копируем ожидаемый
    sortClasses(Partition_sorted);
    sortClasses(PartitionTrue_sorted);
    ASSERT_EQ(PartitionTrue_sorted, Partition_sorted);
}

TEST(DFATests, DFAminTest2_UnreachableStates) {
    // ================ ARRANGE ===============
    // ARRANGE - это подготовка почвы: здесь нужные объявления и операции для создания ситуации,
    // которую хотим проверить
    string start = "A";                                           // начальное состояние
    vector<string> states = {"A", "B", "C", "D", "E", "F", "G"};  // множество состояний
    vector<string> alphabet = {"0", "1"};                         // алфавит
    vector<string> Permitted_states = {"E", "D"};                 // множество допустимых состояний
    unordered_map<string, unordered_map<string, string>>
        transitions;  // Вложенная хеш-таблица переходов: состояние -> (символ -> следующее
                      // состояние)
    transitions["A"]["0"] = "B";
    transitions["A"]["1"] = "C";
    // transitions["B"]["0"] = "B";
    transitions["B"]["1"] = "D";
    transitions["C"]["0"] = "D";
    transitions["C"]["1"] = "E";
    // transitions["D"]["0"] = "D";
    transitions["D"]["1"] = "E";
    transitions["E"]["0"] = "B";
    transitions["E"]["1"] = "D";
    transitions["F"]["0"] = "G";
    transitions["F"]["1"] = "E";
    transitions["G"]["0"] = "F";
    transitions["G"]["1"] = "D";

    // Создание ДКА
    auto dfa =
        DFABuilder{}.withComponents(start, states, Permitted_states, alphabet, transitions).build();

    // ============== ACT ===================
    // ACT - это само действие: то, что нужно проверить
    auto Partition = DFATestHelper::getPartition(dfa);

    // ============== ASSERT ===================
    // ASSERT - проверка результатов: ASSERT определяет, пройден тест или нет
    vector<vector<string>> PartitionTrue = {{"A"}, {"B"}, {"C"}, {"D"}, {"E"}};
    auto Partition_sorted = Partition;          // копируем фактический результат
    auto PartitionTrue_sorted = PartitionTrue;  // копируем ожидаемый
    sortClasses(Partition_sorted);
    sortClasses(PartitionTrue_sorted);
    ASSERT_EQ(PartitionTrue_sorted, Partition_sorted);
}

TEST(DFATests, DFAminTest2_Idempotency) {
    // ================ ARRANGE ===============
    // ARRANGE - это подготовка почвы: здесь нужные объявления и операции для создания ситуации,
    // которую хотим проверить
    string start = "A";                                                // начальное состояние
    vector<string> states = {"A", "B", "C", "D", "E", "F", "G", "H"};  // множество состояний
    vector<string> alphabet = {"0", "1"};                              // алфавит
    vector<string> Permitted_states = {"C"};  // множество допустимых состояний
    unordered_map<string, unordered_map<string, string>>
        transitions;  // Вложенная хеш-таблица переходов: состояние -> (символ -> следующее
                      // состояние)
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
    auto dfa =
        DFABuilder{}.withComponents(start, states, Permitted_states, alphabet, transitions).build();

    // ============== ACT ===================
    // ACT - это само действие: то, что нужно проверить
    dfa.minimize();
    auto Partition = DFATestHelper::getPartition(dfa);

    // ============== ASSERT ===================
    // ASSERT - проверка результатов: ASSERT определяет, пройден тест или нет
    vector<vector<string>> PartitionTrue = {{"A+E"}, {"B+H"}, {"C"}, {"F"}, {"G"}};
    auto Partition_sorted = Partition;          // копируем фактический результат
    auto PartitionTrue_sorted = PartitionTrue;  // копируем ожидаемый
    sortClasses(Partition_sorted);
    sortClasses(PartitionTrue_sorted);
    ASSERT_EQ(PartitionTrue_sorted, Partition_sorted);
}

TEST(DFATests, DFAminTest3_AlreadyMinimal) {
    // ================ ARRANGE ===============
    // ARRANGE - это подготовка почвы: здесь нужные объявления и операции для создания ситуации,
    // которую хотим проверить
    string start = "S";                          // начальное состояние
    vector<string> states = {"S", "S0", "S01"};  // множество состояний
    vector<string> alphabet = {"0", "1"};        // алфавит
    vector<string> Permitted_states = {"S01"};   // множество допустимых состояний
    unordered_map<string, unordered_map<string, string>>
        transitions;  // Вложенная хеш-таблица переходов: состояние -> (символ -> следующее
                      // состояние)
    transitions["S"]["0"] = "S0";
    transitions["S"]["1"] = "S";
    transitions["S0"]["0"] = "S0";
    transitions["S0"]["1"] = "S01";
    transitions["S01"]["0"] = "S0";
    transitions["S01"]["1"] = "S";

    // Создание ДКА
    auto dfa =
        DFABuilder{}.withComponents(start, states, Permitted_states, alphabet, transitions).build();

    // ============== ACT ===================
    // ACT - это само действие: то, что нужно проверить
    auto Partition = DFATestHelper::getPartition(dfa);

    // ============== ASSERT ===================
    // ASSERT - проверка результатов: ASSERT определяет, пройден тест или нет
    vector<vector<string>> PartitionTrue = {{"S01"}, {"S"}, {"S0"}};
    auto Partition_sorted = Partition;          // копируем фактический результат
    auto PartitionTrue_sorted = PartitionTrue;  // копируем ожидаемый
    sortClasses(Partition_sorted);
    sortClasses(PartitionTrue_sorted);
    ASSERT_EQ(PartitionTrue_sorted, Partition_sorted);
}

TEST(DFATests, DFAminTest4_TwoEquivalent) {
    // ================ ARRANGE ===============
    // ARRANGE - это подготовка почвы: здесь нужные объявления и операции для создания ситуации,
    // которую хотим проверить
    string start = "A";                       // начальное состояние
    vector<string> states = {"A", "B", "C"};  // множество состояний
    vector<string> alphabet = {"0", "1"};     // алфавит
    vector<string> Permitted_states = {"C"};  // множество допустимых состояний
    unordered_map<string, unordered_map<string, string>>
        transitions;  // Вложенная хеш-таблица переходов: состояние -> (символ -> следующее
                      // состояние)
    transitions["A"]["0"] = "C";
    transitions["A"]["1"] = "B";
    transitions["B"]["0"] = "C";
    transitions["B"]["1"] = "B";
    transitions["C"]["0"] = "C";
    transitions["C"]["1"] = "C";

    // Создание ДКА
    auto dfa =
        DFABuilder{}.withComponents(start, states, Permitted_states, alphabet, transitions).build();

    // ============== ACT ===================
    // ACT - это само действие: то, что нужно проверить
    auto Partition = DFATestHelper::getPartition(dfa);

    // ============== ASSERT ===================
    // ASSERT - проверка результатов: ASSERT определяет, пройден тест или нет
    vector<vector<string>> PartitionTrue = {{"C"}, {"A", "B"}};
    auto Partition_sorted = Partition;          // копируем фактический результат
    auto PartitionTrue_sorted = PartitionTrue;  // копируем ожидаемый
    sortClasses(Partition_sorted);
    sortClasses(PartitionTrue_sorted);
    ASSERT_EQ(PartitionTrue_sorted, Partition_sorted);
}

TEST(ErrorDFATests, ErrorDFAminTest1_WrongStates) {
    // Ошибка в states: вместо H - K

    // ================ ARRANGE ===============
    // ARRANGE - это подготовка почвы: здесь нужные объявления и операции для создания ситуации,
    // которую хотим проверить
    string start = "A";  // начальное состояние
    vector<string> states = {"A", "B", "C", "D",
                             "E", "F", "G", "K"};  // множество состояний --------------
    vector<string> alphabet = {"0", "1"};          // алфавит
    vector<string> Permitted_states = {"C"};       // множество допустимых состояний
    unordered_map<string, unordered_map<string, string>>
        transitions;  // Вложенная хеш-таблица переходов: состояние -> (символ -> следующее
                      // состояние)
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
    auto dfa =
        DFABuilder{}.withComponents(start, states, Permitted_states, alphabet, transitions).build();

    // ============== ACT + ASSERT ===================
    // ASSERT - проверка результатов: ASSERT определяет, пройден тест или нет
    EXPECT_THROW(DFATestHelper::getPartition(dfa), invalid_argument);
}

TEST(ErrorDFATests, ErrorDFAminTest2_WrongPermitedStates) {
    // Ошибка в Permitted_states: вместо C - K

    // ================ ARRANGE ===============
    // ARRANGE - это подготовка почвы: здесь нужные объявления и операции для создания ситуации,
    // которую хотим проверить
    string start = "A";                                                // начальное состояние
    vector<string> states = {"A", "B", "C", "D", "E", "F", "G", "K"};  // множество состояний
    vector<string> alphabet = {"0", "1"};                              // алфавит
    vector<string> Permitted_states = {
        "K"};  // множество допустимых состояний ---------------------
    unordered_map<string, unordered_map<string, string>>
        transitions;  // Вложенная хеш-таблица переходов: состояние -> (символ -> следующее
                      // состояние)
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
    auto dfa =
        DFABuilder{}.withComponents(start, states, Permitted_states, alphabet, transitions).build();

    // ============== ACT + ASSERT ===================
    // ASSERT - проверка результатов: ASSERT определяет, пройден тест или нет
    EXPECT_THROW(DFATestHelper::getPartition(dfa), invalid_argument);
}

TEST(ErrorDFATests, ErrorDFAminTest3_WrongAlphabet) {
    // Ошибка в transitions (неизвестный символ алфавита): вместо G--1-->E - G--K-->E

    // ================ ARRANGE ===============
    // ARRANGE - это подготовка почвы: здесь нужные объявления и операции для создания ситуации,
    // которую хотим проверить
    string start = "A";                                                // начальное состояние
    vector<string> states = {"A", "B", "C", "D", "E", "F", "G", "K"};  // множество состояний
    vector<string> alphabet = {"0", "1"};     // алфавит ------------------------
    vector<string> Permitted_states = {"K"};  // множество допустимых состояний
    unordered_map<string, unordered_map<string, string>>
        transitions;  // Вложенная хеш-таблица переходов: состояние -> (символ -> следующее
                      // состояние)
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

    // Создание ДКА
    auto dfa =
        DFABuilder{}.withComponents(start, states, Permitted_states, alphabet, transitions).build();

    // ============== ACT + ASSERT ===================
    // ASSERT - проверка результатов: ASSERT определяет, пройден тест или нет
    EXPECT_THROW(DFATestHelper::getPartition(dfa), invalid_argument);
}

TEST(DFAminTest_NotAllAlphabetUse, NotAllAlphabetUse) {
    // ================ ARRANGE ===============
    // ARRANGE - это подготовка почвы: здесь нужные объявления и операции для создания ситуации,
    // которую хотим проверить
    string start = "A";                                                // начальное состояние
    vector<string> states = {"A", "B", "C", "D", "E", "F", "G", "H"};  // множество состояний
    vector<string> alphabet = {"0", "1"};                              // алфавит
    vector<string> Permitted_states = {"C"};  // множество допустимых состояний
    unordered_map<string, unordered_map<string, string>>
        transitions;  // Вложенная хеш-таблица переходов: состояние -> (символ -> следующее
                      // состояние)
    transitions["A"]["0"] = "B";
    transitions["A"]["1"] = "F";
    transitions["B"]["0"] = "G";
    transitions["B"]["1"] = "C";
    transitions["C"]["0"] = "A";
    transitions["C"]["1"] = "C";
    // transitions["D"]["0"] = "C";
    transitions["D"]["1"] = "G";
    transitions["E"]["0"] = "H";
    transitions["E"]["1"] = "F";
    // transitions["F"]["0"] = "C";
    transitions["F"]["1"] = "G";
    // transitions["G"]["0"] = "G";
    transitions["G"]["1"] = "E";
    transitions["H"]["0"] = "G";
    transitions["H"]["1"] = "C";

    // Создание ДКА
    auto dfa =
        DFABuilder{}.withComponents(start, states, Permitted_states, alphabet, transitions).build();

    // ============== ACT ===================
    // ACT - это само действие: то, что нужно проверить
    auto Partition = DFATestHelper::getPartition(dfa);

    // ============== ASSERT ===================
    // ASSERT - проверка результатов: ASSERT определяет, пройден тест или нет
    vector<vector<string>> PartitionTrue = {{"C"}, {"F"}, {"B", "H"}, {"G"}, {"A", "E"}};
    auto Partition_sorted = Partition;          // копируем фактический результат
    auto PartitionTrue_sorted = PartitionTrue;  // копируем ожидаемый
    sortClasses(Partition_sorted);
    sortClasses(PartitionTrue_sorted);
    ASSERT_EQ(PartitionTrue_sorted, Partition_sorted);
}