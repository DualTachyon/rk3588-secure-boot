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

#include <stdbool.h>
#include <string.h>
#include "aarch64.h"
#include "exceptions.h"
#include "otp.h"
#include "printf.h"
#include "swd.h"
#include "uart.h"

static const uint32_t BootKeyHash[8] = {
#error "Remove this line and paste in the data from the OTP generator!"
};

static const uint32_t BootKeyHashCheck = REPLACE_ME; // Paste in the check value from the OTP generator!

static const char Liability[] = "DualTachyon is NOT responsible for any damage or consequences!";

static void Bye(void) __attribute__((noreturn));
static void EnableSecureBoot(void) __attribute__((noreturn));

extern int SimulationMode;
int SimulationMode;

int Main(void);

//

static void Bye(void)
{
	while (1) {
	}
}

static void EnableSecureBoot(void)
{
	uint32_t Value;

	printf("Boot key hash has been programmed successfully!\n");

	if (dtOtpReadSecure(&Value, 8, 1) < 0) {
		printf("Failed to read Secure Boot Enable flag!\n");
	} else {
		if (Value == 0xFF) {
			printf("Secure Boot is already enabled!\n");
		} else if (Value) {
			printf("Secure Boot Enable flag has unexpected content %08X!\n", Value);
			printf("If the only bits set are in the lower 8 bits, you can disable this check to program the rest!\n");
		} else {
			size_t i = 0;

			printf("To unlock the final programming, type the following case sensitive statement:\n");
			printf("%s\n", Liability);

			while (1) {
				uint8_t c;

				c = dtUartRecvByteBlocking();
				if (Liability[i++] == c) {
					printf("%c", c);
					if (i == sizeof(Liability) - 1) {
						printf("\n");
						break;
					}
				} else {
					if (i > 1) {
						printf("\n");
					}
					printf("Invalid character detected, restart the unlock sequence!\n");
					i = 0;
				}
			}
			Value = 0xFF;
			if (dtOtpWriteSecure(&Value, 8, 1) < 0) {
				printf("Failed to enable secure boot!\n");
			}
			if (dtOtpReadSecure(&Value, 8, 1) < 0) {
				printf("Failed to read flag!\n");
			} else {
				if (Value == 0xFF) {
					printf("Secure boot has been enabled successfully!\n");
				} else {
					printf("Secure boot flag has unexpected content %08X!\n", Value);
				}
			}
		}
	}
	Bye();
}

int Main(void)
{
	uint32_t CoreID;
	uint32_t Check;
	uint32_t Data[8];
	size_t i;
	int ret;

	dtEl3ExceptionsInit();

	CoreID = (coreGet_MPIDR_EL1() & 0xFF00) >> 8;

	if (CoreID) {
		// Sleep any other core if for some reason they're running
		while (1) {
			__asm__ volatile("wfe");
		}
	}

	dtUartInit();
	dtSwdInit();

	printf("EL3 start!\n");

	Check = 0;
	for (i = 0; i < 8; i++) {
		Check ^= BootKeyHash[i];
	}
	if (Check != BootKeyHashCheck) {
		printf("Embedded boot key hash is corrupt!\n");
		Bye();
	}

	if (dtOtpReadSecure(Data, 0x270, 8) < 0) {
		printf("Failed to read existing OTP content!\n");
		Bye();
	}

	if (memcmp(BootKeyHash, Data, sizeof(Data)) == 0) {
		EnableSecureBoot();
	}

	for (i = 0; i < 8; i++) {
		if (Data[i]) {
			bool bIncomplete = true;

			for (i = 0; i < 8; i++) {
				if (Data[i] & ~BootKeyHash[i]) {
					bIncomplete = false;
				}
			}
			if (bIncomplete) {
				printf("Boot key hash has been partially written!\n");
				printf("It can be completed by removing the // on the break on the line following this message!\n");
				//break; // Remove the leading // if you want to resume programming the boot key hash!
			} else {
				printf("OTP has unexpected content!\n");
			}
			for (i = 0; i < 8; i++) {
				printf("OTP @ 0x%03X: Got %08X Expected 0x%08X (difference 0x%08X)\n", 0x270 + (i * 4), Data[i], BootKeyHash[i], Data[i] ^ BootKeyHash[i]);
			}
			Bye();
		}
	}

	printf("Writing hash to OTP...\n");

	ret = dtOtpWriteSecure(BootKeyHash, 0x270, 8);
	if (ret < 0) {
		printf("Error writing the key hash to OTP!\n");
	}

	memset(Data, 0x55, sizeof(Data));

	if (dtOtpReadSecure(Data, 0x270, 8) < 0) {
		printf("Error reading the OTP!\n");
	}

	if (memcmp(BootKeyHash, Data, sizeof(Data)) == 0) {
		printf("Boot key hash has been successfully programmed!\n");
		printf("Reboot your device and go back to the README.MD for further instructions!\n");
	} else {
		if (SimulationMode) {
			printf("You are in simulation mode. Please follow the instruction given above.\n");
		} else {
			printf("Programmed hash in OTP has unexpected content!\n");
			for (i = 0; i < 8; i++) {
				printf("OTP @ 0x%03X: Got 0x%08X Expected 0x%08X (difference 0x%08X)\n", 0x270 + (i * 4), Data[i], BootKeyHash[i], Data[i] ^ BootKeyHash[i]);
			}
		}
	}

	Bye();
}

