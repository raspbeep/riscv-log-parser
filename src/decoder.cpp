#include "decoder.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <stdexcept>

inline bool
is_instruction_compressed(uint32_t code)
{
    return (code & 0x03) != 3;
}

void decode_compressed(uint32_t code, DecodedInstruction &inst)
{
    const uint32_t masked_lower = MASK(code, bitmask_1_0);
    const uint32_t masked_upper = MASK(code, bitmask_15_13);

    switch (masked_lower)
    {
    case 0x0:
        switch (masked_upper)
        {
        case 0x0:
            inst.name = C_ADDI4SPN;
            inst.mnemonic = c_insts_mnem_map[C_ADDI4SPN];
            break;
        case 0x1:
            inst.name = C_FLD;
            inst.mnemonic = c_insts_mnem_map[C_FLD];
            break;
        case 0x2:
            inst.name = C_LW;
            inst.mnemonic = c_insts_mnem_map[C_LW];
            break;
        case 0x3:
            // FLW is RV32 only (omitted)
            inst.name = C_LD;
            inst.mnemonic = c_insts_mnem_map[C_LD];
            break;
        case 0x4:
            // reserved
            throw std::runtime_error("Encountered reserved opcode");
            break;
        case 0x5:
            inst.name = C_FSD;
            inst.mnemonic = c_insts_mnem_map[C_FSD];
            break;
        case 0x6:
            inst.name = C_SW;
            inst.mnemonic = c_insts_mnem_map[C_SW];
            break;
        case 0x7:
            // FSW is RV32 only (omitted)
            inst.name = C_SD;
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
            if (MASK(code, bitmask_11_7))
            {
                inst.name = C_ADDI;
                inst.mnemonic = c_insts_mnem_map[C_ADDI];
            }
            else
            {
                inst.name = C_NOP;
                inst.mnemonic = c_insts_mnem_map[C_NOP];
            }
            break;
        case 0x1:
            // JAL is RV32 only (omitted)
            inst.name = C_ADDIW;
            inst.mnemonic = c_insts_mnem_map[C_ADDIW];
            break;
        case 0x2:
            inst.name = C_LI;
            inst.mnemonic = c_insts_mnem_map[C_LI];
            break;
        case 0x3:
            if (MASK(code, bitmask_11_7) == 0x2)
            {
                inst.name = C_ADDI16SP;
                inst.mnemonic = c_insts_mnem_map[C_ADDI16SP];
            }
            else if (MASK(code, bitmask_11_7) != 0x0 && (MASK(code, bitmask_11_7) != 0x2))
            {
                inst.name = C_LUI;
                inst.mnemonic = c_insts_mnem_map[C_LUI];
            }
            else
            {
                throw std::runtime_error("Encountered undefined switch case");
            }
            break;
        case 0x4:
            switch (MASK(code, bitmask_11_10))
            {
            case 0x0:
                inst.name = C_SRLI;
                inst.mnemonic = c_insts_mnem_map[C_SRLI];
                break;
            case 0x1:
                inst.name = C_SRAI;
                inst.mnemonic = c_insts_mnem_map[C_SRAI];
                break;
            case 0x2:
                inst.name = C_ANDI;
                inst.mnemonic = c_insts_mnem_map[C_ANDI];
                break;
            case 0x3:
                switch (MASK(code, bitmask_6_5))
                {
                case 0x0:
                    inst.name = C_SUB;
                    inst.mnemonic = c_insts_mnem_map[C_SUB];
                    break;
                case 0x1:
                    inst.name = C_XOR;
                    inst.mnemonic = c_insts_mnem_map[C_XOR];
                    break;
                case 0x2:
                    inst.name = C_OR;
                    inst.mnemonic = c_insts_mnem_map[C_OR];
                    break;
                case 0x3:
                    inst.name = C_AND;
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
            inst.name = C_J;
            inst.mnemonic = c_insts_mnem_map[C_J];
            break;
        case 0x6:
            inst.name = C_BEQZ;
            inst.mnemonic = c_insts_mnem_map[C_BEQZ];
            break;
        case 0x7:
            inst.name = C_BNEZ;
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
            inst.name = C_SLLI;
            inst.mnemonic = c_insts_mnem_map[C_SLLI];
            break;
        case 0x1:
            inst.name = C_FLDSP;
            inst.mnemonic = c_insts_mnem_map[C_FLDSP];
            break;
        case 0x2:
            inst.name = C_LWSP;
            inst.mnemonic = c_insts_mnem_map[C_LWSP];
            break;
        case 0x3:
            // FLWSP is RV32 only (omitted)
            inst.name = C_LDSP;
            inst.mnemonic = c_insts_mnem_map[C_LDSP];
            break;
        case 0x4:
            if (MASK(code, bitmask_12))
            {
                if (!(MASK(code, bitmask_11_7)) && !(MASK(code, bitmask_6_2)))
                {
                    inst.name = C_EBREAK;
                    inst.mnemonic = c_insts_mnem_map[C_EBREAK];
                    break;
                }

                if (!(MASK(code, bitmask_11_7)))
                {
                    std::cout << inst.line << std::endl;
                    throw std::runtime_error("Illegal operands for instruction (C.ADD/C.JALR): " + std::to_string(code));
                }

                if (MASK(code, bitmask_6_2))
                {
                    inst.name = C_ADD;
                    inst.mnemonic = c_insts_mnem_map[C_ADD];
                }
                else
                {
                    inst.name = C_JALR;
                    inst.mnemonic = c_insts_mnem_map[C_JALR];
                }
            }
            else
            {
                if (MASK(code, bitmask_6_2))
                {
                    inst.name = C_MV;
                    inst.mnemonic = c_insts_mnem_map[C_MV];
                }
                else
                {
                    inst.name = C_JR;
                    inst.mnemonic = c_insts_mnem_map[C_JR];
                }
            }
            break;
        case 0x5:
            inst.name = C_FSDSP;
            inst.mnemonic = c_insts_mnem_map[C_FSDSP];
            break;
        case 0x6:
            inst.name = C_SWSP;
            inst.mnemonic = c_insts_mnem_map[C_SWSP];
            break;
        case 0x7:
            // FSWSP is RV32 only (omitted)
            inst.name = C_SDSP;
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

    return;
}

void decode_vector_op_v_instruction(uint32_t code, DecodedInstruction &inst)
{
    uint32_t code_14_12 = MASK(code, bitmask_14_12);

    uint32_t code_31_10 = MASK(code, bitmask_31_30);

    if (code_14_12 == 0x7)
    {
        // OPCFG scalar-imms
        inst.format = InstFormat::OPCFG;
        switch (code_31_10)
        {
        case 0x0:
            inst.name = V_VSETVLI;
            inst.mnemonic = c_insts_mnem_map[V_VSETVLI];
            break;
        case 0x1:
            inst.name = V_VSETVLI;
            inst.mnemonic = c_insts_mnem_map[V_VSETVLI];
            break;
        case 0x2:
            inst.name = V_VSETVL;
            inst.mnemonic = c_insts_mnem_map[V_VSETVL];
            break;
        case 0x3:
            inst.name = V_VSETIVLI;
            inst.mnemonic = c_insts_mnem_map[V_VSETIVLI];
            break;
        default:
            throw std::runtime_error("Encountered undefined switch case");
            break;
        }
    }
    else
    {
        const uint8_t upper = MASK(code, bitmask_24_20);
        const uint8_t middle = MASK(code, bitmask_19_15);
        const uint8_t lower = MASK(code, bitmask_11_7);
        switch (code_14_12)
        {
        case 0x0:
            // OPIVV vector-vector
            inst.format = InstFormat::OPIVV;
            inst.payload = OPIVVTypeFields{
                .vs2 = upper,
                .vs1 = middle,
                .vd = lower};
            break;
        case 0x1:
            // OPFVV vector-vector
            inst.format = InstFormat::OPFVV;
            inst.payload = OPFVVTypeFields{
                .vs2 = upper,
                .vs1 = middle,
                .vd_rd = lower};
            break;
        case 0x2:
            // OPMVV vector-vector
            inst.format = InstFormat::OPMVV;
            inst.payload = OPMVVTypeFields{
                .vs2 = upper,
                .vs1 = middle,
                .vd_rd = lower};
            break;
        case 0x3:
            // OPIVI vector-immediate
            inst.format = InstFormat::OPIVI;
            inst.payload = OPIVITypeFields{
                .vs2 = upper,
                .imm = middle,
                .vd = lower,
            };
            break;
        case 0x4:
            // OPIVX vector-scalar
            inst.format = InstFormat::OPIVX;
            inst.payload = OPIVXTypeFields{
                .vs2 = upper,
                .rs1 = middle,
                .vd = lower,
            };
            break;
        case 0x5:
            // OPFVF vector-scalar
            inst.format = InstFormat::OPFVF;
            inst.payload = OPFVFTypeFields{
                .vs2 = upper,
                .rs1 = middle,
                .vd = lower,
            };
            break;
        case 0x6:
            // OPMVX vector-scalar
            inst.format = InstFormat::OPMVX;
            inst.payload = OPMVXTypeFields{
                .vs2 = upper,
                .rs1 = middle,
                .vd_rd = lower,
            };
            break;
        default:
            throw std::runtime_error("Encountered undefined switch case");
            break;
        }
    }
}

void decode_vector_load_fp_instruction(uint32_t code, DecodedInstruction &inst)
{
}

void decode_vector_store_fp_instruction(uint32_t code, DecodedInstruction &inst)
{
}

void decode_system_instruction(uint32_t code, DecodedInstruction &inst)
{
}

void decode_common_instruction(uint32_t code, DecodedInstruction &inst)
{
}

void decode_uncompressed(uint32_t code, DecodedInstruction &inst)
{
    // 0100111

    switch (MASK(code, bitmask_6_0))
    {
        // RVV
    case 0x7:
    case 0x27:
    case 0x57:
        decode_vector_op_v_instruction(code, inst);
        break;
    // system
    case 0x73:
        decode_system_instruction(code, inst);
        break;
    default:
        decode_common_instruction(code, inst);
        break;
    }
}

void decode_instruction(uint32_t code, DecodedInstruction &inst)
{
    inst.compressed = is_instruction_compressed(code);

    if (inst.compressed)
    {
        decode_compressed(code, inst);
    }
    else
    {
        decode_uncompressed(code, inst);
    }
}