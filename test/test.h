#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include <stddef.h>

#define YELLOW  "\033[1;33m"
#define GREEN   "\033[1;32m"
#define RED     "\033[1;31m"
#define RESET   "\033[0m"

#define CHAR_N_TIMES(char, times) printf("\n"); for (size_t i = 0; i < times; i++) putchar(char);

static int last_test_status = 1;
static size_t failed_tests = 0;

#define TEST(name) void test_##name(void)

#define ASSERT_TRUE(cond) \
    do { \
        if (!(cond)) { \
            last_test_status = 0; \
            fprintf(stderr, RED "\nAssertion failed (%s:%d): %s\n" RESET, __FILE__, __LINE__ , #cond); \
            return; \
        } \
    } while (0)
    
#define ASSERT_FALSE(cond)  \
    do { \
        if ((cond)) { \
            last_test_status = 0; \
            fprintf(stderr, RED "\nAssertion failed (%s:%d): %s\n" RESET, __FILE__, __LINE__ , #cond); \
            return; \
        } \
    } while (0)
    
#define ASSERT_EQUAL(expected, actual) \
    do { \
        if ((expected) != actual) { \
            last_test_status = 0; \
            fprintf(stderr, RED "\nAssertion failed (%s:%d), expect %lld, got %lld\n" RESET, __FILE__, __LINE__ , (long long) (expected), (long long) (actual)); \
            return; \
        } \
    } while (0)

#define ASSERT_NOT_EQUAL(expected, actual) \
    do { \
        if ((expected) == (actual)) { \
            last_test_status = 0; \
            fprintf(stderr, RED "\nAssertion failed (%s:%d), dit not expect %lld, but got %lld\n" RESET, __FILE__, __LINE__, (long long) (expected), (long long) (actual)); \
            return; \
        } \
    } while (0)

#define ASSERT_NULL(ptr)        ASSERT_FALSE((ptr))
#define ASSERT_NOT_NULL(ptr)    ASSERT_TRUE((ptr))

typedef void (*test_func)(void);

typedef struct {
    const char* name;
    test_func test;
} TestCase;

typedef struct {
    const char* name;
    TestCase* tests;
    size_t tests_num;
} TestSuite;

void run_test(TestCase* test) {
    last_test_status = 1;
    printf("[ RUN ] %s", test->name);
    test->test(); 
    
    if (last_test_status) { 
        printf("\r" GREEN "[ PASS ] " RESET  "%s\n", test->name);
    } else {
        failed_tests++;
        printf("\r" RED "[ FAIL ] " RESET "%s\n", test->name);
    }
}

void run_suite_tests(TestSuite* suite) {
    CHAR_N_TIMES('=', 80);
    printf("\nRunning %s test suite...\n", suite->name);

    size_t tests_num = suite->tests_num;
    for (size_t i = 0; i < tests_num; i++) {
        run_test(&suite->tests[i]);
    }
    CHAR_N_TIMES('=', 80);
    printf("\nSummary: Ran " YELLOW "%zu" RESET " tests, " GREEN "%zu" RESET " passed, " RED "%zu" RESET " failed.", tests_num,  tests_num - failed_tests, failed_tests);
    CHAR_N_TIMES('=', 80);
    printf("\n");
}

#endif // TEST_H