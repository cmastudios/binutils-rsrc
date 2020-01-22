/**
 * RSRC opcode table
 *
 * Copyright 2020 Connor Monahan
 * Licensed under GPLv3+
 */

#include "sysdep.h"
#include "opcode/rsrc.h"

const rsrc_opc_info_t rsrc_opc_info[32] = {
	{0, "nop"},
	{1, "ld"},
	{2, "ldr"},
	{3, "st"},
	{4, "str"},
	{5, "la"},
	{6, "lar"},
	{7, "unused"},
	{8, "br"},
	{9, "brl"},
	{10, "unused"},
	{11, "unused"},
	{12, "add"},
	{13, "addi"},
	{14, "sub"},
	{15, "neg"},
	{16, "unused"},
	{17, "unused"},
	{18, "unused"},
	{19, "unused"},
	{20, "and"},
	{21, "andi"},
	{22, "or"},
	{23, "ori"},
	{24, "not"},
	{25, "unused"},
	{26, "shr"},
	{27, "shra"},
	{28, "shl"},
	{29, "shc"},
	{30, "unused"},
	{31, "unused"}
};
