# level07

We can see that the function `store_number` store the number at address `$index * 4 + 4294956308`.

L'addresse visé par le shellcode:

x = x / 4 - 4294956308

x   = (0804a00c - 4294956308) / 4
    = (134520844 - 4294956308) / 4
    = 134531832 / 4
    = 33632958

    = (134520844 - 4294956324) / 4
    = 134531816 / 4
    = 33632954

    = (134520844 - 4294958228) / 4
    = 33632478
    
0xffffdc94