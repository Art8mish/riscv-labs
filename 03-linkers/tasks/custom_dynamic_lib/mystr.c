#include <stdio.h>
#include <stdlib.h>

int own_strlen(char *str);

char* own_strcpy(char *destination, char *source);

char* own_strchr(char *str, int symbol);

int own_strcmp(const char *str1, const char *str2);

char* own_strcat (char *destination, const char *append);


int own_strlen(char *str)
{
    int index = 0;
    char ch = ' ';
    int symbol_count = 0;

    while ((ch = str[index++]) != '\0')
        symbol_count++;

    return symbol_count;
}

char* own_strcpy(char *destination, char *source)
{
    int index = 0;

    while ((destination[index] = source[index]) != '\0')
        index++;

    return destination;
}

char* own_strchr(char *str, int symbol)
{
    int index = 0;
    int ch = 0;

    while (ch != symbol)
        ch = str[index++];

    if (ch == '\0')
        return 0;

    return (str + index - 1);
}

int own_strcmp(const char *str1, const char *str2)
{
    int index = 0;
    int ch1 = str1[index];
    int ch2 = str2[index];

    while (ch1 == ch2
           && ch2 != '\0'
           && ch1 != '\0')
    {
        ++index;

        ch1 = str1[index];
        ch2 = str2[index];
    }

    return (ch1 - ch2);
}

char *own_strcat (char *destination, const char *append)
{
    int index1 = 0;
    int index2 = 0;
    int ch = destination[index1];

    while ((ch = destination[index1]) != '\0')
        index1++;

    while ((destination[index1++] = append[index2++]) != '\0');

    return destination;

}