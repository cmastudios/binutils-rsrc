/* BFD support for RSRC CPU

   Copyright (C) 2020 Connor Monahan

   Based heavily on file from Moxie port, written by Anthony Green.

   Copyright (C) 1992-2019 Free Software Foundation, Inc.

   This file is part of BFD, the Binary File Descriptor library.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston,
   MA 02110-1301, USA.  */

#ifndef _ELF_RSRC_H
#define _ELF_RSRC_H

#include "elf/reloc-macros.h"

START_RELOC_NUMBERS(elf_rsrc_reloc_type)
	RELOC_NUMBER(R_RSRC_NONE, 0)
	RELOC_NUMBER(R_RSRC_C1, 1)
	RELOC_NUMBER(R_RSRC_C2, 2)
END_RELOC_NUMBERS(R_RSRC_max)

#endif
