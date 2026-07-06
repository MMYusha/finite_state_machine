# собираем приложение! путь: build/app/main
# указваем имя исполняемого файла и исходники
add_executable(main main.cpp)

# линкуемся с библиотекой
# если не знаете, что это значит, гуглим "этапы компиляции C++"
target_link_libraries(main PRIVATE my_math)

# флаги компиляции для всех исходников приложения
target_compile_options(my_math PRIVATE /W4 /std:c++17)