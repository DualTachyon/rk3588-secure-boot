	.section	.text.start
	.global		_start
	.global		Main
	.global		bss_start
	.global		bss_end

#if !defined(__clang__)
	.func       _start
#endif
_start:
	mrs	x3, mpidr_el1
	mov	x2, #512
	and	x3, x3, 0xFF00
	add	x1, x2, x3, lsl #1

	# Setup exception stack per core
	msr	spsel, 1
	adr	x4, ExceptionStack
	add	x4, x4, x1
	mov	sp, x4

	# Setup normal stack per core
	msr	spsel, 0
	adr	x4, MainStack
	add	x4, x4, x1
	mov	sp, x4

	cbnz	x3, skip_bss

	adr	x1, bss_start
	adr	x2, bss_end
1:
	cmp	x1, x2
	b.eq	skip_bss
	strb	wzr, [x1]
	add	x1, x1, 1
	b	1b
skip_bss:
	bl	Main

	b	.

#if !defined(__clang__)
	.endfunc
#endif

	.section .bss

	.align	8

ExceptionStack:
	.space	8 * 512

MainStack:
	.space	8 * 512

