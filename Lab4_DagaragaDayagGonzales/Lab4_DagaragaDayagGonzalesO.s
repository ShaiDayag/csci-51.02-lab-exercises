	.file	"Lab4_DagaragaDayagGonzales.cpp"
	.text
	.globl	_Z5dummyi
	.type	_Z5dummyi, @function
_Z5dummyi:
.LFB0:
	.cfi_startproc
	endbr64
	leal	0(,%rdi,4), %eax
	ret
	.cfi_endproc
.LFE0:
	.size	_Z5dummyi, .-_Z5dummyi
	.globl	_Z6dummy2i
	.type	_Z6dummy2i, @function
_Z6dummy2i:
.LFB1:
	.cfi_startproc
	endbr64
	leal	(%rdi,%rdi,4), %eax
	leal	(%rdi,%rax,8), %eax
	ret
	.cfi_endproc
.LFE1:
	.size	_Z6dummy2i, .-_Z6dummy2i
	.globl	_Z6dummy3i
	.type	_Z6dummy3i, @function
_Z6dummy3i:
.LFB2:
	.cfi_startproc
	endbr64
	movl	%edi, %eax
	sall	$6, %eax
	subl	%edi, %eax
	ret
	.cfi_endproc
.LFE2:
	.size	_Z6dummy3i, .-_Z6dummy3i
	.globl	_Z6dummy4i
	.type	_Z6dummy4i, @function
_Z6dummy4i:
.LFB3:
	.cfi_startproc
	endbr64
	leal	(%rdi,%rdi,4), %eax
	negl	%eax
	ret
	.cfi_endproc
.LFE3:
	.size	_Z6dummy4i, .-_Z6dummy4i
	.globl	_Z6dummy5i
	.type	_Z6dummy5i, @function
_Z6dummy5i:
.LFB4:
	.cfi_startproc
	endbr64
	imull	$61, %edi, %eax
	ret
	.cfi_endproc
.LFE4:
	.size	_Z6dummy5i, .-_Z6dummy5i
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
