#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void decrypt(int res) {
	char encrypted[16] = "Q}|u`sfg~sf{}|a3";

	for (int i = 0; encrypted[i]; i++)
		encrypted[i] ^= res;
	if (strcmp("Congratulations!", encrypted))
		system("/bin/sh");
}

void	test(int buf_1, int n) {
	int res = n - buf_1;

	if (res > 0 && res <= 21) {
		decrypt(res);
	} else {
		decrypt(rand());
	}

	return;	
}

int main(void) {
	int buf_1;

	srand(time(0));

	puts("***********************************");
	puts("*               level03         **");
	puts("***********************************");

	printf("Password:");
	scanf("%d", &buf_1);
	test(buf_1, 322424845);

	return(0);
}
