del *.exe
del *.o
gcc kuharica.c -o kuharica.o -c
gcc funkcije.c -o funkcije.o -c
gcc -o program.exe kuharica.o funkcije.o