# Two classic C bugs and how to fix them

Small, reproducible examples of the bugs I find most often when debugging C projects.

## 1. Data race (lost updates)

Four threads each add 1,000,000 to a shared counter. `counter++` is a
read-modify-write, so updates from different threads overwrite each other.

```
$ ./race && ./race && ./race_fix
expected 4000000, got 1551971  <- lost updates
expected 4000000, got 1480624  <- lost updates
expected 4000000, got 4000000  (correct)
```

The fix (`-DFIXED`) puts a mutex around the update. An atomic increment would also work.

## 2. Memory leak, found with valgrind

`leak.c` builds a linked list and forgets to free it.

```
$ valgrind --leak-check=full ./leak
==268== Memcheck, a memory error detector
==268== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.
==268== Using Valgrind-3.22.0 and LibVEX; rerun with -h for copyright info
==268== Command: ./leak
==268== 
stored 5 syscalls, last added: dup2
==268== 
==268== HEAP SUMMARY:
==268==     in use at exit: 105 bytes in 10 blocks
==268==   total heap usage: 11 allocs, 1 frees, 4,201 bytes allocated
==268== 
==268== 105 (16 direct, 89 indirect) bytes in 1 blocks are definitely lost in loss record 3 of 3
==268==    at 0x4846828: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==268==    by 0x1091C6: push (leak.c:13)
==268==    by 0x10926E: main (leak.c:23)
==268== 
==268== LEAK SUMMARY:
==268==    definitely lost: 16 bytes in 1 blocks
==268==    indirectly lost: 89 bytes in 9 blocks
==268==      possibly lost: 0 bytes in 0 blocks
==268==    still reachable: 0 bytes in 0 blocks
==268==         suppressed: 0 bytes in 0 blocks
==268== 
==268== For lists of detected and suppressed errors, rerun with: -s
==268== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
```

After freeing each node and its string (`-DFIXED`):

```
$ valgrind --leak-check=full ./leak_fix
==274== Memcheck, a memory error detector
==274== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.
==274== Using Valgrind-3.22.0 and LibVEX; rerun with -h for copyright info
==274== Command: ./leak_fix
==274== 
stored 5 syscalls, last added: dup2
==274== 
==274== HEAP SUMMARY:
==274==     in use at exit: 0 bytes in 0 blocks
==274==   total heap usage: 11 allocs, 11 frees, 4,201 bytes allocated
==274== 
==274== All heap blocks were freed -- no leaks are possible
==274== 
==274== For lists of detected and suppressed errors, rerun with: -s
==274== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

## Build

```sh
make            # builds race, race_fix, leak, leak_fix
```

## About

Written by Ahsan (Code Projects on YouTube).
I build and debug operating-systems, graphics and desktop projects. Issues and pull requests are welcome.

Licensed under the MIT License.
