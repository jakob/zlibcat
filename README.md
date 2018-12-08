zlibcat
=======


zlibcat is a utility that reads from standard input, performs zlib compression, and writes to standard output.

unzlibcat uncompresses data.

Example usage:

```bash
$ echo "This is a repetitive string. This is a repetitive string. This is a repetitive string." >test.txt
$ ./zlibcat <test.txt >test.txt.zz
$ ls -lh test.txt test.txt.zz 
-rw-r--r--  1 jakob  staff    87B Dec  8 13:29 test.txt
-rw-r--r--  1 jakob  staff    39B Dec  8 13:29 test.txt.zz
$ ./unzlibcat <test.txt.zz 
This is a repetitive string. This is a repetitive string. This is a repetitive string.
```

This is mostly useful for debugging when you work with zlib.