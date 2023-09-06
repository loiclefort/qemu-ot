/*
 * QEMU OpenTitan EarlGrey One Time Programmable (OTP) life cycle values
 *
 * Copyright (c) 2023 Rivos, Inc.
 *
 * Author(s):
 *  Emmanuel Blot <eblot@rivosinc.com>
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

#include "ot_lcstate.h"

/* Section auto-generated with otpconv.py [-i/-c/-h/-O lc_arrays] script */

/* clang-format off */
static const char lc_states[21u][40u] = {
    [LC_STATE_RAW] = {
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u
    },
    [LC_STATE_TESTUNLOCKED0] = {
        0xf2u, 0x9fu, 0x2eu, 0xb0u, 0x11u, 0xe2u, 0x90u, 0xc9u, 0x21u, 0x0fu,
        0xb1u, 0xd4u, 0x30u, 0x2bu, 0x32u, 0x3du, 0xb0u, 0xe8u, 0x1du, 0xf4u,
        0xa5u, 0x99u, 0x85u, 0xe4u, 0x77u, 0x49u, 0x73u, 0x2cu, 0x6cu, 0x91u,
        0x0du, 0x30u, 0x15u, 0xa6u, 0x2eu, 0x61u, 0xb0u, 0xc3u, 0x83u, 0xc1u
    },
    [LC_STATE_TESTLOCKED0] = {
        0xf2u, 0x9fu, 0x3fu, 0xb4u, 0x11u, 0xe2u, 0x90u, 0xc9u, 0x21u, 0x0fu,
        0xb1u, 0xd4u, 0x30u, 0x2bu, 0x32u, 0x3du, 0xb0u, 0xe8u, 0x1du, 0xf4u,
        0xa5u, 0x99u, 0x85u, 0xe4u, 0x77u, 0x49u, 0x73u, 0x2cu, 0x6cu, 0x91u,
        0x0du, 0x30u, 0x15u, 0xa6u, 0x2eu, 0x61u, 0xb0u, 0xc3u, 0x83u, 0xc1u
    },
    [LC_STATE_TESTUNLOCKED1] = {
        0xf2u, 0x9fu, 0x3fu, 0xb4u, 0x1fu, 0xe3u, 0x90u, 0xc9u, 0x21u, 0x0fu,
        0xb1u, 0xd4u, 0x30u, 0x2bu, 0x32u, 0x3du, 0xb0u, 0xe8u, 0x1du, 0xf4u,
        0xa5u, 0x99u, 0x85u, 0xe4u, 0x77u, 0x49u, 0x73u, 0x2cu, 0x6cu, 0x91u,
        0x0du, 0x30u, 0x15u, 0xa6u, 0x2eu, 0x61u, 0xb0u, 0xc3u, 0x83u, 0xc1u
    },
    [LC_STATE_TESTLOCKED1] = {
        0xf2u, 0x9fu, 0x3fu, 0xb4u, 0x1fu, 0xe3u, 0xd2u, 0xfdu, 0x21u, 0x0fu,
        0xb1u, 0xd4u, 0x30u, 0x2bu, 0x32u, 0x3du, 0xb0u, 0xe8u, 0x1du, 0xf4u,
        0xa5u, 0x99u, 0x85u, 0xe4u, 0x77u, 0x49u, 0x73u, 0x2cu, 0x6cu, 0x91u,
        0x0du, 0x30u, 0x15u, 0xa6u, 0x2eu, 0x61u, 0xb0u, 0xc3u, 0x83u, 0xc1u
    },
    [LC_STATE_TESTUNLOCKED2] = {
        0xf2u, 0x9fu, 0x3fu, 0xb4u, 0x1fu, 0xe3u, 0xd2u, 0xfdu, 0xa7u, 0xafu,
        0xb1u, 0xd4u, 0x30u, 0x2bu, 0x32u, 0x3du, 0xb0u, 0xe8u, 0x1du, 0xf4u,
        0xa5u, 0x99u, 0x85u, 0xe4u, 0x77u, 0x49u, 0x73u, 0x2cu, 0x6cu, 0x91u,
        0x0du, 0x30u, 0x15u, 0xa6u, 0x2eu, 0x61u, 0xb0u, 0xc3u, 0x83u, 0xc1u
    },
    [LC_STATE_TESTLOCKED2] = {
        0xf2u, 0x9fu, 0x3fu, 0xb4u, 0x1fu, 0xe3u, 0xd2u, 0xfdu, 0xa7u, 0xafu,
        0xffu, 0xd6u, 0x30u, 0x2bu, 0x32u, 0x3du, 0xb0u, 0xe8u, 0x1du, 0xf4u,
        0xa5u, 0x99u, 0x85u, 0xe4u, 0x77u, 0x49u, 0x73u, 0x2cu, 0x6cu, 0x91u,
        0x0du, 0x30u, 0x15u, 0xa6u, 0x2eu, 0x61u, 0xb0u, 0xc3u, 0x83u, 0xc1u
    },
    [LC_STATE_TESTUNLOCKED3] = {
        0xf2u, 0x9fu, 0x3fu, 0xb4u, 0x1fu, 0xe3u, 0xd2u, 0xfdu, 0xa7u, 0xafu,
        0xffu, 0xd6u, 0x76u, 0xabu, 0x32u, 0x3du, 0xb0u, 0xe8u, 0x1du, 0xf4u,
        0xa5u, 0x99u, 0x85u, 0xe4u, 0x77u, 0x49u, 0x73u, 0x2cu, 0x6cu, 0x91u,
        0x0du, 0x30u, 0x15u, 0xa6u, 0x2eu, 0x61u, 0xb0u, 0xc3u, 0x83u, 0xc1u
    },
    [LC_STATE_TESTLOCKED3] = {
        0xf2u, 0x9fu, 0x3fu, 0xb4u, 0x1fu, 0xe3u, 0xd2u, 0xfdu, 0xa7u, 0xafu,
        0xffu, 0xd6u, 0x76u, 0xabu, 0xb3u, 0xffu, 0xb0u, 0xe8u, 0x1du, 0xf4u,
        0xa5u, 0x99u, 0x85u, 0xe4u, 0x77u, 0x49u, 0x73u, 0x2cu, 0x6cu, 0x91u,
        0x0du, 0x30u, 0x15u, 0xa6u, 0x2eu, 0x61u, 0xb0u, 0xc3u, 0x83u, 0xc1u
    },
    [LC_STATE_TESTUNLOCKED4] = {
        0xf2u, 0x9fu, 0x3fu, 0xb4u, 0x1fu, 0xe3u, 0xd2u, 0xfdu, 0xa7u, 0xafu,
        0xffu, 0xd6u, 0x76u, 0xabu, 0xb3u, 0xffu, 0xbau, 0xeeu, 0x1du, 0xf4u,
        0xa5u, 0x99u, 0x85u, 0xe4u, 0x77u, 0x49u, 0x73u, 0x2cu, 0x6cu, 0x91u,
        0x0du, 0x30u, 0x15u, 0xa6u, 0x2eu, 0x61u, 0xb0u, 0xc3u, 0x83u, 0xc1u
    },
    [LC_STATE_TESTLOCKED4] = {
        0xf2u, 0x9fu, 0x3fu, 0xb4u, 0x1fu, 0xe3u, 0xd2u, 0xfdu, 0xa7u, 0xafu,
        0xffu, 0xd6u, 0x76u, 0xabu, 0xb3u, 0xffu, 0xbau, 0xeeu, 0xffu, 0xf4u,
        0xa5u, 0x99u, 0x85u, 0xe4u, 0x77u, 0x49u, 0x73u, 0x2cu, 0x6cu, 0x91u,
        0x0du, 0x30u, 0x15u, 0xa6u, 0x2eu, 0x61u, 0xb0u, 0xc3u, 0x83u, 0xc1u
    },
    [LC_STATE_TESTUNLOCKED5] = {
        0xf2u, 0x9fu, 0x3fu, 0xb4u, 0x1fu, 0xe3u, 0xd2u, 0xfdu, 0xa7u, 0xafu,
        0xffu, 0xd6u, 0x76u, 0xabu, 0xb3u, 0xffu, 0xbau, 0xeeu, 0xffu, 0xf4u,
        0xa5u, 0xffu, 0x85u, 0xe4u, 0x77u, 0x49u, 0x73u, 0x2cu, 0x6cu, 0x91u,
        0x0du, 0x30u, 0x15u, 0xa6u, 0x2eu, 0x61u, 0xb0u, 0xc3u, 0x83u, 0xc1u
    },
    [LC_STATE_TESTLOCKED5] = {
        0xf2u, 0x9fu, 0x3fu, 0xb4u, 0x1fu, 0xe3u, 0xd2u, 0xfdu, 0xa7u, 0xafu,
        0xffu, 0xd6u, 0x76u, 0xabu, 0xb3u, 0xffu, 0xbau, 0xeeu, 0xffu, 0xf4u,
        0xa5u, 0xffu, 0xefu, 0xe4u, 0x77u, 0x49u, 0x73u, 0x2cu, 0x6cu, 0x91u,
        0x0du, 0x30u, 0x15u, 0xa6u, 0x2eu, 0x61u, 0xb0u, 0xc3u, 0x83u, 0xc1u
    },
    [LC_STATE_TESTUNLOCKED6] = {
        0xf2u, 0x9fu, 0x3fu, 0xb4u, 0x1fu, 0xe3u, 0xd2u, 0xfdu, 0xa7u, 0xafu,
        0xffu, 0xd6u, 0x76u, 0xabu, 0xb3u, 0xffu, 0xbau, 0xeeu, 0xffu, 0xf4u,
        0xa5u, 0xffu, 0xefu, 0xe4u, 0xffu, 0x4fu, 0x73u, 0x2cu, 0x6cu, 0x91u,
        0x0du, 0x30u, 0x15u, 0xa6u, 0x2eu, 0x61u, 0xb0u, 0xc3u, 0x83u, 0xc1u
    },
    [LC_STATE_TESTLOCKED6] = {
        0xf2u, 0x9fu, 0x3fu, 0xb4u, 0x1fu, 0xe3u, 0xd2u, 0xfdu, 0xa7u, 0xafu,
        0xffu, 0xd6u, 0x76u, 0xabu, 0xb3u, 0xffu, 0xbau, 0xeeu, 0xffu, 0xf4u,
        0xa5u, 0xffu, 0xefu, 0xe4u, 0xffu, 0x4fu, 0x7fu, 0xbeu, 0x6cu, 0x91u,
        0x0du, 0x30u, 0x15u, 0xa6u, 0x2eu, 0x61u, 0xb0u, 0xc3u, 0x83u, 0xc1u
    },
    [LC_STATE_TESTUNLOCKED7] = {
        0xf2u, 0x9fu, 0x3fu, 0xb4u, 0x1fu, 0xe3u, 0xd2u, 0xfdu, 0xa7u, 0xafu,
        0xffu, 0xd6u, 0x76u, 0xabu, 0xb3u, 0xffu, 0xbau, 0xeeu, 0xffu, 0xf4u,
        0xa5u, 0xffu, 0xefu, 0xe4u, 0xffu, 0x4fu, 0x7fu, 0xbeu, 0xedu, 0x9du,
        0x0du, 0x30u, 0x15u, 0xa6u, 0x2eu, 0x61u, 0xb0u, 0xc3u, 0x83u, 0xc1u
    },
    [LC_STATE_DEV] = {
        0xf2u, 0x9fu, 0x3fu, 0xb4u, 0x1fu, 0xe3u, 0xd2u, 0xfdu, 0xa7u, 0xafu,
        0xffu, 0xd6u, 0x76u, 0xabu, 0xb3u, 0xffu, 0xbau, 0xeeu, 0xffu, 0xf4u,
        0xa5u, 0xffu, 0xefu, 0xe4u, 0xffu, 0x4fu, 0x7fu, 0xbeu, 0xedu, 0x9du,
        0xdfu, 0xf2u, 0x15u, 0xa6u, 0x2eu, 0x61u, 0xb0u, 0xc3u, 0x83u, 0xc1u
    },
    [LC_STATE_PROD] = {
        0xf2u, 0x9fu, 0x3fu, 0xb4u, 0x1fu, 0xe3u, 0xd2u, 0xfdu, 0xa7u, 0xafu,
        0xffu, 0xd6u, 0x76u, 0xabu, 0xb3u, 0xffu, 0xbau, 0xeeu, 0xffu, 0xf4u,
        0xa5u, 0xffu, 0xefu, 0xe4u, 0xffu, 0x4fu, 0x7fu, 0xbeu, 0xedu, 0x9du,
        0x0du, 0x30u, 0x9du, 0xb7u, 0x2eu, 0x61u, 0xb0u, 0xc3u, 0x83u, 0xc1u
    },
    [LC_STATE_PRODEND] = {
        0xf2u, 0x9fu, 0x3fu, 0xb4u, 0x1fu, 0xe3u, 0xd2u, 0xfdu, 0xa7u, 0xafu,
        0xffu, 0xd6u, 0x76u, 0xabu, 0xb3u, 0xffu, 0xbau, 0xeeu, 0xffu, 0xf4u,
        0xa5u, 0xffu, 0xefu, 0xe4u, 0xffu, 0x4fu, 0x7fu, 0xbeu, 0xedu, 0x9du,
        0x0du, 0x30u, 0x15u, 0xa6u, 0x7eu, 0xe5u, 0xb0u, 0xc3u, 0x83u, 0xc1u
    },
    [LC_STATE_RMA] = {
        0xf2u, 0x9fu, 0x3fu, 0xb4u, 0x1fu, 0xe3u, 0xd2u, 0xfdu, 0xa7u, 0xafu,
        0xffu, 0xd6u, 0x76u, 0xabu, 0xb3u, 0xffu, 0xbau, 0xeeu, 0xffu, 0xf4u,
        0xa5u, 0xffu, 0xefu, 0xe4u, 0xffu, 0x4fu, 0x7fu, 0xbeu, 0xedu, 0x9du,
        0xdfu, 0xf2u, 0x9du, 0xb7u, 0x2eu, 0x61u, 0xb7u, 0xd3u, 0xd7u, 0xe5u
    },
    [LC_STATE_SCRAP] = {
        0xf2u, 0x9fu, 0x3fu, 0xb4u, 0x1fu, 0xe3u, 0xd2u, 0xfdu, 0xa7u, 0xafu,
        0xffu, 0xd6u, 0x76u, 0xabu, 0xb3u, 0xffu, 0xbau, 0xeeu, 0xffu, 0xf4u,
        0xa5u, 0xffu, 0xefu, 0xe4u, 0xffu, 0x4fu, 0x7fu, 0xbeu, 0xedu, 0x9du,
        0xdfu, 0xf2u, 0x9du, 0xb7u, 0x7eu, 0xe5u, 0xb7u, 0xd3u, 0xd7u, 0xe5u
    },
};

static const char lc_transition_cnts[25u][48u] = {
    [0u] = {
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u
    },
    [1u] = {
        0x3cu, 0xfcu, 0x83u, 0x21u, 0xc4u, 0xf8u, 0x18u, 0xacu, 0x4du, 0x53u,
        0xd2u, 0x44u, 0xa4u, 0xc4u, 0x63u, 0x1eu, 0x90u, 0x65u, 0x64u, 0x23u,
        0x00u, 0x4bu, 0x81u, 0xbau, 0xaau, 0x5bu, 0x69u, 0x2cu, 0x13u, 0xf2u,
        0xf2u, 0x1du, 0x60u, 0x9bu, 0x68u, 0x5eu, 0xc4u, 0x5du, 0x05u, 0x04u,
        0x28u, 0x76u, 0xe8u, 0x62u, 0x8au, 0x8bu, 0x0du, 0xd0u
    },
    [2u] = {
        0x3cu, 0xfcu, 0xfbu, 0x23u, 0xc4u, 0xf8u, 0x18u, 0xacu, 0x4du, 0x53u,
        0xd2u, 0x44u, 0xa4u, 0xc4u, 0x63u, 0x1eu, 0x90u, 0x65u, 0x64u, 0x23u,
        0x00u, 0x4bu, 0x81u, 0xbau, 0xaau, 0x5bu, 0x69u, 0x2cu, 0x13u, 0xf2u,
        0xf2u, 0x1du, 0x60u, 0x9bu, 0x68u, 0x5eu, 0xc4u, 0x5du, 0x05u, 0x04u,
        0x28u, 0x76u, 0xe8u, 0x62u, 0x8au, 0x8bu, 0x0du, 0xd0u
    },
    [3u] = {
        0x3cu, 0xfcu, 0xfbu, 0x23u, 0xeeu, 0xf9u, 0x18u, 0xacu, 0x4du, 0x53u,
        0xd2u, 0x44u, 0xa4u, 0xc4u, 0x63u, 0x1eu, 0x90u, 0x65u, 0x64u, 0x23u,
        0x00u, 0x4bu, 0x81u, 0xbau, 0xaau, 0x5bu, 0x69u, 0x2cu, 0x13u, 0xf2u,
        0xf2u, 0x1du, 0x60u, 0x9bu, 0x68u, 0x5eu, 0xc4u, 0x5du, 0x05u, 0x04u,
        0x28u, 0x76u, 0xe8u, 0x62u, 0x8au, 0x8bu, 0x0du, 0xd0u
    },
    [4u] = {
        0x3cu, 0xfcu, 0xfbu, 0x23u, 0xeeu, 0xf9u, 0x9fu, 0xadu, 0x4du, 0x53u,
        0xd2u, 0x44u, 0xa4u, 0xc4u, 0x63u, 0x1eu, 0x90u, 0x65u, 0x64u, 0x23u,
        0x00u, 0x4bu, 0x81u, 0xbau, 0xaau, 0x5bu, 0x69u, 0x2cu, 0x13u, 0xf2u,
        0xf2u, 0x1du, 0x60u, 0x9bu, 0x68u, 0x5eu, 0xc4u, 0x5du, 0x05u, 0x04u,
        0x28u, 0x76u, 0xe8u, 0x62u, 0x8au, 0x8bu, 0x0du, 0xd0u
    },
    [5u] = {
        0x3cu, 0xfcu, 0xfbu, 0x23u, 0xeeu, 0xf9u, 0x9fu, 0xadu, 0x6fu, 0x7fu,
        0xd2u, 0x44u, 0xa4u, 0xc4u, 0x63u, 0x1eu, 0x90u, 0x65u, 0x64u, 0x23u,
        0x00u, 0x4bu, 0x81u, 0xbau, 0xaau, 0x5bu, 0x69u, 0x2cu, 0x13u, 0xf2u,
        0xf2u, 0x1du, 0x60u, 0x9bu, 0x68u, 0x5eu, 0xc4u, 0x5du, 0x05u, 0x04u,
        0x28u, 0x76u, 0xe8u, 0x62u, 0x8au, 0x8bu, 0x0du, 0xd0u
    },
    [6u] = {
        0x3cu, 0xfcu, 0xfbu, 0x23u, 0xeeu, 0xf9u, 0x9fu, 0xadu, 0x6fu, 0x7fu,
        0xfbu, 0x44u, 0xa4u, 0xc4u, 0x63u, 0x1eu, 0x90u, 0x65u, 0x64u, 0x23u,
        0x00u, 0x4bu, 0x81u, 0xbau, 0xaau, 0x5bu, 0x69u, 0x2cu, 0x13u, 0xf2u,
        0xf2u, 0x1du, 0x60u, 0x9bu, 0x68u, 0x5eu, 0xc4u, 0x5du, 0x05u, 0x04u,
        0x28u, 0x76u, 0xe8u, 0x62u, 0x8au, 0x8bu, 0x0du, 0xd0u
    },
    [7u] = {
        0x3cu, 0xfcu, 0xfbu, 0x23u, 0xeeu, 0xf9u, 0x9fu, 0xadu, 0x6fu, 0x7fu,
        0xfbu, 0x44u, 0xe6u, 0xceu, 0x63u, 0x1eu, 0x90u, 0x65u, 0x64u, 0x23u,
        0x00u, 0x4bu, 0x81u, 0xbau, 0xaau, 0x5bu, 0x69u, 0x2cu, 0x13u, 0xf2u,
        0xf2u, 0x1du, 0x60u, 0x9bu, 0x68u, 0x5eu, 0xc4u, 0x5du, 0x05u, 0x04u,
        0x28u, 0x76u, 0xe8u, 0x62u, 0x8au, 0x8bu, 0x0du, 0xd0u
    },
    [8u] = {
        0x3cu, 0xfcu, 0xfbu, 0x23u, 0xeeu, 0xf9u, 0x9fu, 0xadu, 0x6fu, 0x7fu,
        0xfbu, 0x44u, 0xe6u, 0xceu, 0x7bu, 0x5eu, 0x90u, 0x65u, 0x64u, 0x23u,
        0x00u, 0x4bu, 0x81u, 0xbau, 0xaau, 0x5bu, 0x69u, 0x2cu, 0x13u, 0xf2u,
        0xf2u, 0x1du, 0x60u, 0x9bu, 0x68u, 0x5eu, 0xc4u, 0x5du, 0x05u, 0x04u,
        0x28u, 0x76u, 0xe8u, 0x62u, 0x8au, 0x8bu, 0x0du, 0xd0u
    },
    [9u] = {
        0x3cu, 0xfcu, 0xfbu, 0x23u, 0xeeu, 0xf9u, 0x9fu, 0xadu, 0x6fu, 0x7fu,
        0xfbu, 0x44u, 0xe6u, 0xceu, 0x7bu, 0x5eu, 0xd4u, 0x77u, 0x64u, 0x23u,
        0x00u, 0x4bu, 0x81u, 0xbau, 0xaau, 0x5bu, 0x69u, 0x2cu, 0x13u, 0xf2u,
        0xf2u, 0x1du, 0x60u, 0x9bu, 0x68u, 0x5eu, 0xc4u, 0x5du, 0x05u, 0x04u,
        0x28u, 0x76u, 0xe8u, 0x62u, 0x8au, 0x8bu, 0x0du, 0xd0u
    },
    [10u] = {
        0x3cu, 0xfcu, 0xfbu, 0x23u, 0xeeu, 0xf9u, 0x9fu, 0xadu, 0x6fu, 0x7fu,
        0xfbu, 0x44u, 0xe6u, 0xceu, 0x7bu, 0x5eu, 0xd4u, 0x77u, 0x67u, 0xe7u,
        0x00u, 0x4bu, 0x81u, 0xbau, 0xaau, 0x5bu, 0x69u, 0x2cu, 0x13u, 0xf2u,
        0xf2u, 0x1du, 0x60u, 0x9bu, 0x68u, 0x5eu, 0xc4u, 0x5du, 0x05u, 0x04u,
        0x28u, 0x76u, 0xe8u, 0x62u, 0x8au, 0x8bu, 0x0du, 0xd0u
    },
    [11u] = {
        0x3cu, 0xfcu, 0xfbu, 0x23u, 0xeeu, 0xf9u, 0x9fu, 0xadu, 0x6fu, 0x7fu,
        0xfbu, 0x44u, 0xe6u, 0xceu, 0x7bu, 0x5eu, 0xd4u, 0x77u, 0x67u, 0xe7u,
        0x53u, 0xcbu, 0x81u, 0xbau, 0xaau, 0x5bu, 0x69u, 0x2cu, 0x13u, 0xf2u,
        0xf2u, 0x1du, 0x60u, 0x9bu, 0x68u, 0x5eu, 0xc4u, 0x5du, 0x05u, 0x04u,
        0x28u, 0x76u, 0xe8u, 0x62u, 0x8au, 0x8bu, 0x0du, 0xd0u
    },
    [12u] = {
        0x3cu, 0xfcu, 0xfbu, 0x23u, 0xeeu, 0xf9u, 0x9fu, 0xadu, 0x6fu, 0x7fu,
        0xfbu, 0x44u, 0xe6u, 0xceu, 0x7bu, 0x5eu, 0xd4u, 0x77u, 0x67u, 0xe7u,
        0x53u, 0xcbu, 0xabu, 0xfeu, 0xaau, 0x5bu, 0x69u, 0x2cu, 0x13u, 0xf2u,
        0xf2u, 0x1du, 0x60u, 0x9bu, 0x68u, 0x5eu, 0xc4u, 0x5du, 0x05u, 0x04u,
        0x28u, 0x76u, 0xe8u, 0x62u, 0x8au, 0x8bu, 0x0du, 0xd0u
    },
    [13u] = {
        0x3cu, 0xfcu, 0xfbu, 0x23u, 0xeeu, 0xf9u, 0x9fu, 0xadu, 0x6fu, 0x7fu,
        0xfbu, 0x44u, 0xe6u, 0xceu, 0x7bu, 0x5eu, 0xd4u, 0x77u, 0x67u, 0xe7u,
        0x53u, 0xcbu, 0xabu, 0xfeu, 0xbfu, 0x5fu, 0x69u, 0x2cu, 0x13u, 0xf2u,
        0xf2u, 0x1du, 0x60u, 0x9bu, 0x68u, 0x5eu, 0xc4u, 0x5du, 0x05u, 0x04u,
        0x28u, 0x76u, 0xe8u, 0x62u, 0x8au, 0x8bu, 0x0du, 0xd0u
    },
    [14u] = {
        0x3cu, 0xfcu, 0xfbu, 0x23u, 0xeeu, 0xf9u, 0x9fu, 0xadu, 0x6fu, 0x7fu,
        0xfbu, 0x44u, 0xe6u, 0xceu, 0x7bu, 0x5eu, 0xd4u, 0x77u, 0x67u, 0xe7u,
        0x53u, 0xcbu, 0xabu, 0xfeu, 0xbfu, 0x5fu, 0xe9u, 0x6eu, 0x13u, 0xf2u,
        0xf2u, 0x1du, 0x60u, 0x9bu, 0x68u, 0x5eu, 0xc4u, 0x5du, 0x05u, 0x04u,
        0x28u, 0x76u, 0xe8u, 0x62u, 0x8au, 0x8bu, 0x0du, 0xd0u
    },
    [15u] = {
        0x3cu, 0xfcu, 0xfbu, 0x23u, 0xeeu, 0xf9u, 0x9fu, 0xadu, 0x6fu, 0x7fu,
        0xfbu, 0x44u, 0xe6u, 0xceu, 0x7bu, 0x5eu, 0xd4u, 0x77u, 0x67u, 0xe7u,
        0x53u, 0xcbu, 0xabu, 0xfeu, 0xbfu, 0x5fu, 0xe9u, 0x6eu, 0x77u, 0xf3u,
        0xf2u, 0x1du, 0x60u, 0x9bu, 0x68u, 0x5eu, 0xc4u, 0x5du, 0x05u, 0x04u,
        0x28u, 0x76u, 0xe8u, 0x62u, 0x8au, 0x8bu, 0x0du, 0xd0u
    },
    [16u] = {
        0x3cu, 0xfcu, 0xfbu, 0x23u, 0xeeu, 0xf9u, 0x9fu, 0xadu, 0x6fu, 0x7fu,
        0xfbu, 0x44u, 0xe6u, 0xceu, 0x7bu, 0x5eu, 0xd4u, 0x77u, 0x67u, 0xe7u,
        0x53u, 0xcbu, 0xabu, 0xfeu, 0xbfu, 0x5fu, 0xe9u, 0x6eu, 0x77u, 0xf3u,
        0xf3u, 0x5du, 0x60u, 0x9bu, 0x68u, 0x5eu, 0xc4u, 0x5du, 0x05u, 0x04u,
        0x28u, 0x76u, 0xe8u, 0x62u, 0x8au, 0x8bu, 0x0du, 0xd0u
    },
    [17u] = {
        0x3cu, 0xfcu, 0xfbu, 0x23u, 0xeeu, 0xf9u, 0x9fu, 0xadu, 0x6fu, 0x7fu,
        0xfbu, 0x44u, 0xe6u, 0xceu, 0x7bu, 0x5eu, 0xd4u, 0x77u, 0x67u, 0xe7u,
        0x53u, 0xcbu, 0xabu, 0xfeu, 0xbfu, 0x5fu, 0xe9u, 0x6eu, 0x77u, 0xf3u,
        0xf3u, 0x5du, 0x6fu, 0x9fu, 0x68u, 0x5eu, 0xc4u, 0x5du, 0x05u, 0x04u,
        0x28u, 0x76u, 0xe8u, 0x62u, 0x8au, 0x8bu, 0x0du, 0xd0u
    },
    [18u] = {
        0x3cu, 0xfcu, 0xfbu, 0x23u, 0xeeu, 0xf9u, 0x9fu, 0xadu, 0x6fu, 0x7fu,
        0xfbu, 0x44u, 0xe6u, 0xceu, 0x7bu, 0x5eu, 0xd4u, 0x77u, 0x67u, 0xe7u,
        0x53u, 0xcbu, 0xabu, 0xfeu, 0xbfu, 0x5fu, 0xe9u, 0x6eu, 0x77u, 0xf3u,
        0xf3u, 0x5du, 0x6fu, 0x9fu, 0x68u, 0xffu, 0xc4u, 0x5du, 0x05u, 0x04u,
        0x28u, 0x76u, 0xe8u, 0x62u, 0x8au, 0x8bu, 0x0du, 0xd0u
    },
    [19u] = {
        0x3cu, 0xfcu, 0xfbu, 0x23u, 0xeeu, 0xf9u, 0x9fu, 0xadu, 0x6fu, 0x7fu,
        0xfbu, 0x44u, 0xe6u, 0xceu, 0x7bu, 0x5eu, 0xd4u, 0x77u, 0x67u, 0xe7u,
        0x53u, 0xcbu, 0xabu, 0xfeu, 0xbfu, 0x5fu, 0xe9u, 0x6eu, 0x77u, 0xf3u,
        0xf3u, 0x5du, 0x6fu, 0x9fu, 0x68u, 0xffu, 0xdeu, 0x5du, 0x05u, 0x04u,
        0x28u, 0x76u, 0xe8u, 0x62u, 0x8au, 0x8bu, 0x0du, 0xd0u
    },
    [20u] = {
        0x3cu, 0xfcu, 0xfbu, 0x23u, 0xeeu, 0xf9u, 0x9fu, 0xadu, 0x6fu, 0x7fu,
        0xfbu, 0x44u, 0xe6u, 0xceu, 0x7bu, 0x5eu, 0xd4u, 0x77u, 0x67u, 0xe7u,
        0x53u, 0xcbu, 0xabu, 0xfeu, 0xbfu, 0x5fu, 0xe9u, 0x6eu, 0x77u, 0xf3u,
        0xf3u, 0x5du, 0x6fu, 0x9fu, 0x68u, 0xffu, 0xdeu, 0x5du, 0x55u, 0x64u,
        0x28u, 0x76u, 0xe8u, 0x62u, 0x8au, 0x8bu, 0x0du, 0xd0u
    },
    [21u] = {
        0x3cu, 0xfcu, 0xfbu, 0x23u, 0xeeu, 0xf9u, 0x9fu, 0xadu, 0x6fu, 0x7fu,
        0xfbu, 0x44u, 0xe6u, 0xceu, 0x7bu, 0x5eu, 0xd4u, 0x77u, 0x67u, 0xe7u,
        0x53u, 0xcbu, 0xabu, 0xfeu, 0xbfu, 0x5fu, 0xe9u, 0x6eu, 0x77u, 0xf3u,
        0xf3u, 0x5du, 0x6fu, 0x9fu, 0x68u, 0xffu, 0xdeu, 0x5du, 0x55u, 0x64u,
        0xbeu, 0x76u, 0xe8u, 0x62u, 0x8au, 0x8bu, 0x0du, 0xd0u
    },
    [22u] = {
        0x3cu, 0xfcu, 0xfbu, 0x23u, 0xeeu, 0xf9u, 0x9fu, 0xadu, 0x6fu, 0x7fu,
        0xfbu, 0x44u, 0xe6u, 0xceu, 0x7bu, 0x5eu, 0xd4u, 0x77u, 0x67u, 0xe7u,
        0x53u, 0xcbu, 0xabu, 0xfeu, 0xbfu, 0x5fu, 0xe9u, 0x6eu, 0x77u, 0xf3u,
        0xf3u, 0x5du, 0x6fu, 0x9fu, 0x68u, 0xffu, 0xdeu, 0x5du, 0x55u, 0x64u,
        0xbeu, 0x76u, 0xfdu, 0x6bu, 0x8au, 0x8bu, 0x0du, 0xd0u
    },
    [23u] = {
        0x3cu, 0xfcu, 0xfbu, 0x23u, 0xeeu, 0xf9u, 0x9fu, 0xadu, 0x6fu, 0x7fu,
        0xfbu, 0x44u, 0xe6u, 0xceu, 0x7bu, 0x5eu, 0xd4u, 0x77u, 0x67u, 0xe7u,
        0x53u, 0xcbu, 0xabu, 0xfeu, 0xbfu, 0x5fu, 0xe9u, 0x6eu, 0x77u, 0xf3u,
        0xf3u, 0x5du, 0x6fu, 0x9fu, 0x68u, 0xffu, 0xdeu, 0x5du, 0x55u, 0x64u,
        0xbeu, 0x76u, 0xfdu, 0x6bu, 0xfbu, 0x8fu, 0x0du, 0xd0u
    },
    [24u] = {
        0x3cu, 0xfcu, 0xfbu, 0x23u, 0xeeu, 0xf9u, 0x9fu, 0xadu, 0x6fu, 0x7fu,
        0xfbu, 0x44u, 0xe6u, 0xceu, 0x7bu, 0x5eu, 0xd4u, 0x77u, 0x67u, 0xe7u,
        0x53u, 0xcbu, 0xabu, 0xfeu, 0xbfu, 0x5fu, 0xe9u, 0x6eu, 0x77u, 0xf3u,
        0xf3u, 0x5du, 0x6fu, 0x9fu, 0x68u, 0xffu, 0xdeu, 0x5du, 0x55u, 0x64u,
        0xbeu, 0x76u, 0xfdu, 0x6bu, 0xfbu, 0x8fu, 0xcdu, 0xfbu
    },
};
/* clang-format on */

/* End of auto-generated section */
