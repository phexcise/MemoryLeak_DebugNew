#!/bin/sh
gcc -pthread -I../ -D_DEBUG test.c ../debug_new.c -o ./a.out
./a.out
rm ./a.out
rm ./debug_new.log
