/**
 * RSRC opcode table
 *
 * Copyright 2020 Connor Monahan
 * Licensed under GPLv3+
 */

#include "sysdep.h"
#include "opcode/rsrc.h"

const rsrc_opc_info_t rsrc_opc_info[42] = {
	{0, "nop", RSRC_8, 0},
	{1, "ld", RSRC_1_A_B_C2, 0},
	{2, "ldr", RSRC_2_A_C1, 0},
	{3, "st", RSRC_1_A_B_C2, 0},
	{4, "str", RSRC_2_A_C1, 0},
	{5, "la", RSRC_1_A_B_C2, 0},
	{6, "lar", RSRC_2_A_C1, 0},
	{7, "unused", RSRC_8, 0},
	{8, "br", RSRC_4_B_C_COND, 1},
	{8, "brnv", RSRC_4_B_C_COND, 0},
	{8, "brzr", RSRC_4_B_C_COND, 2},
	{8, "brnz", RSRC_4_B_C_COND, 3},
	{8, "brpl", RSRC_4_B_C_COND, 4},
	{8, "brmi", RSRC_4_B_C_COND, 5},
	{9, "brl", RSRC_5_A_B_C_COND, 1},
	{8, "brlnv", RSRC_5_A_B_C_COND, 0},
	{8, "brlzr", RSRC_5_A_B_C_COND, 2},
	{8, "brlnz", RSRC_5_A_B_C_COND, 3},
	{8, "brlpl", RSRC_5_A_B_C_COND, 4},
	{8, "brlmi", RSRC_5_A_B_C_COND, 5},
	{10, "unused", RSRC_8, 0},
	{11, "unused", RSRC_8, 0},
	{12, "add", RSRC_6_A_B_C, 0},
	{13, "addi", RSRC_1_A_B_C2, 0},
	{14, "sub", RSRC_6_A_B_C, 0},
	{15, "neg", RSRC_3_A_C, 0},
	{16, "unused", RSRC_8, 0},
	{17, "unused", RSRC_8, 0},
	{18, "unused", RSRC_8, 0},
	{19, "unused", RSRC_8, 0},
	{20, "and", RSRC_6_A_B_C, 0},
	{21, "andi", RSRC_1_A_B_C2, 0},
	{22, "or", RSRC_6_A_B_C, 0},
	{23, "ori", RSRC_1_A_B_C2, 0},
	{24, "not", RSRC_3_A_C, 0},
	{25, "unused", RSRC_8, 0},
	{26, "shr", RSRC_7_A_B_C_COUNT, 0},
	{27, "shra", RSRC_7_A_B_C_COUNT, 0},
	{28, "shl", RSRC_7_A_B_C_COUNT, 0},
	{29, "shc", RSRC_7_A_B_C_COUNT, 0},
	{30, "unused", RSRC_8, 0},
	{31, "unused", RSRC_8, 0}
};
