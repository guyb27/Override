int main()
{
	//0xffffd0f0->0xffffd060
	char s[24];
	char str[120];

	fgets(str, 100, 0);
	for (int i = 0;i < strlen(str);i++)
	{
		for (int j = 0;j < strlen(str);j++)
		{
			if (str[j] >= 'A' && str[j] <= 'Z')//>= 0x41 && <= 0x5a
				str[j] -= 32;
		}
	}
	printf(str);
	exit(0);
}
