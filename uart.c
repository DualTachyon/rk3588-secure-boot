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
#include <grf/ioc_pmu2.h>
#include <uart/uart2.h>
#include "uart.h"

static void UART_Configure(void)
{
	// Enable the DLL register
	UART2->LCR = 0
		| UART2_ENUM_BITS(LCR, DIV_LAT_ACCESS, ENABLE)
		| UART2_ENUM_BITS(LCR, PARITY_EN, DISABLED)
		| UART2_ENUM_BITS(LCR, STOP_BITS_NUM, 1_STOP_BIT)
		| UART2_ENUM_BITS(LCR, DATA_LENGTH_SEL, 8BITS);
	UART2->DLL = 1;
	// Disable the DLL register
	UART2->LCR = 0
		| UART2_ENUM_BITS(LCR, DIV_LAT_ACCESS, DISABLE)
		| UART2_ENUM_BITS(LCR, PARITY_EN, DISABLED)
		| UART2_ENUM_BITS(LCR, STOP_BITS_NUM, 1_STOP_BIT)
		| UART2_ENUM_BITS(LCR, DATA_LENGTH_SEL, 8BITS);
	UART2->SFE = UART2_ENUM_BITS(SFE, SHADOW_FIFO_EN, ENABLE);
}

static void GPIO_Configure(void)
{
	IOC_PMU2->GPIO0B_IOMUX_SEL_H = 0
		| IOC_PMU2_ENUM_BITS_WM(GPIO0B_IOMUX_SEL_H, GPIO0B5_SEL, BUS_IOC_GPIO0B_IOMUX_SEL_H)
		| IOC_PMU2_ENUM_BITS_WM(GPIO0B_IOMUX_SEL_H, GPIO0B6_SEL, BUS_IOC_GPIO0B_IOMUX_SEL_H);
	IOC_BUS->GPIO0B_IOMUX_SEL_H = 0
		| IOC_BUS_ENUM_BITS_WM(GPIO0B_IOMUX_SEL_H, GPIO0B5_SEL, UART2_TX_M0)
		| IOC_BUS_ENUM_BITS_WM(GPIO0B_IOMUX_SEL_H, GPIO0B6_SEL, UART2_RX_M0);
}

void dtUartInit(void)
{
	GPIO_Configure();
	UART_Configure();
}

void dtUartSendByte(uint8_t Data)
{
	while (UART2_ENUM_EQUALS(USR, TRANS_FIFO_NOT_FULL, FULL)) {
	}
	UART2->THR = Data;
}

uint8_t dtUartRecvByteBlocking(void)
{
	while (!UART2_ENUM_EQUALS(LSR, DATA_READY, READY)) {
	}

	return (uint8_t)UART2->RBR;
}

