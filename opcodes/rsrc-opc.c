/* Opcode table for RSRC CPU

   Copyright (C) 2020 Connor Monahan

   This file is part of the GNU opcodes library.

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   It is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston,
   MA 02110-1301, USA.  */

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
	{9, "brlnv", RSRC_5_A_B_C_COND, 0},
	{9, "brlzr", RSRC_5_A_B_C_COND, 2},
	{9, "brlnz", RSRC_5_A_B_C_COND, 3},
	{9, "brlpl", RSRC_5_A_B_C_COND, 4},
	{9, "brlmi", RSRC_5_A_B_C_COND, 5},
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
	{31, "stop", RSRC_8, 0}
};
