void secret_backdoor()
{
	system(fgets((void*)NULL, 128, 0));
}

set_msg(char *username)
{
	char str[1040];

	bzero(str, 128);
	puts(">: Msg @Unix-Dude");
	printf(">>: ");
	fgets(str, 1024, stdin);
	strncpy(username+140, str, username+180);
}

set_username(char *username)
{
	char str[160];//0x7fffffffe4f0=>0x7fffffffe450

	memset(str+16, 64);
	puts(">: Enter your username");
	printf(">>: ");
	//rdi == 0x7fffffffe460
	fgets(str+16, 128, stdin);
	str+140+16 = 0x0;
	for (int i = 0;i<41 && str[i];i++) {
		username[140+i] = str[i + 16];
		if (str+16+140+i == 40)
			break ;
	}
	printf(">: Welcome, %s", unknow_var);
}

void handle_msg()
{
	char str[192];

	bzero(str+140, 40);
	str+180 = 140;
	set_username(str);
	set_msg(str);
	puts(">: Msg sent!");
}

int main()
{
	puts("--------------------------------------------\n|   ~Welcome to l33t-m$n ~    v1337        |\n--------------------------------------------");
	handle_msg();
	return 0;
}
