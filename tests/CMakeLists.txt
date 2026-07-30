# следующие три строчки обеспечивают работу gtest
find_package(GTest REQUIRED)
include(GoogleTest)
enable_testing()

# собираем тесты! путь: build/tests/my_math_test
# при желании можно разделить их на несколько исполняемых файлов,
# что иногда полезно на больших проектах, особенно с долгим рантаймом
add_executable(my_DFA_test
    #transit.test.cpp
    DFAmin.test.cpp
    CreaateNewTransitions.test.cpp
    #DFAinput.test.cpp
    #writeDFA.test.cpp
)

# линкуемся нашей библиотекой, а также с дефолтным мейном, чтобы не засорять им кодовую базу
# при желании его можно написать самостоятельно и, например, запускать тесты в опредленном порядке
# по умолчанию он просто запускает все тесты
target_link_libraries(my_DFA_test PRIVATE
    GTest::gtest_main
    func_DFA
)

# в данном случае не нужно, но в ваших серьезных проектах в тесты надо будет подключать публичные и приватные хэдеры
target_include_directories(my_DFA_test PRIVATE
    include
    ../src
)

# добавляем флаги компиляции для всех исходников с тестами
target_compile_options(my_DFA_test PRIVATE
    -Wall -Wextra -Wpedantic
)

# нужно для работы gtest
gtest_discover_tests(my_DFA_test)