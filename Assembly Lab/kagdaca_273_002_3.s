/* begin assembly stub 
	Kendric Agdaca
	ECE 2730_002
	Lab 3
	2/17/21

	Description: Takes the 3 inputs from the main driver and compares them to each other. It then tries to see ifthe inputted triangle is an equilateral or isoceles triangle or not a triangle at all

*/



.globl classify
.type classify,@function
classify:
	/*prolog*/
	pushl %ebp
	pushl %ebx
	movl %esp, %ebp

	#count matching sides.
	movl $0, match
	movl i, %eax
	cmpl j, %eax #if i != j, jump to match2
	jne match2
	addl $1, match #if i == j, match++


#match2, 3, and count matching sides handle the three if statements comparing i, j, and k
match2:
	cmpl k, %eax #if i != k, jump to match3
	jne match3
	addl $2, match #if i==k, match++


match3: 
	movl j, %eax
	cmpl k, %eax #if j != k, start tri_search1
	jne tri_search1
	addl $3, match #if j == k, match++


#starts if(match)
tri_search1:
	movl match, %eax
	cmpl $0, %eax 
	je tri_det2  #if match = 0, jump straight to "check to see if this is a triangle" label

	cmp $1, %eax
	jne tri_search2 #if match != 1, jump to equilateral triangle label (tri_search2)

	movl i, %ebx
	addl j, %ebx
	cmpl k, %ebx  #compare i + j & k
	jg tri_det1  #if i + j > k, tri is an isoceles
	movl $0, tri_type
	jmp return


#Checks if tri is equilateral
tri_search2:
	movl match, %eax
	cmp $2, %eax #Checks if match = 2
	je tri_search3 #If match != 2, jump to tri_search 3, which double checks input param
	
	cmpl $6, %eax #compare match = 6
	jne tri_search4 #if match != 6, jump to tri_search4, which double checks input param
	movl $1, tri_type
	jmp return


#Double checks input param. If no tri is detected, jmp to the end of if(match)
tri_search3:
	movl i, %eax
	addl k, %eax #i+k
	cmp j, %eax 
	jg tri_det1 #if i + k > j, jump to end of if(match)
	movl $0, tri_type
	jmp return


#Simlar to tri_serach3. If no tri is detected, jump to end of if(match)
tri_search4:
	movl j, %eax
	addl k, %eax #j + k
	cmpl i, %eax
	jg tri_det1 #if j + k > i, jump to end of if(match)
	movl $0, tri_type
	jmp return

#Isoceles return
tri_det1:
	movl $2, tri_type
	jmp return


#Double checks if the inputted lengths can form tri
tri_det2:
	movl i, %eax
	addl j, %eax #i + j
	cmpl k, %eax 
	jle not_tri #if i + j <= k, jump to not_tri function

	movl j, %eax
	addl k, %eax #j + k
	cmpl i, %eax
	jle not_tri #if j + k <= i, jump to not_tri

	movl i, %eax
	addl k, %eax#i + k
	cmp j, %eax
	jle not_tri #if i + k <= j, jump to not_tri
	movl $3, tri_type
	jmp return

#Cuts the if's early and returns that inputs aren't a tri
not_tri:
	movl $0, tri_type
	jmp return


	return:
	/*epilog*/
	movl %ebp, %esp
	popl %ebx
	popl %ebp
	ret

	.comm match, 4


	/* end assembly stub */
