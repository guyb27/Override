# level00

The program asks for a password:

```bash
***********************************
*            -Level00 -           *
***********************************
Password:
dsf

Invalid Password!
```

By looking at the code, we can see a call to `scanf` and after a comparison with 5276.

We simply put 5276 as password:

```bash
level00@OverRide:~$ ./level00 
***********************************
*            -Level00 -           *
***********************************
Password:5276

Authenticated!
$ whoami
level01
$ cd /home/users/level01
$ cat .pass
uSq2ehEGT6c9S24zbshexZQBXUGrncxn5sD5QfGL
```
