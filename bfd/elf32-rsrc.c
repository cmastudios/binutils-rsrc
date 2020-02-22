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
#include "elf/rsrc.h"

static bfd_reloc_status_type rsrc_elf_reloc(bfd *abfd, arelent *reloc_entry, asymbol *symbol_in, void * data, asection *input_section, bfd *output_bfd, char **error_message ATTRIBUTE_UNUSED) 
{
	unsigned long insn;
	bfd_vma sym_value;
	enum elf_rsrc_reloc_type r_type;
	bfd_vma addr = reloc_entry->address;
	bfd_byte *hit_data = addr + (bfd_byte *) data;

	r_type = (enum elf_rsrc_reloc_type) reloc_entry->howto->type;

	if (output_bfd != NULL) {
		/* Partial linking--do nothing.  */
		reloc_entry->address += input_section->output_offset;
		return bfd_reloc_ok;
	}

	if (symbol_in != NULL && bfd_is_und_section(symbol_in->section))
		return bfd_reloc_undefined;

	if (bfd_is_com_section(symbol_in->section))
		sym_value = 0;
	else
		sym_value = (symbol_in->value +
			 symbol_in->section->output_section->vma +
			 symbol_in->section->output_offset);

	switch (r_type) {
	case R_RSRC_C1:
		insn = bfd_get_32(abfd, hit_data);
		insn += sym_value + reloc_entry->addend;
		bfd_put_32(abfd, (bfd_vma) insn, hit_data);
		break;

	default:
		abort();
		break;
	}

	return bfd_reloc_ok;

}

static reloc_howto_type rsrc_elf_howto_table[] = {
    /* No relocation.  */
    HOWTO(R_RSRC_NONE,		/* type */
	  0,			/* rightshift */
	  0,			/* size (0 = byte, 1 = short, 2 = long) */
	  0,			/* bitsize */
	  FALSE,		/* pc_relative */
	  0,			/* bitpos */
	  complain_overflow_dont,	/* complain_on_overflow */
	  NULL,	/* special_function */
	  "R_RSRC_NONE",		/* name */
	  FALSE,		/* partial_inplace */
	  0,			/* src_mask */
	  0,			/* dst_mask */
	  FALSE),		/* pcrel_offset */

    /* 32 bit absolute relocation.  Setting partial_inplace to TRUE and
       src_mask to a non-zero value is similar to the COFF toolchain.  */
    HOWTO(R_RSRC_C1,		/* type */
	  0,			/* rightshift */
	  2,			/* size (0 = byte, 1 = short, 2 = long) */
	  22,			/* bitsize */
	  TRUE,		/* pc_relative */
	  0,			/* bitpos */
	  complain_overflow_bitfield,	/* complain_on_overflow */
	  NULL,	/* special_function */
	  "R_RSRC_C1",	/* name */
	  TRUE,			/* partial_inplace */
	  0x3fffff,		/* src_mask */
	  0x3fffff,		/* dst_mask */
	  FALSE),		/* pcrel_offset */

    /* 32 bit absolute relocation.  Setting partial_inplace to TRUE and
       src_mask to a non-zero value is similar to the COFF toolchain.  */
    HOWTO(R_RSRC_C2,		/* type */
	  0,			/* rightshift */
	  2,			/* size (0 = byte, 1 = short, 2 = long) */
	  17,			/* bitsize */
	  FALSE,		/* pc_relative */
	  0,			/* bitpos */
	  complain_overflow_bitfield,	/* complain_on_overflow */
	  NULL,	/* special_function */
	  "R_RSRC_C2",	/* name */
	  TRUE,			/* partial_inplace */
	  0x1ffff,		/* src_mask */
	  0x1ffff,		/* dst_mask */
	  FALSE),		/* pcrel_offset */
};

/* This structure is used to map BFD reloc codes to ggx elf relocs.  */

struct elf_reloc_map {
    bfd_reloc_code_real_type bfd_reloc_val;
    unsigned char elf_reloc_val;
};

/* An array mapping BFD reloc codes to ggx elf relocs.  */

static const struct elf_reloc_map rsrc_reloc_map[] = {
    { BFD_RELOC_NONE, R_RSRC_NONE },
    { BFD_RELOC_32, R_RSRC_C2 },
    { BFD_RELOC_32_PCREL, R_RSRC_C1 }
};

/* Given a BFD reloc code, return the howto structure for the
   corresponding ggx elf reloc.  */

static reloc_howto_type *rsrc_elf_reloc_type_lookup(bfd *
						   abfd ATTRIBUTE_UNUSED,
						   bfd_reloc_code_real_type
						   code)
{
    unsigned int i;

    for (i = 0; i < sizeof(rsrc_reloc_map) / sizeof(struct elf_reloc_map); i++)
	{
		if (rsrc_reloc_map[i].bfd_reloc_val == code)
			return &rsrc_elf_howto_table[(int) rsrc_reloc_map[i].elf_reloc_val];
	}

    return NULL;
}

static reloc_howto_type *rsrc_elf_reloc_name_lookup(bfd *
						   abfd ATTRIBUTE_UNUSED,
						   const char *r_name)
{
    unsigned int i;

    for (i = 0; i < sizeof(rsrc_elf_howto_table) / sizeof(rsrc_elf_howto_table[0]); i++)
	{
		if (rsrc_elf_howto_table[i].name != NULL
			&& strcasecmp(rsrc_elf_howto_table[i].name, r_name) == 0)
			return &rsrc_elf_howto_table[i];
	}

    return NULL;
}

/* Given an ELF reloc, fill in the howto field of a relent.  */

static int
rsrc_elf_info_to_howto(bfd * abfd ATTRIBUTE_UNUSED,
		      arelent * cache_ptr, Elf_Internal_Rela * dst)
{
    unsigned int r;

    r = ELF32_R_TYPE(dst->r_info);

    BFD_ASSERT(r < (unsigned int) R_RSRC_max);

    cache_ptr->howto = &rsrc_elf_howto_table[r];

	return TRUE;
}

#define TARGET_BIG_SYM		rsrc_elf32_vec
#define TARGET_BIG_NAME		"elf32-rsrc"
#define ELF_ARCH			bfd_arch_rsrc
#define ELF_MACHINE_CODE	EM_RSRC
#define ELF_MAXPAGESIZE		1

#define bfd_elf32_bfd_reloc_type_lookup		rsrc_elf_reloc_type_lookup
#define bfd_elf32_bfd_reloc_name_lookup		rsrc_elf_reloc_name_lookup
#define elf_info_to_howto					rsrc_elf_info_to_howto

#include "elf32-target.h"
