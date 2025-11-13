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

enum InstructionType
{
    R_t,
    I_t,
    S_t,
    B_t,
    U_t,
    J_t,
};

enum AllInstructions
{
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
    C_ADD,
    C_FSDSP,
    C_SWSP,
    C_FSWSP,
    C_SDSP,

    // RVV - CONFIG
    VSETVLI_1,
    VSETVLI_2,
    VSETIVLI,
    VSETVL,

    // RVV
    OPIVV,
    OPFVV,
    OPMVV,
    OPIVI,
    OPIVX,
    OPFVF,
    OPMVX,
};

struct BaseInstruction
{
    std::string line;
    uint32_t code;
    std::string mnemonic;
    AllInstructions kind;
    bool compressed;

    virtual void print() {};
};

using c_inst = std::pair<std::string_view, AllInstructions>;
using c_inst_map = std::unordered_map<uint16_t, c_inst>;
using c_insts_map_t = std::unordered_map<AllInstructions, std::string_view>;

extern c_inst_map c_quadrant_0;
extern c_inst_map c_quadrant_1;
extern c_inst_map c_quadrant_2;
extern std::array<c_inst_map, 3> c_insts;
extern c_insts_map_t c_insts_mnem_map;

extern c_inst_map rvv_config_insts;
extern c_inst_map rvv_insts;

// constexpr auto c_quadrant_0 = std::to_array<c_inst>({
//     {0x0, "C.ADDI4SPN", C_ADDI4SPN},
//     {0x2, "C.LW", C_LW},
//     {0x3, "C.FLW", C_FLW},
//     {0x7, "C.FSW", C_FSW},
// });

// constexpr auto c_quadrant_1 = std::to_array<c_inst>({
//     {0x0, "C.ADDI", C_ADDI},
//     {0x1, "C.JAL", C_JAL},
//     {0x2, "C.LI", C_LI},
//     {0x3, "C.ADDI16SP", C_ADDI16SP},
//     {0x4, "C.ADDW", C_ADDW},
//     {0x5, "C.J", C_J},
//     {0x6, "C.BEQZ", C_BEQZ},
//     {0x7, "C.BNEZ", C_BNEZ},
// });

// constexpr auto c_quadrant_2 = std::to_array<c_inst>({
//     {0x0, "C.SLLI", C_SLLI},
//     {0x1, "C.FLDSP", C_FLDSP},
//     {0x2, "C.LWSP", C_LWSP},
//     {0x3, "C.FLWSP", C_FLWSP},
//     {0x4, "C.MV", C_MV},
//     {0x5, "C.FSDSP", C_FSDSP},
//     {0x6, "C.SWSP", C_SWSP},
//     {0x7, "C.FSWSP", C_FSWSP},
// });

// template <typename T, std::size_t... Ns>
// constexpr auto join_arrays(const std::array<T, Ns> &...arrays)
// {
//     std::array<T, (Ns + ...)> result{};
//     T *out = result.data();
//     ((out = std::copy(arrays.begin(), arrays.end(), out)), ...);
//     return result;
// }

// template <std::size_t... Ns>
// constexpr auto offsets()
// {
//     constexpr std::size_t num = sizeof...(Ns);
//     std::array<std::size_t, num + 1> arr{};
//     std::size_t acc = 0;
//     std::size_t values[] = {Ns...};
//     for (std::size_t i = 0; i < num; ++i)
//     {
//         arr[i] = acc;
//         acc += values[i];
//     }
//     arr[num] = acc;
//     return arr;
// }

// constexpr auto c_insts = join_arrays(c_quadrant_0, c_quadrant_1, c_quadrant_2);
// constexpr auto c_insts_group_offsets = offsets<c_quadrant_0.size(), c_quadrant_1.size(), c_quadrant_2.size()>();

// static_assert(c_insts_group_offsets.size() == 4);
// static_assert(c_insts_group_offsets[0] == 0);
// static_assert(c_insts_group_offsets[1] == c_quadrant_0.size());
// static_assert(c_insts_group_offsets[2] == c_quadrant_0.size() + c_quadrant_1.size());
// static_assert(c_insts_group_offsets[3] == c_insts.size());

#endif