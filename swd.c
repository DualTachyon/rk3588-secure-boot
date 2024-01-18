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

#include <grf/ioc_bus.h>
#include "swd.h"

void dtSwdInit(void)
{
	// Depending on boot mode, SWD will be muxed out if the MicroSD port was initialised, even if not booted from.
	// Skip if MicroSD was the boot mode.
	if (*(volatile uint32_t *)0xFF000010 != 5) {
		IOC_BUS->GPIO4D_IOMUX_SEL_L = 0
			| IOC_BUS_ENUM_BITS_WM(GPIO4D_IOMUX_SEL_L, GPIO4D2_SEL, JTAG_TCK_M0)
			| IOC_BUS_ENUM_BITS_WM(GPIO4D_IOMUX_SEL_L, GPIO4D3_SEL, JTAG_TMS_M0);
	}
}

