# level03


Dans ce challenge, il y a des protections sur le binaire:

RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
Partial RELRO   Canary found      NX enabled    No PIE          No RPATH   No RUNPATH   /home/users/level03/level03

Personnellement, je n ai pas eu le moindre changement avec ces protections !

Nous sommes sur un fichier 32 bits not stripped:

level03: setuid setgid ELF 32-bit LSB executable, Intel 80386, version 1 (SYSV), dynamically linked (uses shared libs), for GNU/Linux 2.6.24, BuildID[sha1]=0x9e834af52f4b2400d5bd38b3dac04d1a5faa1729, not stripped

(gdb info functions)
0x080485f4  clear_stdin
0x08048617  get_unum
0x0804864f  prog_timeout
0x08048660  decrypt
0x08048747  test

A *main+103 il y a un scanf(%d, my_input).
A *test+14 il y a le calcul 322424845-my_input.
A *test+21 le resultat de *test+14 devra etre < 0x15 (21) et > 0 afin de ne pas envoyer un nombre random dans le fonction decrypt() et de lui envoyer notre resultat.
De *decrypt+19 a *decrypt+47 inclus, la chaine "Q}|u`sfg~sf{}|a3" est creer.
A *decrypt+127 Nous sommes dans un boucle et les characteres de la chaine "Q}|u`sfg~sf{}|a3" vont etre modifier un a un selon leur index de la boucle avec le resultat du XOR a *decrypt+117
A *decrypt+141 c'est ici qu'on atteri quand les operations XOR sont finis sur la string"Q}|u`sfg~sf{}|a3". Maintenant cette string sera comparer avec la chaine "Congratulations!".
A *decrypt+177 c'est la ou il y aura la comparaison des deux strings, si elles sont identiques nous auront notre shell a *decrypt+188!


afin de convertir la chaine "Q}|u`sfg~sf{}|a3" en "Q}|u`sfg~sf{}|a3" char par char, il faut lui faire un xor de 0x12(18).

gdb-peda$ p/x 322424845-322424827
$36 = 0x12
gdb-peda$ p/d 322424845-322424827
$37 = 18

        01010001 = Q
XOR     00010010 = 18
    =   01000011 = C
Nous devons donc mettre "322424827" dans l'input

level03@OverRide:~$ (python -c 'print "322424827"';cat) | ./level03 
***********************************
*               level03         **
***********************************
whoami
level04
