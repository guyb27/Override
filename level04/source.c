#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <unistd.h>

#define PTRACE_TRACEME		0
#define PTRACE_PEEKUSER		3

int main()
{
	int status;
	char str[148];
	int trace;
	int pid;

	pid = fork();
	bzero(str, 32);
	if (pid == 0) {
		prctl(1, 1);
		ptrace(PTRACE_TRACEME, 0, 0, 0);
		puts("Give me some shellcode, k");
		gets(str);
	}
	else {
		do {
			wait(&status);
			if ((status & 127) == 0 || (((status & 127) + 1) / 2) > 0) {
				puts("child is exiting...");
				return (0);
			}
			// Return 11 (0xb) if user try to execute `exec`
			trace = ptrace(PTRACE_PEEKUSER, pid, 44, 0);
		} while (trace != 11);

		puts("no exec() for you");
		kill(pid, 9);
	}

	return (0);
}
