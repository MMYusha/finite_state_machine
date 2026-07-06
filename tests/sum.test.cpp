// заголовочный файл тестового фреймворка
#include <gtest/gtest.h>

#include <my_math/sum.hpp>

using namespace my_math;


TEST(                               // это стандартный тестовый макрост gtest
    MyMathTests,                    // это имя тест-сьюта
    ResultIsSumOfIntegers           // это имя самого теста
) {
    // хороший тест делится на три части - Arrange-Act-Assert (либо Given-When-Then, называйте как нравится)
    
    // ARRANGE - это подготовка почвы; здесь нужные объявления и операции для создания ситуации, которую хотим проверить
    // в данном случае для примера инициализируем переменные
    int a = 5;
    int b = 17;

    // ACT - это само действие; именно то, что нам нужно проверить
    int result = sum(a, b);

    // ASSERT - проверка результатов; именно ASSERT определяет, пройден тест или нет
    ASSERT_EQ(22, result);
}

TEST(MyMathTests, HasCommutativeProperty) {
    // при этом короткий тест на самом деле хорош, если он читаем и понятен
    // в предыдущем тесте я рассказала про общий случай

    ASSERT_EQ(sum(3, 5), sum(5, 3));
}

TEST(ExampleFailingTests, ThisOneShouldFail) {
    // в gtest на самом деле много разных проверок
    // например, проверка на отсутствие исключения при выполнении
    ASSERT_NO_THROW(sum(1, 4));

    // при этом различаются ASSERT и EXPECT
    // expect в случае неудачи не прерывает тест
    EXPECT_EQ(-1, sum(1, 1));

    // а вот assert - да
    ASSERT_EQ(-1, sum(1, 1));
    // при этом при запуске теста gtest покажет, где именно он упал, а также ожидаемое и фактическое значение

    // вот эта проверка уже не выполнится, тк ASSERT зафейлит тест
    EXPECT_EQ(2, sum(1, 1));
}

TEST(ExampleFailingTests, ThisOneAlsoFails) {
    // если у вас сложная логика или вас не устраивают существующие ASSERT'ы и EXPECT'ы,
    // то можно зафейлить тест напрямую
    FAIL();
}

// если материал выше легко усвоили, почитайте также про TEST_F (test fixtures) и setup/teardown
// в любом случае, вам поможет gtest primer 