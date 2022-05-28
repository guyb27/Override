void decrypt(int res)
{
	char encrypted="Q}|u`sfg~sf{}|a3";

	for (int i = 0; encrypted[i]; i++)
		encrypted[i] ^= res;
	if (strcmp("Congratulations!", encrypted))
		system("/bin/sh");
}

void 	test(int my_input, int v2)
{
	int res = my_input-v2;
	if ( res  > 0 && res <= 21)
		return decrypt(res);
	return decrypt(rand());	
}

int main()
{
	int my_input;

	srand(time(0));
	puts('*' <repeats 35 times>);
	puts("*\t\tlevel03\t\t**");
	puts('*' <repeats 35 times>);
	printf("Password:");
	scanf(%d, &my_input);
	test(my_input, 22424845);
}
