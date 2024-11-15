# oopd-a3

This repository contains the source files for the assignment 3A for the OOPD course at IIITD.

## Instructions for running
- Running `make` or `make all` will compile both the versions of the program, i.e `debug` meant for debugging using -g flag and `optimized` compiled using O2 flag.
- Running `make clean` will clean up the executables.
- The file `test_railway.cpp` contains the source of the user test program.
- The `lib/` folder contains the header and cpp file for the train library which is compiled into a static library by the Makefile provided.
- The file `input.txt` contains the inputs as separate lines handling all major cases such as adding of stations both int and string, adding of platform and lines associated, adding of trains. The user program combined with the library are responsible for checking sanity of inputs, preliminary checks are done by the user program itself while conflict checking is done by the library.

![images/g8.png](images/g8.png)
## References

- [ANSI Escape Sequences](https://prirai.github.io/blogs/ansi-esc/)
- [Smart Pointers](https://en.cppreference.com/book/intro/smart_pointers)
- [Weak Pointer](https://en.cppreference.com/w/cpp/memory/weak_ptr )
- [fstream](https://cplusplus.com/reference/fstream/fstream)
- [vector](https://cplusplus.com/reference/vector/vector)
