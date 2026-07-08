# собираем приложение! путь: build/app/main
# указваем имя исполняемого файла и исходники
add_executable(main main.cpp)

# линкуемся с библиотекой
# если не знаете, что это значит, гуглим "этапы компиляции C++"
target_link_libraries(main PRIVATE
    func_minimization
    func_transition
)

# флаги компиляции для всех исходников приложения

target_compile_options(func_transition PRIVATE
    -Wall -Wextra -Wpedantic
)
