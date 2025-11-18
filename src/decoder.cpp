#include "decoder.hpp"
#include "utils.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <stdexcept>

inline bool
is_instruction_compressed(uint32_t code)
{
    return (code & 0x03) != 3;
}

void decode_compressed_operands(uint32_t code, DecodedInstruction &inst)
{
    switch (inst.format)
    {
    case InstFormat::CR:
        inst.payload = CRTypeFields(
            (uint8_t)MASK(code, bitmask_11_7),
            (uint8_t)MASK(code, bitmask_6_2));
        break;
    case InstFormat::CI:
        inst.payload = CITypeFields(
            (uint8_t)MASK(code, bitmask_12),
            (uint8_t)MASK(code, bitmask_11_7),
            (uint8_t)MASK(code, bitmask_6_2));
        break;
    case InstFormat::CSS:
        inst.payload = CSSTypeFields(
            (uint8_t)MASK(code, bitmask_12_7),
            (uint8_t)MASK(code, bitmask_6_2));
        break;
    case InstFormat::CIW:
        inst.payload = CIWTypeFields(
            (uint8_t)MASK(code, bitmask_12_5),
            (uint8_t)MASK(code, bitmask_4_2));
        break;
    case InstFormat::CL:
        inst.payload = CLTypeFields(
            (uint8_t)MASK(code, bitmask_12_10),
            (uint8_t)MASK(code, bitmask_9_7),
            (uint8_t)MASK(code, bitmask_6_5),
            (uint8_t)MASK(code, bitmask_4_2));
        break;
    case InstFormat::CS:
        inst.payload = CSTypeFields(
            (uint8_t)MASK(code, bitmask_12_10),
            (uint8_t)MASK(code, bitmask_9_7),
            (uint8_t)MASK(code, bitmask_6_5),
            (uint8_t)MASK(code, bitmask_4_2));
        break;
    case InstFormat::CA:
        inst.payload = CATypeFields(
            (uint8_t)MASK(code, bitmask_9_7),
            (uint8_t)MASK(code, bitmask_4_2));
        break;
    case InstFormat::CB:
        inst.payload = CBTypeFields(
            (uint8_t)MASK(code, bitmask_12_10),
            (uint8_t)MASK(code, bitmask_9_7),
            (uint8_t)MASK(code, bitmask_6_2));
        break;
    case InstFormat::CJ:
        inst.payload = CJTypeFields(
            (uint16_t)MASK(code, bitmask_12_2));

        if (inst.name == C_J)
        {
            std::cout << uint32_t_to_dec_hex_bin(MASK(code, bitmask_12_2)) << std::endl;

            uint32_t offset_raw = (uint32_t)MASK(code, bitmask_12_2);
            uint16_t offset = extract_c_j_offset(offset_raw);

            inst.payload = CJTypeFields(offset, sign_extend(offset, 12));
        }
        break;
    default:
        throw std::runtime_error("Encountered unhandled instruction format in switch case.");
    }
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
            inst.mnemonic = insts_mnem_map[C_ADDI4SPN];
            inst.format = InstFormat::CIW;
            break;
        case 0x1:
            inst.name = C_FLD;
            inst.mnemonic = insts_mnem_map[C_FLD];
            inst.format = InstFormat::CL;
            break;
        case 0x2:
            inst.name = C_LW;
            inst.mnemonic = insts_mnem_map[C_LW];
            inst.format = InstFormat::CL;
            break;
        case 0x3:
            // FLW is RV32 only (omitted)
            inst.name = C_LD;
            inst.mnemonic = insts_mnem_map[C_LD];
            inst.format = InstFormat::CL;
            break;
        case 0x4:
            // reserved
            throw std::runtime_error("Encountered reserved opcode");
            break;
        case 0x5:
            inst.name = C_FSD;
            inst.mnemonic = insts_mnem_map[C_FSD];
            inst.format = InstFormat::CS;
            break;
        case 0x6:
            inst.name = C_SW;
            inst.mnemonic = insts_mnem_map[C_SW];
            inst.format = InstFormat::CS;
            break;
        case 0x7:
            // FSW is RV32 only (omitted)
            inst.name = C_SD;
            inst.mnemonic = insts_mnem_map[C_SD];
            inst.format = InstFormat::CS;
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
                inst.mnemonic = insts_mnem_map[C_ADDI];
                inst.format = InstFormat::CI;
            }
            else
            {
                inst.name = C_NOP;
                inst.mnemonic = insts_mnem_map[C_NOP];
                inst.format = InstFormat::CI;
            }
            break;
        case 0x1:
            // JAL is RV32 only (omitted)
            inst.name = C_ADDIW;
            inst.mnemonic = insts_mnem_map[C_ADDIW];
            inst.format = InstFormat::CI;
            break;
        case 0x2:
            inst.name = C_LI;
            inst.mnemonic = insts_mnem_map[C_LI];
            inst.format = InstFormat::CI;
            break;
        case 0x3:
            if (MASK(code, bitmask_11_7) == 0x2)
            {
                inst.name = C_ADDI16SP;
                inst.mnemonic = insts_mnem_map[C_ADDI16SP];
                inst.format = InstFormat::CI;
            }
            else if (MASK(code, bitmask_11_7) != 0x0 && (MASK(code, bitmask_11_7) != 0x2))
            {
                inst.name = C_LUI;
                inst.mnemonic = insts_mnem_map[C_LUI];
                inst.format = InstFormat::CI;
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
                inst.mnemonic = insts_mnem_map[C_SRLI];
                inst.format = InstFormat::CB;
                break;
            case 0x1:
                inst.name = C_SRAI;
                inst.mnemonic = insts_mnem_map[C_SRAI];
                inst.format = InstFormat::CB;
                break;
            case 0x2:
                inst.name = C_ANDI;
                inst.mnemonic = insts_mnem_map[C_ANDI];
                inst.format = InstFormat::CB;
                break;
            case 0x3:
                inst.format = InstFormat::CA;
                switch (MASK(code, bitmask_6_5))
                {
                case 0x0:
                    inst.name = C_SUB;
                    inst.mnemonic = insts_mnem_map[C_SUB];
                    break;
                case 0x1:
                    inst.name = C_XOR;
                    inst.mnemonic = insts_mnem_map[C_XOR];
                    break;
                case 0x2:
                    inst.name = C_OR;
                    inst.mnemonic = insts_mnem_map[C_OR];
                    break;
                case 0x3:
                    inst.name = C_AND;
                    inst.mnemonic = insts_mnem_map[C_AND];
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
            inst.mnemonic = insts_mnem_map[C_J];
            inst.format = InstFormat::CJ;
            break;
        case 0x6:
            inst.name = C_BEQZ;
            inst.mnemonic = insts_mnem_map[C_BEQZ];
            inst.format = InstFormat::CB;
            break;
        case 0x7:
            inst.name = C_BNEZ;
            inst.mnemonic = insts_mnem_map[C_BNEZ];
            inst.format = InstFormat::CB;
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
            inst.mnemonic = insts_mnem_map[C_SLLI];
            inst.format = InstFormat::CI;
            break;
        case 0x1:
            inst.name = C_FLDSP;
            inst.mnemonic = insts_mnem_map[C_FLDSP];
            inst.format = InstFormat::CI;
            break;
        case 0x2:
            inst.name = C_LWSP;
            inst.mnemonic = insts_mnem_map[C_LWSP];
            inst.format = InstFormat::CI;
            break;
        case 0x3:
            // FLWSP is RV32 only (omitted)
            inst.name = C_LDSP;
            inst.mnemonic = insts_mnem_map[C_LDSP];
            inst.format = InstFormat::CI;
            break;
        case 0x4:
            if (MASK(code, bitmask_12))
            {
                if ((MASK(code, bitmask_11_7)) == 0x0 && (MASK(code, bitmask_6_2)) == 0x0)
                {
                    inst.name = C_EBREAK;
                    inst.mnemonic = insts_mnem_map[C_EBREAK];
                    inst.format = InstFormat::CR;
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
                    inst.mnemonic = insts_mnem_map[C_ADD];
                    inst.format = InstFormat::CR;
                }
                else
                {
                    inst.name = C_JALR;
                    inst.mnemonic = insts_mnem_map[C_JALR];
                    inst.format = InstFormat::CA;
                }
            }
            else
            {
                if (MASK(code, bitmask_6_2))
                {
                    inst.name = C_MV;
                    inst.mnemonic = insts_mnem_map[C_MV];
                    inst.format = InstFormat::CR;
                }
                else
                {
                    inst.name = C_JR;
                    inst.mnemonic = insts_mnem_map[C_JR];
                    inst.format = InstFormat::CR;
                }
            }
            break;
        case 0x5:
            inst.name = C_FSDSP;
            inst.mnemonic = insts_mnem_map[C_FSDSP];
            inst.format = InstFormat::CSS;
            break;
        case 0x6:
            inst.name = C_SWSP;
            inst.mnemonic = insts_mnem_map[C_SWSP];
            inst.format = InstFormat::CSS;
            break;
        case 0x7:
            // FSWSP is RV32 only (omitted)
            inst.name = C_SDSP;
            inst.mnemonic = insts_mnem_map[C_SDSP];
            inst.format = InstFormat::CSS;
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

    decode_compressed_operands(code, inst);

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
            inst.mnemonic = insts_mnem_map[V_VSETVLI];
            break;
        case 0x1:
            inst.name = V_VSETVLI;
            inst.mnemonic = insts_mnem_map[V_VSETVLI];
            break;
        case 0x2:
            inst.name = V_VSETVL;
            inst.mnemonic = insts_mnem_map[V_VSETVL];
            break;
        case 0x3:
            inst.name = V_VSETIVLI;
            inst.mnemonic = insts_mnem_map[V_VSETIVLI];
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
            inst.payload = OPIVVTypeFields(upper, middle, lower);
            break;
        case 0x1:
            // OPFVV vector-vector
            inst.format = InstFormat::OPFVV;
            inst.payload = OPFVVTypeFields(upper, middle, lower);
            break;
        case 0x2:
            // OPMVV vector-vector
            inst.format = InstFormat::OPMVV;
            inst.payload = OPMVVTypeFields(upper, middle, lower);
            break;
        case 0x3:
            // OPIVI vector-immediate
            inst.format = InstFormat::OPIVI;
            inst.payload = OPIVITypeFields(upper, middle, lower);
            break;
        case 0x4:
            // OPIVX vector-scalar
            inst.format = InstFormat::OPIVX;
            inst.payload = OPIVXTypeFields(upper, middle, lower);
            break;
        case 0x5:
            // OPFVF vector-scalar
            inst.format = InstFormat::OPFVF;
            inst.payload = OPFVFTypeFields(upper, middle, lower);
            break;
        case 0x6:
            // OPMVX vector-scalar
            inst.format = InstFormat::OPMVX;
            inst.payload = OPMVXTypeFields(upper, middle, lower);
            break;
        default:
            throw std::runtime_error("Encountered undefined switch case (3)");
            break;
        }

        switch (inst.format)
        {
        case InstFormat::OPIVV:
        case InstFormat::OPIVX:
        case InstFormat::OPIVI:
            switch (MASK(code, bitmask_31_26))
            {
            case 0x0:
                inst.name = V_ADD;
                inst.mnemonic = insts_mnem_map[V_ADD];
                break;
            case 0x2:
                inst.name = V_SUB;
                inst.mnemonic = insts_mnem_map[V_SUB];
                break;
            case 0x3:
                inst.name = V_RSUB;
                inst.mnemonic = insts_mnem_map[V_RSUB];
                break;
            case 0xC:
                inst.name = V_SLIDEDOWN;
                inst.mnemonic = insts_mnem_map[V_SLIDEDOWN];
                break;
            case 0xf:
                inst.name = V_RGATHER;
                inst.mnemonic = insts_mnem_map[V_RGATHER];
                break;

            case 0x17:
                inst.name = V_MERGE;
                inst.mnemonic = insts_mnem_map[V_MERGE];
                break;
            case 0x25:
                inst.name = V_SLL;
                inst.mnemonic = insts_mnem_map[V_SLL];
                break;
            case 0x27:
                inst.name = V_SMUL;
                inst.mnemonic = insts_mnem_map[V_SMUL];
                break;

            default:
                inst.print();
                std::cout << "mask: " << uint32_t_to_dec_hex_bin(MASK(code, bitmask_31_26)) << std::endl;
                throw std::runtime_error("Encountered undefined switch case (4)");
                break;
            }
            break;
        case InstFormat::OPMVV:
        case InstFormat::OPMVX:
            switch (MASK(code, bitmask_31_26))
            {
            case 0x0:
                inst.name = V_REDSUM;
                inst.mnemonic = insts_mnem_map[V_REDSUM];
                break;
            case 0x2d:
                inst.name = V_MACC;
                inst.mnemonic = insts_mnem_map[V_MACC];
                break;
            case 0x10:
                if (auto ptr = std::get_if<OPMVXTypeFields>(&inst.payload))
                {
                    switch (ptr->vs2)
                    {
                    case 0x0:
                        inst.name = V_MV_S_X;
                        inst.mnemonic = insts_mnem_map[V_MV_S_X];
                        break;
                    default:
                        inst.print();
                        std::cout << "mask: " << uint32_t_to_dec_hex_bin(MASK(code, bitmask_31_26)) << std::endl;
                        std::cout << "rs1: " << uint32_t_to_dec_hex_bin(ptr->rs1) << " vs2: " << uint32_t_to_dec_hex_bin(ptr->vs2) << std::endl;
                        throw std::runtime_error("Encountered undefined if branch (1)");
                        break;
                    }
                }
                else if (auto ptr = std::get_if<OPMVVTypeFields>(&inst.payload))
                {
                    switch (ptr->vs1)
                    {
                    case 0x0:
                        inst.name = V_MV_X_S;
                        inst.mnemonic = insts_mnem_map[V_MV_X_S];
                        break;
                    case 0x10:
                        inst.name = V_POPC;
                        inst.mnemonic = insts_mnem_map[V_POPC];
                        break;
                    case 0x11:
                        inst.name = V_FIRST;
                        inst.mnemonic = insts_mnem_map[V_FIRST];
                        break;
                    default:
                        std::cout << "mask: " << uint32_t_to_dec_hex_bin(MASK(code, bitmask_31_26)) << std::endl;
                        std::cout << "vs1: " << uint32_t_to_dec_hex_bin(ptr->vs1) << " vs2: " << uint32_t_to_dec_hex_bin(ptr->vs2) << std::endl;
                        throw std::runtime_error("Encountered undefined if branch (8)");
                        break;
                    }
                }
                else
                {
                    inst.print();
                    std::cout << "mask: " << uint32_t_to_dec_hex_bin(MASK(code, bitmask_31_26)) << std::endl;
                    throw std::runtime_error("Encountered undefined if branch (3)");
                }
                break;
            case 0x14:
                if (auto ptr = std::get_if<OPMVVTypeFields>(&inst.payload))
                {
                    switch (ptr->vs1)
                    {
                    case 0x1:
                        inst.name = V_MSBF;
                        inst.mnemonic = insts_mnem_map[V_MSBF];
                        break;
                    case 0x2:
                        inst.name = V_MSOF;
                        inst.mnemonic = insts_mnem_map[V_MSOF];
                        break;
                    case 0x3:
                        inst.name = V_MSIF;
                        inst.mnemonic = insts_mnem_map[V_MSIF];
                        break;
                    case 0x10:
                        inst.name = V_IOTA;
                        inst.mnemonic = insts_mnem_map[V_IOTA];
                        break;
                    case 0x11:
                        inst.name = V_ID_V;
                        inst.mnemonic = insts_mnem_map[V_ID_V];
                        break;
                    }
                }
                break;
            default:
                inst.print();
                std::cout << "mask: " << uint32_t_to_dec_hex_bin(MASK(code, bitmask_31_26)) << std::endl;
                throw std::runtime_error("Encountered undefined switch case (6)");
                break;
            }
            break;
        case InstFormat::OPFVV:
        case InstFormat::OPFVF:
            switch (MASK(code, bitmask_31_26))
            {
            case 0x0:
                inst.name = V_FADD;
                inst.mnemonic = insts_mnem_map[V_FADD];
                break;
            case 0x1:
                inst.name = V_FREDUSUM;
                inst.mnemonic = insts_mnem_map[V_FREDUSUM];
                break;
            case 0x3:
                inst.name = V_FREDOSUM;
                inst.mnemonic = insts_mnem_map[V_FREDOSUM];
                break;
            case 0x10:
                if (auto ptr = std::get_if<OPFVVTypeFields>(&inst.payload))
                {
                    if (ptr->vs2 == 0x0)
                    {
                        inst.name = V_FMV_S_F;
                        inst.mnemonic = insts_mnem_map[V_FMV_S_F];
                    }
                    else if (ptr->vs1 == 0x0)
                    {
                        inst.name = V_FMV_F_S;
                        inst.mnemonic = insts_mnem_map[V_FMV_F_S];
                    }
                    else
                    {
                        throw std::runtime_error("Encountered undefined instruction arg value (1)");
                    }
                }
                // TODO: this if is wrong. I do not understand how this is shared
                else if (auto ptr = std::get_if<OPFVFTypeFields>(&inst.payload))
                {
                    if (ptr->vs2 == 0x0)
                    {
                        inst.name = V_FMV_S_F;
                        inst.mnemonic = insts_mnem_map[V_FMV_S_F];
                    }
                    else if (ptr->rs1 == 0x0)
                    {
                        inst.name = V_FMV_F_S;
                        inst.mnemonic = insts_mnem_map[V_FMV_F_S];
                    }
                    else
                    {
                        throw std::runtime_error("Encountered undefined instruction arg value (1)");
                    }
                }
                else
                {
                    throw std::runtime_error("Encountered undefined instruction arg value (2)");
                }
                break;
            case 0x17:
                // TODO: shared space with vfmerge.vfm
                inst.name = V_FMV;
                inst.mnemonic = insts_mnem_map[V_FMV];
                break;
            case 0x1c:
                inst.name = V_MFNE;
                inst.mnemonic = insts_mnem_map[V_MFNE];
                break;
            case 0x24:
                inst.name = V_FMUL;
                inst.mnemonic = insts_mnem_map[V_FMUL];
                break;
            case 0x2b:
                inst.name = V_FNMSUB;
                inst.mnemonic = insts_mnem_map[V_FNMSUB];
                break;
            case 0x2c:
                inst.name = V_FMACC;
                inst.mnemonic = insts_mnem_map[V_FMACC];
                break;
            case 0x2f:
                inst.name = V_FNMSAC;
                inst.mnemonic = insts_mnem_map[V_FNMSAC];
                break;

            default:
                inst.print();
                std::cout << "mask: " << uint32_t_to_dec_hex_bin(MASK(code, bitmask_31_26)) << std::endl;
                throw std::runtime_error("Encountered undefined switch case (5)");
                break;
            }
            break;
        default:
            throw std::runtime_error("Encountered undefined switch case (7)");
            break;
        }
    }
}

void decode_load_fp_instruction(uint32_t code, DecodedInstruction &inst)
{
    inst.name = LOAD_PLACEHOLDER;
    inst.mnemonic = insts_mnem_map[LOAD_PLACEHOLDER];
    // TODO: implement this
    // inst.print();
    // throw std::runtime_error("Unknown load fp instruction");
}

void decode_store_fp_instruction(uint32_t code, DecodedInstruction &inst)
{
    inst.name = STORE_PLACEHOLDER;
    inst.mnemonic = insts_mnem_map[STORE_PLACEHOLDER];
    // TODO: implement
    // inst.print();
    // throw std::runtime_error("Unknown store fp instruction");
}

void decode_system_instruction(uint32_t code, DecodedInstruction &inst)
{
    switch (MASK(code, bitmask_6_0))
    {
    case 0x73:
        inst.name = CSRRS;
        inst.mnemonic = insts_mnem_map[CSRRS];
        break;
    default:
        inst.print();
        std::cout << "Low 6 bits: " << uint32_t_to_dec_hex_bin(MASK(code, bitmask_6_0)) << std::endl;
        throw std::runtime_error("Unknown system instruction");
    }
}

void decode_common_instruction_operands(uint32_t code, DecodedInstruction &inst)
{
    const uint8_t bits_31_27 = MASK(code, bitmask_31_27);
    const uint8_t bits_31_25 = MASK(code, bitmask_31_25);
    const uint8_t bits_30_25 = MASK(code, bitmask_30_25);
    const uint8_t bits_24_20 = MASK(code, bitmask_24_20);
    const uint8_t bits_19_15 = MASK(code, bitmask_19_15);
    const uint8_t bits_11_7 = MASK(code, bitmask_11_7);
    const uint16_t bits_31_20 = MASK(code, bitmask_31_20);
    const uint8_t bits_11_8 = MASK(code, bitmask_11_8);
    const uint8_t bits_7 = MASK(code, bitmask_7);
    const uint32_t bits_31_12 = MASK(code, bitmask_31_12);
    const uint16_t bits_30_21 = MASK(code, bitmask_30_21);
    const uint8_t bits_20 = MASK(code, bitmask_20);
    const uint8_t bits_19_12 = MASK(code, bitmask_19_12);

    switch (inst.format)
    {
    case InstFormat::R:
        inst.payload = RTypeFields(
            (uint8_t)bits_31_25,
            (uint8_t)bits_24_20,
            (uint8_t)bits_19_15,
            (uint8_t)bits_11_7);

        break;
    case InstFormat::R_4:
        inst.payload = R_4TypeFields(
            (uint8_t)bits_31_27,
            (uint8_t)bits_24_20,
            (uint8_t)bits_19_15,
            (uint8_t)bits_11_7);

        break;
    case InstFormat::I:
        inst.payload = ITypeFields(
            (uint16_t)bits_31_20,
            (uint8_t)bits_19_15,
            (uint8_t)bits_11_7);

        break;
    case InstFormat::S:
        inst.payload = STypeFields(
            (uint8_t)bits_31_25,
            (uint8_t)bits_24_20,
            (uint8_t)bits_19_15,
            (uint8_t)bits_11_7);

        break;
    case InstFormat::B:
        inst.payload = BTypeFields(
            (uint8_t)bits_30_25,
            (uint8_t)bits_24_20,
            (uint8_t)bits_19_15,
            (uint8_t)bits_11_8,
            (uint8_t)bits_7);

        break;
    case InstFormat::U:
        inst.payload = UTypeFields(
            (uint32_t)bits_31_12,
            (uint8_t)bits_11_7);

        break;
    case InstFormat::J:
        inst.payload = JTypeFields(
            (uint16_t)bits_30_21,
            (uint8_t)bits_20,
            (uint8_t)bits_19_12,
            (uint8_t)bits_11_7);

        break;
    default:
        throw std::runtime_error("Unknown instruction format to decode operands");
    }
}

void decode_common_instruction(uint32_t code, DecodedInstruction &inst)
{
    switch (MASK(code, bitmask_6_0))
    {
    case 0x3:
        switch (MASK(code, bitmask_14_12))
        {
        case 0x0:
            inst.name = LB;
            inst.mnemonic = insts_mnem_map[LB];
            inst.format = InstFormat::I;
            break;
        case 0x1:
            inst.name = LH;
            inst.mnemonic = insts_mnem_map[LH];
            inst.format = InstFormat::I;
            break;
        case 0x2:
            inst.name = LW;
            inst.mnemonic = insts_mnem_map[LW];
            inst.format = InstFormat::I;
            break;
        case 0x4:
            inst.name = LBU;
            inst.mnemonic = insts_mnem_map[LBU];
            inst.format = InstFormat::I;
            break;
        case 0x5:
            inst.name = LHU;
            inst.mnemonic = insts_mnem_map[LHU];
            inst.format = InstFormat::I;
            break;
        case 0x6:
            inst.name = LWU;
            inst.mnemonic = insts_mnem_map[LWU];
            inst.format = InstFormat::I;
            break;
        case 0x3:
            inst.name = LD;
            inst.mnemonic = insts_mnem_map[LD];
            inst.format = InstFormat::I;
            break;
        default:
            inst.print();
            throw std::runtime_error("Unknown common instruction (10)");
            break;
        }
        break;
    case 0x13:
        inst.name = ADDI;
        inst.mnemonic = insts_mnem_map[ADDI];
        inst.format = InstFormat::I;
        break;
    case 0x17:
        inst.name = AUIPC;
        inst.mnemonic = insts_mnem_map[AUIPC];
        inst.format = InstFormat::U;
        break;
    case 0x1b:
        inst.name = SLRIW;
        inst.mnemonic = insts_mnem_map[SLRIW];
        inst.format = InstFormat::I;
        break;
    case 0xf:
        inst.name = FENCE;
        inst.mnemonic = insts_mnem_map[FENCE];
        inst.format = InstFormat::I;
        break;
    case 0x23:
        inst.name = SD;
        inst.mnemonic = insts_mnem_map[SD];
        inst.format = InstFormat::S;
        break;
    case 0x2f:
        inst.name = LR;
        inst.mnemonic = insts_mnem_map[LR];
        inst.format = InstFormat::R;
        break;
    case 0x33:
        inst.name = ADD;
        inst.mnemonic = insts_mnem_map[ADD];
        inst.format = InstFormat::R;
        break;
    case 0x37:
        inst.name = LUI;
        inst.mnemonic = insts_mnem_map[LUI];
        inst.format = InstFormat::U;
        break;
    case 0x3b:
        inst.name = SUBW;
        inst.mnemonic = insts_mnem_map[SUBW];
        inst.format = InstFormat::R;
        break;
    case 0x43:
        inst.name = FMADD;
        inst.mnemonic = insts_mnem_map[FMADD];
        inst.format = InstFormat::R_4;
        break;
    case 0x4b:
        inst.name = FNMSUB;
        inst.mnemonic = insts_mnem_map[FNMSUB];
        inst.format = InstFormat::R_4;
        break;
    case 0x53:
        inst.name = FMV;
        inst.mnemonic = insts_mnem_map[FMV];
        inst.format = InstFormat::R;
        break;
    case 0x63:
        inst.name = BNE;
        inst.mnemonic = insts_mnem_map[BNE];
        inst.format = InstFormat::B;
        break;
    case 0x67:
        inst.name = JALR;
        inst.mnemonic = insts_mnem_map[JALR];
        inst.format = InstFormat::I;
        break;
    case 0x6f:
        inst.name = JAL;
        inst.mnemonic = insts_mnem_map[JAL];
        inst.format = InstFormat::J;
        break;
    default:
        inst.print();
        std::cout << "Low 6 bits: " << uint32_t_to_dec_hex_bin(MASK(code, bitmask_6_0)) << std::endl;
        throw std::runtime_error("Unknown common instruction");
    }
    if (inst.format == InstFormat::UNKNOWN)
    {
        inst.print();
        throw std::runtime_error("Unknown format");
    }

    decode_common_instruction_operands(code, inst);
}

void decode_uncompressed(uint32_t code, DecodedInstruction &inst)
{
    switch (MASK(code, bitmask_6_0))
    {
    // load store opcodes are overloaded for RVV and F/D
    case 0x7:
        decode_load_fp_instruction(code, inst);
        break;
    case 0x27:
        decode_store_fp_instruction(code, inst);
        break;
    // RVV
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