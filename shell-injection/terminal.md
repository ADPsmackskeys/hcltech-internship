```bash
$ echo 0 | sudo tee /proc/sys/kernel/randomize_va_space # Disabling ASLR
$ gcc roots.c -o roots -fno-stack-protector -no-pie -z execstack
$ python3 exploit.py
```
