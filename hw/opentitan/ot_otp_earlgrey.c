/*
 * QEMU OpenTitan EarlGrey One Time Programmable (OTP) memory controller
 *
 * Copyright (c) 2023 Rivos, Inc.
 *
 * Author(s):
 *  Emmanuel Blot <eblot@rivosinc.com>
 *  Loïc Lefort <loic@rivosinc.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "qemu/osdep.h"
#include "qemu/log.h"
#include "qemu/main-loop.h"
#include "qemu/timer.h"
#include "qemu/typedefs.h"
#include "qapi/error.h"
#include "hw/irq.h"
#include "hw/opentitan/ot_alert.h"
#include "hw/opentitan/ot_edn.h"
#include "hw/opentitan/ot_otp_earlgrey.h"
#include "hw/qdev-properties-system.h"
#include "hw/qdev-properties.h"
#include "hw/registerfields.h"
#include "hw/riscv/ibex_common.h"
#include "hw/riscv/ibex_irq.h"
#include "hw/sysbus.h"
#include "sysemu/block-backend.h"
#include "trace.h"


/* clang-format off */
/* Core registers */
REG32(INTR_STATE, 0x0u)
    SHARED_FIELD(INTR_OTP_OPERATION_DONE, 0u, 1u)
    SHARED_FIELD(INTR_OTP_ERROR, 1u, 1u)
REG32(INTR_ENABLE, 0x4u)
REG32(INTR_TEST, 0x8u)
REG32(ALERT_TEST, 0xcu)
    FIELD(ALERT_TEST, FATAL_MACRO_ERROR, 0u, 1u)
    FIELD(ALERT_TEST, FATAL_CHECK_ERROR, 1u, 1u)
    FIELD(ALERT_TEST, FATAL_BUS_INTEG_ERROR, 2u, 1u)
    FIELD(ALERT_TEST, FATAL_PRIM_OTP_ALERT, 3u, 1u)
    FIELD(ALERT_TEST, RECOV_PRIM_OTP_ALERT, 4u, 1u)
REG32(STATUS, 0x10u)
    FIELD(STATUS, VENDOR_TEST_ERROR, 0u, 1u)
    FIELD(STATUS, CREATOR_SW_CFG_ERROR, 1u, 1u)
    FIELD(STATUS, OWNER_SW_CFG_ERROR, 2u, 1u)
    FIELD(STATUS, HW_CFG_ERROR, 3u, 1u)
    FIELD(STATUS, SECRET0_ERROR, 4u, 1u)
    FIELD(STATUS, SECRET1_ERROR, 5u, 1u)
    FIELD(STATUS, SECRET2_ERROR, 6u, 1u)
    FIELD(STATUS, LIFE_CYCLE_ERROR, 7u, 1u)
    FIELD(STATUS, DAI_ERROR, 8u, 1u)
    FIELD(STATUS, LCI_ERROR, 9u, 1u)
    FIELD(STATUS, TIMEOUT_ERROR, 10u, 1u)
    FIELD(STATUS, LFSR_FSM_ERROR, 11u, 1u)
    FIELD(STATUS, SCRAMBLING_FSM_ERROR, 12u, 1u)
    FIELD(STATUS, KEY_DERIV_FSM_ERROR, 13u, 1u)
    FIELD(STATUS, BUS_INTEG_ERROR, 14u, 1u)
    FIELD(STATUS, DAI_IDLE, 15u, 1u)
    FIELD(STATUS, CHECK_PENDING, 16u, 1u)
REG32(ERR_CODE, 0x14u)
    FIELD(ERR_CODE, VALUE_NO_ERROR, 0x0u, 1u)
    FIELD(ERR_CODE, VALUE_MACRO_ERROR, 0x1u, 1u)
    FIELD(ERR_CODE, VALUE_MACRO_ECC_CORR_ERROR, 0x2u, 1u)
    FIELD(ERR_CODE, VALUE_MACRO_ECC_UNCORR_ERROR, 0x3u, 1u)
    FIELD(ERR_CODE, VALUE_MACRO_WRITE_BLANK_ERROR, 0x4u, 1u)
    FIELD(ERR_CODE, VALUE_ACCESS_ERROR, 0x5u, 1u)
    FIELD(ERR_CODE, VALUE_CHECK_FAIL_ERROR, 0x6u, 1u)
    FIELD(ERR_CODE, VALUE_FSM_STATE_ERROR, 0x7u, 1u)
REG32(DIRECT_ACCESS_REGWEN, 0x18u)
    FIELD(DIRECT_ACCESS_REGWEN, REGWEN, 0u, 1u)
REG32(DIRECT_ACCESS_CMD, 0x1cu)
    FIELD(DIRECT_ACCESS_CMD, RD, 0u, 1u)
    FIELD(DIRECT_ACCESS_CMD, WR, 1u, 1u)
    FIELD(DIRECT_ACCESS_CMD, DIGEST, 2u, 1u)
REG32(DIRECT_ACCESS_ADDRESS, 0x20u)
REG32(DIRECT_ACCESS_WDATA_0, 0x24u)
REG32(DIRECT_ACCESS_WDATA_1, 0x28u)
REG32(DIRECT_ACCESS_RDATA_0, 0x2cu)
REG32(DIRECT_ACCESS_RDATA_1, 0x30u)
REG32(CHECK_TRIGGER_REGWEN, 0x34u)
    FIELD(CHECK_TRIGGER_REGWEN, REGWEN, 0u, 1u)
REG32(CHECK_TRIGGER, 0x38u)
    FIELD(CHECK_TRIGGER, INTEGRITY, 0u, 1u)
    FIELD(CHECK_TRIGGER, CONSISTENCY, 1u, 1u)
REG32(CHECK_REGWEN, 0x3cu)
    FIELD(CHECK_REGWEN, CHECK_REGWEN, 0u, 1u)
REG32(CHECK_TIMEOUT, 0x40u)
REG32(INTEGRITY_CHECK_PERIOD, 0x44u)
REG32(CONSISTENCY_CHECK_PERIOD, 0x48u)
REG32(VENDOR_TEST_READ_LOCK, 0x4cu)
    SHARED_FIELD(READ_LOCK, 0u, 1u)
REG32(CREATOR_SW_CFG_READ_LOCK, 0x50u)
REG32(OWNER_SW_CFG_READ_LOCK, 0x54u)
REG32(VENDOR_TEST_DIGEST_0, 0x58u)
REG32(VENDOR_TEST_DIGEST_1, 0x5cu)
REG32(CREATOR_SW_CFG_DIGEST_0, 0x60u)
REG32(CREATOR_SW_CFG_DIGEST_1, 0x64u)
REG32(OWNER_SW_CFG_DIGEST_0, 0x68u)
REG32(OWNER_SW_CFG_DIGEST_1, 0x6cu)
REG32(HW_CFG_DIGEST_0, 0x70u)
REG32(HW_CFG_DIGEST_1, 0x74u)
REG32(SECRET0_DIGEST_0, 0x78u)
REG32(SECRET0_DIGEST_1, 0x7cu)
REG32(SECRET1_DIGEST_0, 0x80u)
REG32(SECRET1_DIGEST_1, 0x84u)
REG32(SECRET2_DIGEST_0, 0x88u)
REG32(SECRET2_DIGEST_1, 0x8cu)
/* Software Config Window registers (at offset SW_CFG_WINDOW = +0x1000) */
REG32(SCRATCH, 0u)
REG32(VENDOR_TEST_DIGEST, 56u)
REG32(CREATOR_SW_CFG_AST_CFG, 64u)
REG32(CREATOR_SW_CFG_AST_INIT_EN, 220u)
REG32(CREATOR_SW_CFG_ROM_EXT_SKU, 224u)
REG32(CREATOR_SW_CFG_SIGVERIFY_RSA_MOD_EXP_IBEX_EN, 228u)
REG32(CREATOR_SW_CFG_SIGVERIFY_RSA_KEY_EN, 232u)
REG32(CREATOR_SW_CFG_SIGVERIFY_SPX_EN, 240u)
REG32(CREATOR_SW_CFG_SIGVERIFY_SPX_KEY_EN, 244u)
REG32(CREATOR_SW_CFG_FLASH_DATA_DEFAULT_CFG, 252u)
REG32(CREATOR_SW_CFG_FLASH_INFO_BOOT_DATA_CFG, 256u)
REG32(CREATOR_SW_CFG_FLASH_HW_INFO_CFG_OVERRIDE, 260u)
REG32(CREATOR_SW_CFG_RNG_EN, 264u)
REG32(CREATOR_SW_CFG_JITTER_EN, 268u)
REG32(CREATOR_SW_CFG_RET_RAM_RESET_MASK, 272u)
REG32(CREATOR_SW_CFG_MANUF_STATE, 276u)
REG32(CREATOR_SW_CFG_ROM_EXEC_EN, 280u)
REG32(CREATOR_SW_CFG_CPUCTRL, 284u)
REG32(CREATOR_SW_CFG_MIN_SEC_VER_ROM_EXT, 288u)
REG32(CREATOR_SW_CFG_MIN_SEC_VER_BL0, 292u)
REG32(CREATOR_SW_CFG_DEFAULT_BOOT_DATA_IN_PROD_EN, 296u)
REG32(CREATOR_SW_CFG_RMA_SPIN_EN, 300u)
REG32(CREATOR_SW_CFG_RMA_SPIN_CYCLES, 304u)
REG32(CREATOR_SW_CFG_RNG_REPCNT_THRESHOLDS, 308u)
REG32(CREATOR_SW_CFG_RNG_REPCNTS_THRESHOLDS, 312u)
REG32(CREATOR_SW_CFG_RNG_ADAPTP_HI_THRESHOLDS, 316u)
REG32(CREATOR_SW_CFG_RNG_ADAPTP_LO_THRESHOLDS, 320u)
REG32(CREATOR_SW_CFG_RNG_BUCKET_THRESHOLDS, 324u)
REG32(CREATOR_SW_CFG_RNG_MARKOV_HI_THRESHOLDS, 328u)
REG32(CREATOR_SW_CFG_RNG_MARKOV_LO_THRESHOLDS, 332u)
REG32(CREATOR_SW_CFG_RNG_EXTHT_HI_THRESHOLDS, 336u)
REG32(CREATOR_SW_CFG_RNG_EXTHT_LO_THRESHOLDS, 340u)
REG32(CREATOR_SW_CFG_RNG_ALERT_THRESHOLD, 344u)
REG32(CREATOR_SW_CFG_RNG_HEALTH_CONFIG_DIGEST, 348u)
REG32(CREATOR_SW_CFG_SRAM_KEY_RENEW_EN, 352u)
REG32(CREATOR_SW_CFG_DIGEST, 856u)
REG32(OWNER_SW_CFG_ROM_ERROR_REPORTING, 864u)
REG32(OWNER_SW_CFG_ROM_BOOTSTRAP_DIS, 868u)
REG32(OWNER_SW_CFG_ROM_ALERT_CLASS_EN, 872u)
REG32(OWNER_SW_CFG_ROM_ALERT_ESCALATION, 876u)
REG32(OWNER_SW_CFG_ROM_ALERT_CLASSIFICATION, 880u)
REG32(OWNER_SW_CFG_ROM_LOCAL_ALERT_CLASSIFICATION, 1200u)
REG32(OWNER_SW_CFG_ROM_ALERT_ACCUM_THRESH, 1264u)
REG32(OWNER_SW_CFG_ROM_ALERT_TIMEOUT_CYCLES, 1280u)
REG32(OWNER_SW_CFG_ROM_ALERT_PHASE_CYCLES, 1296u)
REG32(OWNER_SW_CFG_ROM_ALERT_DIGEST_PROD, 1360u)
REG32(OWNER_SW_CFG_ROM_ALERT_DIGEST_PROD_END, 1364u)
REG32(OWNER_SW_CFG_ROM_ALERT_DIGEST_DEV, 1368u)
REG32(OWNER_SW_CFG_ROM_ALERT_DIGEST_RMA, 1372u)
REG32(OWNER_SW_CFG_ROM_WATCHDOG_BITE_THRESHOLD_CYCLES, 1376u)
REG32(OWNER_SW_CFG_ROM_KEYMGR_ROM_EXT_MEAS_EN, 1380u)
REG32(OWNER_SW_CFG_MANUF_STATE, 1384u)
REG32(OWNER_SW_CFG_ROM_RSTMGR_INFO_EN, 1388u)
REG32(OWNER_SW_CFG_DIGEST, 1656u)
REG32(DEVICE_ID, 1664u)
REG32(MANUF_STATE, 1696u)
REG32(HW_CFG_ENABLE, 1728u)
    FIELD(HW_CFG_ENABLE, EN_SRAM_IFETCH, 0u, 8u)
    FIELD(HW_CFG_ENABLE, EN_CSRNG_SW_APP_READ, 8u, 8u)
    FIELD(HW_CFG_ENABLE, EN_ENTROPY_SRC_FW_READ, 16u, 8u)
    FIELD(HW_CFG_ENABLE, EN_ENTROPY_SRC_FW_OVER, 24u, 8u)
REG32(HW_CFG_DIGEST, 1736u)
REG32(TEST_UNLOCK_TOKEN, 1744u)
REG32(TEST_EXIT_TOKEN, 1760u)
REG32(SECRET0_DIGEST, 1776u)
REG32(FLASH_ADDR_KEY_SEED, 1784u)
REG32(FLASH_DATA_KEY_SEED, 1816u)
REG32(SRAM_DATA_KEY_SEED, 1848u)
REG32(SECRET1_DIGEST, 1864u)
REG32(RMA_TOKEN, 1872u)
REG32(CREATOR_ROOT_KEY_SHARE0, 1888u)
REG32(CREATOR_ROOT_KEY_SHARE1, 1920u)
REG32(SECRET2_DIGEST, 1952u)
REG32(LC_TRANSITION_CNT, 1960u)
REG32(LC_STATE, 2008u)
/* clang-format on */

#define SCRATCH_SIZE                                     56u
#define VENDOR_TEST_DIGEST_SIZE                          8u
#define CREATOR_SW_CFG_AST_CFG_SIZE                      156u
#define CREATOR_SW_CFG_SIGVERIFY_RSA_KEY_EN_SIZE         8u
#define CREATOR_SW_CFG_SIGVERIFY_SPX_KEY_EN_SIZE         8u
#define CREATOR_SW_CFG_DIGEST_SIZE                       8u
#define OWNER_SW_CFG_ROM_ALERT_CLASSIFICATION_SIZE       320u
#define OWNER_SW_CFG_ROM_LOCAL_ALERT_CLASSIFICATION_SIZE 64u
#define OWNER_SW_CFG_ROM_ALERT_ACCUM_THRESH_SIZE         16u
#define OWNER_SW_CFG_ROM_ALERT_TIMEOUT_CYCLES_SIZE       16u
#define OWNER_SW_CFG_ROM_ALERT_PHASE_CYCLES_SIZE         64u
#define OWNER_SW_CFG_DIGEST_SIZE                         8u
#define DEVICE_ID_SIZE                                   32u
#define MANUF_STATE_SIZE                                 32u
#define HW_CFG_DIGEST_SIZE                               8u
#define TEST_UNLOCK_TOKEN_SIZE                           16u
#define TEST_EXIT_TOKEN_SIZE                             16u
#define SECRET0_DIGEST_SIZE                              8u
#define FLASH_ADDR_KEY_SEED_SIZE                         32u
#define FLASH_DATA_KEY_SEED_SIZE                         32u
#define SRAM_DATA_KEY_SEED_SIZE                          16u
#define SECRET1_DIGEST_SIZE                              8u
#define RMA_TOKEN_SIZE                                   16u
#define CREATOR_ROOT_KEY_SHARE0_SIZE                     32u
#define CREATOR_ROOT_KEY_SHARE1_SIZE                     32u
#define SECRET2_DIGEST_SIZE                              8u
#define LC_TRANSITION_CNT_SIZE                           48u
#define LC_STATE_SIZE                                    40u

#define INTR_MASK (INTR_OTP_OPERATION_DONE_MASK | INTR_OTP_ERROR_MASK)
#define ALERT_TEST_MASK \
    (R_ALERT_TEST_FATAL_MACRO_ERROR_MASK | \
     R_ALERT_TEST_FATAL_CHECK_ERROR_MASK | \
     R_ALERT_TEST_FATAL_BUS_INTEG_ERROR_MASK | \
     R_ALERT_TEST_FATAL_PRIM_OTP_ALERT_MASK | \
     R_ALERT_TEST_RECOV_PRIM_OTP_ALERT_MASK)

/* clang-format off */
REG32(CSR0, 0x0u)
    FIELD(CSR0, FIELD0, 0u, 1u)
    FIELD(CSR0, FIELD1, 1u, 1u)
    FIELD(CSR0, FIELD2, 2u, 1u)
    FIELD(CSR0, FIELD3, 4u, 10u)
    FIELD(CSR0, FIELD4, 16u, 11u)
REG32(CSR1, 0x4u)
    FIELD(CSR1, FIELD0, 0u, 7u)
    FIELD(CSR1, FIELD1, 7u, 1u)
    FIELD(CSR1, FIELD2, 8u, 7u)
    FIELD(CSR1, FIELD3, 15u, 1u)
    FIELD(CSR1, FIELD4, 16u, 16u)
REG32(CSR2, 0x8u)
    FIELD(CSR2, FIELD0, 0u, 1u)
REG32(CSR3, 0xcu)
    FIELD(CSR3, FIELD0, 0u, 3u)
    FIELD(CSR3, FIELD1, 4u, 10u)
    FIELD(CSR3, FIELD2, 16u, 1u)
    FIELD(CSR3, FIELD3, 17u, 1u)
    FIELD(CSR3, FIELD4, 18u, 1u)
    FIELD(CSR3, FIELD5, 19u, 1u)
    FIELD(CSR3, FIELD6, 20u, 1u)
    FIELD(CSR3, FIELD7, 21u, 1u)
    FIELD(CSR3, FIELD8, 22u, 1u)
REG32(CSR4, 0x10u)
    FIELD(CSR4, FIELD0, 0u, 10u)
    FIELD(CSR4, FIELD1, 12u, 1u)
    FIELD(CSR4, FIELD2, 13u, 1u)
    FIELD(CSR4, FIELD3, 14u, 1u)
REG32(CSR5, 0x14u)
    FIELD(CSR5, FIELD0, 0u, 6u)
    FIELD(CSR5, FIELD1, 6u, 2u)
    FIELD(CSR5, FIELD2, 8u, 1u)
    FIELD(CSR5, FIELD3, 9u, 3u)
    FIELD(CSR5, FIELD4, 12u, 1u)
    FIELD(CSR5, FIELD5, 13u, 1u)
    FIELD(CSR5, FIELD6, 16u, 16u)
REG32(CSR6, 0x18u)
    FIELD(CSR6, FIELD0, 0u, 10u)
    FIELD(CSR6, FIELD1, 11u, 1u)
    FIELD(CSR6, FIELD2, 12u, 1u)
    FIELD(CSR6, FIELD3, 16u, 16u)
REG32(CSR7, 0x1cu)
    FIELD(CSR7, FIELD0, 0u, 6u)
    FIELD(CSR7, FIELD1, 8u, 3u)
    FIELD(CSR7, FIELD2, 14u, 1u)
    FIELD(CSR7, FIELD3, 15u, 1u)
/* clang-format on */

#define SW_CFG_WINDOW      0x1000u
#define SW_CFG_WINDOW_SIZE 0x800u

#define DAI_DELAY_NS 100000u /* 100us */

#define R32_OFF(_r_) ((_r_) / sizeof(uint32_t))

#define R_LAST_REG (R_SECRET2_DIGEST_1)
#define REGS_COUNT (R_LAST_REG + 1u)
#define REGS_SIZE  (REGS_COUNT * sizeof(uint32_t))
#define REG_NAME(_reg_) \
    ((((_reg_) <= REGS_COUNT) && REG_NAMES[_reg_]) ? REG_NAMES[_reg_] : "?")

#define R_LAST_CSR (R_CSR7)
#define CSRS_COUNT (R_LAST_CSR + 1u)
#define CSRS_SIZE  (CSRS_COUNT * sizeof(uint32_t))
#define CSR_NAME(_reg_) \
    ((((_reg_) <= CSRS_COUNT) && CSR_NAMES[_reg_]) ? CSR_NAMES[_reg_] : "?")

#define REG_NAME_ENTRY(_reg_) [R_##_reg_] = stringify(_reg_)
static const char *REG_NAMES[REGS_COUNT] = {
    REG_NAME_ENTRY(INTR_STATE),
    REG_NAME_ENTRY(INTR_ENABLE),
    REG_NAME_ENTRY(INTR_TEST),
    REG_NAME_ENTRY(ALERT_TEST),
    REG_NAME_ENTRY(STATUS),
    REG_NAME_ENTRY(ERR_CODE),
    REG_NAME_ENTRY(DIRECT_ACCESS_REGWEN),
    REG_NAME_ENTRY(DIRECT_ACCESS_CMD),
    REG_NAME_ENTRY(DIRECT_ACCESS_ADDRESS),
    REG_NAME_ENTRY(DIRECT_ACCESS_WDATA_0),
    REG_NAME_ENTRY(DIRECT_ACCESS_WDATA_1),
    REG_NAME_ENTRY(DIRECT_ACCESS_RDATA_0),
    REG_NAME_ENTRY(DIRECT_ACCESS_RDATA_1),
    REG_NAME_ENTRY(CHECK_TRIGGER_REGWEN),
    REG_NAME_ENTRY(CHECK_TRIGGER),
    REG_NAME_ENTRY(CHECK_REGWEN),
    REG_NAME_ENTRY(CHECK_TIMEOUT),
    REG_NAME_ENTRY(INTEGRITY_CHECK_PERIOD),
    REG_NAME_ENTRY(CONSISTENCY_CHECK_PERIOD),
    REG_NAME_ENTRY(VENDOR_TEST_READ_LOCK),
    REG_NAME_ENTRY(CREATOR_SW_CFG_READ_LOCK),
    REG_NAME_ENTRY(OWNER_SW_CFG_READ_LOCK),
    REG_NAME_ENTRY(VENDOR_TEST_DIGEST_0),
    REG_NAME_ENTRY(VENDOR_TEST_DIGEST_1),
    REG_NAME_ENTRY(CREATOR_SW_CFG_DIGEST_0),
    REG_NAME_ENTRY(CREATOR_SW_CFG_DIGEST_1),
    REG_NAME_ENTRY(OWNER_SW_CFG_DIGEST_0),
    REG_NAME_ENTRY(OWNER_SW_CFG_DIGEST_1),
    REG_NAME_ENTRY(HW_CFG_DIGEST_0),
    REG_NAME_ENTRY(HW_CFG_DIGEST_1),
    REG_NAME_ENTRY(SECRET0_DIGEST_0),
    REG_NAME_ENTRY(SECRET0_DIGEST_1),
    REG_NAME_ENTRY(SECRET1_DIGEST_0),
    REG_NAME_ENTRY(SECRET1_DIGEST_1),
    REG_NAME_ENTRY(SECRET2_DIGEST_0),
    REG_NAME_ENTRY(SECRET2_DIGEST_1),
};
#undef REG_NAME_ENTRY

#define CSR_NAME_ENTRY(_reg_) [R_##_reg_] = stringify(_reg_)
static const char CSR_NAMES[CSRS_COUNT][6u] = {
    CSR_NAME_ENTRY(CSR0), CSR_NAME_ENTRY(CSR1), CSR_NAME_ENTRY(CSR2),
    CSR_NAME_ENTRY(CSR3), CSR_NAME_ENTRY(CSR4), CSR_NAME_ENTRY(CSR5),
    CSR_NAME_ENTRY(CSR6), CSR_NAME_ENTRY(CSR7),
};
#undef CSR_NAME_ENTRY

#define OTP_PART_VENDOR_TEST_OFFSET    0u
#define OTP_PART_VENDOR_TEST_SIZE      64u
#define OTP_PART_CREATOR_SW_CFG_OFFSET 64u
#define OTP_PART_CREATOR_SW_CFG_SIZE   800u
#define OTP_PART_OWNER_SW_CFG_OFFSET   864u
#define OTP_PART_OWNER_SW_CFG_SIZE     800u
#define OTP_PART_HW_CFG_OFFSET         1664u
#define OTP_PART_HW_CFG_SIZE           80u
#define OTP_PART_SECRET0_OFFSET        1744u
#define OTP_PART_SECRET0_SIZE          40u
#define OTP_PART_SECRET1_OFFSET        1784u
#define OTP_PART_SECRET1_SIZE          88u
#define OTP_PART_SECRET2_OFFSET        1872u
#define OTP_PART_SECRET2_SIZE          88u
#define OTP_PART_LIFE_CYCLE_OFFSET     1960u
#define OTP_PART_LIFE_CYCLE_SIZE       88u

typedef enum {
    OTP_PART_VENDOR_TEST,
    OTP_PART_CREATOR_SW_CFG,
    OTP_PART_OWNER_SW_CFG,
    OTP_PART_HW_CFG,
    OTP_PART_SECRET0,
    OTP_PART_SECRET1,
    OTP_PART_SECRET2,
    OTP_PART_LIFE_CYCLE,
    _OTP_PART_LIFE_COUNT,
} OtOTPPartitionType;

typedef enum {
    OTP_NO_ERROR,
    OTP_MACRO_ERROR,
    OTP_MACRO_ECC_CORR_ERROR,
    OTP_MACRO_ECC_UNCORR_ERROR,
    OTP_MACRO_WRITE_BLANK_ERROR,
    OTP_ACCESS_ERROR,
    OTP_CHECK_FAIL_ERROR,
    OTP_FSM_STATE_ERROR,
} OtOTPError;

typedef struct {
    uint16_t size;
    uint16_t offset;
    uint16_t digest_offset;
    bool hw_digest;
    bool secret;
    bool buffered;
    bool wr_lockable;
    bool rd_lockable;
    bool ecc_fatal_alert;
    bool wide; /* false: 32-bit granule, true: 64-bit granule */
} OtOTPPartition;

typedef struct {
    uint32_t *storage; /* overall buffer for the storage backend */
    uint32_t *data; /* data buffer (all partitions) */
    uint32_t *ecc; /* ecc buffer for date */
    unsigned size; /* overall storage size in bytes */
    unsigned data_size; /* data buffer size in bytes */
    unsigned ecc_size; /* ecc buffer size in bytes */
    unsigned ecc_bit_count; /* count of ECC bit for each data granule */
    unsigned ecc_granule; /* size of a granule in bytes */
} OtOTPStorage;

#define OtOTPEgState OtOTPEarlGreyState

struct OtOTPEgState {
    OtOTPState parent_obj;

    struct {
        MemoryRegion ctrl;
        struct {
            MemoryRegion regs;
            MemoryRegion swcfg;
        } sub;
    } mmio;
    struct {
        MemoryRegion csrs;
    } prim;
    struct {
        uint32_t state;
        unsigned tcount;
    } lc;
    IbexIRQ irqs[2u];
    IbexIRQ alert;

    QEMUTimer *dai_delay; /**< Simulate delayed access completion */

    uint32_t regs[REGS_COUNT];
    bool dai_busy;

    OtOTPStorage otp;
    OtOTPHWCfg *hw_cfg;
    OtOTPEntropyCfg *entropy_cfg;

    BlockBackend *blk; /* OTP backend */

    OtEDNState *edn;
    uint8_t edn_ep;
};

static const OtOTPPartition OtOTPPartitions[] = {
    [OTP_PART_VENDOR_TEST] = {
        .size = OTP_PART_VENDOR_TEST_SIZE,
        .offset = OTP_PART_VENDOR_TEST_OFFSET,
        .digest_offset = R_VENDOR_TEST_DIGEST,
        .hw_digest = false,
        .secret = false,
        .buffered = false,
        .wr_lockable = true,
        .rd_lockable = true,
        .ecc_fatal_alert = false,
        .wide = false,
    },
    [OTP_PART_CREATOR_SW_CFG] = {
        .size = OTP_PART_CREATOR_SW_CFG_SIZE,
        .offset = OTP_PART_CREATOR_SW_CFG_OFFSET,
        .digest_offset = R_CREATOR_SW_CFG_DIGEST,
        .hw_digest = false,
        .secret = false,
        .buffered = false,
        .wr_lockable = true,
        .rd_lockable = true,
        .ecc_fatal_alert = true,
        .wide = false,
    },
    [OTP_PART_OWNER_SW_CFG] = {
        .size = OTP_PART_OWNER_SW_CFG_SIZE,
        .offset = OTP_PART_OWNER_SW_CFG_OFFSET,
        .digest_offset = R_OWNER_SW_CFG_DIGEST,
        .hw_digest = false,
        .secret = false,
        .buffered = false,
        .wr_lockable = true,
        .rd_lockable = true,
        .ecc_fatal_alert = true,
        .wide = false,
    },
    [OTP_PART_HW_CFG] = {
        .size = OTP_PART_HW_CFG_SIZE,
        .offset = OTP_PART_HW_CFG_OFFSET,
        .digest_offset = R_HW_CFG_DIGEST,
        .hw_digest = true,
        .secret = false,
        .buffered = true,
        .wr_lockable = true,
        .rd_lockable = false,
        .ecc_fatal_alert = true,
        .wide = false,
    },
    [OTP_PART_SECRET0] = {
        .size = OTP_PART_SECRET0_SIZE,
        .offset = OTP_PART_SECRET0_OFFSET,
        .digest_offset = R_SECRET0_DIGEST,
        .hw_digest = true,
        .secret = true,
        .buffered = true,
        .wr_lockable = true,
        .rd_lockable = true,
        .ecc_fatal_alert = true,
        .wide = true,
    },
    [OTP_PART_SECRET1] = {
        .size = OTP_PART_SECRET1_SIZE,
        .offset = OTP_PART_SECRET1_OFFSET,
        .digest_offset = R_SECRET1_DIGEST,
        .hw_digest = true,
        .secret = true,
        .buffered = true,
        .wr_lockable = true,
        .rd_lockable = true,
        .ecc_fatal_alert = true,
        .wide = true,
    },
    [OTP_PART_SECRET2] = {
        .size = OTP_PART_SECRET2_SIZE,
        .offset = OTP_PART_SECRET2_OFFSET,
        .digest_offset = R_SECRET2_DIGEST,
        .hw_digest = true,
        .secret = true,
        .buffered = true,
        .wr_lockable = true,
        .rd_lockable = true,
        .ecc_fatal_alert = true,
        .wide = true,
    },
    [OTP_PART_LIFE_CYCLE] = {
        .size = OTP_PART_LIFE_CYCLE_SIZE,
        .offset = OTP_PART_LIFE_CYCLE_OFFSET,
        .digest_offset = UINT16_MAX,
        .hw_digest = false,
        .secret = false,
        .buffered = true,
        .wr_lockable = false,
        .rd_lockable = false,
        .ecc_fatal_alert = true,
        .wide = false,
    },
};

/* clang-format on */

/* NOLINTNEXTLINE */
#include "ot_otp_earlgrey_lcvalues.c"

#define LC_TRANSITION_COUNT_MAX 24u
#define LC_ENCODE_STATE(_x_) \
    (((_x_) << 0u) | ((_x_) << 5u) | ((_x_) << 10u) | ((_x_) << 15u) | \
     ((_x_) << 20u) | ((_x_) << 25u))

static void ot_otp_eg_set_error(OtOTPEgState *s, int part, OtOTPError err)
{
    unsigned err_off = (unsigned)part * 3u;
    uint32_t err_mask = ((1u << (err_off + 1u)) - 1u) & ~((1u << err_off) - 1u);
    s->regs[R_ERR_CODE] &= ~err_mask;
    s->regs[R_ERR_CODE] |= (((uint32_t)err) & 0x7u) << err_off;

    switch (err) {
    case OTP_MACRO_ERROR:
    case OTP_MACRO_ECC_UNCORR_ERROR:
    case OTP_CHECK_FAIL_ERROR:
    case OTP_FSM_STATE_ERROR:
        ibex_irq_set(&s->alert, 1u);
        break;
    default:
        break;
    }
}

static uint32_t ot_otp_eg_get_status(OtOTPEgState *s)
{
    uint32_t status = 0;
    for (unsigned ix = 0; ix < ARRAY_SIZE(OtOTPPartitions); ix++) {
        unsigned err_off = 3u << ix;
        uint32_t err_mask =
            ((1u << (err_off + 1u)) - 1u) & ~((1u << err_off) - 1u);
        if (s->regs[R_ERR_CODE] & err_mask) {
            status |= 1u << ix;
        }
    }

    /* tmp */
    status = FIELD_DP32(status, STATUS, DAI_IDLE, !s->dai_busy);

    return status;
}

static int ot_otp_eg_swcfg_get_part(hwaddr addr)
{
    for (unsigned ix = 0; ix < ARRAY_SIZE(OtOTPPartitions); ix++) {
        const OtOTPPartition *part = &OtOTPPartitions[ix];
        if ((addr >= part->offset) &&
            ((addr + sizeof(uint32_t)) <= (part->offset + part->size))) {
            return (OtOTPPartitionType)ix;
        }
    }
    return -1;
}

static uint16_t ot_otp_eg_swcfg_get_part_digest_offset(int part)
{
    switch (part) {
    case OTP_PART_VENDOR_TEST:
    case OTP_PART_CREATOR_SW_CFG:
    case OTP_PART_OWNER_SW_CFG:
    case OTP_PART_HW_CFG:
    case OTP_PART_SECRET0:
    case OTP_PART_SECRET1:
    case OTP_PART_SECRET2:
    case OTP_PART_LIFE_CYCLE:
        return OtOTPPartitions[part].digest_offset;
    default:
        return UINT16_MAX;
    }
}

static uint64_t ot_otp_eg_swcfg_get_part_digest(OtOTPEgState *s, int part)
{
    uint16_t offset = ot_otp_eg_swcfg_get_part_digest_offset(part);

    if (offset == UINT16_MAX) {
        return 0u;
    }

    offset >>= 2u;
    const uint32_t *data = s->otp.data;

    uint64_t digest = data[offset];
    digest |= ((uint64_t)data[offset + sizeof(uint32_t)]) << 32u;

    return digest;
}

static bool ot_otp_eg_swcfg_is_part_digest_offset(int part, hwaddr addr)
{
    uint16_t offset = ot_otp_eg_swcfg_get_part_digest_offset(part);

    return (addr == offset) || ((addr + (sizeof(uint32_t))) == offset);
}

static void ot_otp_eg_update_irqs(OtOTPEgState *s)
{
    uint32_t level = s->regs[R_INTR_STATE] & s->regs[R_INTR_ENABLE];

    for (unsigned ix = 0; ix < ARRAY_SIZE(s->irqs); ix++) {
        ibex_irq_set(&s->irqs[ix], (int)((level >> ix) & 0x1));
    }
}

static bool ot_otp_eg_is_readable(OtOTPEgState *s, int partition, unsigned addr)
{
    /* "in all partitions, the digest itself is ALWAYS readable." */
    bool rdaccess = ot_otp_eg_swcfg_is_part_digest_offset(partition, addr);

    if (!rdaccess) {
        if (!OtOTPPartitions[partition].rd_lockable) {
            /* read lock is not supported for the this partition */
            return true;
        }
    }

    if (!rdaccess) {
        switch (partition) {
        case OTP_PART_VENDOR_TEST:
            rdaccess = (bool)SHARED_FIELD_EX32(s->regs[R_VENDOR_TEST_READ_LOCK],
                                               READ_LOCK);
            break;
        case OTP_PART_CREATOR_SW_CFG:
            rdaccess =
                (bool)SHARED_FIELD_EX32(s->regs[R_CREATOR_SW_CFG_READ_LOCK],
                                        READ_LOCK);
            break;
        case OTP_PART_OWNER_SW_CFG:
            rdaccess =
                (bool)SHARED_FIELD_EX32(s->regs[R_OWNER_SW_CFG_READ_LOCK],
                                        READ_LOCK);
            break;
        case OTP_PART_SECRET0:
        case OTP_PART_SECRET1:
        case OTP_PART_SECRET2:
            rdaccess = ot_otp_eg_swcfg_get_part_digest(s, partition) == 0u;
            break;
        default:
            break;
        }
    }

    return rdaccess;
}

static bool ot_otp_eg_is_wide_granule(int partition)
{
    if (partition >= 0 && partition < ARRAY_SIZE(OtOTPPartitions)) {
        return OtOTPPartitions[partition].wide;
    }

    return false;
}

static bool ot_otp_eg_is_buffered(int partition)
{
    if (partition >= 0 && partition < ARRAY_SIZE(OtOTPPartitions)) {
        return OtOTPPartitions[partition].buffered;
    }

    return false;
}

static void ot_otp_eg_complete_dai(void *opaque)
{
    OtOTPEgState *s = opaque;

    s->dai_busy = false;
}

static void ot_otp_eg_direct_read(OtOTPEgState *s)
{
    if (s->dai_busy) {
        return;
    }

    s->dai_busy = true;

    unsigned address = s->regs[R_DIRECT_ACCESS_ADDRESS];

    int partition = ot_otp_eg_swcfg_get_part(address);

    if (partition >= 0) {
        if (ot_otp_eg_is_readable(s, partition, (unsigned)address)) {
            const uint32_t *data = s->otp.data;
            address >>= 2u;
            s->regs[R_DIRECT_ACCESS_RDATA_0] = data[address];
            if (ot_otp_eg_is_wide_granule(partition)) {
                s->regs[R_DIRECT_ACCESS_RDATA_1] = data[address + 1u];
            }
            ot_otp_eg_set_error(s, partition, OTP_NO_ERROR);
        } else {
            ot_otp_eg_set_error(s, partition, OTP_ACCESS_ERROR);
        }
    }

    if (!ot_otp_eg_is_buffered(partition)) {
        timer_mod(s->dai_delay,
                  qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL) + DAI_DELAY_NS);
        return;
    }

    s->dai_busy = false;
}

static void ot_otp_eg_direct_write(OtOTPEgState *s)
{
    (void)s;
    qemu_log_mask(LOG_UNIMP, "%s: OTP write is not supported\n", __func__);
}

static void ot_otp_eg_direct_digest(OtOTPEgState *s)
{
    (void)s;
    qemu_log_mask(LOG_UNIMP, "%s: OTP change is not supported\n", __func__);
}

static uint64_t ot_otp_eg_regs_read(void *opaque, hwaddr addr, unsigned size)
{
    OtOTPEgState *s = OT_OTP_EARLGREY(opaque);
    (void)size;
    uint32_t val32;

    hwaddr reg = R32_OFF(addr);
    switch (reg) {
    case R_INTR_STATE:
    case R_INTR_ENABLE:
    case R_ERR_CODE:
    case R_DIRECT_ACCESS_WDATA_0:
    case R_DIRECT_ACCESS_WDATA_1:
    case R_DIRECT_ACCESS_RDATA_0:
    case R_DIRECT_ACCESS_RDATA_1:
    case R_DIRECT_ACCESS_ADDRESS:
    case R_VENDOR_TEST_READ_LOCK:
    case R_CREATOR_SW_CFG_READ_LOCK:
    case R_OWNER_SW_CFG_READ_LOCK:
        val32 = s->regs[reg];
        break;
    case R_STATUS:
        val32 = ot_otp_eg_get_status(s);
        break;
    case R_DIRECT_ACCESS_REGWEN:
        val32 =
            FIELD_DP32(0, DIRECT_ACCESS_REGWEN, REGWEN, (uint32_t)!s->dai_busy);
        break;
    /* NOLINTNEXTLINE */
    case R_DIRECT_ACCESS_CMD:
        val32 = 0; /* R0W1C */
        break;
    case R_CHECK_TRIGGER_REGWEN:
    case R_CHECK_TRIGGER:
    case R_CHECK_REGWEN:
    case R_CHECK_TIMEOUT:
    case R_INTEGRITY_CHECK_PERIOD:
    case R_CONSISTENCY_CHECK_PERIOD:
        /* TODO: not yet implemented */
        val32 = 0;
        break;
    /* in all partitions, the digest itself is ALWAYS readable."*/
    case R_VENDOR_TEST_DIGEST_0:
        val32 =
            (uint32_t)ot_otp_eg_swcfg_get_part_digest(s, OTP_PART_VENDOR_TEST);
        break;
    case R_VENDOR_TEST_DIGEST_1:
        val32 =
            (uint32_t)(ot_otp_eg_swcfg_get_part_digest(s,
                                                       OTP_PART_VENDOR_TEST) >>
                       32u);
        break;
    case R_CREATOR_SW_CFG_DIGEST_0:
        val32 = (uint32_t)
            ot_otp_eg_swcfg_get_part_digest(s, OTP_PART_CREATOR_SW_CFG);
        break;
    case R_CREATOR_SW_CFG_DIGEST_1:
        val32 = (uint32_t)(ot_otp_eg_swcfg_get_part_digest(
                               s, OTP_PART_CREATOR_SW_CFG) >>
                           32u);
        break;
    case R_OWNER_SW_CFG_DIGEST_0:
        val32 =
            (uint32_t)ot_otp_eg_swcfg_get_part_digest(s, OTP_PART_OWNER_SW_CFG);
        break;
    case R_OWNER_SW_CFG_DIGEST_1:
        val32 =
            (uint32_t)(ot_otp_eg_swcfg_get_part_digest(s,
                                                       OTP_PART_OWNER_SW_CFG) >>
                       32u);
        break;
    case R_HW_CFG_DIGEST_0:
        val32 = (uint32_t)ot_otp_eg_swcfg_get_part_digest(s, OTP_PART_HW_CFG);
        break;
    case R_HW_CFG_DIGEST_1:
        val32 =
            (uint32_t)(ot_otp_eg_swcfg_get_part_digest(s, OTP_PART_HW_CFG) >>
                       32u);
        break;
    case R_SECRET0_DIGEST_0:
        val32 = (uint32_t)ot_otp_eg_swcfg_get_part_digest(s, OTP_PART_SECRET0);
        break;
    case R_SECRET0_DIGEST_1:
        val32 =
            (uint32_t)(ot_otp_eg_swcfg_get_part_digest(s, OTP_PART_SECRET0) >>
                       32u);
        break;
    case R_SECRET1_DIGEST_0:
        val32 = (uint32_t)ot_otp_eg_swcfg_get_part_digest(s, OTP_PART_SECRET1);
        break;
    case R_SECRET1_DIGEST_1:
        val32 =
            (uint32_t)(ot_otp_eg_swcfg_get_part_digest(s, OTP_PART_SECRET1) >>
                       32u);
        break;
    case R_SECRET2_DIGEST_0:
        val32 = (uint32_t)ot_otp_eg_swcfg_get_part_digest(s, OTP_PART_SECRET2);
        break;
    case R_SECRET2_DIGEST_1:
        val32 =
            (uint32_t)(ot_otp_eg_swcfg_get_part_digest(s, OTP_PART_SECRET2) >>
                       32u);
        break;
    case R_INTR_TEST:
    case R_ALERT_TEST:
        qemu_log_mask(LOG_GUEST_ERROR,
                      "%s: W/O register 0x%03" HWADDR_PRIx " (%s)\n", __func__,
                      addr, REG_NAME(reg));
        val32 = 0;
        break;
    default:
        qemu_log_mask(LOG_GUEST_ERROR, "%s: Bad offset 0x%" HWADDR_PRIx "\n",
                      __func__, addr);
        val32 = 0;
        break;
    }

    uint64_t pc = ibex_get_current_pc();
    trace_ot_otp_io_read_out((unsigned)addr, REG_NAME(reg), val32, pc);

    return (uint64_t)val32;
}

static void ot_otp_eg_regs_write(void *opaque, hwaddr addr, uint64_t value,
                                 unsigned size)
{
    OtOTPEgState *s = OT_OTP_EARLGREY(opaque);
    (void)size;
    uint32_t val32 = (uint32_t)value;

    hwaddr reg = R32_OFF(addr);

    uint64_t pc = ibex_get_current_pc();
    trace_ot_otp_io_write((unsigned)addr, REG_NAME(reg), val32, pc);

    switch (reg) {
    case R_INTR_STATE:
        val32 &= INTR_MASK;
        s->regs[R_INTR_STATE] &= ~val32; /* RW1C */
        ot_otp_eg_update_irqs(s);
        break;
    case R_INTR_ENABLE:
        val32 &= INTR_MASK;
        s->regs[R_INTR_ENABLE] = val32;
        ot_otp_eg_update_irqs(s);
        break;
    case R_INTR_TEST:
        val32 &= INTR_MASK;
        s->regs[R_INTR_STATE] = val32;
        ot_otp_eg_update_irqs(s);
        break;
    case R_ALERT_TEST:
        val32 &= ALERT_TEST_MASK;
        if (val32) {
            ibex_irq_set(&s->alert, (int)val32);
        }
        break;
    case R_DIRECT_ACCESS_CMD:
        if (FIELD_EX32(val32, DIRECT_ACCESS_CMD, RD)) {
            ot_otp_eg_direct_read(s);
        } else if (FIELD_EX32(val32, DIRECT_ACCESS_CMD, WR)) {
            ot_otp_eg_direct_write(s);
        } else if (FIELD_EX32(val32, DIRECT_ACCESS_CMD, DIGEST)) {
            ot_otp_eg_direct_digest(s);
        }
        break;
    case R_DIRECT_ACCESS_ADDRESS:
        val32 &= (1u << 11u) - 1u;
        s->regs[reg] = val32;
        break;
    case R_DIRECT_ACCESS_WDATA_0:
    case R_DIRECT_ACCESS_WDATA_1:
        s->regs[reg] = val32;
        break;
    case R_VENDOR_TEST_READ_LOCK:
    case R_CREATOR_SW_CFG_READ_LOCK:
    case R_OWNER_SW_CFG_READ_LOCK:
        val32 &= READ_LOCK_MASK;
        s->regs[reg] &= val32; /* RW0C */
        break;
    case R_CHECK_TRIGGER_REGWEN:
    case R_CHECK_TRIGGER:
    case R_CHECK_REGWEN:
    case R_CHECK_TIMEOUT:
    case R_INTEGRITY_CHECK_PERIOD:
    case R_CONSISTENCY_CHECK_PERIOD:
        /* TODO: not yet implemented */
        break;
    case R_STATUS:
    case R_ERR_CODE:
    case R_DIRECT_ACCESS_REGWEN:
    case R_DIRECT_ACCESS_RDATA_0:
    case R_DIRECT_ACCESS_RDATA_1:
    case R_VENDOR_TEST_DIGEST_0:
    case R_VENDOR_TEST_DIGEST_1:
    case R_CREATOR_SW_CFG_DIGEST_0:
    case R_CREATOR_SW_CFG_DIGEST_1:
    case R_OWNER_SW_CFG_DIGEST_0:
    case R_OWNER_SW_CFG_DIGEST_1:
    case R_HW_CFG_DIGEST_0:
    case R_HW_CFG_DIGEST_1:
    case R_SECRET0_DIGEST_0:
    case R_SECRET0_DIGEST_1:
    case R_SECRET1_DIGEST_0:
    case R_SECRET1_DIGEST_1:
    case R_SECRET2_DIGEST_0:
    case R_SECRET2_DIGEST_1:
        qemu_log_mask(LOG_GUEST_ERROR,
                      "%s: R/O register 0x%03" HWADDR_PRIx " (%s)\n", __func__,
                      addr, REG_NAME(reg));
        break;
    default:
        qemu_log_mask(LOG_GUEST_ERROR, "%s: Bad offset 0x%" HWADDR_PRIx "\n",
                      __func__, addr);
        break;
    }
}

static const char *ot_otp_eg_swcfg_reg_name(unsigned swreg)
{
#define CASE_SCALAR(_reg_) \
    case R_##_reg_: \
        return stringify(_reg_)
#define CASE_RANGE(_reg_) \
    case R_##_reg_...(R_##_reg_ + ((_reg_##_SIZE) / 4u) - 1u): \
        return stringify(_reg_)

    switch (swreg) {
        CASE_RANGE(CREATOR_SW_CFG_AST_CFG);
        CASE_SCALAR(CREATOR_SW_CFG_AST_INIT_EN);
        CASE_SCALAR(CREATOR_SW_CFG_ROM_EXT_SKU);
        CASE_SCALAR(CREATOR_SW_CFG_SIGVERIFY_RSA_MOD_EXP_IBEX_EN);
        CASE_RANGE(CREATOR_SW_CFG_SIGVERIFY_RSA_KEY_EN);
        CASE_SCALAR(CREATOR_SW_CFG_SIGVERIFY_SPX_EN);
        CASE_RANGE(CREATOR_SW_CFG_SIGVERIFY_SPX_KEY_EN);
        CASE_SCALAR(CREATOR_SW_CFG_FLASH_DATA_DEFAULT_CFG);
        CASE_SCALAR(CREATOR_SW_CFG_FLASH_INFO_BOOT_DATA_CFG);
        CASE_SCALAR(CREATOR_SW_CFG_FLASH_HW_INFO_CFG_OVERRIDE);
        CASE_SCALAR(CREATOR_SW_CFG_RNG_EN);
        CASE_SCALAR(CREATOR_SW_CFG_JITTER_EN);
        CASE_SCALAR(CREATOR_SW_CFG_RET_RAM_RESET_MASK);
        CASE_SCALAR(CREATOR_SW_CFG_MANUF_STATE);
        CASE_SCALAR(CREATOR_SW_CFG_ROM_EXEC_EN);
        CASE_SCALAR(CREATOR_SW_CFG_CPUCTRL);
        CASE_SCALAR(CREATOR_SW_CFG_MIN_SEC_VER_ROM_EXT);
        CASE_SCALAR(CREATOR_SW_CFG_MIN_SEC_VER_BL0);
        CASE_SCALAR(CREATOR_SW_CFG_DEFAULT_BOOT_DATA_IN_PROD_EN);
        CASE_SCALAR(CREATOR_SW_CFG_RMA_SPIN_EN);
        CASE_SCALAR(CREATOR_SW_CFG_RMA_SPIN_CYCLES);
        CASE_SCALAR(CREATOR_SW_CFG_RNG_REPCNT_THRESHOLDS);
        CASE_SCALAR(CREATOR_SW_CFG_RNG_REPCNTS_THRESHOLDS);
        CASE_SCALAR(CREATOR_SW_CFG_RNG_ADAPTP_HI_THRESHOLDS);
        CASE_SCALAR(CREATOR_SW_CFG_RNG_ADAPTP_LO_THRESHOLDS);
        CASE_SCALAR(CREATOR_SW_CFG_RNG_BUCKET_THRESHOLDS);
        CASE_SCALAR(CREATOR_SW_CFG_RNG_MARKOV_HI_THRESHOLDS);
        CASE_SCALAR(CREATOR_SW_CFG_RNG_MARKOV_LO_THRESHOLDS);
        CASE_SCALAR(CREATOR_SW_CFG_RNG_EXTHT_HI_THRESHOLDS);
        CASE_SCALAR(CREATOR_SW_CFG_RNG_EXTHT_LO_THRESHOLDS);
        CASE_SCALAR(CREATOR_SW_CFG_RNG_ALERT_THRESHOLD);
        CASE_SCALAR(CREATOR_SW_CFG_RNG_HEALTH_CONFIG_DIGEST);
        CASE_SCALAR(CREATOR_SW_CFG_SRAM_KEY_RENEW_EN);
        CASE_RANGE(CREATOR_SW_CFG_DIGEST);
        CASE_SCALAR(OWNER_SW_CFG_ROM_ERROR_REPORTING);
        CASE_SCALAR(OWNER_SW_CFG_ROM_BOOTSTRAP_DIS);
        CASE_SCALAR(OWNER_SW_CFG_ROM_ALERT_CLASS_EN);
        CASE_SCALAR(OWNER_SW_CFG_ROM_ALERT_ESCALATION);
        CASE_RANGE(OWNER_SW_CFG_ROM_ALERT_CLASSIFICATION);
        CASE_RANGE(OWNER_SW_CFG_ROM_LOCAL_ALERT_CLASSIFICATION);
        CASE_RANGE(OWNER_SW_CFG_ROM_ALERT_ACCUM_THRESH);
        CASE_RANGE(OWNER_SW_CFG_ROM_ALERT_TIMEOUT_CYCLES);
        CASE_RANGE(OWNER_SW_CFG_ROM_ALERT_PHASE_CYCLES);
        CASE_SCALAR(OWNER_SW_CFG_ROM_ALERT_DIGEST_PROD);
        CASE_SCALAR(OWNER_SW_CFG_ROM_ALERT_DIGEST_PROD_END);
        CASE_SCALAR(OWNER_SW_CFG_ROM_ALERT_DIGEST_DEV);
        CASE_SCALAR(OWNER_SW_CFG_ROM_ALERT_DIGEST_RMA);
        CASE_SCALAR(OWNER_SW_CFG_ROM_WATCHDOG_BITE_THRESHOLD_CYCLES);
        CASE_SCALAR(OWNER_SW_CFG_ROM_KEYMGR_ROM_EXT_MEAS_EN);
        CASE_SCALAR(OWNER_SW_CFG_MANUF_STATE);
        CASE_SCALAR(OWNER_SW_CFG_ROM_RSTMGR_INFO_EN);
        CASE_RANGE(OWNER_SW_CFG_DIGEST);
        CASE_RANGE(DEVICE_ID);
        CASE_RANGE(MANUF_STATE);
        CASE_RANGE(HW_CFG_DIGEST);
        CASE_RANGE(TEST_UNLOCK_TOKEN);
        CASE_RANGE(TEST_EXIT_TOKEN);
        CASE_RANGE(SECRET0_DIGEST);
        CASE_RANGE(FLASH_ADDR_KEY_SEED);
        CASE_RANGE(FLASH_DATA_KEY_SEED);
        CASE_RANGE(SRAM_DATA_KEY_SEED);
        CASE_RANGE(SECRET1_DIGEST);
        CASE_RANGE(RMA_TOKEN);
        CASE_RANGE(CREATOR_ROOT_KEY_SHARE0);
        CASE_RANGE(CREATOR_ROOT_KEY_SHARE1);
        CASE_RANGE(SECRET2_DIGEST);
        CASE_RANGE(LC_TRANSITION_CNT);
        CASE_RANGE(LC_STATE);
    default:
        return "<?>";
    }

#undef CASE_SCALAR
#undef CASE_RANGE
}

static uint64_t ot_otp_eg_swcfg_read(void *opaque, hwaddr addr, unsigned size)
{
    OtOTPEgState *s = OT_OTP_EARLGREY(opaque);
    uint32_t val32;

    g_assert(addr + size <= SW_CFG_WINDOW_SIZE);

    hwaddr reg = R32_OFF(addr);
    int partition = ot_otp_eg_swcfg_get_part(addr);

    if (partition >= 0) {
        if (ot_otp_eg_is_readable(s, partition, (unsigned)addr)) {
            const uint32_t *data = s->otp.data;
            val32 = data[reg];
            ot_otp_eg_set_error(s, partition, OTP_NO_ERROR);
        } else {
            val32 = 0u;
            trace_ot_otp_access_error_on(partition, (unsigned)addr);
            ot_otp_eg_set_error(s, partition, OTP_ACCESS_ERROR);
        }
    } else {
        trace_ot_otp_access_error_on(partition, (unsigned)addr);
        val32 = 0;
    }

    uint64_t pc;

    pc = ibex_get_current_pc();
    trace_ot_otp_io_read_out((unsigned)addr, ot_otp_eg_swcfg_reg_name(reg),
                             val32, pc);

    return (uint64_t)val32;
}

static void ot_otp_eg_swcfg_write(void *opaque, hwaddr addr, uint64_t value,
                                  unsigned size)
{
    (void)opaque;
    (void)value;

    g_assert(addr + size <= SW_CFG_WINDOW_SIZE);

    hwaddr reg = R32_OFF(addr);

    qemu_log_mask(LOG_GUEST_ERROR,
                  "%s: R/O register 0x%03" HWADDR_PRIx " (%s)\n", __func__,
                  addr, ot_otp_eg_swcfg_reg_name(reg));
}

static uint64_t ot_otp_eg_csrs_read(void *opaque, hwaddr addr, unsigned size)
{
    (void)opaque;
    (void)size;
    uint32_t val32;

    hwaddr reg = R32_OFF(addr);
    switch (reg) {
    case R_CSR0:
    case R_CSR1:
    case R_CSR2:
    case R_CSR3:
    case R_CSR4:
    case R_CSR5:
    case R_CSR6:
    case R_CSR7:
        /* TODO: not yet implemented */
        val32 = 0;
        break;
    default:
        qemu_log_mask(LOG_GUEST_ERROR, "%s: Bad offset 0x%" HWADDR_PRIx "\n",
                      __func__, addr);
        val32 = 0;
        break;
    }

    uint64_t pc = ibex_get_current_pc();
    trace_ot_otp_io_read_out((unsigned)addr, CSR_NAME(reg), val32, pc);

    return (uint64_t)val32;
}

static void ot_otp_eg_csrs_write(void *opaque, hwaddr addr, uint64_t value,
                                 unsigned size)
{
    (void)opaque;
    (void)size;
    uint32_t val32 = (uint32_t)value;

    hwaddr reg = R32_OFF(addr);

    uint64_t pc = ibex_get_current_pc();
    trace_ot_otp_io_write((unsigned)addr, CSR_NAME(reg), val32, pc);

    switch (reg) {
    case R_CSR0:
    case R_CSR1:
    case R_CSR2:
    case R_CSR3:
    case R_CSR4:
    case R_CSR5:
    case R_CSR6:
        /* TODO: not yet implemented */
        break;
    case R_CSR7:
        qemu_log_mask(LOG_GUEST_ERROR,
                      "%s: R/O register 0x%01" HWADDR_PRIx " (%s)\n", __func__,
                      addr, CSR_NAME(reg));
        break;
    default:
        qemu_log_mask(LOG_GUEST_ERROR, "%s: Bad offset 0x%" HWADDR_PRIx "\n",
                      __func__, addr);
        break;
    }
}

static void ot_otp_eg_decode_lc_partition(OtOTPEgState *s)
{
    OtOTPStorage *otp = &s->otp;
    s->lc.state = LC_ENCODE_STATE(LC_STATE_INVALID);
    s->lc.tcount = LC_TRANSITION_COUNT_MAX + 1u;
    for (unsigned ix = 0; ix < ARRAY_SIZE(lc_states); ix++) {
        if (!memcmp(&otp->data[R_LC_STATE], lc_states[ix], LC_STATE_SIZE)) {
            s->lc.state = LC_ENCODE_STATE(ix);
            break;
        }
    }
    for (unsigned ix = 0; ix < ARRAY_SIZE(lc_transition_cnts); ix++) {
        if (!memcmp(&otp->data[R_LC_TRANSITION_CNT], lc_transition_cnts[ix],
                    LC_TRANSITION_CNT_SIZE)) {
            s->lc.tcount = ix;
            break;
        }
    }
    trace_ot_otp_lifecycle(s->lc.state, s->lc.tcount);
}

static void ot_otp_eg_load_hw_cfg(OtOTPEgState *s)
{
    OtOTPStorage *otp = &s->otp;
    OtOTPHWCfg *hw_cfg = s->hw_cfg;
    OtOTPEntropyCfg *entropy_cfg = s->entropy_cfg;

    memcpy(hw_cfg->device_id, &otp->data[R_DEVICE_ID],
           sizeof(*hw_cfg->device_id));
    memcpy(hw_cfg->manuf_state, &otp->data[R_MANUF_STATE],
           sizeof(*hw_cfg->manuf_state));
    uint32_t cfg = otp->data[R_HW_CFG_ENABLE];
    hw_cfg->en_sram_ifetch =
        (uint8_t)FIELD_EX32(cfg, HW_CFG_ENABLE, EN_SRAM_IFETCH);

    entropy_cfg->en_csrng_sw_app_read =
        (uint8_t)FIELD_EX32(cfg, HW_CFG_ENABLE, EN_CSRNG_SW_APP_READ);
    entropy_cfg->en_entropy_src_fw_read =
        (uint8_t)FIELD_EX32(cfg, HW_CFG_ENABLE, EN_ENTROPY_SRC_FW_READ);
    entropy_cfg->en_entropy_src_fw_over =
        (uint8_t)FIELD_EX32(cfg, HW_CFG_ENABLE, EN_ENTROPY_SRC_FW_OVER);
}

static void ot_otp_eg_ctrl_get_lc_info(const OtOTPState *s, uint32_t *lc_state,
                                       unsigned *tcount)
{
    OtOTPEgState *ds = OT_OTP_EARLGREY(s);

    if (lc_state) {
        *lc_state = ds->lc.state;
    }
    if (tcount) {
        *tcount = ds->lc.tcount;
    }
}

static const OtOTPHWCfg *ot_otp_eg_ctrl_get_hw_cfg(const OtOTPState *s)
{
    OtOTPEgState *ds = OT_OTP_EARLGREY(s);

    return ds->hw_cfg;
}

static const OtOTPEntropyCfg *
ot_otp_eg_ctrl_get_entropy_cfg(const OtOTPState *s)
{
    OtOTPEgState *ds = OT_OTP_EARLGREY(s);

    return ds->entropy_cfg;
}

static Property ot_otp_eg_properties[] = {
    DEFINE_PROP_DRIVE("drive", OtOTPEgState, blk),
    DEFINE_PROP_LINK("edn", OtOTPEgState, edn, TYPE_OT_EDN, OtEDNState *),
    DEFINE_PROP_UINT8("edn-ep", OtOTPEgState, edn_ep, UINT8_MAX),
    DEFINE_PROP_END_OF_LIST(),
};

static const MemoryRegionOps ot_otp_eg_regs_ops = {
    .read = &ot_otp_eg_regs_read,
    .write = &ot_otp_eg_regs_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
    .impl.min_access_size = 4,
    .impl.max_access_size = 4,
};

static const MemoryRegionOps ot_otp_eg_swcfg_ops = {
    .read = &ot_otp_eg_swcfg_read,
    .write = &ot_otp_eg_swcfg_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
    .impl.min_access_size = 4,
    .impl.max_access_size = 4,
};

static const MemoryRegionOps ot_otp_eg_csrs_ops = {
    .read = &ot_otp_eg_csrs_read,
    .write = &ot_otp_eg_csrs_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
    .impl.min_access_size = 4,
    .impl.max_access_size = 4,
};

static void ot_otp_eg_reset(DeviceState *dev)
{
    OtOTPEgState *s = OT_OTP_EARLGREY(dev);

    timer_del(s->dai_delay);

    memset(&s->regs, 0, sizeof(s->regs));

    s->regs[R_DIRECT_ACCESS_REGWEN] = 0x1u;
    s->regs[R_CHECK_TRIGGER_REGWEN] = 0x1u;
    s->regs[R_CHECK_REGWEN] = 0x1u;
    s->regs[R_VENDOR_TEST_READ_LOCK] = 0x1u;
    s->regs[R_CREATOR_SW_CFG_READ_LOCK] = 0x1u;
    s->regs[R_OWNER_SW_CFG_READ_LOCK] = 0x1u;
    s->dai_busy = false;

    ot_otp_eg_update_irqs(s);
    ibex_irq_set(&s->alert, 0);
}

static void ot_otp_eg_load(OtOTPEgState *s, Error **errp)
{
    /*
     * HEADER_FORMAT
     *
     *  | magic    | 4 char   | "vOFTP"                                |
     *  | hlength  | uint32_t | count of header bytes after this point |
     *  | version  | uint32_t | version of the header (v1)             |
     *  | eccbits  | uint16_t | ECC size in bits                       |
     *  | eccgran  | uint16_t | ECC granule                            |
     *  | dlength  | uint32_t | count of data bytes (% uint64_t)       |
     *  | elength  | uint32_t | count of ecc bytes (% uint64_t)        |
     */

    struct otp_header {
        char magic[4];
        uint32_t hlength;
        uint32_t version;
        uint16_t eccbits;
        uint16_t eccgran;
        uint32_t data_len;
        uint32_t ecc_len;
    };

    /* data following header should always be 64-bit aligned */
    static_assert((sizeof(struct otp_header) % sizeof(uint64_t)) == 0,
                  "invalid header definition");

    size_t header_size = sizeof(struct otp_header);
    size_t data_size = 0u;
    size_t ecc_size = 0u;

    for (unsigned ix = 0u; ix < ARRAY_SIZE(OtOTPPartitions); ix++) {
        size_t psize = (size_t)OtOTPPartitions[ix].size;
        size_t dsize = ROUND_UP(psize, sizeof(uint64_t));
        data_size += dsize;
        /* up to 1 ECC byte for 2 data bytes */
        ecc_size += DIV_ROUND_UP(dsize, 2u);
    }
    size_t otp_size = header_size + data_size + ecc_size;

    otp_size = ROUND_UP(otp_size, 4096u);

    OtOTPStorage *otp = &s->otp;

    otp->storage = blk_blockalign(s->blk, otp_size);
    uintptr_t base = (uintptr_t)otp->storage;
    g_assert(!(base & (sizeof(uint64_t) - 1u)));

    if (s->blk) {
        uint64_t perm = BLK_PERM_CONSISTENT_READ |
                        (blk_supports_write_perm(s->blk) ? BLK_PERM_WRITE : 0);
        (void)blk_set_perm(s->blk, perm, perm, errp);

        int rc = blk_pread(s->blk, 0, (int64_t)otp_size, otp->storage, 0);
        if (rc < 0) {
            error_setg(errp, "failed to read the initial OTP content: %d", rc);
            return;
        }

        const struct otp_header *otp_hdr = (const struct otp_header *)base;

        if (memcmp(otp_hdr->magic, "vOTP", sizeof(otp_hdr->magic)) != 0) {
            error_setg(errp, "OTP file is not a valid OTP backend");
            return;
        }
        if (otp_hdr->version != 1u) {
            error_setg(errp, "OTP file version is not supported");
            return;
        }

        uintptr_t data_offset = otp_hdr->hlength + 8u;
        uintptr_t ecc_offset = data_offset + otp_hdr->data_len;

        otp->data = (uint32_t *)(base + data_offset);
        otp->ecc = (uint32_t *)(base + ecc_offset);
        otp->ecc_bit_count = otp_hdr->eccbits;
        otp->ecc_granule = otp_hdr->eccgran;
    } else {
        memset(otp->storage, 0, otp_size);

        otp->data = (uint32_t *)(base + sizeof(struct otp_header));
        otp->ecc = NULL;
        otp->ecc_bit_count = 0u;
        otp->ecc_granule = 0u;
    }

    otp->data_size = data_size;
    otp->ecc_size = ecc_size;

    ot_otp_eg_decode_lc_partition(s);
    ot_otp_eg_load_hw_cfg(s);
}

static void ot_otp_eg_realize(DeviceState *dev, Error **errp)
{
    OtOTPEgState *s = OT_OTP_EARLGREY(dev);
    (void)errp;

    ot_otp_eg_load(s, &error_fatal);
}

static void ot_otp_eg_init(Object *obj)
{
    OtOTPEgState *s = OT_OTP_EARLGREY(obj);

    memory_region_init(&s->mmio.ctrl, obj, TYPE_OT_OTP "-ctrl", 0x2000u);
    sysbus_init_mmio(SYS_BUS_DEVICE(s), &s->mmio.ctrl);

    memory_region_init_io(&s->mmio.sub.regs, obj, &ot_otp_eg_regs_ops, s,
                          TYPE_OT_OTP "-regs", REGS_SIZE);
    memory_region_add_subregion(&s->mmio.ctrl, 0u, &s->mmio.sub.regs);

    /* TODO: it might be worthwhile to use a ROM-kind here */
    memory_region_init_io(&s->mmio.sub.swcfg, obj, &ot_otp_eg_swcfg_ops, s,
                          TYPE_OT_OTP "-swcfg", SW_CFG_WINDOW_SIZE);
    memory_region_add_subregion(&s->mmio.ctrl, SW_CFG_WINDOW,
                                &s->mmio.sub.swcfg);

    memory_region_init_io(&s->prim.csrs, obj, &ot_otp_eg_csrs_ops, s,
                          TYPE_OT_OTP "-prim", CSRS_SIZE);
    sysbus_init_mmio(SYS_BUS_DEVICE(s), &s->prim.csrs);

    for (unsigned ix = 0; ix < ARRAY_SIZE(s->irqs); ix++) {
        ibex_sysbus_init_irq(obj, &s->irqs[ix]);
    }
    ibex_qdev_init_irq(obj, &s->alert, OPENTITAN_DEVICE_ALERT);

    s->hw_cfg = g_new0(OtOTPHWCfg, 1u);
    s->entropy_cfg = g_new0(OtOTPEntropyCfg, 1u);
    s->dai_delay = timer_new_ns(QEMU_CLOCK_VIRTUAL, &ot_otp_eg_complete_dai, s);
}

static void ot_otp_eg_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    (void)data;

    dc->reset = &ot_otp_eg_reset;
    dc->realize = &ot_otp_eg_realize;
    device_class_set_props(dc, ot_otp_eg_properties);
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);

    OtOTPStateClass *odc = OT_OTP_CLASS(klass);

    odc->get_lc_info = &ot_otp_eg_ctrl_get_lc_info;
    odc->get_hw_cfg = &ot_otp_eg_ctrl_get_hw_cfg;
    odc->get_entropy_cfg = &ot_otp_eg_ctrl_get_entropy_cfg;
}

static const TypeInfo ot_otp_eg_info = {
    .name = TYPE_OT_OTP_EARLGREY,
    .parent = TYPE_OT_OTP,
    .instance_size = sizeof(OtOTPEgState),
    .instance_init = &ot_otp_eg_init,
    .class_size = sizeof(OtOTPStateClass),
    .class_init = &ot_otp_eg_class_init,
};

static void ot_otp_eg_register_types(void)
{
    type_register_static(&ot_otp_eg_info);
}

type_init(ot_otp_eg_register_types)
