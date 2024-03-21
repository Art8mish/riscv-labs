#ifndef IO_H_INCLUDED
#define IO_H_INCLUDED

#include "solve_square.h"

//------------------------------------------------------------
//! Read coefficients for a square equation ax2 + bx + c = 0
//!
//! @param [out] a Pointer to the a-coefficient
//! @param [out] b Pointer to the b-coefficient
//! @param [out] c Pointer to the c-coefficient
//!
//! @note Program doesn't let to put incorrect coefficient
//------------------------------------------------------------

void guarded_input(double *a, double *b, double *c);


//------------------------------------------------------------------
//! Prints solution of a square equation ax2 + bx + c = 0
//!
//! @param [in] num_roots Root of solution
//! @param [in] x1 1st solution
//! @param [in] x2 2nd solution if exist
//!
//! @note Function print_solutions takes only (-1 <= num_roots <= 2)
//------------------------------------------------------------------

void print_solution(Num_Roots num_roots, double x1, double x2);

void greeting(void);

void clean_buffer(void);

#endif // IO_H_INCLUDED
