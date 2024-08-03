#include "../minunit.h"
#include "mips_asm.h"

// ********* function tests ********* //

MU_TEST(test_test)
{
    mu_assert(0 == 0, "Something's wrong");
}

MU_TEST_SUITE(function_tests)
{
    MU_RUN_TEST(test_test);
}


int main()
{
    MU_RUN_SUITE(function_tests);

    MU_REPORT();
    return MU_EXIT_CODE;
}

// ********* Helper functions ********* //