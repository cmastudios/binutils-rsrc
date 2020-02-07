/**
 * RSRC assembler
 *
 * Copyright 2020 Connor Monahan
 * Licensed under GPLv3+
 */

#include "as.h"
#include "safe-ctype.h"
#include "opcode/rsrc.h"

extern const rsrc_opc_info_t rsrc_opc_info[32];

const char comment_chars[] = "#";
const char line_separator_chars[] = ";";
const char line_comment_chars[] = "#";

static int pending_reloc;
static struct hash_control *opcode_hash_control;

const pseudo_typeS md_pseudo_table[] = {
	{0, 0, 0}
};

const char FLT_CHARS[] = "rRsSfFdDxXpP";
const char EXP_CHARS[] = "eE";

void md_operand(expressionS *op __attribute__((unused)))
{
}

void md_begin(void)
{
	opcode_hash_control = hash_new();

	for (int i = 0; i < 32; i++)
	{
		hash_insert(opcode_hash_control, rsrc_opc_info[i].name, (char *) &rsrc_opc_info[i]);
	}

	bfd_set_arch_mach(stdoutput, TARGET_ARCH, 0);
}

// Parse args of the format ra, c2(rb) where (rb) is optional
static int parse_args_displacement(rsrc_inst_t *output, char *str)
{
	int argc, ra, c2, rb;
	ra = c2 = rb = 0;
	argc = sscanf(str, " r%d,%d(r%d)", &ra, &c2, &rb);
	output->form2.ra = ra;
	output->form2.c2 = c2;
	output->form2.rb = rb;

	if (argc < 2) {
		as_bad("Not enough arguments");
		return -1;
	}
	return 0;
}

void md_assemble(char *str)
{
	char *op_start;
	char *op_end;
	char *arg_start;
	char old_op_end;

	rsrc_opc_info_t *opcode;
	rsrc_inst_t *output;

	// find the opcode
	op_end = op_start = str;
	while (*op_end && *op_end != ' ') // loop until space or end of line
		++op_end;
	old_op_end = *op_end;
	*op_end = '\0'; // null terminate (required for hash_find)

	opcode = (rsrc_opc_info_t *) hash_find(opcode_hash_control, op_start);
	*op_end = old_op_end; // restore

	if (!opcode)
	{
		as_bad(_("unknown opcode %s"), op_start);
		return;
	}

	output = frag_more(4);
	output->data = 0;
	output->form1.op = opcode->opcode;

	switch (opcode->opcode)
	{
		case RSRC_NOP:
			break;
		case RSRC_LA:
			if (parse_args_displacement(output, op_end) < 0) return;
			break;
		default:
			as_bad("what are ya doin in my swamp %d", opcode->opcode);
			return;
	}

	output->data = RSRC_HTODL(output->data);

	if (pending_reloc)
	{
		as_bad("whoops forgot to take out the trash");
	}
}

const char * md_atof(int type, char *litP, int *sizeP)
{
	int prec;
	LITTLENUM_TYPE words[4];
	char *t;
	int i;

	switch (type)
	{
		case 'f':
			prec = 2;
			break;

		case 'd':
			prec = 4;
			break;

		default:
			*sizeP = 0;
			return _("bad call to md_atof");
	}

	t = atof_ieee(input_line_pointer, type, words);
	if (t)
	{
		input_line_pointer = t;
	}

	*sizeP = prec * 2;

	for (i = prec -1; i >= 0; --i)
	{
		md_number_to_chars(litP, (valueT) words[i], 2);
		litP += 2;
	}

	return NULL;
}

const char *md_shortopts = "";

struct option md_longopts[] = {
	{NULL, no_argument, NULL, 0}
};

size_t md_longopts_size = sizeof(md_longopts);

int md_parse_option(int c ATTRIBUTE_UNUSED, const char *arg ATTRIBUTE_UNUSED)
{
	return 0;
}

void md_show_usage(FILE *stream ATTRIBUTE_UNUSED)
{
}

void md_apply_fix(fixS *fixP ATTRIBUTE_UNUSED, valueT *valP ATTRIBUTE_UNUSED, segT seg ATTRIBUTE_UNUSED)
{
}

void md_number_to_chars(char *ptr, valueT use, int nbytes)
{
	number_to_chars_bigendian(ptr, use, nbytes);
}

arelent * tc_gen_reloc(asection *section ATTRIBUTE_UNUSED, fixS *fixp)
{
	arelent *rel;
	bfd_reloc_code_real_type r_type;

	rel = xmalloc(sizeof(arelent));
	rel->sym_ptr_ptr = xmalloc(sizeof(asymbol *));
	*rel->sym_ptr_ptr = symbol_get_bfdsym(fixp->fx_addsy);
	rel->address = fixp->fx_frag->fr_address + fixp->fx_where;

	r_type = fixp->fx_r_type;
	rel->addend = fixp->fx_addnumber;
	rel->howto = bfd_reloc_type_lookup(stdoutput, r_type);

	if (!rel->howto)
	{
		as_bad_where(fixp->fx_file, fixp->fx_line, _("Cannot represent relocation type %s"), bfd_get_reloc_code_name(r_type));
		// fill it with trash
		rel->howto = bfd_reloc_type_lookup(stdoutput, BFD_RELOC_32);
		if (!rel->howto)
			as_bad_where(fixp->fx_file, fixp->fx_line, "FUCK FUCK FUCK");
	}

	return rel;
}


