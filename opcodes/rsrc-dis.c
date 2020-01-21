/* Instruction disassemble for RSRC CPU

   Copyright (C) 2020 Connor Monahan

   Based heavily on file from Moxie port, written by Anthony Green.

   Copyright (C) 2009-2019 Free Software Foundation, Inc.

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
#define STATIC_TABLE
#define DEFINE_TABLE

#include "opcode/rsrc.h"
#include "dis-asm.h"
#include <stdio.h>

extern const rsrc_opc_info_t rsrc_opc_info[42];

static fprintf_ftype fpr;
static void *stream;

static rsrc_opc_info_t get_inst(rsrc_inst_t raw)
{
	int i;
	rsrc_opc_info_t info;
	// premature optimization is the root of all evil
	for (i = 0; i < 42; ++i)
	{
		info = rsrc_opc_info[i];
		if (info.opcode == raw.form1.op)
		{
			if (info.opcode == RSRC_BR || info.opcode == RSRC_BRL)
			{
				if (info.cond == raw.formcond.cond)
				{
					return info;
				}
			}
			else
			{
				return info;
			}
		}
	}
	info.opcode = 999;
	return info;
}

int c1(int val)
{
	return val | ((val & 0x200000) ? 0xFFE00000 : 0);
}

int c2(int val)
{
	return val | ((val & 0x10000) ? 0xFFFF0000 : 0);
}

int print_insn_rsrc(bfd_vma addr, struct disassemble_info *info)
{
	int status;
	rsrc_inst_t instruction_raw;
	int opcode;
	rsrc_opc_info_t instruction;

	stream = info->stream;
	fpr = info->fprintf_func;

	if ((status = info->read_memory_func(addr, (unsigned char *) &instruction_raw.data, 4, info)))
		goto fail;

	// convert endianness if necessary
	instruction_raw.data = RSRC_HTODL(instruction_raw.data);

	instruction = get_inst(instruction_raw);

	switch (instruction_raw.form1.op)
	{
		case RSRC_LD:
		case RSRC_ST:
		case RSRC_LA:
			fpr(stream, "%s r%d, %d(r%d)", instruction.name, instruction_raw.form2.ra,
					c2(instruction_raw.form2.c2), instruction_raw.form2.rb);
			break;
		case RSRC_LDR:
		case RSRC_STR:
			fpr(stream, "%s r%d, %d", instruction.name, instruction_raw.form1.ra,
					c1(instruction_raw.form1.c1));
			break;
		case RSRC_ADD:
		case RSRC_SUB:
		case RSRC_OR:
		case RSRC_AND:
			fpr(stream, "%s r%d, r%d, r%d", instruction.name, instruction_raw.formcount.ra,
					instruction_raw.formcount.rb, instruction_raw.formcount.rc);
			break;
		case RSRC_NEG:
		case RSRC_NOT:
			fpr(stream, "%s r%d, r%d", instruction.name, instruction_raw.formcount.ra,
					instruction_raw.formcount.rc);
			break;
		case RSRC_ADDI:
		case RSRC_ORI:
		case RSRC_ANDI:
			fpr(stream, "%s r%d, r%d, %d", instruction.name, instruction_raw.form2.ra,
					instruction_raw.form2.rb, c2(instruction_raw.form2.c2));
			break;
		case RSRC_SHR:
		case RSRC_SHRA:
		case RSRC_SHL:
		case RSRC_SHC:
			if (instruction_raw.formcount.count == 0)
				fpr(stream, "%s r%d, r%d, r%d", instruction.name, instruction_raw.formcount.ra,
						instruction_raw.formcount.rb, instruction_raw.formcount.rc);
			else
				fpr(stream, "%s r%d, r%d, %d", instruction.name, instruction_raw.formcount.ra,
						instruction_raw.formcount.rb, instruction_raw.formcount.count);
			break;
		case RSRC_BR:
			fpr(stream, "%s r%d, r%d", instruction.name, instruction_raw.formcount.rb,
					instruction_raw.formcount.rc);
			break;
		case RSRC_BRL:
			fpr(stream, "%s r%d, r%d, r%d", instruction.name, instruction_raw.formcount.ra,
					instruction_raw.formcount.rb, instruction_raw.formcount.rc);
			break;
		default:
			fpr(stream, "%s", instruction.name);
			break;
	}
	return 4;

fail:
	info->memory_error_func(status, addr, info);
	return -1;
}

