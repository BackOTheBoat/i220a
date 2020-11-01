	.text
	.globl get_parity
#edi contains n	
get_parity:
	
	#@TODO: add code here to set eax to 1 iff edi has even parity
	movl	%eax, %edi
	movl	%eax, 1
	testl	%eax, %eax
	jpe 	.L3
.L2:
	ret
.L3:
	movl	1, %eax
	ret
