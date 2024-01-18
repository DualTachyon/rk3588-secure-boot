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

#include <otp/otp_s.h>
#include "printf.h"
#include "otp.h"

extern int SimulationMode;

int dtOtpReadSecure(uint32_t *pValue, uint32_t Index, uint32_t Count)
{
	volatile uint32_t Timeout;
	volatile uint32_t *pDout = &OTP_S_DOUT0;

	// More than 8 words will trigger an error from the OTP peripheral
	if (!pValue || !Count || Count > 8) {
		return -1;
	}

	OTP_S_INT_ST = OTP_S_INT_ST;

	OTP_S_AUTO_CTRL = 0
		| (Index << OTP_S_AUTO_CTRL_ADDR_SHIFT)
		| (Count << OTP_S_AUTO_CTRL_BURST_SHIFT)
		| REG_ENUM_BITS(OTP_S, AUTO_CTRL, CMD, READ);

	OTP_S_AUTO_EN = REG_ENUM_BITS(OTP_S, AUTO_EN, EN, ENABLE);

	Timeout = 100000;

	while (!REG_ENUM_EQUALS(OTP_S, INT_ST, RD_DONE, DONE)) {
		if (--Timeout == 0) {
			OTP_S_INT_ST = OTP_S_INT_ST;
			printf("%04X: Failed\n", Index);
			return -1;
		}
	}

	OTP_S_INT_ST = OTP_S_INT_ST;

	while (Count-- > 0) {
		*pValue++ = *pDout++;
	}

	return 0;
}

int dtOtpWriteSecure(const uint32_t *pValue, uint32_t Index, uint32_t Count)
{
	volatile uint32_t Timeout;
	uint32_t Value;

	if (!Count || Count > 8 || Index >= 0x300 || Index + Count > 0x300) {
		return -1;
	}

#if 1 // Change this to 0 to enable OTP programming!
	SimulationMode = 1;

	while (Count-- > 0) {
		printf("OTP programming is disabled! Change the #if 1 to #if 0 above this line in the source code!\n");
		printf("%s: Simulated programming 0x%03X with 0x%08X\n", __FUNCTION__, Index++, *pValue++);
	}

	return 0;
#endif

	while (Count > 0) {
		if (dtOtpReadSecure(&Value, Index, 1) < 0) {
			printf("Failed to check OTP before write!\n");
			break;
		}

		// Only program bits that aren't already 1
		Value = *pValue++ & ~Value;
		if (Value) {
			// Restart the programming sequence for every word instead of using batch mode
			OTP_S_AUTO_CTRL = 0
				| (Index << OTP_S_AUTO_CTRL_ADDR_SHIFT)
				| REG_ENUM_BITS(OTP_S, AUTO_CTRL, CMD, WRITE);
			OTP_S_PROG_DATA = Value;
			OTP_S_AUTO_EN = REG_ENUM_BITS(OTP_S, AUTO_EN, EN, ENABLE);

			Timeout = 100000;

			while (1) {
				const uint32_t Status = OTP_S_INT_ST;

				if (OTP_S_INT_ST_GET_WR_DONE(Status) == REG_ENUM_VALUE(OTP_S, INT_ST, WR_DONE, DONE)) {
					break;
				}
				if (OTP_S_INT_ST_GET_VERIFY(Status) == REG_ENUM_VALUE(OTP_S, INT_ST, VERIFY, FAILED)) {
					printf("OTP 0x%03X: Verification failed!\n", Index);
					return -1;
				}
				if (OTP_S_INT_ST_GET_ERROR(Status) == REG_ENUM_VALUE(OTP_S, INT_ST, ERROR, ERROR)) {
					printf("OTP 0x%03X: Error detected!\n", Index);
					return -1;
				}
				if (--Timeout == 0) {
					printf("OTP 0x%03X: Timeout error!\n", Index);
					return -1;
				}
			}

			OTP_S_INT_ST = OTP_S_INT_ST;

			printf("%s: Success programming 0x%03X with 0x%08X\n", __FUNCTION__, Index, Value);
		}

		Count--;
		Index++;
	}

	return 0;
}

