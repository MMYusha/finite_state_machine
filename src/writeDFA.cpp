#include <func_DFA/DFAinput.hpp>
#include <fstream>
#include <iostream>

namespace func_input {

void writeDFA(const std::string& filename, const Result& res) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл для записи: " << filename << "\n";
        return;
    }
    const unsigned char bom[] = {0xEF, 0xBB, 0xBF};
    file.write(reinterpret_cast<const char*>(bom), sizeof(bom));
    // 1) Строка "переход;0;1;0;1"
    file << "переход;";
    for (size_t i = 0; i < res.string_transition.size(); ++i) {
        if (i > 0) file << ";";
        file << res.string_transition[i];
    }
    file << "\n";

    // 2) Строка "допустимые состояния;C;;;"
    file << "допустимые состояния;";
    for (size_t i = 0; i < res.permited_state.size(); ++i) {
        if (i > 0) file << ";";
        file << res.permited_state[i];
    }
    file << "\n";

    // 3) Строка "Начальное состояние;A;;;"
    file << "Начальное состояние;" << res.start_state << ";;;\n";

    // 4) Строка "состояния/алфавит;0;1;;"
    file << "состояния/алфавит;";
    for (size_t i = 0; i < res.alphabet.size(); ++i) {
        if (i > 0) file << ";";
        file << res.alphabet[i];
    }
    file << "\n";

    // 5) Строки состояний и переходов
    for (const auto& state : res.states) {
        file << state << ";";

        // Переходы по каждому символу алфавита
        auto itState = res.transitions.find(state);
        if (itState != res.transitions.end()) {
            const auto& transMap = itState->second;
            for (size_t i = 0; i < res.alphabet.size(); ++i) {
                if (i > 0) file << ";";
                auto itSym = transMap.find(res.alphabet[i]);
                if (itSym != transMap.end()) {
                    file << itSym->second;
                }
                // если перехода нет – оставляем пустое поле
            }
        } else {
            // если состояние вообще отсутствует в таблице переходов – пишем пустые поля
            for (size_t i = 0; i < res.alphabet.size(); ++i) {
                if (i > 0) file << ";";
                // пусто
            }
        }
        // Добавляем два пустых поля (;;) для соответствия формату примера
        file << ";;\n";
    }
    printf("ДКА сохранен в %s\n\n",filename.c_str());
}

} // namespace func_input