
#include <math.h>
#include "../include/common_library.h"
#include "../include/solve_square.h"
#include "../include/io.h"

void greeting(void)
{
    printf(" # This program solves quadratic equations. (2022) (C) Unnamed artist\n");
}

void clean_buffer(void)
{
    int ch = 0;

    while ((ch = getchar()) != '\n') { }
}

void guarded_input(double *a, double *b, double *c)
{
    SOFT_ASSERT(a != NULL);
    SOFT_ASSERT(b != NULL);
    SOFT_ASSERT(c != NULL);

    SOFT_ASSERT(a != b);
    SOFT_ASSERT(b != c);
    SOFT_ASSERT(a != c);

    const int input_scanf_return = 3;

    int ch = 0;
    bool is_input_correct = false;

    printf(" # Enter the coefficients a, b, c"
           " separated by a space in equation ax^2+bx+c.\n"
           " # Example: 14.5 167.43 -54.4               \n"
           " # Enter your coefficients:                 \n");

    while (!is_input_correct)
    {
        while (scanf("%lg %lg %lg", a, b, c) != input_scanf_return)
        {
            // if input error line skips
            clean_buffer();

            printf(" # Input error, try again: \n");
        }

        is_input_correct = true;

        // if any symbols after taking 3 numbers error input
        while ((ch = getchar()) != '\n')
            if (!isspace(ch))
                is_input_correct = false;

        if (!is_input_correct)
            printf(" # Input error, try again: \n");
    }

    printf(" # Accepted a, b, c: %lg, %lg, %lg.\n", *a, *b, *c);
}

void print_solution(Num_Roots num_roots, double x1, double x2)
{
    SOFT_ASSERT(isfinite(x1));
    SOFT_ASSERT(isfinite(x2));

    SOFT_ASSERT(num_roots >= INF_SOL);
    SOFT_ASSERT(num_roots <= TWO_SOL);

    switch (num_roots)
    {
        case  NO_SOL:
            printf(" # The equation has no solutions. \n");
            break;

        case ONE_SOL:
            printf(" # The equation has 1 solution: x = %lg. \n", x1);
            break;

        case TWO_SOL:
            printf(" # The equation has 2 solutions: x1 = %lg, x2 = %lg. \n", x1, x2);
            break;

        case INF_SOL:
            printf(" # The equation has an infinite number of solutions. \n");
            break;

        default:
            printf(" # print_solution(): ERROR: sol_numb = %d. \n", num_roots);
            break;
    }
}
