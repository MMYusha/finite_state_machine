#include <cstdio>  // для printf
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>

#include <func_DFA/DFA.hpp>

using namespace std;
using namespace func_DFA;


int main() {
    system("chcp 65001 > nul"); 

    // Чтение ДКА из csv файла
    DFA5 dfa = DFA5::fromCSV("input.csv");
    // Вывод ДКА в консоль
    dfa.print();
    // Запись ДКА в файл
    dfa.exportCSV("output.csv");

    cout << "\nНажмите Enter, чтобы закрыть окно...";
    cin.get();  // ждёт нажатия Enter
    return 0;
}