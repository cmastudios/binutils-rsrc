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

const bfd_arch_info_type bfd_rsrc_arch = {
	32,
	32,
	8,
	bfd_arch_rsrc,
	bfd_mach_rsrc,
	"rsrc",
	"rsrc",
	2,
	TRUE,
	bfd_default_compatible,
	bfd_default_scan,
	0
};
