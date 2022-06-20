#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main() {
	int i;
	char buf_1[112];

	fgets(buf_1, 100, stdin);
	i = 0;
	while (1) {
		if (i >= strlen(buf_1)) {
			break;
		}

		if (buf_1[i] >= 'A' && buf_1[i <= 'Z']) {
			buf_1[i] ^= 32;
		}
	}

	printf(buf_1);
	exit(0);
}
