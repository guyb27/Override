#include <string.h>
#include <stdio.h>

char save[184];

void secret_backdoor()
{
	system(fgets((void*)NULL, 128, 0));
}

void set_msg(char *username)
{
	char c_1[1024];	// -1024
	int64_t i_1;	// -1032

	bzero(c_1, 1024);
	
	puts(">: Msg @Unix-Dude");
	printf(">>: ");

	fgets(c_1, 1024, stdin);
	
	strncpy(username, c_1, (int)&username[180]);
}

void set_username(char *username)
{	
	int i_2;		// -4
	char c_1[128];	// -144
	int64_t	i_1;	// -152

	bzero(c_1, 128);

	puts(">: Enter your username");
	printf(">>: ");
	fgets(c_1, 128, stdin);

	for (i_2 = 0; i_2 < 41 && c_1[i_2] != '\0' ; i_2 += 1) {
		username[140 + i_2] = c_1[i_2];
	}

	printf(">: Welcome, %s", &username[140]);
}

void handle_msg()
{
	int64_t i_6 = 140;
	int64_t i_5 = 0;
	int64_t i_4 = 0;
	int64_t i_3 = 0;
	int64_t i_2 = 0;
	int64_t i_1 = 0;	// -52
	char c_1[140];		// -192

	set_username(c_1);
	set_msg(c_1);
	puts(">: Msg sent!");
}

int main()
{
	puts("--------------------------------------------\n|   ~Welcome to l33t-m$n ~    v1337        |\n--------------------------------------------");
	handle_msg();
	return 0;
}
