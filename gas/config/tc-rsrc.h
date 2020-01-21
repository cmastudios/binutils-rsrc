/* GNU assembler for RSRC CPU

   Copyright (C) 2020 Connor Monahan

   Based heavily on file from Moxie port, written by Anthony Green.

   Copyright (C) 2009-2019 Free Software Foundation, Inc.

   This file is part of GAS, the GNU Assembler.

   GAS is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   GAS is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GAS; see the file COPYING.  If not, write to
   the Free Software Foundation, 51 Franklin Street - Fifth Floor,
   Boston, MA 02110-1301, USA.  */

#define TC_RSRC 1
#define TARGET_BYTES_BIG_ENDIAN 1
#define WORKING_DOT_WORD

#define TARGET_FORMAT "elf32-rsrc"

#define TARGET_ARCH bfd_arch_rsrc

#define md_undefined_symbol(NAME) 0

#define md_estimate_size_before_relax(A, B) (as_fatal(_("estimate size\n")), 0)
#define md_convert_frag(B, S, F) (as_fatal(_("convert_frag\n")), 0)

#define MD_PCREL_FROM_SECTION(FIX, SEC) md_pcrel_from(FIX)
//#define md_pcrel_from(FIX) ((FIX)->fx_where + (FIX)->fx_frag->fr_address)
extern long md_pcrel_from(struct fix *);

#define md_section_align(SEGMENT, SIZE) (SIZE)

