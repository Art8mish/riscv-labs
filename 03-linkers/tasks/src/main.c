
#include <stdio.h>

extern int own_strlen(char *str);
extern char* own_strcpy(char *destination, char *source);
extern char* own_strcat (char *destination, const char *append);

int main() {
  char str1[14] = "Hallo, ";
  char *str2 = "welt!";


  own_strcat(str1, str2);
  printf("%s (strlen - %d)\n", str1, own_strlen(str1));

  return 0;
}
