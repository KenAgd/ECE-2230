/*
	Kendric Agdaca
	ECE 273_002
	3/4/21
	kagdaca_273_002_4.s
	
	Purpose: converts the char user input into an integer
*/



.globl AtoI
.type AtoI,@function
AtoI:
	pushl %ebp
	movl %esp, %ebp
	pushl %ebx
	pushl %esi
	pushl %edi

	
	movl $1, sign


asciiCheck1: #while loop first part
	movl ascii, %ebx
	cmpb $32, (%ebx) #checks if ascii == ' '
	jne asciiCheck2 #if ascii != ' ', jump to the or statement within while loop
	cmpb $9, (%ebx)
	jne asciiCheck2
	incl ascii
	jmp asciiCheck1
	


asciiCheck2: #first if statement
	cmpb $43, (%ebx) #ascii == '+'?
	jne asciiCheck3 #ascii != '+', jump to else if
	incl ascii


asciiCheck3: #else if statment
	cmpb $45, (%ebx) #ascii == '-'?
	jne afterAsciiCheck2 #ascii!= '-', jump to first for loop
	movl $-1, sign
	incl ascii


afterAsciiCheck2: #setup for the for loops
	movl intptr, %ecx
	movl $0 , (%ecx) #initialize *intptr
	movl $0, i
	movl i, %edi #initialize counter variable i


for1: #increments thru the ascii array
	movl ascii, %edx
	cmpb $48, (%edx, %edi, 1)
	jl for1Code
	cmpb $57, (%edx, %edi, 1)
	jg for1Code

for1Cont:
	addl $1, %edi
	jmp for1

for1Code:
	subl $1, %edi
	movl $1, multiplier

for2: 
	cmpl $0 , %edi
	jl for2Code
	movl $0, %eax
	movl ascii, %edx
	movb (%edx, %edi, 1), %al
	subl $48, %eax
	mull multiplier
	addl %eax, (%ecx)
	movl $10, %eax
	mull multiplier
	movl %eax, multiplier
	subl $1, %edi
	jmp for2


for2Code:
	movl intptr, %ecx
	movl sign, %eax
	mull (%ecx)
	movl %eax, (%ecx)
	

return:
	popl %edi
	popl %esi
	popl %ebx
	movl %ebp, %esp
	popl %ebp
	ret

/* end assembly stub */
