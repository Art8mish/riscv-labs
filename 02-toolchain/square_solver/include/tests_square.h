#ifndef TESTS_SQUARE_H_INCLUDED
#define TESTS_SQUARE_H_INCLUDED

//------------------------------------------------------
//! Tests SquareSolver
//!
//! @note Include tests reading from the file
//------------------------------------------------------

void test_square(void);


typedef enum Test_Result
{
    TEST_SUCCESS = 0,
    TEST_FAILED  = 1
} Test_Result;

Test_Result once_test_square(double a, double b, double c,
                             double wanted_x1, double wanted_x2,
                             int wanted_num_roots, int test_num);

#endif // TESTS_SQUARE_H_INCLUDED
