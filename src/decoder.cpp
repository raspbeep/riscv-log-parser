#include "decoder.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <stdexcept>

inline bool is_instruction_compressed(uint32_t code)
{
    return (code & 0x03) != 3;
}

void decode_compressed(uint32_t code, BaseInstruction &inst)
{
    uint32_t lower_2_bits_mask = 0x3;
    uint32_t upper_3_bits_mask = 0xe000;
    uint32_t masked_lower = code & lower_2_bits_mask;
    uint32_t masked_upper = (code >> 13) & 0x7;

    switch (masked_lower)
    {
    case 0x0:
        switch (masked_upper)
        {
        case 0x0:
            // ADDI4SPN
            inst.kind = C_ADDI4SPN;
            inst.mnemonic = c_insts_mnem_map[C_ADDI4SPN];
            break;
        case 0x1:
            // FLD
            inst.kind = C_FLD;
            inst.mnemonic = c_insts_mnem_map[C_FLD];
            break;
        case 0x2:
            // LW
            inst.kind = C_LW;
            inst.mnemonic = c_insts_mnem_map[C_LW];
            break;
        case 0x3:
            // FLW
            // LD
            break;
        case 0x4:
            // reserved
            break;
        case 0x5:
            // FSD
            inst.kind = C_FSD;
            inst.mnemonic = c_insts_mnem_map[C_FSD];
            break;
        case 0x6:
            // SW
            inst.kind = C_SW;
            inst.mnemonic = c_insts_mnem_map[C_SW];
            break;
        case 0x7:
            // FSW
            // SD
            break;
        default:
            break;
        }
        break;
    case 0x1:
        switch (masked_upper)
        {
        case 0x0:
            // ADDI
            inst.kind = C_ADDI;
            inst.mnemonic = c_insts_mnem_map[C_ADDI];
            break;
        case 0x1:
            // JAL
            // ADDIW
            break;
        case 0x2:
            // LI
            inst.kind = C_LI;
            inst.mnemonic = c_insts_mnem_map[C_LI];
            break;
        case 0x3:
            // LUI
            // ADDI16SP
            break;
        case 0x4:
            // MISC-ALU
            break;
        case 0x5:
            // J
            inst.kind = C_J;
            inst.mnemonic = c_insts_mnem_map[C_J];
            break;
        case 0x6:
            // BEQZ
            inst.kind = C_BEQZ;
            inst.mnemonic = c_insts_mnem_map[C_BEQZ];
            break;
        case 0x7:
            // BNEZ
            inst.kind = C_BNEZ;
            inst.mnemonic = c_insts_mnem_map[C_BNEZ];
            break;
        default:
            break;
        }
        break;
    case 0x2:
        switch (masked_upper)
        {
        case 0x0:
            // SLLI
            inst.kind = C_SLLI;
            inst.mnemonic = c_insts_mnem_map[C_SLLI];
            break;
        case 0x1:
            // FLDSP
            inst.kind = C_FLDSP;
            inst.mnemonic = c_insts_mnem_map[C_FLDSP];
            break;
        case 0x2:
            // LWSP
            inst.kind = C_LWSP;
            inst.mnemonic = c_insts_mnem_map[C_LWSP];
            break;
        case 0x3:
            // FLWSP
            // LDSP
            break;
        case 0x4:
            // J[AL]R/MV/ADD
            break;
        case 0x5:
            // FSDSP
            inst.kind = C_FSDSP;
            inst.mnemonic = c_insts_mnem_map[C_FSDSP];
            break;
        case 0x6:
            // SWSP
            inst.kind = C_SWSP;
            inst.mnemonic = c_insts_mnem_map[C_SWSP];
            break;
        case 0x7:
            // FSWSP
            // SDSP
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }

    // auto quadrant = &c_insts[masked_lower];
    // auto it = quadrant->find(masked_upper);
    // if (it != quadrant->end())
    // {
    //     auto instruction = it->second;
    //     inst.mnemonic = instruction.first;
    //     inst.kind = instruction.second;
    //     std::ostringstream oss;
    //     oss << "Mnemonic: " << inst.mnemonic << " code: 0x"
    //         << std::hex << std::setw(2) << std::setfill('0') << code
    //         << " in quadrant 0x"
    //         << std::hex << std::setw(2) << std::setfill('0') << masked_lower
    //         << " with code: 0x"
    //         << std::hex << std::setw(2) << std::setfill('0') << masked_upper;
    //     std::cout << oss.str() << std::endl;
    // }
    // else
    // {
    //     std::ostringstream oss;
    //     oss << "Compressed instruction with code: 0x"
    //         << std::hex << std::setw(2) << std::setfill('0') << code
    //         << " in quadrant 0x"
    //         << std::hex << std::setw(2) << std::setfill('0') << masked_lower
    //         << " with code: 0x"
    //         << std::hex << std::setw(2) << std::setfill('0') << masked_upper
    //         << " not found";

    //     throw std::runtime_error(oss.str());
    // }

    return;
}

void decode_vector_instruction(uint32_t code, BaseInstruction &inst)
{
    uint32_t mask_14_to_12 = 0x7000;
    uint32_t masked_middle = code & mask_14_to_12;

    uint32_t mask_31_to_30 = 0xc0000000;
    uint32_t masked_upper = code & mask_31_to_30;

    if (masked_middle == 0x7)
    {
        // RVV configuration instructions
    }
    else
    {
    }
}

void decode_system_instruction(uint32_t code, BaseInstruction &inst)
{
}

void decode_uncompressed(uint32_t code, BaseInstruction &inst)
{
    uint32_t lower_7_bits_mask = 0x7f;
    uint32_t masked = code & lower_7_bits_mask;

    switch (masked)
    {
    // RVV
    case 0x57:
        decode_vector_instruction(code, inst);
        break;
    // system
    case 0x73:
        decode_vector_instruction(code, inst);
        break;
    default:
        throw std::runtime_error("Uncompressed instruction kind: " + std::to_string(masked));
        break;
    }
}

BaseInstruction decode_instruction(uint32_t code)
{
    BaseInstruction inst;
    inst.compressed = is_instruction_compressed(code);

    if (inst.compressed)
    {
        decode_compressed(code, inst);
    }
    else
    {
        decode_uncompressed(code, inst);
    }

    return inst;
}