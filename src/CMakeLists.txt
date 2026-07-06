# собираем библиотеку!
# my_math - это target library, целевая библиотека
# STATIC - собираем статическую библиотеку
# в последнюю очередь указываем все исходники библиотеки
# в итоге в build будет библиотека libmy_math.a
add_library(my_math STATIC
    sum.cpp
)

# делаем доступными публичные заголовочные файлы
# благодаря этому в sum.cpp заголовочный файл мы подключаем как системный
target_include_directories(my_math
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# флаги компиляции для target'а, в данном случае для всех исходников нашей библиотеки
# флаги -Wall -Wextra -Wpedantic рекомендую включать на всех проектах
# ...по крайней мере при использовании gcc или llvm тулчейнов; у msvc другие флаги, например
target_compile_options(my_math PRIVATE
    -Wall -Wextra -Wpedantic
)