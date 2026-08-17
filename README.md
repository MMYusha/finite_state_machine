# Проект по работе с ДКА

Библиотека для работы с детерминированными конечными автоматами (ДКА) на C++.

## Возможности

- Класс `DFA5` – представление ДКА (пятикортеж + дополнительные поля)
- Построитель `DFABuilder` – создание автомата из CSV, генерация случайных ДКА, ручная сборка
- Алгоритм минимизации **Хопкрофта** (`HopcroftMinimizer`) – сложность O(|Σ|·n·log n)
- Вывод автомата в консоль и экспорт в CSV
- Переход по строке (метод `transit_string`)
- Бенчмарк для замера производительности минимизации
- Набор юнит-тестов (Google Test)

## Зависимости

- Компилятор с поддержкой **C++20** (например, GCC ≥ 10, Clang ≥ 10)
- **CMake** (≥ 3.10)
- **Google Test** (для сборки тестов)
- **Gnuplot** (опционально, для построения графиков в бенчмарке)

Установка на Windows (через MSYS2):
```bash
pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-gtest

## Структура проекта

finite_state_machine/
├── CMakeLists.txt              # корневой файл сборки
├── .clang-format               # стиль форматирования
├── README.md
│
├── app/
│   └── main.cpp                # консольное приложение
│
├── include/
│   └── func_DFA/
│       └── DFA.hpp             # единый публичный заголовок
│
├── src/
│   ├── core/
│   │   ├── DFA.cpp
│   │   └── DFABuilder.cpp
│   ├── algorithms/
│   │   └── DFAmin.cpp          # минимизация Хопкрофта
│   ├── tools/
│   │   └── transit.cpp         # переход по строке
│   ├── benchmark/
│   │   ├── benchmark.cpp
│   │   └── benchmarkBuilder.cpp
│   └── utils/
│       └── DFATestHelper.cpp   # доступ к приватным методам для тестов
│
├── tests/
│   ├── algorithms/
│   │   ├── DFAmin.test.cpp
│   │   └── CreateNewTransitions.test.cpp
│   ├── io/
│   │   ├── DFAinput.test.cpp
│   │   └── writeDFA.test.cpp
│   └── tools/
│       └── transit.test.cpp
│
├── data/                       # входные/выходные файлы
│   ├── input.csv
│   ├── output.csv
│   └── (файлы бенчмарка)
│
└── build/                      # сборка (игнорируется)

---
## Сборка проекта
cd build
cmake -G "MinGW Makefiles" -D CMAKE_CXX_COMPILER=C:/msys64/ucrt64/bin/g++.exe -D BUILD_TESTS=ON ..
cmake --build .

После сборки исполняемый файл приложения:
/build/app/main.exe

Запуск тестов:
cd build
/build/tests/my_DFA_test.exe

## Бенчмарк
Результаты сохраняются в папку data/:
dfa_benchmark_data.txt – данные для графика
dfa_min_time.png – график зависимости времени от числа состояний


## Примечания
Для минимизации требуется хотя бы одно допускающее состояние.
CSV-файл должен соответствовать формату, описанному в тестах (пример: data/input.csv).
Для работы бенчмарка требуется установленный Gnuplot.
