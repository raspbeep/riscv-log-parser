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
    R,
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
    CB,
    CJ,
    OPIVV,
    OPFVV,
    OPMVV,
    OPIVI,
    OPIVX,
    OPFVF,
    OPMVX,
    OPCFG,
    UNKNOWN
};

enum InstEnum
{
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
    V_VSETVLI,
    V_VSETVL,
    V_VSETIVLI,

    // Integer adds
    V_ADD_VV,
    V_ADD_VX,
    V_ADD_VI,

    // Integer subtract
    V_SUB_VV,
    V_SUB_VX,

    // Integer reverse subtract
    VR_SUB_VX,
    VR_SUB_VI,

    // Widening unsigned integer add/subtract, 2*SEW = SEW +/- SEW
    VW_ADDU_VV,
    VW_ADDU_VX,
    VW_SUBU_VV,
    VW_SUBU_VX,

    // Widening signed integer add/subtract, 2*SEW = SEW +/- SEW
    VW_ADD_VV,
    VW_ADD_VX,
    VW_SUB_VV,
    VW_SUB_VX,

    // Widening unsigned integer add/subtract, 2*SEW = 2*SEW +/- SEW
    VW_ADDU_WV,
    VW_ADDU_WX,
    VW_SUBU_WV,
    VW_SUBU_WX,

    // Widening signed integer add/subtract, 2*SEW = 2*SEW +/- SEW
    VW_ADD_WV,
    VW_ADD_WX,
    VW_SUB_WV,
    VW_SUB_WX,

    // Zero-extend SEW/2 source to SEW destination
    V_ZEXT_VF2,

    // Sign-extend SEW/2 source to SEW destination
    V_SEXT_VF2,

    // Zero-extend SEW/4 source to SEW destination
    V_ZEXT_VF4,

    // Sign-extend SEW/4 source to SEW destination
    V_SEXT_VF4,

    // Zero-extend SEW/8 source to SEW destination
    V_ZEXT_VF8,

    // Sign-extend SEW/8 source to SEW destination
    V_SEXT_VF8,

};

struct RTypeFields
{
    uint8_t rd, rs1, rs2, funct3, funct7;
};

struct OPIVVTypeFields
{
    uint8_t vs2;
    uint8_t vs1;
    uint8_t vd;
};

struct OPFVVTypeFields
{
    uint8_t vs2;
    uint8_t vs1;
    uint8_t vd_rd;
};

struct OPMVVTypeFields
{
    uint8_t vs2;
    uint8_t vs1;
    uint8_t vd_rd;
};

struct OPIVITypeFields
{
    uint8_t vs2;
    uint8_t imm;
    uint8_t vd;
};

struct OPIVXTypeFields
{
    uint8_t vs2;
    uint8_t rs1;
    uint8_t vd;
};

struct OPFVFTypeFields
{
    uint8_t vs2;
    uint8_t rs1;
    uint8_t vd;
};

struct OPMVXTypeFields
{
    uint8_t vs2;
    uint8_t rs1;
    uint8_t vd_rd;
};

// struct OPCFGTypeFields
// {
//     uint8_t vs2;
//     uint8_t;
//     uint8_t;
// };

using FormatPayload = std::variant<
    std::monostate,
    RTypeFields,
    OPIVVTypeFields,
    OPFVVTypeFields,
    OPMVVTypeFields,
    OPIVITypeFields,
    OPIVXTypeFields,
    OPFVFTypeFields,
    OPMVXTypeFields>;

struct DecodedInstruction
{
    uint32_t code;
    std::string_view mnemonic;
    std::string line;
    InstEnum name;
    InstFormat format;
    FormatPayload payload;

    bool compressed;

    virtual void print() {};
};

using c_insts_map_t = std::unordered_map<InstEnum, std::string_view>;
extern c_insts_map_t c_insts_mnem_map;
using inst_u_ptr = std::unique_ptr<DecodedInstruction>;

#endif