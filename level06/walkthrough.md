# level06

By looking at the code, we can see the use of canary, checking if the stack have been overrided:

```gdb
(gdb) disas main
Dump of assembler code for function main:
...
   0x08048889 <+16>:    mov    eax,gs:0x14
...
   0x08048972 <+249>:   xor    edx,DWORD PTR gs:0x14
   0x08048979 <+256>:   je     0x8048980 <main+263>
   0x0804897b <+258>:   call   0x8048580 <__stack_chk_fail@plt>
```

We can also see that the program call `ptrace` in the `auth` function, checking if we are actually debugging it:

```gdb
(gdb) disas auth
Dump of assembler code for function auth:
...
   0x080487b5 <+109>:   call   0x80485f0 <ptrace@plt>
...
```

In the `main` function, we can see that if `auth` return 0, it opens a shell:

```gdb
(gdb) disas auth
Dump of assembler code for function auth:
...
   0x08048941 <+200>:   call   0x8048748 <auth>
   0x08048946 <+205>:   test   eax,eax
...
```

The program takes to input of 32 bytes length, and perform a complex operation with XOR and SHIFTS on the first argument in the `auth` function.

At the end, we can see that it checks the result of the operation with the second argument passed to the program:

```gdb
   0x08048863 <+283>:   mov    eax,DWORD PTR [ebp+0xc]
   0x08048866 <+286>:   cmp    eax,DWORD PTR [ebp-0x10]
```

We try to debug by bypassing the `ptrace` to see what is the result of the operation.

We fill the first buffer with 32 bytes and fill the second with a dummy string :

```gdb
(gdb) b *auth+109
Breakpoint 1 at 0x80487b5
(gdb) b *auth+286
Breakpoint 2 at 0x8048866
(gdb) r <<< $(python -c "print 'A' * 31 + '\n'";python -c "print 'serial' + '\n'")
Starting program: /home/users/level06/level06 <<< $(python -c "print 'A' * 31 + '\n'";python -c "print 'serial' + '\n'")
***********************************
*               level06           *
***********************************
-> Enter Login: ***********************************
***** NEW ACCOUNT DETECTED ********
***********************************

Breakpoint 1, 0x080487b5 in auth ()
```

At this point, we can jump the `ptrace` and see the xored variable corresponding to our buffer:

```gdb
(gdb) jump *auth+165
Continuing at 0x80487ed.

Breakpoint 2, 0x08048866 in auth ()
(gdb) x/wx $ebp-0x10
0xffffd668:     0x005f592d
(gdb) p/d 0x005f592d
$1 = 6248749
```

We can now pass this number as our second argument:

```bash
level06@OverRide:~$ (python -c "print 'A' * 31 + '\n'";python -c "print '6248749' + '\n'";cat) | ./level06
***********************************
*               level06           *
***********************************
-> Enter Login: ***********************************
***** NEW ACCOUNT DETECTED ********
***********************************
-> Enter Serial: Authenticated!
whoami
level07
cd /home/users/level07
cat .pass
GbcPDRgsFK77LNnnuh7QyFYA2942Gp8yKj9KrWD8
```
