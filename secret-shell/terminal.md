```bash
# Case 1
$ gcc ab.c -o vuln -g -w -fno-stack-protector -no-pie
$ python3 exploit.py # Spawns a normal shell.

# Case 2
$ sudo chown root:root vuln # Changing ownership of file to root
$ sudo chmod u+s vuln # Setuid is now allowed
$ python3 exploit.py # Spawns a root shell.
```
