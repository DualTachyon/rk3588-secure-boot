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

#ifndef DT_CORE_CONTEXT_H
#define DT_CORE_CONTEXT_H

#if !defined(__ASSEMBLY__)
#include <stdint.h>
#endif

#define CORE_CONTEXT_GPR_X0		0
#define CORE_CONTEXT_GPR_X1		1
#define CORE_CONTEXT_GPR_X2		2
#define CORE_CONTEXT_GPR_X3		3
#define CORE_CONTEXT_GPR_X4		4
#define CORE_CONTEXT_GPR_X5		5
#define CORE_CONTEXT_GPR_X6		6
#define CORE_CONTEXT_GPR_X7		7
#define CORE_CONTEXT_GPR_X8		8
#define CORE_CONTEXT_GPR_X9		9
#define CORE_CONTEXT_GPR_X10		10
#define CORE_CONTEXT_GPR_X11		11
#define CORE_CONTEXT_GPR_X12		12
#define CORE_CONTEXT_GPR_X13		13
#define CORE_CONTEXT_GPR_X14		14
#define CORE_CONTEXT_GPR_X15		15
#define CORE_CONTEXT_GPR_X16		16
#define CORE_CONTEXT_GPR_X17		17
#define CORE_CONTEXT_GPR_X18		18
#define CORE_CONTEXT_GPR_X19		19
#define CORE_CONTEXT_GPR_X20		20
#define CORE_CONTEXT_GPR_X21		21
#define CORE_CONTEXT_GPR_X22		22
#define CORE_CONTEXT_GPR_X23		23
#define CORE_CONTEXT_GPR_X24		24
#define CORE_CONTEXT_GPR_X25		25
#define CORE_CONTEXT_GPR_X26		26
#define CORE_CONTEXT_GPR_X27		27
#define CORE_CONTEXT_GPR_X28		28
#define CORE_CONTEXT_GPR_X29		29
#define CORE_CONTEXT_GPR_X30		30
#define CORE_CONTEXT_GPR_SP_EL0		31
#define CORE_CONTEXT_GPR_MAX		32

#if !defined(__ASSEMBLY__)

typedef struct {
	uint64_t Gpr[CORE_CONTEXT_GPR_MAX];
} CoreContext_t;

#endif

#endif

