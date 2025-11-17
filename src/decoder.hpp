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

#define MASK(c, m) ((c & m) >> m##_shift)

void decode_instruction(uint32_t code, DecodedInstruction &inst);

#endif
