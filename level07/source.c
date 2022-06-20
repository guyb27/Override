#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int read_number(int arg) {
    printf(" Index: ");
    index = get_unum();

    printf(" Number at data[%u] is %u\n", index, *(index * 4 + arg));

    return (0);
}

int store_number(int arg) {
    int number;
    int index;

    printf(" Number: ");
    number = get_unum();

    printf(" Index: ");
    index = get_unum();

    if (buf_2 % 3 == 0 || buf_1 >> 24 == 183) {
        puts(" *** ERROR! ***");
        puts("   This index is reserved for wil!");
        puts(" *** ERROR! ***");
        return (1);
    }

    *(index * 4 + arg) = number;

    return (0);
}

int main(int argc, char *argv[], char *env[]) {
    int i_7;
    char **s_3;
    char **s_2;
    char s_1[406];
    int i_6;
    int i_5;
    int i_4;
    int i_3;
    int i_2;
    int i_1;
    int canary;

    s_2 = (char **)argv;
    s_3 = (char **)env;
    memset(s_1, 0, 100);

    if (s_2[0] != NULL) {
        memset(s_2[0], 0, strlen(s_2[0]));
    }


    
    return(0);
}