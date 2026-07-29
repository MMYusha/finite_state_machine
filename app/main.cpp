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

    
    auto dfa = DFABuilder{}.withCSV("input.csv").build(); // Чтение ДКА из csv файла
    dfa.print(); // Вывод ДКА в консоль
    dfa.transit_fromCSV();
    dfa.transit_Input({"0","1","0","1"});
    dfa.minimize(); // Минимизация ДКА
    dfa.exportCSV("output.csv"); // Запись нового ДКА в файл


    cout << "\nНажмите Enter, чтобы закрыть окно...";
    cin.get();  // ждёт нажатия Enter
    return 0;
}