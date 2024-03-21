#ifndef COMMON_LIBRARY_H_INCLUDED
#define COMMON_LIBRARY_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>


//#define SOFT_ASSERT

#ifndef SOFT_ASSERT

#define SOFT_ASSERT(condition)                                            \
            do                                                            \
            {                                                             \
                if (!(condition))                                         \
                    printf("Error in %s; file: %s; num of line: %d \n",   \
                           #condition, __FILE__, __LINE__);               \
            } while(false)

#else

#define SOFT_ASSERT(condition)

#endif

#endif // COMMON_LIBRARY_H_INCLUDED
