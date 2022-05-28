int main()
{
	int status;
	char str[128];
	int ret_ptrace;
	int ret_fork;

	ret_fork = fork();
	bzero(str, 32);
	if (!ret_fork)//child
	{
		prctl(1, 1);//Pas utile dans notre cas, le fils recoit un SIGHUP si le pere meurt.
		ptrace(PTRACE_TRACEME, 0, 0, 0);Permet de se faire "tracer" par le pere, une espece de controle parental
		puts("Give me some shellcode, k");
		gets(str);
		return (0);
	}
	else//parent
	{
		while (ret_ptrace != 0xb)
		{
			wait(&status);
			if ((status & 127) == 0 || (((status & 127) + 1) / 2) > 0)
        		{
            			puts("child is exiting...");
         	   		return 0;
        		}
			ret_ptrace = ptrace(PTRACE_PEEKUSER, ret_fork, 44, 0);//Retourn 0xb si on utilise une fonction exec() dans un shellcode
		}
	}
	puts("no exec() for you");
	kill(ret_fork, 9);
	return 0;
}
