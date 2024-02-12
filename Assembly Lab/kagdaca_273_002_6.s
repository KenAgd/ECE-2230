/*
	kendric agdaca
	kagdaca
	ECE 2730
	4/5/21

	Purpose: calculates the factorial of the user input
*/

.globl Factorial
.type Factorial, @function
Factorial:
/* prolog */
	pushl %ebx
	pushl %ebp
	movl %esp, %ebx


if1:
	movl 12(%ebp), %eax #moves user input "n" into A reg
	cmpl $0, %eax
	jne if2 	    #n == 0? If true, return 1. If false, move to second if
	movl $1, %eax
	jmp return	


if2:
	cmpl $1, %eax
	jne else	    #n == 1? If true, return 1. If false, move to else
	jmp return


else:
	subl $1, %eax
	pushl %eax
	call Factorial      #recursive call Factorial(n-1)

	movl 12(%ebp), %ebx
	mull %ebx           #n * Factorial(n-1)

	

return:
	movl %ebp, %esp
	popl %ebp
	popl %ebx
	ret

/* end assembly stub */
