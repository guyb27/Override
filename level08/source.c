void log_wrapper(FILE *file, char *str, char *argv)
{
	char str2[304];//0x130
	
	strcpy(str2, str);
	int res = strlen(av[1]);
	int calc = 254 - res - 1;//0xfe
	res = strlen(str2);
	snprintf(str2 + res, calc, argv);
	str[strcspn(str2, "\n")] = 0x0;
	fprintf(file, "LOG: %s\n", str2);
}

int main()
{
	char str[165];//0xb0
	char letter;
	void *canari;
	char str2[8];

	if (argc != 2)
	{
		printf("Usage: %s filename\n",
			"/home/kali/asm/Override/level08/level08");
		exit(1);
	}
	if (FILE *file1 = fopen("./backups/.log", 'w') == 0x0)
	{
		printf("ERROR: Failed to open %s\n", "./backups/.log");
		exit(1);
	}
	log_wrapper(file, "Starting back up: ", argv[1]);
	
	if (FILE *file2 = fopen(argv[1], 'r') == 0x0)
	{
		printf("ERROR: Failed to open %s\n", av[1]);
		exit(1);
	}
	strlen("./backups/");
	str = strncat("./backups/", av[1], 0x59);
	if (int fd = open(str, 0xc1, 0x1b0) == -1)//creer le fichier si il n existe pas, sinon fail
	{
		printf("ERROR: Failed to open %s%s\n",
			"./backups/",
			"/home/kali/asm/Override/level08/fake_flag");
		exit(0x4a);
	}
	letter = 0x0;
	while (letter != 0xff)
	{
		write(fd, &letter, 1);
		int letter = fgetc(file2);
	}
	log_wrapper(file1, "Finished back up ", av[1]);
	fclose(file2);
	close(fd);
}
