#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <unistd.h>

int main()
{
	int status;
	char str[128];
	int ret_ptrace;
	int pid;

	pid = fork();
	bzero(str, 32);
	if (!pid)	// child
	{
		prctl(1, 1);	// Pas utile dans notre cas, le fils recoit un SIGHUP si le pere meurt.
		ptrace(PT_TRACE_ME, 0, 0, 0);	// Permet de se faire "tracer" par le pere, une espece de controle parental
		puts("Give me some shellcode, k");
		gets(str);
		return (0);
	}
	else	// parent
	{
		while (ret_ptrace != 0xb)
		{
			wait(&status);
			if ((status & 127) == 0 || (((status & 127) + 1) / 2) > 0)
        		{
            			puts("child is exiting...");
         	   		return 0;
        		}
			ret_ptrace = ptrace(PTRACE_PEEKUSER, pid, 44, 0);	// Retourne 0xb si on utilise une fonction exec() dans un shellcode
		}
	}
	puts("no exec() for you");
	kill(pid, 9);
	return 0;
}
