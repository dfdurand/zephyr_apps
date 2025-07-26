#include <zephyr/ztest.h>
#include <stdio.h>

static int shared_counter;

static void *math_suite_setup(void)
{
    printk(" **** math_suite_setup: initializing shared state ****\n");
    shared_counter = 0;
    return NULL;  // Return value passed to before/after/teardown
}

static void math_suite_before(void *fixture)
{
    printk(" !!!!  math_suite_before: resetting counter !!!! \n");
    shared_counter = 0;
}

static void math_suite_after(void *fixture)
{
    printk(" ##### math_suite_after #####: counter was %d\n", shared_counter);
}

static void math_suite_teardown(void *fixture)
{
    printk(" <---- math_suite_teardown ----> : cleaning up\n");
}

ZTEST_SUITE(math_suite,
            NULL,
            math_suite_setup,
            math_suite_before,
            math_suite_after,
            math_suite_teardown);

ZTEST(math_suite, test_addition)
{
    shared_counter += 5;
    zassert_equal(shared_counter, 5, "Addition logic failed");
}

ZTEST(math_suite, test_subtraction)
{
    shared_counter -= 20;
    zassert_equal(shared_counter, -20, "Subtraction logic failed");
}
ZTEST(math_suite, test_multiplication)
{
    shared_counter *= 2;
    zassert_equal(shared_counter, 0, "Multiplication logic failed");
}


ZTEST(math_suite, test_division)
{
    if (shared_counter == 0) {
        ztest_test_skip();
    }
    shared_counter /= 2;
    zassert_equal(shared_counter, 0, "Division logic failed");
}


/*  Second test suite: String Tests */

static void *string_suite_setup(void) {
    printk(" string_suite_setup\n");
    return NULL;
}

static void string_suite_before(void *f) {
    printk("  string_suite_before\n");
}

static void string_suite_after(void *f) {
    printk("  string_suite_after\n");
}

static void string_suite_teardown(void *f) {
    printk(" string_suite_teardown\n");
}

ZTEST_SUITE(string_suite,
            NULL,
            string_suite_setup,
            string_suite_before,
            string_suite_after,
            string_suite_teardown);

ZTEST(string_suite, test_strlen)
{
    const char *str = "zephyr";
    zassert_equal(strlen(str), 6, "String length mismatch");
}

ZTEST(string_suite, test_strcmp)
{
    const char *a = "hello";
    const char *b = "hello";
    zassert_true(strcmp(a, b) == 0, "Strings should be equal");
}
ZTEST(string_suite, test_strcat)
{
    char dest[20] = "Hello, ";
    const char *src = "World!";
    strcat(dest, src);
    zassert_true(strcmp(dest, "Hello, World!") == 0, "String concatenation failed");
}