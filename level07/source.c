#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void clear_stdin() {
    char c;

    do {
        if ((c = getchar()) == '\n')
            return;
    } while (c != -1);

    return ;
}

int get_unum() {
    int buf;

    buf = 0;
    scanf("%u", &buf);
    clear_stdin();

    return (buf);
}

int read_number(int arg) {
    int index;

    printf(" Index: ");
    index = get_unum();
    printf(" Number at data[%u] is %u\n", index, *(int *)(arg + index * 4));

    return (0);
}

int store_number(int arg) {
    int number;
    int index;

    printf(" Number: ");
    number = get_unum();

    printf(" Index: ");
    index = get_unum();

    if (index % 3 == 0 || number >> 24 == 183) {
        puts(" *** ERROR! ***");
        puts("   This index is reserved for wil!");
        puts(" *** ERROR! ***");
        return (1);
    }

    *(int *)(arg + index * 4) = number;

    return (0);
}

int main(int argc, char *argv[], char *env[]) {
    char *s;
    int i_7;
    char **s_3;
    char **s_2;
    char s_1[100];
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

    while (*s_2 != NULL) {
        memset(*s_2, 0, strlen(*s_2));
        s_2++;
    };
    while (*s_3 != NULL) {
        memset(*s_3, 0, strlen(*s_3));
        s_3++;
    };

    puts("----------------------------------------------------  \n\
Welcome to wil's crappy number storage service!                 \n\
----------------------------------------------------            \n\
 Commands:                                                      \n\
    store - store a number into the data storage                \n\
    read  - read a number from the data storage                 \n\
    quit  - exit the program                                    \n\
----------------------------------------------------            \n\
   wil has reserved some storage :>                             \n\
----------------------------------------------------            \n");
    
    do {
        printf("Input command: ");
        fgets((char *)&i_5, 20, stdin);
        s = &i_5;
        s[strlen((char *)&i_5) - 1] = 0;

        if (!strncmp((char *)&i_5, "store", 5)) {
            i_6 = store_number(s_1);
        } else if (!strncmp((char *)&i_5, "read", 4)) {
            i_6 = read_number(s_1);
        } else if (!strncmp((char *)&i_5, "quit", 4)) {
            break;
        }

        if (i_6 == 0) {
            printf(" Completed %s command successfully\n", (char *)&i_5);
        } else {
            printf(" Failed to do %s command\n", (char *)&i_5);
        }
    } while (1);

    return (0);
}