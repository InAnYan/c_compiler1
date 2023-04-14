./ccomp test.c > test.s
gcc -g -m32 -nostdlib test.s
