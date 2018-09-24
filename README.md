Shelldump - A tiny utility that dumps files as escaped byte sequences

Shelldump is intended to make it easy to embed files in scripts and programs. My primary use is embedding shellcode in C programs or perl scripts for CTFs.

Files can be dumped as a single long string, or in rows.

Ex: a file `somefile` containing `0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0a`:

```
$ ./shelldump somefile
"\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a"
```

```
$ ./shelldump -vr 5 somefile

somefile:
"\x01\x02\x03\x04\x05"
"\x06\x07\x08\x09\x0a"
""
```