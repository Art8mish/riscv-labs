
#include <stdio.h>

extern unsigned fact(unsigned x); 

int main() {
  unsigned f = fact(5);
  printf("%u\n", f);
  return 0;
}
