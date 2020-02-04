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
	unsigned char opcode;

	stream = info->stream;
	fpr = info->fprintf_func;

	if ((status = info->read_memory_func(addr, &opcode, 1, info)))
		goto fail;

	fpr(stream, "%s", rsrc_opc_info[opcode].name);

	return 1;

fail:
	info->memory_error_func(status, addr, info);
	return -1;
}

