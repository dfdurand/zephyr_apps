#include <zephyr/ztest.h>
#include <zephyr/ztest_assert.h>
#include <string.h>

/*
 * Test Suite 1: Math Tests
 */
ZTEST_SUITE(zassert_calc_suite, NULL, NULL, NULL, NULL, NULL);

ZTEST(zassert_calc_suite, test_add)
{
    int result = 2 + 3;
    zassert_equal(result, 5, "Addition failed");
}

ZTEST(zassert_calc_suite, test_sub)
{
    int result = 10 - 4;
    zassert_equal(result, 6, "Subtraction failed");
}

/*
 * Test Suite 2: String Tests
 */
ZTEST_SUITE(zassert_str_suite, NULL, NULL, NULL, NULL, NULL);

ZTEST(zassert_str_suite, test_len)
{
    const char *str = "zephyr";
    zassert_equal(strlen(str), 6, "String length mismatch");
}

ZTEST(zassert_str_suite, test_cmp)
{
    const char *a = "hello";
    const char *b = "hello";
    zassert_true(strcmp(a, b) == 0, "Strings should be equal");
    zassert_mem_equal(a, b, strlen(a), "Memory comparison failed");
}

/*
 * Test Suite 1: Math Tests — using ZASSUME
 */
ZTEST_SUITE(zassume_calc_suite, NULL, NULL, NULL, NULL, NULL);

ZTEST(zassume_calc_suite, test_add)
{
    int result = 2 + 3;
    zassume_equal(result, 5, "Skipping: Unexpected addition result");
}

ZTEST(zassume_calc_suite, test_sub)
{
    int result = 10 - 4;
    zassume_equal(result, 6, "Skipping: Unexpected subtraction result");
}

/*
 * Test Suite 2: String Tests — using zassume
 */
ZTEST_SUITE(zassume_str_suite, NULL, NULL, NULL, NULL, NULL);

ZTEST(zassume_str_suite, test_len)
{
    const char *str = "zephyr";
    zassume_equal(strlen(str), 6, "Skipping: String length mismatch");
}

ZTEST(zassume_str_suite, test_cmp)
{
    const char *a = "hello";
    const char *b = "hello";
    zassume_true(strcmp(a, b) == 0, "Skipping: Strings do not match");
}
ZTEST(zassume_str_suite, test_memcmp)
{
    const char *a = "hello";
    const char *b = "hello";
    zassume_mem_equal(a, b, strlen(a), "Skipping: Memory comparison failed");
}