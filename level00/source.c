#include <stdio.h>
#include <stdlib.h>

int main() {
    int n;

    puts("***********************************");
    puts("* \t     -Level00 -\t\t  *");
    puts("***********************************");

    printf("Password:");
    scanf("%d", &n);

    if (n != 5276) {
        puts("\nInvalid Password!");
    } else {
        puts("\nAuthenticated!");
        system("/bin/sh");
    }

    return (0);
}