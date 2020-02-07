/**
 * RSRC opcode table
 *
 * Copyright 2020 Connor Monahan
 * Licensed under GPLv3+
 */

#include "sysdep.h"
#include "opcode/rsrc.h"

const rsrc_opc_info_t rsrc_opc_info[32] = {
	{0, "nop", RSRC_8},
	{1, "ld", RSRC_1_A_B_C2},
	{2, "ldr", RSRC_2_A_C1},
	{3, "st", RSRC_1_A_B_C2},
	{4, "str", RSRC_2_A_C1},
	{5, "la", RSRC_1_A_B_C2},
	{6, "lar", RSRC_2_A_C1},
	{7, "unused", RSRC_8},
	{8, "br", RSRC_4_B_C_COND},
	{9, "brl", RSRC_5_A_B_C_COND},
	{10, "unused", RSRC_8},
	{11, "unused", RSRC_8},
	{12, "add", RSRC_6_A_B_C},
	{13, "addi", RSRC_1_A_B_C2},
	{14, "sub", RSRC_6_A_B_C},
	{15, "neg", RSRC_3_A_C},
	{16, "unused", RSRC_8},
	{17, "unused", RSRC_8},
	{18, "unused", RSRC_8},
	{19, "unused", RSRC_8},
	{20, "and", RSRC_6_A_B_C},
	{21, "andi", RSRC_1_A_B_C2},
	{22, "or", RSRC_6_A_B_C},
	{23, "ori", RSRC_1_A_B_C2},
	{24, "not", RSRC_3_A_C},
	{25, "unused", RSRC_8},
	{26, "shr", RSRC_7_A_B_C_COUNT},
	{27, "shra", RSRC_7_A_B_C_COUNT},
	{28, "shl", RSRC_7_A_B_C_COUNT},
	{29, "shc", RSRC_7_A_B_C_COUNT},
	{30, "unused", RSRC_8},
	{31, "unused", RSRC_8}
};
