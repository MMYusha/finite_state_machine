// заголовочный файл тестового фреймворка
#include <gtest/gtest.h>
#include <iostream>
#include <unordered_map>
#include <string>
#include <func_transition/transit.hpp>

using namespace func_transition;

TEST(DFATests, TransitTest) {
    // хороший тест делится на три части - Arrange-Act-Assert (либо Given-When-Then, называйте как нравится)
    
    // ARRANGE - это подготовка почвы; здесь нужные объявления и операции для создания ситуации, которую хотим проверить
    using State = char;
    using Symbol = char;

    // Вложенная хеш-таблица переходов:
    // состояние -> (символ -> следующее состояние)
    std::unordered_map<State, std::unordered_map<Symbol, State>> transitions;

    // Заполнение переходов согласно таблице
    transitions['A']['0'] = 'B';
    transitions['A']['1'] = 'F';

    transitions['B']['0'] = 'G';
    transitions['B']['1'] = 'C';

    transitions['C']['0'] = 'A';
    transitions['C']['1'] = 'C';

    transitions['D']['0'] = 'C';
    transitions['D']['1'] = 'G';

    transitions['E']['0'] = 'H';
    transitions['E']['1'] = 'F';

    transitions['F']['0'] = 'C';
    transitions['F']['1'] = 'G';

    transitions['G']['0'] = 'G';
    transitions['G']['1'] = 'E';

    transitions['H']['0'] = 'G';
    transitions['H']['1'] = 'C';

    State start = 'A';  // начальное состояние
    State current = start;


    // ACT - это само действие; именно то, что нам нужно проверить
    std::string input1 = "0101"; // пример входной строки
    State final_state1 = func_transition::transit(input1, current, transitions);
    std::string input2 = "00010010111011"; // пример входной строки A 0B 0G 0G 1E 0H 0G 1E 0H 1C 1C 1C 0A 1F 1G
    State final_state2 = func_transition::transit(input2, current, transitions);
    std::string input3 = "0"; // пример входной строки
    State final_state3 = func_transition::transit(input3, current, transitions);
    std::string input4 = "01E01"; // пример входной строки
    State final_state4 = func_transition::transit(input4, current, transitions);
    std::string input5 = ""; // пример входной строки
    State final_state5 = func_transition::transit(input5, current, transitions);

    // ASSERT - проверка результатов; именно ASSERT определяет, пройден тест или нет
    ASSERT_EQ('F', final_state1);
    ASSERT_EQ('G', final_state2);
    ASSERT_EQ('B', final_state3);
    ASSERT_EQ('C', final_state4);
    ASSERT_EQ('A', final_state5);
}


// если материал выше легко усвоили, почитайте также про TEST_F (test fixtures) и setup/teardown
// в любом случае, вам поможет gtest primer 