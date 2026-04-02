	.file	"ops.c"
	.text
	.globl	slow_add
	.type	slow_add, @function
slow_add:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movl	%esi, -28(%rbp)
	movq	$0, -8(%rbp)
	jmp	.L2
.L3:
	movq	-24(%rbp), %rax
	movl	(%rax), %eax
	leal	1(%rax), %edx
	movq	-24(%rbp), %rax
	movl	%edx, (%rax)
	addq	$1, -8(%rbp)
.L2:
	movl	-28(%rbp), %eax
	cltq
	cmpq	%rax, -8(%rbp)
	jb	.L3
	nop
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	slow_add, .-slow_add
	.globl	fast_add
	.type	fast_add, @function
fast_add:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movl	%esi, -28(%rbp)
	movl	$0, -4(%rbp)
	movq	$0, -16(%rbp)
	jmp	.L5
.L6:
	addl	$1, -4(%rbp)
	addq	$1, -16(%rbp)
.L5:
	movl	-28(%rbp), %eax
	cltq
	cmpq	%rax, -16(%rbp)
	jb	.L6
	movq	-24(%rbp), %rax
	movl	-4(%rbp), %edx
	movl	%edx, (%rax)
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	fast_add, .-fast_add
	.ident	"GCC: (GNU) 15.2.1 20260123 (Red Hat 15.2.1-7)"
	.section	.note.GNU-stack,"",@progbits
