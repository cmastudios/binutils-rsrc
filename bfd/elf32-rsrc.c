/**
 * RSRC support in BFD
 *
 * Copyright (c) 2020 Connor Monahan
 *
 * Distributed under the GPLv3+ license.
 */
#include "sysdep.h"
#include "bfd.h"
#include "libbfd.h"
#include "elf-bfd.h"

#define TARGET_BIG_SYM		rsrc_elf32_vec
#define TARGET_BIG_NAME		"elf32-rsrc"
#define ELF_ARCH			bfd_arch_rsrc
#define ELF_MACHINE_CODE	EM_RSRC
#define ELF_MAXPAGESIZE		1
#define bfd_elf32_bfd_reloc_type_lookup	bfd_default_reloc_type_lookup

#define bfd_elf32_bfd_reloc_type_lookup		bfd_default_reloc_type_lookup
#define bfd_elf32_bfd_reloc_name_lookup		_bfd_norelocs_bfd_reloc_name_lookup
#define elf_info_to_howto					_bfd_elf_no_info_to_howto

#include "elf32-target.h"
