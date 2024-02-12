/*
Kendric Agdaca
ECE 273_002
3/22/21
kagdaca_273_002_05.s

PURPOSE: serves as an auxiliary program to lab5dvr.c that calculates the Fibonacci sequence of length n+1, given user input n
*/


/* begin assembly stub */
.globl Fib
.type Fib,@function
Fib:

/* prolog */
	pushl %ebp
	pushl %ebx
	movl %esp, %ebp
	subl $8, %esp


	movl global_var, %ecx
	movl %ecx, -4(%ebp) 


//if
	cmpl $0, -4(%ebp) #local_var == 0?
	je return  #if true, jump to return


//else if
	cmpl $1, -4(%ebp)  #local_var == 1?
	je return #if true, jump to return


//else
	//global_var = local_var - 1
	movl -4(%ebp), %eax
	subl $1, %ecx
	movl %ecx, global_var
	call Fib
	

	//temp_var = global_var
	movl global_var, %ebx
	movl %ebx, -8(%ebp)     
	movl -4(%ebp), %ebx


elseP2:
	//global_var = local_var - 2
	subl $2, %ebx
	movl %ebx, global_var
	call Fib


	
	movl global_var, %ecx
	addl -8(%ebp), %ecx  #temp_var = temp_var + global_var
	movl %ecx, global_var  #global_var = temp_var


return:
	/* epilog */
	movl %ebp, %esp
	popl %ebx
	popl %ebp
	ret

/* end assembly stub */

