#include "decoder.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <stdexcept>

const uint32_t bitmask_12 = 0x1000;
const uint8_t bitmask_12_shift = 12;

const uint32_t bitmask_1_0 = 0x3;
const uint8_t bitmask_1_0_shift = 0;

const uint32_t bitmask_2_0 = 0xe000;
const uint8_t bitmask_2_0_shift = 0;

const uint32_t bitmask_11_7 = 0x7c0;
const uint8_t bitmask_11_7_shift = 7;

const uint32_t bitmask_6_2 = 0x7c;
const uint8_t bitmask_6_2_shift = 2;

const uint32_t bitmask_11_10 = 0xc00;
const uint8_t bitmask_11_10_shift = 10;

const uint32_t bitmask_6_5 = 0x60;
const uint8_t bitmask_6_5_shift = 5;

const uint32_t bitmask_15_13 = 0xe000;
const uint8_t bitmask_15_13_shift = 13;

inline bool
is_instruction_compressed(uint32_t code)
{
    return (code & 0x03) != 3;
}

void decode_compressed(uint32_t code, BaseInstruction &inst)
{
    uint32_t masked_lower = (code & bitmask_1_0) >> bitmask_1_0_shift;
    uint32_t masked_upper = (code & bitmask_15_13) >> bitmask_15_13_shift;

    switch (masked_lower)
    {
    case 0x0:
        switch (masked_upper)
        {
        case 0x0:
            inst.kind = C_ADDI4SPN;
            inst.mnemonic = c_insts_mnem_map[C_ADDI4SPN];
            break;
        case 0x1:
            inst.kind = C_FLD;
            inst.mnemonic = c_insts_mnem_map[C_FLD];
            break;
        case 0x2:
            inst.kind = C_LW;
            inst.mnemonic = c_insts_mnem_map[C_LW];
            break;
        case 0x3:
            // FLW is RV32 only (omitted)
            inst.kind = C_LD;
            inst.mnemonic = c_insts_mnem_map[C_LD];
            break;
        case 0x4:
            // reserved
            throw std::runtime_error("Encountered reserved opcode");
            break;
        case 0x5:
            inst.kind = C_FSD;
            inst.mnemonic = c_insts_mnem_map[C_FSD];
            break;
        case 0x6:
            inst.kind = C_SW;
            inst.mnemonic = c_insts_mnem_map[C_SW];
            break;
        case 0x7:
            // FSW is RV32 only (omitted)
            inst.kind = C_SD;
            inst.mnemonic = c_insts_mnem_map[C_SD];
            break;
        default:
            throw std::runtime_error("Encountered undefined switch case");
            break;
        }
        break;
    case 0x1:
        switch (masked_upper)
        {
        case 0x0:
            if ((code & bitmask_11_7) >> bitmask_11_7_shift)
            {
                inst.kind = C_ADDI;
                inst.mnemonic = c_insts_mnem_map[C_ADDI];
            }
            else
            {
                inst.kind = C_NOP;
                inst.mnemonic = c_insts_mnem_map[C_NOP];
            }
            break;
        case 0x1:
            // JAL is RV32 only (omitted)
            inst.kind = C_ADDIW;
            inst.mnemonic = c_insts_mnem_map[C_ADDIW];
            break;
        case 0x2:
            inst.kind = C_LI;
            inst.mnemonic = c_insts_mnem_map[C_LI];
            break;
        case 0x3:
            if (((code & bitmask_11_7) >> bitmask_11_7_shift) == 0x2)
            {
                inst.kind = C_ADDI16SP;
                inst.mnemonic = c_insts_mnem_map[C_ADDI16SP];
            }
            else if (((code & bitmask_11_7) >> bitmask_11_7_shift) != 0x0 && (((code & bitmask_11_7) >> bitmask_11_7_shift) != 0x2))
            {
                inst.kind = C_LUI;
                inst.mnemonic = c_insts_mnem_map[C_LUI];
            }
            else
            {
                throw std::runtime_error("Encountered undefined switch case");
            }
            break;
        case 0x4:
            switch ((code & bitmask_11_10) >> bitmask_11_10_shift)
            {
            case 0x0:
                inst.kind = C_SRLI;
                inst.mnemonic = c_insts_mnem_map[C_SRLI];
                break;
            case 0x1:
                inst.kind = C_SRAI;
                inst.mnemonic = c_insts_mnem_map[C_SRAI];
                break;
            case 0x2:
                inst.kind = C_ANDI;
                inst.mnemonic = c_insts_mnem_map[C_ANDI];
                break;
            case 0x3:
                switch ((code & bitmask_6_5) >> bitmask_6_5_shift)
                {
                case 0x0:
                    inst.kind = C_SUB;
                    inst.mnemonic = c_insts_mnem_map[C_SUB];
                    break;
                case 0x1:
                    inst.kind = C_XOR;
                    inst.mnemonic = c_insts_mnem_map[C_XOR];
                    break;
                case 0x2:
                    inst.kind = C_OR;
                    inst.mnemonic = c_insts_mnem_map[C_OR];
                    break;
                case 0x3:
                    inst.kind = C_AND;
                    inst.mnemonic = c_insts_mnem_map[C_AND];
                    break;
                default:
                    throw std::runtime_error("Encountered undefined switch case");
                    break;
                }
                break;
            default:
                throw std::runtime_error("Encountered undefined switch case");
                break;
            }
            break;
        case 0x5:
            inst.kind = C_J;
            inst.mnemonic = c_insts_mnem_map[C_J];
            break;
        case 0x6:
            inst.kind = C_BEQZ;
            inst.mnemonic = c_insts_mnem_map[C_BEQZ];
            break;
        case 0x7:
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
            inst.kind = C_SLLI;
            inst.mnemonic = c_insts_mnem_map[C_SLLI];
            break;
        case 0x1:
            inst.kind = C_FLDSP;
            inst.mnemonic = c_insts_mnem_map[C_FLDSP];
            break;
        case 0x2:
            inst.kind = C_LWSP;
            inst.mnemonic = c_insts_mnem_map[C_LWSP];
            break;
        case 0x3:
            // FLWSP is RV32 only (omitted)
            inst.kind = C_LDSP;
            inst.mnemonic = c_insts_mnem_map[C_LDSP];
            break;
        case 0x4:
            if ((code & bitmask_12) >> bitmask_12_shift)
            {
                if (!((code & bitmask_11_7) >> bitmask_11_7_shift) && !((code && bitmask_6_2) >> bitmask_6_2_shift))
                {
                    inst.kind = C_EBREAK;
                    inst.mnemonic = c_insts_mnem_map[C_EBREAK];
                    break;
                }

                if (!((code & bitmask_11_7) >> bitmask_11_7_shift))
                {
                    throw std::runtime_error("Illegal operands for instruction (C.ADD/C.JALR): " + std::to_string(code));
                }

                if ((code & bitmask_6_2) >> bitmask_6_2_shift)
                {
                    inst.kind = C_ADD;
                    inst.mnemonic = c_insts_mnem_map[C_ADD];
                }
                else
                {
                    inst.kind = C_JALR;
                    inst.mnemonic = c_insts_mnem_map[C_JALR];
                }
            }
            else
            {
                if ((code & bitmask_6_2) >> bitmask_6_2_shift)
                {
                    inst.kind = C_MV;
                    inst.mnemonic = c_insts_mnem_map[C_MV];
                }
                else
                {
                    inst.kind = C_JR;
                    inst.mnemonic = c_insts_mnem_map[C_JR];
                }
            }
            break;
        case 0x5:
            inst.kind = C_FSDSP;
            inst.mnemonic = c_insts_mnem_map[C_FSDSP];
            break;
        case 0x6:
            inst.kind = C_SWSP;
            inst.mnemonic = c_insts_mnem_map[C_SWSP];
            break;
        case 0x7:
            // FSWSP is RV32 only (omitted)
            inst.kind = C_SDSP;
            inst.mnemonic = c_insts_mnem_map[C_SDSP];
            break;
        default:
            throw std::runtime_error("Encountered undefined switch case");
            break;
        }
        break;
    default:
        throw std::runtime_error("Encountered undefined switch case");
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
        // throw std::runtime_error("Uncompressed instruction kind: " + std::to_string(masked));
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