#ifndef DECODER_HPP
#define DECODER_HPP

#include <cstdint>

#include "instructions.hpp"

BaseInstruction decode_instruction(uint32_t code);

#endif
