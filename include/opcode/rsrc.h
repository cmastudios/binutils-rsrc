/**
 * RSRC opcode definitions
 *
 * Copyright 2020 Connor Monahan
 * Licensed under GPLv3+
 */

#include <stdint.h>

enum rsrc_itype {
RSRC_1_A_B_C2,
RSRC_2_A_C1,
RSRC_3_A_C,
RSRC_4_B_C_COND,
RSRC_5_A_B_C_COND,
RSRC_6_A_B_C,
RSRC_7_A_B_C_COUNT,
RSRC_8
};

#define RSRC_CLEAR(buf) {buf[0]=buf[1]=buf[2]=buf[3]=0;}
#define RSRC_SET_OP(buf,op) {buf[0]|=(op<<3);}
#define RSRC_GET_OP(buf) ((buf[0]>>3)&31)

#ifdef LITTLE_ENDIAN
#define RSRC_HTODL(x)    (((x >> 24) & 0x000000FF) | ((x >> 8) & 0x0000FF00) | ((x << 8) & 0x00FF0000) | ((x << 24) & 0xFF000000))
#else
#define RSRC_HTODL(x) (x)
#endif

typedef struct {
#ifdef LITTLE_ENDIAN
	uint32_t c1:22;
	uint32_t ra:5;
	uint32_t op:5;
#else
	uint32_t op:5;
	uint32_t ra:5;
	uint32_t c1:22;
#endif
} rsrc_inst1;

typedef struct {
#ifdef LITTLE_ENDIAN
	uint32_t c2:17;
	uint32_t rb:5;
	uint32_t ra:5;
	uint32_t op:5;
#else
	uint32_t op:5;
	uint32_t ra:5;
	uint32_t rb:5;
	uint32_t c2:17;
#endif
} rsrc_inst2;

typedef struct {
#ifdef LITTLE_ENDIAN
	uint32_t cond:3;
	uint32_t pad:9;
	uint32_t rc:5;
	uint32_t rb:5;
	uint32_t ra:5;
	uint32_t op:5;
#else
	uint32_t op:5;
	uint32_t ra:5;
	uint32_t rb:5;
	uint32_t rc:5;
	uint32_t pad:9;
	uint32_t cond:3;
#endif
} rsrc_inst3;

typedef struct {
#ifdef LITTLE_ENDIAN
	uint32_t count:5;
	uint32_t pad:7;
	uint32_t rc:5;
	uint32_t rb:5;
	uint32_t ra:5;
	uint32_t op:5;
#else
	uint32_t op:5;
	uint32_t ra:5;
	uint32_t rb:5;
	uint32_t rc:5;
	uint32_t pad:7;
	uint32_t count:5;
#endif
} rsrc_inst4;

typedef union {
	rsrc_inst1 form1;
	rsrc_inst2 form2;
	rsrc_inst3 formcond;
	rsrc_inst4 formcount;
	uint32_t data;
	uint8_t buf[4];
} rsrc_inst_t;

typedef struct rsrc_opc_info_t
{
	unsigned char opcode;
	const char * name;
	enum rsrc_itype itype;
} rsrc_opc_info_t;

#define RSRC_NOP 0
#define RSRC_LD 1
#define RSRC_LDR 2
#define RSRC_ST 3
#define RSRC_STR 4
#define RSRC_LA 5
#define RSRC_LAR 6
#define RSRC_BR 8
#define RSRC_BRL 9
#define RSRC_ADD 12
#define RSRC_ADDI 13
#define RSRC_SUB 14
#define RSRC_NEG 15
#define RSRC_AND 20
#define RSRC_ANDI 21
#define RSRC_OR 22
#define RSRC_ORI 23
#define RSRC_NOT 24
#define RSRC_SHR 26
#define RSRC_SHRA 27
#define RSRC_SHL 28
#define RSRC_SHC 29


