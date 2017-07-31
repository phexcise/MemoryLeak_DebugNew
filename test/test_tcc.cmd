@echo off
tcc -I../ -D_DEBUG test.c ../debug_new.c
test.exe
rem del test.exe
del debug_new.log
