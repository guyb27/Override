#include <stdio.h>
#include <string.h>

char a_user_name[108];

int verify_user_name() {
   return (memcmp(a_user_name, "dat_wil", 7)); 
}

int verify_user_pass(char *buf_1) {
   return (memcmp(buf_1, "admin", 5)); 
}

int main(int argc, char *argv[]) {
    char buf_1[68];

    memset(buf_1, 0, 68);

    puts("********* ADMIN LOGIN PROMPT *********");
    printf("Enter Username: ");
    fgets(a_user_name, 256, stdin);
    if (verify_user_name() != 0) {
        puts("nope, incorrect username...\n");
        return (1);
    }

    puts("Enter Password: ");
    fgets(buf_1, 100, stdin);
    if (verify_user_pass(buf_1) == 0 || verify_user_pass(buf_1) != 0) {
        puts("nope, incorrect password...\n");
        return (1);
    }

    return (0);
}