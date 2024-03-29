// Automatically generated by https://github.com/DualTachyon/rk3588-svd

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

#ifndef RK3588_OTP_S_H
#define RK3588_OTP_S_H

#include <stdint.h>

#ifndef REQ_ENUM_EQUALS
#define REG_ENUM_EQUALS(p, r, b, e)       (p##_##r##_GET_##b(p##_##r) == p##_##r##_##b##_VALUE_##e)
#endif

#ifndef REQ_ENUM_BITS
#define REG_ENUM_BITS(p, r, b, e)         (p##_##r##_SET_##b(p##_##r##_##b##_VALUE_##e))
#endif

#ifndef REQ_ENUM_BITS_WM
#define REG_ENUM_BITS_WM(p, r, b, e)      (p##_##r##_SET_##b(p##_##r##_##b##_VALUE_##e) | (p##_##r##_##b##_MASK << 16))
#endif

#ifndef REQ_ENUM_GET
#define REG_ENUM_GET(p, r, b, e)          (p##_##r##_GET_##b(p##_##r))
#endif

#ifndef REQ_ENUM_VALUE
#define REG_ENUM_VALUE(p, r, b, e)        (p##_##r##_##b##_VALUE_##e)
#endif

#define OTP_S_BASE                        0x0FE3A0000
#define OTP_S_SIZE                        0x000010000

#define OTP_S_AUTO_CTRL_ADDR              (OTP_S_BASE + 0x0004)
#define OTP_S_AUTO_CTRL                   (*(volatile uint32_t *)OTP_S_AUTO_CTRL_ADDR)
#define OTP_S_AUTO_CTRL_ADDR_SHIFT        16U
#define OTP_S_AUTO_CTRL_ADDR_WIDTH        10U
#define OTP_S_AUTO_CTRL_ADDR_MASK         (((1U << OTP_S_AUTO_CTRL_ADDR_WIDTH) - 1U) << OTP_S_AUTO_CTRL_ADDR_SHIFT)
#define OTP_S_AUTO_CTRL_GET_ADDR(v)       (((v) & OTP_S_AUTO_CTRL_ADDR_MASK) >> OTP_S_AUTO_CTRL_ADDR_SHIFT)
#define OTP_S_AUTO_CTRL_SET_ADDR(v)       (((v) << OTP_S_AUTO_CTRL_ADDR_SHIFT) & OTP_S_AUTO_CTRL_ADDR_MASK)

#define OTP_S_AUTO_CTRL_BURST_SHIFT       8U
#define OTP_S_AUTO_CTRL_BURST_WIDTH       8U
#define OTP_S_AUTO_CTRL_BURST_MASK        (((1U << OTP_S_AUTO_CTRL_BURST_WIDTH) - 1U) << OTP_S_AUTO_CTRL_BURST_SHIFT)
#define OTP_S_AUTO_CTRL_GET_BURST(v)      (((v) & OTP_S_AUTO_CTRL_BURST_MASK) >> OTP_S_AUTO_CTRL_BURST_SHIFT)
#define OTP_S_AUTO_CTRL_SET_BURST(v)      (((v) << OTP_S_AUTO_CTRL_BURST_SHIFT) & OTP_S_AUTO_CTRL_BURST_MASK)

#define OTP_S_AUTO_CTRL_CMD_SHIFT         0U
#define OTP_S_AUTO_CTRL_CMD_WIDTH         8U
#define OTP_S_AUTO_CTRL_CMD_MASK          (((1U << OTP_S_AUTO_CTRL_CMD_WIDTH) - 1U) << OTP_S_AUTO_CTRL_CMD_SHIFT)
#define OTP_S_AUTO_CTRL_GET_CMD(v)        (((v) & OTP_S_AUTO_CTRL_CMD_MASK) >> OTP_S_AUTO_CTRL_CMD_SHIFT)
#define OTP_S_AUTO_CTRL_SET_CMD(v)        (((v) << OTP_S_AUTO_CTRL_CMD_SHIFT) & OTP_S_AUTO_CTRL_CMD_MASK)
#define OTP_S_AUTO_CTRL_CMD_VALUE_WRITE   0x2U
#define OTP_S_AUTO_CTRL_CMD_VALUE_READ    0x0U

#define OTP_S_AUTO_EN_ADDR                (OTP_S_BASE + 0x0008)
#define OTP_S_AUTO_EN                     (*(volatile uint32_t *)OTP_S_AUTO_EN_ADDR)
#define OTP_S_AUTO_EN_EN_SHIFT            0U
#define OTP_S_AUTO_EN_EN_WIDTH            1U
#define OTP_S_AUTO_EN_EN_MASK             (((1U << OTP_S_AUTO_EN_EN_WIDTH) - 1U) << OTP_S_AUTO_EN_EN_SHIFT)
#define OTP_S_AUTO_EN_GET_EN(v)           (((v) & OTP_S_AUTO_EN_EN_MASK) >> OTP_S_AUTO_EN_EN_SHIFT)
#define OTP_S_AUTO_EN_SET_EN(v)           (((v) << OTP_S_AUTO_EN_EN_SHIFT) & OTP_S_AUTO_EN_EN_MASK)
#define OTP_S_AUTO_EN_EN_VALUE_ENABLE     0x1U
#define OTP_S_AUTO_EN_EN_VALUE_DISABLE    0x0U

#define OTP_S_PROG_DATA_ADDR              (OTP_S_BASE + 0x0010)
#define OTP_S_PROG_DATA                   (*(volatile uint32_t *)OTP_S_PROG_DATA_ADDR)
#define OTP_S_DOUT0_ADDR                  (OTP_S_BASE + 0x0020)
#define OTP_S_DOUT0                       (*(volatile uint32_t *)OTP_S_DOUT0_ADDR)
#define OTP_S_DOUT1_ADDR                  (OTP_S_BASE + 0x0024)
#define OTP_S_DOUT1                       (*(volatile uint32_t *)OTP_S_DOUT1_ADDR)
#define OTP_S_DOUT2_ADDR                  (OTP_S_BASE + 0x0028)
#define OTP_S_DOUT2                       (*(volatile uint32_t *)OTP_S_DOUT2_ADDR)
#define OTP_S_DOUT3_ADDR                  (OTP_S_BASE + 0x002C)
#define OTP_S_DOUT3                       (*(volatile uint32_t *)OTP_S_DOUT3_ADDR)
#define OTP_S_DOUT4_ADDR                  (OTP_S_BASE + 0x0030)
#define OTP_S_DOUT4                       (*(volatile uint32_t *)OTP_S_DOUT4_ADDR)
#define OTP_S_DOUT5_ADDR                  (OTP_S_BASE + 0x0034)
#define OTP_S_DOUT5                       (*(volatile uint32_t *)OTP_S_DOUT5_ADDR)
#define OTP_S_DOUT6_ADDR                  (OTP_S_BASE + 0x0038)
#define OTP_S_DOUT6                       (*(volatile uint32_t *)OTP_S_DOUT6_ADDR)
#define OTP_S_DOUT7_ADDR                  (OTP_S_BASE + 0x003C)
#define OTP_S_DOUT7                       (*(volatile uint32_t *)OTP_S_DOUT7_ADDR)

#define OTP_S_INT_ST_ADDR                 (OTP_S_BASE + 0x0084)
#define OTP_S_INT_ST                      (*(volatile uint32_t *)OTP_S_INT_ST_ADDR)
#define OTP_S_INT_ST_ERROR_SHIFT          4U
#define OTP_S_INT_ST_ERROR_WIDTH          1U
#define OTP_S_INT_ST_ERROR_MASK           (((1U << OTP_S_INT_ST_ERROR_WIDTH) - 1U) << OTP_S_INT_ST_ERROR_SHIFT)
#define OTP_S_INT_ST_GET_ERROR(v)         (((v) & OTP_S_INT_ST_ERROR_MASK) >> OTP_S_INT_ST_ERROR_SHIFT)
#define OTP_S_INT_ST_SET_ERROR(v)         (((v) << OTP_S_INT_ST_ERROR_SHIFT) & OTP_S_INT_ST_ERROR_MASK)
#define OTP_S_INT_ST_ERROR_VALUE_ERROR    0x1U
#define OTP_S_INT_ST_ERROR_VALUE_NO_ERROR 0x0U

#define OTP_S_INT_ST_WR_DONE_SHIFT        3U
#define OTP_S_INT_ST_WR_DONE_WIDTH        1U
#define OTP_S_INT_ST_WR_DONE_MASK         (((1U << OTP_S_INT_ST_WR_DONE_WIDTH) - 1U) << OTP_S_INT_ST_WR_DONE_SHIFT)
#define OTP_S_INT_ST_GET_WR_DONE(v)       (((v) & OTP_S_INT_ST_WR_DONE_MASK) >> OTP_S_INT_ST_WR_DONE_SHIFT)
#define OTP_S_INT_ST_SET_WR_DONE(v)       (((v) << OTP_S_INT_ST_WR_DONE_SHIFT) & OTP_S_INT_ST_WR_DONE_MASK)
#define OTP_S_INT_ST_WR_DONE_VALUE_DONE   0x1U

#define OTP_S_INT_ST_VERIFY_SHIFT         2U
#define OTP_S_INT_ST_VERIFY_WIDTH         1U
#define OTP_S_INT_ST_VERIFY_MASK          (((1U << OTP_S_INT_ST_VERIFY_WIDTH) - 1U) << OTP_S_INT_ST_VERIFY_SHIFT)
#define OTP_S_INT_ST_GET_VERIFY(v)        (((v) & OTP_S_INT_ST_VERIFY_MASK) >> OTP_S_INT_ST_VERIFY_SHIFT)
#define OTP_S_INT_ST_SET_VERIFY(v)        (((v) << OTP_S_INT_ST_VERIFY_SHIFT) & OTP_S_INT_ST_VERIFY_MASK)
#define OTP_S_INT_ST_VERIFY_VALUE_FAILED  0x1U
#define OTP_S_INT_ST_VERIFY_VALUE_OK      0x0U

#define OTP_S_INT_ST_RD_DONE_SHIFT        1U
#define OTP_S_INT_ST_RD_DONE_WIDTH        1U
#define OTP_S_INT_ST_RD_DONE_MASK         (((1U << OTP_S_INT_ST_RD_DONE_WIDTH) - 1U) << OTP_S_INT_ST_RD_DONE_SHIFT)
#define OTP_S_INT_ST_GET_RD_DONE(v)       (((v) & OTP_S_INT_ST_RD_DONE_MASK) >> OTP_S_INT_ST_RD_DONE_SHIFT)
#define OTP_S_INT_ST_SET_RD_DONE(v)       (((v) << OTP_S_INT_ST_RD_DONE_SHIFT) & OTP_S_INT_ST_RD_DONE_MASK)
#define OTP_S_INT_ST_RD_DONE_VALUE_DONE   0x1U

#endif

