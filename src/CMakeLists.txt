# собираем библиотеку!
# func_transition - это target library, целевая библиотека
# STATIC - собираем статическую библиотеку
# в последнюю очередь указываем все исходники библиотеки
# в итоге в build будет библиотека libfunc_transition.a
add_library(func_transition STATIC
    transit.cpp
)
add_library(func_minimization STATIC
    DFAmin.cpp
)
# делаем доступными публичные заголовочные файлы
# благодаря этому в sum.cpp заголовочный файл мы подключаем как системный
target_include_directories(func_transition
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
target_include_directories(func_minimization
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# флаги компиляции для target'а, в данном случае для всех исходников нашей библиотеки
# флаги -Wall -Wextra -Wpedantic рекомендую включать на всех проектах
# ...по крайней мере при использовании gcc или llvm тулчейнов; у msvc другие флаги, например

target_compile_options(func_transition PRIVATE
    -Wall -Wextra -Wpedantic
)
target_compile_options(func_minimization PRIVATE
    -Wall -Wextra -Wpedantic
)
