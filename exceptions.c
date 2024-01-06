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

#include <inttypes.h>
#include "aarch64.h"
#include "core-context.h"
#include "exceptions.h"
#include "printf.h"

void ExceptionHandlerCEL(CoreContext_t *pContext) __attribute__((noreturn));

void ExceptionHandlerCEL(CoreContext_t *pContext)
{
	uint32_t SPSR, ESR;
	uint8_t EC;
	uint32_t ISS;
	uint64_t ELR, FAR;
	(void)pContext;

	printf("%s: start\n", __FUNCTION__);

	SPSR = coreGet_SPSR_EL3();
	ELR = coreGet_ELR_EL3();
	ESR = coreGet_ESR_EL3();
	FAR = coreGet_FAR_EL3();
	printf("ESR: 0x%08X\n", ESR);
	printf("ELR: 0x%08" PRIX64 "\n", ELR);
	printf("FAR: 0x%08" PRIX64 "\n", FAR);
	printf("SPSR: 0x%08X\n", SPSR);

	if (CORE_SPSR_EL3_GET_A64_MODE(SPSR) == CORE_SPSR_EL3_VALUE_A64_MODE_A32) {
		printf("EL3 Exception was taken from AARCH32.\n");
	} else {
		printf("EL3 Exception was taken from AARCH64 EL%" PRIu64 "%c.\n", CORE_SPSR_EL3_GET_A64_M(SPSR), CORE_SPSR_EL3_GET_A64_SP(SPSR) ? 'H' : 'T');
		printf("Exception Link Register: 0x%016" PRIX64 "\n", ELR);
	}

	EC = CORE_ESR_EL3_GET_EC(ESR);
	ISS = CORE_ESR_EL3_GET_ISS(ESR);

	switch (EC) {
	case CORE_ESR_EL3_VALUE_EC_INSN_ABORT_T:
	case CORE_ESR_EL3_VALUE_EC_DATA_ABORT_T:
		printf("%s abort in thread mode\n", (EC == CORE_ESR_EL3_VALUE_EC_INSN_ABORT_T) ? "Instruction" : "Data");
		printf("\tISS: 0x%08X\n", ISS);
		printf("\tFault address: 0x%08" PRIX64 "\n", FAR);
		break;

	case CORE_ESR_EL3_VALUE_EC_INSN_ABORT_H:
	case CORE_ESR_EL3_VALUE_EC_PC_ALIGNMENT:
	case CORE_ESR_EL3_VALUE_EC_DATA_ABORT_H:
		printf("%s abort in Handler mode\n", (EC == CORE_ESR_EL3_VALUE_EC_INSN_ABORT_H) ? "Instruction" : ((EC == CORE_ESR_EL3_VALUE_EC_PC_ALIGNMENT) ? "PC Alignment" : "Data"));
		printf("\tISS: 0x%08X\n", ISS);
		printf("\tFault address: 0x%08" PRIX64 "\n", FAR);
		break;

	case CORE_ESR_EL3_VALUE_EC_A64_SMC:
		printf("\tSMC 0x%X\n", ISS);
		break;

	case CORE_ESR_EL3_VALUE_EC_BRK:
		printf("\tBreakpoint #%d\n", ISS & 0xFFFF);
		break;

	default:
		printf("Exception Syndrome Register: 0x%08X\n", ESR);
		printf("\tClass: 0x%02X\n", EC);
		printf("\tISS: 0x%08X\n", ISS);
		printf("\tUnimplemented Exception Class: 0x%02X\n", EC);
		break;
	}

	while (1) {
	}
}

void dtEl3ExceptionsInit(void)
{
	coreSet_VBAR_EL3((uint64_t)(uintptr_t)ExceptionVectorBase);
	coreInstructionSyncBarrier();
}

