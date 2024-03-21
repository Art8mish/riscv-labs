#ifndef SOLVE_SQUARE_H_INCLUDED
#define SOLVE_SQUARE_H_INCLUDED

typedef enum Num_Roots
{
    NO_SOL  =  0,
    ONE_SOL =  1,
    TWO_SOL =  2,
    INF_SOL = -1
}Num_Roots;

//---------------------------------------------------------
//! Solve a square equation ax2 + bx + c = 0
//!
//! @param [in] a a-coefficient
//! @param [in] b b-coefficient
//! @param [in] c c-coefficient
//!
//! @param [out] x1 Pointer to the 1st root
//! @param [out] x2 Pointer to the 2nd root
//!
//! @return Quantity of solution
//!
//! @note Return (-1) if infinity quantity of solutions
//---------------------------------------------------------

Num_Roots solve_square(double a, double b, double c, double *x1, double *x2);

Num_Roots solve_linear(double b, double c, double *x);


//--------------------------------------------------
//! Compares double with 0
//!
//! @param [in] value Double argument
//!
//! @return True if (value == 0) and False if not
//!
//! @note If (value is very close to 0) True
//--------------------------------------------------

int is_zero(double value);


//--------------------------------------------------
//! Return 0 if argument is -0
//!
//! @param [in] x Double argument
//!
//! @return 0 if argument is -0 and argument if not
//!
//! @note If (value is very close to 0) True
//--------------------------------------------------

double removal_minus_zero(double x);

#endif // SOLVE_SQUARE_H_INCLUDED
