
#include "../include/common_library.h"
#include "../include/io.h"
#include "../include/tests_square.h"
#include "../include/solve_square.h"

typedef struct Test_Set
{
    double a;
    double b;
    double c;

    double wanted_x1;
    double wanted_x2;
    int wanted_num_roots;
} Test_Set;

Test_Result once_test_square(double a, double b, double c,
                     double wanted_x1, double wanted_x2,
                     int wanted_num_roots, int test_num)
{
    double x1 = 0;
    double x2 = 0;

    Num_Roots num_roots = solve_square(a, b, c, &x1, &x2);

    if ( !(is_zero(x1 - wanted_x1)) ||
         !(is_zero(x2 - wanted_x2)) ||
         !(is_zero(num_roots - wanted_num_roots)))
    {
        printf(" # ERROR in test %d: a = %lg, b = %lg, c = %lg, x1 = %lg, x2 = %lg, num_roots = %d \n"
               "Expected: wanted_x1 = %lg, wanted_x2 = %lg, wanted_num_roots = %d                  \n",
               test_num, a, b, c, x1, x2, num_roots,
               wanted_x1, wanted_x2, wanted_num_roots);

        return TEST_FAILED;
    }

    printf("Test %d success. \n", test_num);

    return TEST_SUCCESS;
}

void test_square(void)
{
    const char *test_file_path = "../tests/builds_for_test.txt";

    const int test_scanf_return = 6;

    int error_test_count = 0;
    int       test_count = 0;

    Test_Set one_test = {0};

    FILE *test_file = fopen(test_file_path, "r");

    if (test_file == NULL)
    {
        printf("Couldn't open the file. \n");
    }

    else
    {

        while (fscanf(test_file, "%lg %lg %lg %lg %lg %d",
                                 &one_test.a, &one_test.b, &one_test.c,
                                 &one_test.wanted_x1, &one_test.wanted_x2,
                                 &one_test.wanted_num_roots) == test_scanf_return)
        {
            error_test_count += (int)once_test_square(one_test.a, one_test.b, one_test.c,
                                                      one_test.wanted_x1, one_test.wanted_x2,
                                                      one_test.wanted_num_roots, ++test_count);
        }

        fclose(test_file);

        if (error_test_count == 0)
            printf(" # All %d tests completed. \n", test_count);

        else
            printf(" # Test errors: %d out of %d \n", error_test_count, test_count);
    }

}
