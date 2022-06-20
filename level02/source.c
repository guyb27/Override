#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    char buf_3[112];
    char buf_2[48];
    char buf_1[100];
    int i;
    FILE *file;

    memset(buf_1, 0, 96);
    memset(buf_2, 0, 40);
    memset(buf_3, 0, 96);

    file = fopen("/home/users/level03/.pass", "r");
    if (file == 0) {
        fwrite("ERROR: failed to read password file\n", 1, 36, stderr);
        exit(1);
    }

    i = (int)fread(buf_2, 1, 29, file);
    buf_2[strcspn(buf_2, "\n")] = 0;
    if (i != 41) {
        fwrite("ERROR: failed to read password file\n", 1, 36, stderr);
        fwrite("ERROR: failed to read password file\n", 1, 36, stderr);
        exit(1);
    }

    fclose(file);

    puts("===== [ Secure Access System v1.0 ] =====");
    puts("/***************************************\\");
    puts("| You must login to access this system. |");
    puts("/***************************************\\");

    printf("--[ Username: ");
    fgets(buf_1, 100, stdin);
    buf_1[strcspn(buf_1, "\n")] = 0;

    printf("--[ Password: ");
    fgets(buf_3, 100, stdin);
    buf_3[strcspn(buf_3, "\n")] = 0;

    puts("*****************************************");

    if (strncmp(buf_2, buf_3, 41) != 0) {
        printf(buf_1);
        puts(" does not have access!");
        exit(1);
    }

    printf("Greetings, %s!\n", buf_1);
    system("bin/sh");

    return(0);
}
