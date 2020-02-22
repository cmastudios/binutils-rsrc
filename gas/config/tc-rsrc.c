/**
 * RSRC assembler
 *
 * Copyright 2020 Connor Monahan
 * Licensed under GPLv3+
 */

#include "as.h"
#include "safe-ctype.h"
#include "opcode/rsrc.h"

extern const rsrc_opc_info_t rsrc_opc_info[42];

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

	for (int i = 0; i < 42; i++)
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

static char * parse_exp_save_ilp(char *s, expressionS *op)
{
	char *save = input_line_pointer;
	input_line_pointer = s;
	expression(op);
	s = input_line_pointer;
	input_line_pointer = save;
	return s;
}

static int parse_args_type(rsrc_inst_t *output, char *str, enum rsrc_itype type, int cond)
{
	int ra, rb, rc, c1, c2, c3;
	expressionS arg;
	ra = rb = rc = c1 = c2 = c3 = 0;
	switch (type)
	{
		case RSRC_1_A_B_C2:
			if (sscanf(str, " r%d", &ra) < 1) {
				as_bad("Not enough arguments (expected ra, c2(rb))");
				return -1;
			}
			output->form2.ra = ra;
			str = strchr(str, ',');
			if (!str) {
				return -1;
			}
			if (strchr(str, 'r')) { // displacement-based
				if (sscanf(str + 1, "%d(r%d)", &c2, &rb) < 2) {
					as_bad("Not enough arguments (expected ra, c2(rb))");
					return -1;
				}
				output->form2.c2 = c2;
				output->form2.rb = rb;
			} else { // fixup just in case
				str = parse_exp_save_ilp(str + 1, &arg);
				fix_new_exp(frag_now, ((char*)output - frag_now->fr_literal), 4, &arg, FALSE, BFD_RELOC_32);
			}
			break;
		case RSRC_2_A_C1: // read register directly and then forward to fixup
			if (sscanf(str, " r%d", &ra) < 1) {
				as_bad("Not enough arguments (expected ra, c1)");
				return -1;
			}
			output->form1.ra = ra;
			str = strchr(str, ',');
			if (!str) {
				as_bad("Not enough arguments (expected ra, c1) 2");
				return -1;
			}
			str = parse_exp_save_ilp(str + 1, &arg);
			fix_new_exp(frag_now, ((char*)output - frag_now->fr_literal), 4, &arg, TRUE, BFD_RELOC_32_PCREL);
			break;
		case RSRC_3_A_C:
			if (sscanf(str, " r%d,r%d", &ra, &rc) < 2) {
				as_bad("Not enough arguments (expected ra, rc)");
				return -1;
			}
			output->formcond.ra = ra;
			output->formcond.rc = rc;
			break;
		case RSRC_4_B_C_COND: // assume caller processes the condition
			output->formcond.cond = cond;
			if (sscanf(str, " r%d,r%d", &rb, &rc) < 0) {
				as_bad("Not enough arguments (expected rb, rc or rb or nothing)");
				return -1;
			}
			output->formcond.rb = rb;
			output->formcond.rc = rc;
			break;
		case RSRC_5_A_B_C_COND: // assume caller processes the condition
			output->formcond.cond = cond;
			if (sscanf(str, " r%d,r%d,r%d", &ra, &rb, &rc) < 1) {
				as_bad("Not enough arguments (expected ra, rb, rc or ra, rb or even ra)");
				return -1;
			}
			output->formcond.ra = ra;
			output->formcond.rb = rb;
			output->formcond.rc = rc;
			break;
		case RSRC_6_A_B_C:
			if (sscanf(str, " r%d,r%d,r%d", &ra, &rb, &rc) < 3) {
				as_bad("Not enough arguments (expected ra, rb, rc)");
				return -1;
			}
			output->formcond.ra = ra;
			output->formcond.rb = rb;
			output->formcond.rc = rc;
			break;
		case RSRC_7_A_B_C_COUNT: // this one is funny as the third param could either be a register or immediate
			if (sscanf(str, " r%d,r%d,r%d", &ra, &rb, &rc) < 3) {
				// try a constant instead
				if (sscanf(str, " r%d,r%d,%d", &ra, &rb, &c3) < 3) {
					as_bad("Not enough arguments (expected ra, rb, rc or ra, rb, c3)");
					return -1;
				}
				output->formcount.count = c3;
			}
			else
			{
				output->formcount.rc = rc;
			}
			output->formcount.ra = ra;
			output->formcount.rb = rb;
			break;
		case RSRC_8:
			break;
		default:
			as_bad("Who are you");
			break;
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

	if (parse_args_type(output, op_end, opcode->itype, opcode->cond) < 0) return;


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

void md_apply_fix(fixS *fixP, valueT *valP, segT seg ATTRIBUTE_UNUSED)
{
	//printf("Time to make america great again\n");

	char *buf = fixP->fx_where + fixP->fx_frag->fr_literal;
	long val = *valP;
	long max, min;
	int shift;

	max = min = 0;
	shift = 0;
	switch (fixP->fx_r_type) {
	case BFD_RELOC_32:
		val &= 0x1FFFF; // TODO try to get the actual fix mask from somewhere, maybe fixP
		break;
	case BFD_RELOC_32_PCREL:
		val &= 0x3FFFFF;
		break;
	default:
		abort();
	}
	switch (fixP->fx_r_type) {
	case BFD_RELOC_32:
	case BFD_RELOC_32_PCREL:
		*(buf++) |= val >> 24;
		*(buf++) |= val >> 16;
		*(buf++) |= val >> 8;
		*(buf++) |= val >> 0;
		break;
	default:
		abort();
	}

	if (max != 0 && (val < min || val > max))
		as_bad_where(fixP->fx_file, fixP->fx_line, _("offset out of range"));

	if (fixP->fx_addsy == NULL && fixP->fx_pcrel == 0)
		fixP->fx_done = 1;
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

long md_pcrel_from(fixS *fixP)
{
	valueT addr = fixP->fx_where + fixP->fx_frag->fr_address;

	//fprintf(stderr, "MDPCRELFROM 0x%d\n", fixP->fx_r_type);

	switch (fixP->fx_r_type)
	{
		case BFD_RELOC_32:
		case BFD_RELOC_32_PCREL:
			return addr;
		default:
			abort();
			return addr;
	}
}
