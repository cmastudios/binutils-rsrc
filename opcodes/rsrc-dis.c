/**
 * Disassemble instructions for rsrc.
 *
 * Copyright 2020 Connor Monahan
 * Licensed under GPLv3+
 */

#include "sysdep.h"
#define STATIC_TABLE
#define DEFINE_TABLE

#include "opcode/rsrc.h"
#include "dis-asm.h"
#include <stdio.h>

extern const rsrc_opc_info_t rsrc_opc_info[32];

static fprintf_ftype fpr;
static void *stream;

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

	instruction = rsrc_opc_info[instruction_raw.form1.op];

	switch (instruction.opcode)
	{
		case RSRC_LD:
		case RSRC_ST:
		case RSRC_LA:
			fpr(stream, "%s r%d, %d(r%d)", instruction.name, instruction_raw.form2.ra, instruction_raw.form2.c2, instruction_raw.form2.rb);
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

