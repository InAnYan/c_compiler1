# Compiled using InAnYan first C compiler v0.1.

.globl _start
_start:
    call main
    movl %eax, %ebx
    movl $1, %eax
    int $0x80

.globl main
main:
    pushl %ebx
    movl %esp, %ebp
    
    movl $0, %eax
    pushl %eax
    movl $8, %eax
    movl %eax, %ebx
    movl -4(%ebp), %eax
    addl %ebx, %eax
    movl %eax, -4(%ebp)
    movl $2, %eax
    movl %eax, %ebx
    movl -4(%ebp), %eax
    imul %ebx
    movl %eax, -4(%ebp)
    movl -4(%ebp), %eax
    movl -4(%ebp), %eax
    incl %eax
    movl %eax, -4(%ebp)
    
    movl %ebp, %esp
    popl %ebx
    ret
    
    ret

# End of compiling.
