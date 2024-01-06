/* Copyright 2024 Dual Tachyon
 * https://github.com/DualTachyon
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *     Unless required by applicable law or agreed to in writing, software
 *     distributed under the License is distributed on an "AS IS" BASIS,
 *     WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *     See the License for the specific language governing permissions and
 *     limitations under the License.
 */

#include "core-context.h"

	.section    .text.exception

	.global ExceptionVectorBase
	.global ExceptionHandlerCEL

	.macro ExceptionMacro handler
		stp     x0, x1, [sp, -(8 * CORE_CONTEXT_GPR_MAX)]!
		stp     x2, x3, [sp, CORE_CONTEXT_GPR_X2 * 8]
		stp     x4, x5, [sp, CORE_CONTEXT_GPR_X4 * 8]
		stp     x6, x7, [sp, CORE_CONTEXT_GPR_X6 * 8]
		stp     x8, x9, [sp, CORE_CONTEXT_GPR_X8 * 8]
		stp     x10, x11, [sp, CORE_CONTEXT_GPR_X10 * 8]
		stp     x12, x13, [sp, CORE_CONTEXT_GPR_X12 * 8]
		stp     x14, x15, [sp, CORE_CONTEXT_GPR_X14 * 8]
		stp     x16, x17, [sp, CORE_CONTEXT_GPR_X16 * 8]
		stp     x18, x19, [sp, CORE_CONTEXT_GPR_X18 * 8]
		stp     x20, x21, [sp, CORE_CONTEXT_GPR_X20 * 8]
		stp     x22, x23, [sp, CORE_CONTEXT_GPR_X22 * 8]
		stp     x24, x25, [sp, CORE_CONTEXT_GPR_X24 * 8]
		stp     x26, x27, [sp, CORE_CONTEXT_GPR_X26 * 8]
		stp     x28, x29, [sp, CORE_CONTEXT_GPR_X28 * 8]
		mrs     x0, SP_EL0
		stp     x30, x0, [sp, CORE_CONTEXT_GPR_X30 * 8]
		mov	x0, sp
		bl      \handler
		b       dtExceptionReturn
	.endm

.align  11
ExceptionVectorBase:
        // Synchronous at Current EL with SP_EL0
	ExceptionMacro ExceptionHandlerCEL

.align  7
        // IRQ / vIRQ at Current EL with SP_EL0
	b	.

.align  7
        // FIQ / vFIQ at Current EL with SP_EL0
	b	.

.align  7
        // SError / vSError at Current EL with SP_EL0
	ExceptionMacro ExceptionHandlerCEL

.align  7
        // Synchronous at Current EL with SP_ELx
	ExceptionMacro ExceptionHandlerCEL

.align  7
        // IRQ / vIRQ at Current EL with SP_ELx
	b	.

.align  7
        // FIQ / vFIQ at Current EL with SP_ELx
	b	.

.align  7
        // SError / vSError at Current EL with SP_ELx
	ExceptionMacro ExceptionHandlerCEL

.align  7
        // Synchronous at Lower EL AARCH64
	b	.

.align  7
        // IRQ / vIRQ at Lower EL AARCH64
	b	.

.align  7
        // FIQ / vFIQ at Lower EL AARCH64
	b	.

.align  7
        // SError / vSError at Lower EL AARCH64
	b	.

.align  7
        // Synchronous at Lower EL AARCH32
	b	.

.align  7
        // IRQ / vIRQ at Lower EL AARCH32
	b	.

.align  7
        // FIQ / vFIQ at Lower EL AARCH32
	b	.

.align  7
        // SError / vSError at Lower EL AARCH32
	b	.

	.section .text
	.global dtExceptionReturn
#if !defined(__clang__)
	.func dtExceptionReturn
#endif

dtExceptionReturn:
	ldp     x2, x3, [sp, CORE_CONTEXT_GPR_X2 * 8]
	ldp     x4, x5, [sp, CORE_CONTEXT_GPR_X4 * 8]
	ldp     x6, x7, [sp, CORE_CONTEXT_GPR_X6 * 8]
	ldp     x8, x9, [sp, CORE_CONTEXT_GPR_X8 * 8]
	ldp     x10, x11, [sp, CORE_CONTEXT_GPR_X10 * 8]
	ldp     x12, x13, [sp, CORE_CONTEXT_GPR_X12 * 8]
	ldp     x14, x15, [sp, CORE_CONTEXT_GPR_X14 * 8]
	ldp     x16, x17, [sp, CORE_CONTEXT_GPR_X16 * 8]
	ldp     x18, x19, [sp, CORE_CONTEXT_GPR_X18 * 8]
	ldp     x20, x21, [sp, CORE_CONTEXT_GPR_X20 * 8]
	ldp     x22, x23, [sp, CORE_CONTEXT_GPR_X22 * 8]
	ldp     x24, x25, [sp, CORE_CONTEXT_GPR_X24 * 8]
	ldp     x26, x27, [sp, CORE_CONTEXT_GPR_X26 * 8]
	ldp     x28, x29, [sp, CORE_CONTEXT_GPR_X28 * 8]
	ldp     x30, x0, [sp, CORE_CONTEXT_GPR_X30 * 8]
	msr     SP_EL0, x0
	ldp     x0, x1, [sp], 8 * CORE_CONTEXT_GPR_MAX
	eret
#if !defined(__clang__)
	.endfunc
#endif

