@echo off
g++ -I../ -D_DEBUG test.c ../debug_new.c
a.exe
del a.exe
del debug_new.log
