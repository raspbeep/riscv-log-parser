#ifndef DECODER_HPP
#define DECODER_HPP

#include <cstdint>

#include "instructions.hpp"

#define bitmask_12 0x1000
#define bitmask_12_shift 12

#define bitmask_1_0 0x3
#define bitmask_1_0_shift 0

#define bitmask_2_0 0x7
#define bitmask_2_0_shift 0

#define bitmask_11_7 0xf80
#define bitmask_11_7_shift 7

#define bitmask_6_2 0x7c
#define bitmask_6_2_shift 2

#define bitmask_11_10 0xc00
#define bitmask_11_10_shift 10

#define bitmask_6_5 0x60
#define bitmask_6_5_shift 5

#define bitmask_15_13 0xe000
#define bitmask_15_13_shift 13

#define bitmask_14_12 0x7000
#define bitmask_14_12_shift 12

#define bitmask_31_30 0xc0000000
#define bitmask_31_30_shift 30

#define bitmask_6_0 0x7f
#define bitmask_6_0_shift 0

#define bitmask_24_20 0x1f00000
#define bitmask_24_20_shift 20

#define bitmask_19_15 0xf8000
#define bitmask_19_15_shift 15

#define bitmask_31_26 0xfc000000
#define bitmask_31_26_shift 26

#define bitmask_9_7 0x380
#define bitmask_9_7_shift 7

#define bitmask_4_2 0x1c
#define bitmask_4_2_shift 2

#define bitmask_12_7 0x1f80
#define bitmask_12_7_shift 7

#define bitmask_12_5 0x1fe0
#define bitmask_12_5_shift 5

#define bitmask_12_10 0x1c00
#define bitmask_12_10_shift 10

#define bitmask_12_2 0x1ffc
#define bitmask_12_2_shift 2

#define bitmask_31_25 0xfe000000
#define bitmask_31_25_shift 25

#define bitmask_30_25 0x7e000000
#define bitmask_30_25_shift 25

#define bitmask_31_20 0xfff00000
#define bitmask_31_20_shift 20

#define bitmask_11_8 0xf00
#define bitmask_11_8_shift 8

#define bitmask_7 0x80
#define bitmask_7_shift 7

#define bitmask_31_12 0xfffff000
#define bitmask_31_12_shift 12

#define bitmask_31 0x80000000
#define bitmask_31_shift 31

#define bitmask_30_21 0x7fe00000
#define bitmask_30_21_shift 21

#define bitmask_20 0x100000
#define bitmask_20_shift 20

#define bitmask_19_12 0xff000
#define bitmask_19_12_shift 12

#define bitmask_31_27 0xf8000000
#define bitmask_31_27_shift 27

#define bitmask_11 0x800
#define bitmask_11_shift 11

#define bitmask_4 0x10
#define bitmask_4_shift 4

#define bitmask_9_8 0x300
#define bitmask_9_8_shift 8

#define bitmask_10 0x400
#define bitmask_10_shift 10

#define bitmask_6 0x40
#define bitmask_6_shift 6

#define bitmask_7 0x80
#define bitmask_7_shift 7

#define bitmask_3_1 0xe
#define bitmask_3_1_shift 1

#define bitmask_5 0x20
#define bitmask_5_shift 5

#define MASK(c, m) ((c & m) >> m##_shift)

void decode_instruction(uint32_t code, DecodedInstruction &inst);

#endif
