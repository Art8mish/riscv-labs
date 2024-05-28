#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern void *memcpy (void *destination, const void *source, size_t n); 

int main() {
    char source[] = "Hello, world!";
    char destination[20];

    memcpy(destination, source, strlen(source) + 1);

    printf("Copied string: %s\n", destination);

    return 0;
}