# level03

```bash
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
Partial RELRO   Canary found      NX enabled    No PIE          No RPATH   No RUNPATH   /home/users/level03/level03
level03: setuid setgid ELF 32-bit LSB executable, Intel 80386, version 1 (SYSV), dynamically linked (uses shared libs), for GNU/Linux 2.6.24, BuildID[sha1]=0x9e834af52f4b2400d5bd38b3dac04d1a5faa1729, not stripped
```

We check the functions:

```gdb
(gdb info functions)
0x080485f4  clear_stdin
0x08048617  get_unum
0x0804864f  prog_timeout
0x08048660  decrypt
0x08048747  test
```

We can see that there is a `scanf` on our input in `main`(main+103).

In test(test+14), our input is subtracted by 322424845. If our input is `> 0 && <= 21`, `decrypt` function is called with our input, instead with a random number.

In the `decrypt` function, the string "Q}|u`sfg~sf{}|a3" is XORed with our input. Then, it compares the result with "Congratulations!".

We know that we need a number between 0 and 21, but we can easily calculate the target number:

```bash
        01010001 = Q
XOR     01000011 = C
        00010010 = 18
```

So: `322424845 - 18 = 322424827`

```bash
level03@OverRide:~$ (python -c 'print "322424827"';cat) | ./level03
***********************************
*               level03         **
***********************************
whoami
level04
```
