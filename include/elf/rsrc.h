/**
 * RSRC support in ELF
 *
 * Copyright (c) 2020 Connor Monahan
 *
 * Distributed under the GPLv3+ license.
 */

#ifndef _ELF_RSRC_H
#define _ELF_RSRC_H

#include "elf/reloc-macros.h"

START_RELOC_NUMBERS(elf_rsrc_reloc_type)
	RELOC_NUMBER(R_RSRC_NONE, 0)
	RELOC_NUMBER(R_RSRC_C1, 1)
	RELOC_NUMBER(R_RSRC_C2, 2)
END_RELOC_NUMBERS(R_RSRC_max)

#endif
