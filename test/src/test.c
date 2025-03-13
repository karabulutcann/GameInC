#include "unity.h"
#include "core/safe_strings.h"
#include "malloc.h"
#include "string.h"

void setUp(void) {
    // Setup code (before each test)
}

void tearDown(void) {
    // Cleanup code (after each test)
}


void testSafeString(void){
    struct DisposableString disposableS = {0};
    disposableS.string = malloc(strlen("hello world!") + 1);
    strcpy(disposableS.string,"hello world!");
}

void testTheUnityLib(void)
{
    TEST_ASSERT_EQUAL_HEX8(80, 450 / 5);
    TEST_ASSERT_EQUAL_HEX8(127, 127);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(testTheUnityLib);
    return UNITY_END();
}
