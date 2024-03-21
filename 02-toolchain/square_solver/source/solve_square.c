
#include "../include/common_library.h"
#include "../include/solve_square.h"
#include <math.h>

int is_zero(double value)
{
    const double EPS = 1e-7;
    return (fabs(value) < EPS); // comparison of absolute value with small constant
}


double removal_minus_zero(double x)
{
    double answer = (is_zero(fabs(x))) ? 0 : x;

    return answer;
}

Num_Roots solve_linear(double b, double c, double *x)
{
    SOFT_ASSERT(isfinite(b));
    SOFT_ASSERT(isfinite(c));

    SOFT_ASSERT(x != NULL);

    if (is_zero(b))
    {
        // 0 * x + 0 = 0
        if (is_zero(c))
            return INF_SOL;

        // 0 * x + const = 0
        if (!(is_zero(c)))
            return NO_SOL;
    }

    // const * x + c = 0
    if (!(is_zero(b)))
    {
        *x = (-c) / b;

        return ONE_SOL;
    }

    return NO_SOL;
}

Num_Roots solve_square(double a, double b, double c, double *x1, double *x2)
{
    SOFT_ASSERT(isfinite(a));
    SOFT_ASSERT(isfinite(b));
    SOFT_ASSERT(isfinite(c));

    SOFT_ASSERT(x1 != NULL);
    SOFT_ASSERT(x2 != NULL);
    SOFT_ASSERT(x1 != x2);

    double discriminant = b*b - 4*a*c;

    // 0 * x^2 + b * x + c = 0
    if (is_zero(a))
        return solve_linear(b, c, x1);

    if (is_zero(c))
    {
        *x1 = 0;
        solve_linear(a, b, x2);

        return TWO_SOL;
    }

    // D < 0; no solutions
    if (discriminant < 0)
        return NO_SOL;

    // D = 0; 1 solution
    if (is_zero(discriminant))
    {
        *x1 = (-b) / (2*a);

        return ONE_SOL;
    }

    // D > 0; 2 solutions
    if (discriminant > 0)
    {
        double sqrt_discriminant = sqrt(discriminant);

        *x1 = ((-b) - sqrt_discriminant) / (2*a);
        *x2 = ((-b) + sqrt_discriminant) / (2*a);

        return TWO_SOL;
    }

    return NO_SOL;
}
