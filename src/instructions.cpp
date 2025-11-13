#include "instructions.hpp"

c_inst_map c_quadrant_0 = c_inst_map({
    {0x0, {"C.ADDI4SPN", C_ADDI4SPN}},
    {0x2, {"C.LW", C_LW}},
    {0x3, {"C.FLW", C_FLW}},
    {0x7, {"C.FSW", C_FSW}},
});

c_inst_map c_quadrant_1 = c_inst_map({
    {0x0, {"C.ADDI", C_ADDI}},
    {0x1, {"C.JAL", C_JAL}},
    {0x2, {"C.LI", C_LI}},
    {0x3, {"C.ADDI16SP", C_ADDI16SP}},
    {0x4, {"C.ADDW", C_MISC_ALU_ADDW}},
    {0x5, {"C.J", C_J}},
    {0x6, {"C.BEQZ", C_BEQZ}},
    {0x7, {"C.BNEZ", C_BNEZ}},
});

c_inst_map c_quadrant_2 = c_inst_map({
    {0x0, {"C.SLLI", C_SLLI}},
    {0x1, {"C.FLDSP", C_FLDSP}},
    {0x2, {"C.LWSP", C_LWSP}},
    {0x3, {"C.FLWSP", C_FLWSP}},
    {0x4, {"C.MV", C_MV}},
    {0x5, {"C.FSDSP", C_FSDSP}},
    {0x6, {"C.SWSP", C_SWSP}},
    {0x7, {"C.FSWSP", C_FSWSP}},
});

std::array<c_inst_map, 3> c_insts = std::to_array<c_inst_map, 3>({c_quadrant_0, c_quadrant_1, c_quadrant_2});

c_insts_map_t c_insts_mnem_map = c_insts_map_t({
    {C_ADDI4SPN, "C.ADDI4SPN"},
    {C_FLD, "C.FLD"},
    {C_LW, "C.LW"},
    {C_FLW, "C.FLW"},
    {C_LD, "C.LD"},
    {C_FSD, "C.FSD"},
    {C_SW, "C.SW"},
    {C_FSW, "C.FSW"},
    {C_SD, "C.SD"},
    {C_ADDI, "C.ADDI"},
    {C_JAL, "C.JAL"},
    {C_ADDIW, "C.ADDIW"},
    {C_LI, "C.LI"},
    {C_LUI, "C.LUI"},
    {C_ADDI16SP, "C.ADDI16SP"},
    {C_MISC_ALU_ADDW, "C.MISC_ALU_ADDW"},
    {C_MISC_ALU_, "C.MISC_ALU_"},
    {C_J, "C.J"},
    {C_BEQZ, "C.BEQZ"},
    {C_BNEZ, "C.BNEZ"},
    {C_SLLI, "C.SLLI"},
    {C_FLDSP, "C.FLDSP"},
    {C_LWSP, "C.LWSP"},
    {C_FLWSP, "C.FLWSP"},
    {C_LDSP, "C.LDSP"},
    {C_MV, "C.MV"},
    {C_JR, "C.JR"},
    {C_ADD, "C.ADD"},
    {C_FSDSP, "C.FSDSP"},
    {C_SWSP, "C.SWSP"},
    {C_FSWSP, "C.FSWSP"},
    {C_SDSP, "C.SDSP"},
    {C_EBREAK, "C.EBREAK"},
    {C_NOP, "C.NOP"},
    {C_SUB, "C.SUB"},
    {C_XOR, "C.XOR"},
    {C_OR, "C.OR"},
    {C_AND, "C.AND"},
    {C_SRLI, "C.SRLI"},
    {C_SRAI, "C.SRAI"},
    {C_ANDI, "C.ANDI"},
});

c_inst_map rvv_config_insts = c_inst_map({
    {0x0, {"VSETVLI", VSETVLI_1}},
    {0x1, {"VSETVLI", VSETVLI_2}},
    {0x3, {"VSETIVLI", VSETIVLI}},
    {0x2, {"VSETVL", VSETVL}},
});

c_inst_map rvv_insts = c_inst_map({
    {0x0, {"OPIVV", OPIVV}},
    {0x1, {"OPFVV", OPFVV}},
    {0x2, {"OPMVV", OPMVV}},
    {0x3, {"OPIVI", OPIVI}},
    {0x4, {"OPIVX", OPIVX}},
    {0x5, {"OPFVF", OPFVF}},
    {0x6, {"OPMVX", OPMVX}},
});