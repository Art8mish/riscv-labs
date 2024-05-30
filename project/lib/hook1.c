
#include <stdlib.h>

void *memcpy (void *destination, const void *source, size_t n)
{
    for (size_t i=0; i < n - 1; i++)
      *((char *)destination + i) = *((const char *)source + (n - 2) - i);

    *((char *)destination + n - 1) = '\0';
    return destination;
}