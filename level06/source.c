#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ptrace.h>

int auth(char *buf_2, unsigned int buf_3) {
    unsigned int xored;
    int size;

    buf_2[strcspn(buf_2, "\n")] = 0x0;
    size = strnlen(buf_2, 32);

    if (size <= 5) {
        return (1);
    }

    if (ptrace(0, 0, (caddr_t)1, 0) == -1) {
        puts("\033[32m.---------------------------.");
        puts("\033[31m| !! TAMPERING DETECTED !!  |");
        puts("\033[31m'---------------------------'");
     
        return(1);
    }

    xored = buf_2[3] ^ 4919;
    xored += 6221293;

    for (int i = 0; i < size; i += 1) {
        if (buf_2[i] <= 31) {
            return(1);
        }

        xored += ((int32_t)buf_2[i] ^ xored) % 0x539;
    }

    if (buf_3 != xored) {
        return (1);
    }

    return(0);
}

int main() {
    unsigned int buf_3;
    char buf_2[32];
    int canary;

    puts("***********************************");
    puts("*               level06           *");
    puts("***********************************");

    printf("-> Enter Login: ");
    fgets(buf_2, 32, stdin);

    puts("**********************************");
    puts("***** NEW ACCOUNT DETECTED ********");
    puts("**********************************");
    
    printf("-> Enter Serial: ");
    scanf("%u", &buf_3, stdin);

    if (auth(buf_2, buf_3)) {
        return(1);
    }

    puts("Authenticated!");
    system("/bin/sh");
    
    return(0);
}
