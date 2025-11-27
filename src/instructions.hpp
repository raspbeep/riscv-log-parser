#ifndef INSTRUCTIONS_HPP
#define INSTRUCTIONS_HPP

#include <string>
#include <cstdint>
#include <array>
#include <string_view>
#include <utility>
#include <tuple>
#include <vector>
#include <unordered_map>
#include <memory>
#include <variant>
#include <iostream>

#include "utils.hpp"

enum InstType
{
    R_t,
    I_t,
    S_t,
    B_t,
    U_t,
    J_t,
    RV,
    C
};

enum class InstFormat
{
    UNKNOWN,
    R,
    R_4,
    I,
    S,
    B,
    U,
    J,
    CR,
    CI,
    CSS,
    CIW,
    CL,
    CS,
    CA,
    CB,
    CJ,
    OPIVV,
    OPFVV,
    OPMVV,
    OPIVI,
    OPIVX,
    OPFVF,
    OPMVX,
    OPCFG
};

enum InstEnum
{
    LOAD_PLACEHOLDER,
    STORE_PLACEHOLDER,
    // common
    LB,
    LH,
    LW,
    LBU,
    LHU,
    LWU,
    LD,
    JALR,
    JAL,
    CSRRS,
    ADDI,
    AUIPC,
    BNE,
    ADD,
    LUI,
    SLRIW,
    SD,
    SUBW,
    FMV,
    LR,
    FENCE,
    FMADD,
    FNMSUB,

    // RVC
    C_ADDI4SPN,
    C_FLD,
    C_LW,
    C_FLW,
    C_LD,
    C_FSD,
    C_SW,
    C_FSW,
    C_SD,
    C_ADDI,
    C_JAL,
    C_ADDIW,
    C_LI,
    C_LUI,
    C_ADDI16SP,
    C_MISC_ALU_ADDW,
    C_MISC_ALU_,
    C_J,
    C_BEQZ,
    C_BNEZ,
    C_SLLI,
    C_FLDSP,
    C_LWSP,
    C_FLWSP,
    C_LDSP,
    C_MV,
    C_JR,
    C_ADD,
    C_FSDSP,
    C_SWSP,
    C_FSWSP,
    C_SDSP,
    C_EBREAK,
    C_JALR,
    C_NOP,
    C_SUB,
    C_XOR,
    C_OR,
    C_AND,
    C_SRLI,
    C_SRAI,
    C_ANDI,

    // RVV - CONFIG
    V_VSETVLI,
    V_VSETVL,
    V_VSETIVLI,

    // Integer adds
    V_ADD,

    // Integer subtract
    V_SUB,
    V_MERGE,
    V_SMUL,
    V_SLL,
    V_SLIDEDOWN,
    V_RSUB,
    V_RGATHER,

    V_MACC,
    V_MV_S_X,
    V_REDSUM,
    V_MV_X_S,
    V_POPC,
    V_FIRST,
    V_FADD,
    V_FMV,

    V_MSBF,
    V_MSOF,
    V_MSIF,
    V_IOTA,
    V_ID_V,
    V_FMUL,

    V_FMV_S_F,
    V_FMV_F_S,
    V_MFNE,
    V_FREDOSUM,
    V_FREDUSUM,
    V_FMACC,
    V_FNMSAC,
    V_FNMSUB
};

using if_string_map_t = std::unordered_map<InstFormat, std::string_view>;
extern if_string_map_t InstructionFormatStringMap;

enum RVC_reg_num
{
    x8 = 0,
    x9,
    x10,
    x11,
    x12,
    x13,
    x14,
    x15,
};

enum RVC_reg_abi_name
{
    s0 = 0,
    s1,
    a0,
    a1,
    a2,
    a3,
    a4,
    a5,
};

enum RVC_fp_reg_num
{
    f8 = 0,
    f9,
    f10,
    f11,
    f12,
    f13,
    f14,
    f15,
};

enum RVC_fp_reg_abi_name
{
    fs0 = 0,
    fs1,
    fa0,
    fa1,
    fa2,
    fa3,
    fa4,
    fa5,
};

using RVC_reg_num_names_map_t = std::unordered_map<RVC_reg_num, std::string_view>;
extern RVC_reg_num_names_map_t RVC_reg_num_names_map;

static std::string_view get_rvc_reg_name(uint8_t reg_num)
{
    return RVC_reg_num_names_map[static_cast<RVC_reg_num>(reg_num)];
}

struct TypeFields
{
    virtual void print_operands() const = 0;
};

struct OPIVVTypeFields : TypeFields
{
    uint8_t vs2;
    uint8_t vs1;
    uint8_t vd;

    OPIVVTypeFields(uint8_t vs2, uint8_t vs1, uint8_t vd) : vs2(vs2), vs1(vs1), vd(vd) {}

    void print_operands() const override
    {
        std::cout << "vs2: " << +vs2 << " vs1: " << +vs1 << " vd: " << +vd;
    }
};

struct OPFVVTypeFields : TypeFields
{
    uint8_t vs2, vs1, vd_rd;
    OPFVVTypeFields(uint8_t vs2, uint8_t vs1, uint8_t vd_rd)
        : vs2(vs2), vs1(vs1), vd_rd(vd_rd) {}
    void print_operands() const override
    {
        std::cout << "vs2: " << +vs2 << " vs1: " << +vs1 << " vd/rd: " << +vd_rd;
    }
};

struct OPMVVTypeFields : TypeFields
{
    uint8_t vs2, vs1, vd_rd;
    OPMVVTypeFields(uint8_t vs2, uint8_t vs1, uint8_t vd_rd)
        : vs2(vs2), vs1(vs1), vd_rd(vd_rd) {}
    void print_operands() const override
    {
        std::cout << "vs2: " << +vs2 << " vs1: " << +vs1 << " vd/rd: " << +vd_rd;
    }
};

struct OPIVITypeFields : TypeFields
{
    uint8_t vs2, imm, vd;
    OPIVITypeFields(uint8_t vs2, uint8_t imm, uint8_t vd)
        : vs2(vs2), imm(imm), vd(vd) {}
    void print_operands() const override
    {
        std::cout << "vs2: " << +vs2 << " imm: " << +imm << " vd: " << +vd;
    }
};

struct OPIVXTypeFields : TypeFields
{
    uint8_t vs2, rs1, vd;
    OPIVXTypeFields(uint8_t vs2, uint8_t rs1, uint8_t vd)
        : vs2(vs2), rs1(rs1), vd(vd) {}
    void print_operands() const override
    {
        std::cout << "vs2: " << +vs2 << " rs1: " << +rs1 << " vd: " << +vd;
    }
};

struct OPFVFTypeFields : TypeFields
{
    uint8_t vs2, rs1, vd;
    OPFVFTypeFields(uint8_t vs2, uint8_t rs1, uint8_t vd)
        : vs2(vs2), rs1(rs1), vd(vd) {}
    void print_operands() const override
    {
        std::cout << "vs2: " << +vs2 << " rs1: " << +rs1 << " vd: " << +vd;
    }
};

struct OPMVXTypeFields : TypeFields
{
    uint8_t vs2, rs1, vd_rd;
    OPMVXTypeFields(uint8_t vs2, uint8_t rs1, uint8_t vd_rd)
        : vs2(vs2), rs1(rs1), vd_rd(vd_rd) {}
    void print_operands() const override
    {
        std::cout << "vs2: " << +vs2 << " rs1: " << +rs1 << " vd/rd: " << +vd_rd;
    }
};

struct CRTypeFields : TypeFields
{
    uint8_t rd_rs1, rs2;
    CRTypeFields(uint8_t rd_rs1, uint8_t rs2)
        : rd_rs1(rd_rs1), rs2(rs2) {}
    void print_operands() const override
    {
        std::cout << "x" << +rd_rs1 << ", x" << +rs2;
    }
};

struct CITypeFields : TypeFields
{
    uint8_t imm_upper, rd_rs1, imm_lower;
    CITypeFields(uint8_t imm_upper, uint8_t rd_rs1, uint8_t imm_lower)
        : imm_upper(imm_upper), rd_rs1(rd_rs1), imm_lower(imm_lower) {}
    void print_operands() const override
    {
        std::cout << "imm_upper: " << +imm_upper << " rd/rs1: " << +rd_rs1 << " imm_lower: " << +imm_lower;
    }
};

struct CSSTypeFields : TypeFields
{
    uint8_t imm, rs2;
    CSSTypeFields(uint8_t imm, uint8_t rs2)
        : imm(imm), rs2(rs2) {}
    void print_operands() const override
    {
        std::cout << "imm: " << +imm << " rs2: " << +rs2;
    }
};

struct CIWTypeFields : TypeFields
{
    uint8_t imm, rd;
    CIWTypeFields(uint8_t imm, uint8_t rd)
        : imm(imm), rd(rd) {}
    void print_operands() const override
    {
        std::cout << "imm: " << +imm << ", " << get_rvc_reg_name(rd);
    }
};

struct CLTypeFields : TypeFields
{
    uint8_t imm_upper, rs1, imm_lower, rd;
    CLTypeFields(uint8_t imm_upper, uint8_t rs1, uint8_t imm_lower, uint8_t rd)
        : imm_upper(imm_upper), rs1(rs1), imm_lower(imm_lower), rd(rd) {}
    void print_operands() const override
    {
        std::cout << "imm_upper: " << +imm_upper << ", " << get_rvc_reg_name(rs1) << " imm_lower: " << +imm_lower << ", " << get_rvc_reg_name(rd);
    }
};

struct CSTypeFields : TypeFields
{
    uint8_t imm_upper, rs1, imm_lower, rs2;
    CSTypeFields(uint8_t imm_upper, uint8_t rs1, uint8_t imm_lower, uint8_t rs2)
        : imm_upper(imm_upper), rs1(rs1), imm_lower(imm_lower), rs2(rs2) {}
    void print_operands() const override
    {
        std::cout << "imm_upper: " << +imm_upper << ", " << get_rvc_reg_name(rs1) << " imm_lower: " << +imm_lower << ", " << get_rvc_reg_name(rs2);
    }
};

struct CATypeFields : TypeFields
{
    uint8_t rd_rs1, rs2;
    CATypeFields(uint8_t rd_rs1, uint8_t rs2)
        : rd_rs1(rd_rs1), rs2(rs2) {}
    void print_operands() const override
    {
        std::cout << get_rvc_reg_name(rd_rs1) << ", " << get_rvc_reg_name(rs2);
    }
};

struct CBTypeFields : TypeFields
{
    uint8_t offset_upper, rd_rs1, offset_lower;
    int8_t offset;

    CBTypeFields(uint8_t offset_upper, uint8_t rd_rs1, uint8_t offset_lower)
        : offset_upper(offset_upper), rd_rs1(rd_rs1), offset_lower(offset_lower), offset(0) {}

    CBTypeFields(uint8_t offset_upper, uint8_t rd_rs1, uint8_t offset_lower, int8_t offset)
        : offset_upper(offset_upper), rd_rs1(rd_rs1), offset_lower(offset_lower), offset(offset) {}

    void print_operands() const override
    {
        std::cout << get_rvc_reg_name(rd_rs1) << ", " << +offset;
    }
};

struct CJTypeFields : TypeFields
{
    uint16_t imm;
    int32_t offset;
    CJTypeFields(uint16_t imm)
        : imm(imm)
    {
    }

    CJTypeFields(uint16_t imm, int32_t offset)
        : imm(imm), offset(offset)
    {
    }

    void print_operands() const override
    {
        std::cout << "imm: " << uint32_t_to_dec_hex_bin(imm) << " offset: " << offset;
    }
};

struct RTypeFields : TypeFields
{
    uint8_t funct7;
    uint8_t rs2;
    uint8_t rs1;
    uint8_t rd;
    RTypeFields(uint8_t funct7, uint8_t rs2, uint8_t rs1, uint8_t rd)
        : funct7(funct7), rs2(rs2), rs1(rs1), rd(rd) {}
    void print_operands() const override
    {
        std::cout << "funct7: " << +funct7
                  << ", rs2: " << +rs2
                  << ", rs1: " << +rs1
                  << ", rd: " << +rd;
    }
};

struct R_4TypeFields : TypeFields
{
    uint8_t rs3;
    uint8_t rs2;
    uint8_t rs1;
    uint8_t rm;
    R_4TypeFields(uint8_t rs3, uint8_t rs2, uint8_t rs1, uint8_t rm)
        : rs3(rs3), rs2(rs2), rs1(rs1), rm(rm) {}
    void print_operands() const override
    {
        std::cout << "rs3: " << +rs3
                  << ", rs2: " << +rs2
                  << ", rs1: " << +rs1
                  << ", rm: " << +rm;
    }
};

struct ITypeFields : TypeFields
{
    uint16_t imm;
    uint8_t rs1;
    uint8_t rd;
    ITypeFields(uint16_t imm, uint8_t rs1, uint8_t rd)
        : imm(imm), rs1(rs1), rd(rd) {}
    void print_operands() const override
    {
        std::cout << "imm: " << +imm
                  << ", rs1: " << +rs1
                  << ", rd: " << +rd;
    }
};

struct STypeFields : TypeFields
{
    uint8_t imm_upper;
    uint8_t rs2;
    uint8_t rs1;
    uint8_t imm_lower;
    STypeFields(uint8_t imm_upper, uint8_t rs2, uint8_t rs1, uint8_t imm_lower)
        : imm_upper(imm_upper), rs2(rs2), rs1(rs1), imm_lower(imm_lower) {}
    void print_operands() const override
    {
        std::cout << "imm_upper: " << +imm_upper
                  << ", rs2: " << +rs2
                  << ", rs1: " << +rs1
                  << ", imm_lower: " << +imm_lower;
    }
};

struct BTypeFields : TypeFields
{
    uint8_t imm_upper;
    uint8_t rs2;
    uint8_t rs1;
    uint8_t imm_lower;
    uint8_t bit_7;

    int16_t immediate;

    BTypeFields(uint8_t imm_upper, uint8_t rs2, uint8_t rs1,
                uint8_t imm_lower, uint8_t bit_7)
        : imm_upper(imm_upper), rs2(rs2), rs1(rs1), imm_lower(imm_lower), bit_7(bit_7) {}

    BTypeFields(uint8_t imm_upper, uint8_t rs2, uint8_t rs1,
                uint8_t imm_lower, uint8_t bit_7, int16_t imm)
        : imm_upper(imm_upper), rs2(rs2), rs1(rs1), imm_lower(imm_lower), bit_7(bit_7), immediate(imm) {}
    void print_operands() const override
    {
        std::cout << "imm_upper: " << +imm_upper
                  << ", x" << +rs2
                  << ", x" << +rs1
                  << ", imm_lower: " << +imm_lower
                  << ", bit_7: " << +bit_7
                  << ", imm: " << immediate;
    }
};

struct UTypeFields : TypeFields
{
    uint32_t imm;
    uint8_t rd;
    UTypeFields(uint32_t imm, uint8_t rd)
        : imm(imm), rd(rd) {}
    void print_operands() const override
    {
        std::cout << "imm: " << +imm
                  << ", rd: " << +rd;
    }
};

struct JTypeFields : TypeFields
{
    uint16_t imm_upper;
    uint8_t bit_20;
    uint8_t imm_lower;
    uint8_t rd;
    JTypeFields(uint16_t imm_upper, uint8_t bit_20, uint8_t imm_lower, uint8_t rd)
        : imm_upper(imm_upper), bit_20(bit_20), imm_lower(imm_lower), rd(rd) {}
    void print_operands() const override
    {
        std::cout << "imm_upper: " << +imm_upper
                  << ", bit_20: " << +bit_20
                  << ", imm_lower: " << +imm_lower
                  << ", rd: " << +rd;
    }
};

using FormatPayload = std::variant<
    std::monostate,

    RTypeFields,
    R_4TypeFields,
    ITypeFields,
    STypeFields,
    BTypeFields,
    UTypeFields,
    JTypeFields,

    // RTypeFields,
    OPIVVTypeFields,
    OPFVVTypeFields,
    OPMVVTypeFields,
    OPIVITypeFields,
    OPIVXTypeFields,
    OPFVFTypeFields,
    OPMVXTypeFields,
    CRTypeFields,
    CITypeFields,
    CSSTypeFields,
    CIWTypeFields,
    CLTypeFields,
    CSTypeFields,
    CATypeFields,
    CBTypeFields,
    CJTypeFields>;

struct DecodedInstruction
{
    uint32_t code;
    std::string_view mnemonic;
    std::string line;
    InstEnum name;
    InstFormat format;
    FormatPayload payload;

    bool compressed;

    void print()
    {
        std::cout << "[Instruction print]" << std::endl;
        std::cout << line << std::endl;
        std::cout << "format: " << InstructionFormatStringMap[format] << std::endl;
    };

    void print_payload() const
    {
        std::visit([](const auto &payload)
                   {
        if constexpr (!std::is_same_v<std::decay_t<decltype(payload)>, std::monostate>)
        {
            payload.print_operands();
        } }, payload);
    }
};

using insts_map_t = std::unordered_map<InstEnum, std::string_view>;
extern insts_map_t insts_mnem_map;
using inst_u_ptr = std::unique_ptr<DecodedInstruction>;

#include <bitset>
#include <array>

static constexpr std::array<std::pair<int, int>, 11> c_j_offset_bit_map = {{{11, 10}, // to from
                                                                            {4, 9},
                                                                            {9, 8},
                                                                            {8, 7},
                                                                            {10, 6},
                                                                            {6, 5},
                                                                            {7, 4},
                                                                            {3, 3},
                                                                            {2, 2},
                                                                            {1, 1},
                                                                            {5, 0}}};

static constexpr std::array<std::pair<int, int>, 8> c_b_offset_bit_map = {{{8, 7}, // to from
                                                                           {4, 6},
                                                                           {3, 5},
                                                                           {7, 4},
                                                                           {6, 3},
                                                                           {2, 2},
                                                                           {1, 1},
                                                                           {5, 0}}};

template <typename OFFSET_B_M_T>
uint32_t extract_offset(uint16_t offset_raw, OFFSET_B_M_T bitmap)
{
    uint16_t offset = 0;
    for (const auto &[to, from] : bitmap)
        offset |= ((offset_raw >> from) & 0x1) << to;
    return offset;
}

#endif