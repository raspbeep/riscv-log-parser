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
            break;
        case 0x1:
            inst.name = C_FLD;
            inst.mnemonic = insts_mnem_map[C_FLD];
            break;
        case 0x2:
            inst.name = C_LW;
            inst.mnemonic = insts_mnem_map[C_LW];
            break;
        case 0x3:
            // FLW is RV32 only (omitted)
            inst.name = C_LD;
            inst.mnemonic = insts_mnem_map[C_LD];
            break;
        case 0x4:
            // reserved
            throw std::runtime_error("Encountered reserved opcode");
            break;
        case 0x5:
            inst.name = C_FSD;
            inst.mnemonic = insts_mnem_map[C_FSD];
            break;
        case 0x6:
            inst.name = C_SW;
            inst.mnemonic = insts_mnem_map[C_SW];
            break;
        case 0x7:
            // FSW is RV32 only (omitted)
            inst.name = C_SD;
            inst.mnemonic = insts_mnem_map[C_SD];
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
            }
            else
            {
                inst.name = C_NOP;
                inst.mnemonic = insts_mnem_map[C_NOP];
            }
            break;
        case 0x1:
            // JAL is RV32 only (omitted)
            inst.name = C_ADDIW;
            inst.mnemonic = insts_mnem_map[C_ADDIW];
            break;
        case 0x2:
            inst.name = C_LI;
            inst.mnemonic = insts_mnem_map[C_LI];
            break;
        case 0x3:
            if (MASK(code, bitmask_11_7) == 0x2)
            {
                inst.name = C_ADDI16SP;
                inst.mnemonic = insts_mnem_map[C_ADDI16SP];
            }
            else if (MASK(code, bitmask_11_7) != 0x0 && (MASK(code, bitmask_11_7) != 0x2))
            {
                inst.name = C_LUI;
                inst.mnemonic = insts_mnem_map[C_LUI];
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
                break;
            case 0x1:
                inst.name = C_SRAI;
                inst.mnemonic = insts_mnem_map[C_SRAI];
                break;
            case 0x2:
                inst.name = C_ANDI;
                inst.mnemonic = insts_mnem_map[C_ANDI];
                break;
            case 0x3:
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
            break;
        case 0x6:
            inst.name = C_BEQZ;
            inst.mnemonic = insts_mnem_map[C_BEQZ];
            break;
        case 0x7:
            inst.name = C_BNEZ;
            inst.mnemonic = insts_mnem_map[C_BNEZ];
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
            break;
        case 0x1:
            inst.name = C_FLDSP;
            inst.mnemonic = insts_mnem_map[C_FLDSP];
            break;
        case 0x2:
            inst.name = C_LWSP;
            inst.mnemonic = insts_mnem_map[C_LWSP];
            break;
        case 0x3:
            // FLWSP is RV32 only (omitted)
            inst.name = C_LDSP;
            inst.mnemonic = insts_mnem_map[C_LDSP];
            break;
        case 0x4:
            if (MASK(code, bitmask_12))
            {
                if (!(MASK(code, bitmask_11_7)) && !(MASK(code, bitmask_6_2)))
                {
                    inst.name = C_EBREAK;
                    inst.mnemonic = insts_mnem_map[C_EBREAK];
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
                }
                else
                {
                    inst.name = C_JALR;
                    inst.mnemonic = insts_mnem_map[C_JALR];
                }
            }
            else
            {
                if (MASK(code, bitmask_6_2))
                {
                    inst.name = C_MV;
                    inst.mnemonic = insts_mnem_map[C_MV];
                }
                else
                {
                    inst.name = C_JR;
                    inst.mnemonic = insts_mnem_map[C_JR];
                }
            }
            break;
        case 0x5:
            inst.name = C_FSDSP;
            inst.mnemonic = insts_mnem_map[C_FSDSP];
            break;
        case 0x6:
            inst.name = C_SWSP;
            inst.mnemonic = insts_mnem_map[C_SWSP];
            break;
        case 0x7:
            // FSWSP is RV32 only (omitted)
            inst.name = C_SDSP;
            inst.mnemonic = insts_mnem_map[C_SDSP];
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
                break;
            case 0x2d:
                inst.name = V_MACC;
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
    // TODO: implement this
    // inst.print();
    // throw std::runtime_error("Unknown load fp instruction");
}

void decode_store_fp_instruction(uint32_t code, DecodedInstruction &inst)
{
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

void decode_common_instruction(uint32_t code, DecodedInstruction &inst)
{
    switch (MASK(code, bitmask_6_0))
    {
    case 0x3:
        inst.name = LD;
        inst.mnemonic = insts_mnem_map[LD];
        inst.format = InstFormat::I;
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