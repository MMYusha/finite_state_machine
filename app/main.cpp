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

    
    DFA5 dfa = DFA5::fromCSV("input.csv"); // Чтение ДКА из csv файла
    dfa.print(); // Вывод ДКА в консоль
    dfa.minimize(); // Минимизация ДКА
    dfa.print(); // Вывод нового ДКА в консоль
    dfa.exportCSV("output.csv"); // Запись нового ДКА в файл


    cout << "\nНажмите Enter, чтобы закрыть окно...";
    cin.get();  // ждёт нажатия Enter
    return 0;
}