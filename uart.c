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

#include "hw/ioc_bus.h"
#include "hw/ioc_pmu2.h"
#include "hw/uart2.h"
#include "uart.h"

static void UART_Configure(void)
{
	// Enable the DLL register
	UART2_LCR = 0
		| REG_ENUM_BITS(UART2, LCR, DIV_LAT_ACCESS, ENABLE)
		| REG_ENUM_BITS(UART2, LCR, PARITY_EN, DISABLED)
		| REG_ENUM_BITS(UART2, LCR, STOP_BITS_NUM, 1_STOP_BIT)
		| REG_ENUM_BITS(UART2, LCR, DATA_LENGTH_SEL, 8BITS);
	UART2_DLL = 1;
	// Disable the DLL register
	UART2_LCR = 0
		| REG_ENUM_BITS(UART2, LCR, DIV_LAT_ACCESS, DISABLE)
		| REG_ENUM_BITS(UART2, LCR, PARITY_EN, DISABLED)
		| REG_ENUM_BITS(UART2, LCR, STOP_BITS_NUM, 1_STOP_BIT)
		| REG_ENUM_BITS(UART2, LCR, DATA_LENGTH_SEL, 8BITS);
	UART2_SFE = REG_ENUM_BITS(UART2, SFE, SHADOW_FIFO_EN, ENABLE);
}

static void GPIO_Configure(void)
{
	IOC_PMU2_GPIO0B_IOMUX_SEL_H = 0
		| REG_ENUM_BITS_WM(IOC_PMU2, GPIO0B_IOMUX_SEL_H, GPIO0B5_SEL, BUS_IOC_GPIO0B_IOMUX_SEL_H)
		| REG_ENUM_BITS_WM(IOC_PMU2, GPIO0B_IOMUX_SEL_H, GPIO0B6_SEL, BUS_IOC_GPIO0B_IOMUX_SEL_H);
	IOC_BUS_GPIO0B_IOMUX_SEL_H = 0
		| REG_ENUM_BITS_WM(IOC_BUS, GPIO0B_IOMUX_SEL_H, GPIO0B5_SEL, UART2_TX_M0)
		| REG_ENUM_BITS_WM(IOC_BUS, GPIO0B_IOMUX_SEL_H, GPIO0B6_SEL, UART2_RX_M0);
}

void dtUartInit(void)
{
	GPIO_Configure();
	UART_Configure();
}

void dtUartSendByte(uint8_t Data)
{
	while (REG_ENUM_EQUALS(UART2, USR, TRANS_FIFO_NOT_FULL, FULL)) {
	}
	UART2_THR = Data;
}

uint8_t dtUartRecvByteBlocking(void)
{
	while (!REG_ENUM_EQUALS(UART2, LSR, DATA_READY, READY)) {
	}

	return (uint8_t)UART2_RBR;
}

