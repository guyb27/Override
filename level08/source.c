#include <string.h>
#include <stdio.h>

void log_wrapper(FILE *file, char *str, char *argv)
{
	char str2[304];//0x130
	char *str_cpy;
	
	strcpy(&str_cpy, str);
	int res = strlen(argv);
	int calc = 254 - res - 1;//0xfe
	res = strlen(str2);
	snprintf(str2 + res, calc, argv);
	str[strcspn(str2, "\n")] = 0x0;
	fprintf(file, "LOG: %s\n", str2);
}

int main(int argc, char **argv)
{
	int fd;
	int letter;
	FILE *file1;
	FILE *file2;
	char str[165];//0xb0
	char letter;
	char str2[8];
	int64_t canari;

	if (argc != 2) {
		printf("Usage: %s filename\n", *argv);
	}

	if (file1 = fopen("./backups/.log", 'w') == 0) {
		printf("ERROR: Failed to open %s\n", "./backups/.log");
		exit(1);
	}

	log_wrapper(file1, "Starting back up: ", argv[1]);
	
	if (file2 = fopen(argv[1], 'r') == 0x0) {
		printf("ERROR: Failed to open %s\n", argv[1]);
		exit(1);
	}

	// create file if can't find
	if ((fd = open(strncat("./backups/", argv[1], strlen(argv[1])), 193, 432)) == -1) {
		printf("ERROR: Failed to open %s%s\n", "./backups/", argv[1]);
		exit(1);
	}

	do {
		letter = fgetc(file2);
		write(fd, &letter, 1);
	} while (letter != -1);

	log_wrapper(file1, "Finished back up ", argv[1]);

	fclose(file2);
	close(fd);

	return (0);
}
