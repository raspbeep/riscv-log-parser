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
    // common
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

using if_string_map_t = std::unordered_map<InstFormat, std::string_view>;
extern if_string_map_t InstructionFormatStringMap;

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
};

using insts_map_t = std::unordered_map<InstEnum, std::string_view>;
extern insts_map_t insts_mnem_map;
using inst_u_ptr = std::unique_ptr<DecodedInstruction>;

#endif