	.file	"multiplyBy61.cpp"
	.text
	.section	.text._ZN8IntArray7getSizeEv,"axG",@progbits,_ZN8IntArray7getSizeEv,comdat
	.align 2
	.weak	_ZN8IntArray7getSizeEv
	.type	_ZN8IntArray7getSizeEv, @function
_ZN8IntArray7getSizeEv:
.LFB20:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movl	(%rax), %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE20:
	.size	_ZN8IntArray7getSizeEv, .-_ZN8IntArray7getSizeEv
	.section	.text._ZN8IntArray10getElementEi,"axG",@progbits,_ZN8IntArray10getElementEi,comdat
	.align 2
	.weak	_ZN8IntArray10getElementEi
	.type	_ZN8IntArray10getElementEi, @function
_ZN8IntArray10getElementEi:
.LFB22:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movl	%esi, -12(%rbp)
	movq	-8(%rbp), %rax
	movq	8(%rax), %rax
	movl	-12(%rbp), %edx
	movslq	%edx, %rdx
	salq	$2, %rdx
	addq	%rdx, %rax
	movl	(%rax), %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE22:
	.size	_ZN8IntArray10getElementEi, .-_ZN8IntArray10getElementEi
	.section	.text._ZN8IntArray10setElementEii,"axG",@progbits,_ZN8IntArray10setElementEii,comdat
	.align 2
	.weak	_ZN8IntArray10setElementEii
	.type	_ZN8IntArray10setElementEii, @function
_ZN8IntArray10setElementEii:
.LFB23:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movl	%esi, -12(%rbp)
	movl	%edx, -16(%rbp)
	movq	-8(%rbp), %rax
	movq	8(%rax), %rax
	movl	-12(%rbp), %edx
	movslq	%edx, %rdx
	salq	$2, %rdx
	addq	%rax, %rdx
	movl	-16(%rbp), %eax
	movl	%eax, (%rdx)
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE23:
	.size	_ZN8IntArray10setElementEii, .-_ZN8IntArray10setElementEii
	.text
	.globl	_Z12multiplyBy61P8IntArray
	.type	_Z12multiplyBy61P8IntArray, @function
_Z12multiplyBy61P8IntArray:
.LFB24:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$40, %rsp
	.cfi_offset 3, -24
	movq	%rdi, -40(%rbp)
	movl	$0, -20(%rbp)
	jmp	.L7
.L8:
	movq	-40(%rbp), %rbx
	movl	-20(%rbp), %edx
	movq	-40(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	_ZN8IntArray10getElementEi
	movl	$0, %edx
	sall	$5, %eax
	addl	%eax, %edx
	sarl	$5, %eax
	sall	$4, %eax
	addl	%eax, %edx
	sarl	$4, %eax
	sall	$3, %eax
	addl	%eax, %edx
	sarl	$3, %eax
	sall	$2, %eax
	addl	%eax, %edx
	sarl	$2, %eax
	addl	%eax, %edx
	movl	-20(%rbp), %eax
	movl	%eax, %esi
	movq	%rbx, %rdi
	call	_ZN8IntArray10setElementEii
	addl	$1, -20(%rbp)
.L7:
	movq	-40(%rbp), %rax
	movq	%rax, %rdi
	call	_ZN8IntArray7getSizeEv
	cmpl	%eax, -20(%rbp)
	setl	%al
	testb	%al, %al
	jne	.L8
	nop
	nop
	movq	-8(%rbp), %rbx
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE24:
	.size	_Z12multiplyBy61P8IntArray, .-_Z12multiplyBy61P8IntArray
	.ident	"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
