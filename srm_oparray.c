/*
   +----------------------------------------------------------------------+
   | PHP Version 4                                                        |
   +----------------------------------------------------------------------+
   | Copyright (c) 1997, 1998, 1999, 2000, 2001 The PHP Group             |
   +----------------------------------------------------------------------+
   | This source file is subject to version 2.02 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available at through the world-wide-web at                           |
   | http://www.php.net/license/2_02.txt.                                 |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Authors:  Derick Rethans <d.rethans@jdimedia.nl>                     |
   |           Andrei Zmievski <andrei@gravitonic.com>                    |
   |           Marcus Börger <marcus.boerger@t-online.de>                 |
   +----------------------------------------------------------------------+
 */


#include "php.h"
#include "srm_oparray.h"
#include "ext/standard/url.h"

/* Input zend_compile.h
 * And replace [^(...)(#define )([^ \t]+).*$]
 * BY     [/=*  \1 *=/  { "\3", ALL_USED },] REMEMBER to remove the two '=' signs
 */
static const op_usage opcodes[] = {
	/*  0 */	{ "NOP", NONE_USED },
	/*  1 */	{ "ADD", ALL_USED },
	/*  2 */	{ "SUB", ALL_USED },
	/*  3 */	{ "MUL", ALL_USED },
	/*  4 */	{ "DIV", ALL_USED },
	/*  5 */	{ "MOD", ALL_USED },
	/*  6 */	{ "SL", ALL_USED },
	/*  7 */	{ "SR", ALL_USED },
	/*  8 */	{ "CONCAT", ALL_USED },
	/*  9 */	{ "BW_OR", ALL_USED },
	/*  10 */	{ "BW_AND", ALL_USED },
	/*  11 */	{ "BW_XOR", ALL_USED },
	/*  12 */	{ "BW_NOT", RES_USED | OP1_USED },
	/*  13 */	{ "BOOL_NOT", RES_USED | OP1_USED },
	/*  14 */	{ "BOOL_XOR", ALL_USED },
	/*  15 */	{ "IS_IDENTICAL", ALL_USED },
	/*  16 */	{ "IS_NOT_IDENTICAL", ALL_USED },
	/*  17 */	{ "IS_EQUAL", ALL_USED },
	/*  18 */	{ "IS_NOT_EQUAL", ALL_USED },
	/*  19 */	{ "IS_SMALLER", ALL_USED },
	/*  20 */	{ "IS_SMALLER_OR_EQUAL", ALL_USED },
	/*  21 */	{ "CAST", ALL_USED },
	/*  22 */	{ "QM_ASSIGN", ALL_USED },
	/*  23 */	{ "ASSIGN_ADD", ALL_USED },
	/*  24 */	{ "ASSIGN_SUB", ALL_USED },
	/*  25 */	{ "ASSIGN_MUL", ALL_USED },
	/*  26 */	{ "ASSIGN_DIV", ALL_USED },
	/*  27 */	{ "ASSIGN_MOD", ALL_USED },
	/*  28 */	{ "ASSIGN_SL", ALL_USED },
	/*  29 */	{ "ASSIGN_SR", ALL_USED },
	/*  30 */	{ "ASSIGN_CONCAT", ALL_USED },
	/*  31 */	{ "ASSIGN_BW_OR", ALL_USED },
	/*  32 */	{ "ASSIGN_BW_AND", ALL_USED },
	/*  33 */	{ "ASSIGN_BW_XOR", ALL_USED },
	/*  34 */	{ "PRE_INC", OP1_USED | RES_USED },
	/*  35 */	{ "PRE_DEC", OP1_USED | RES_USED },
	/*  36 */	{ "POST_INC", OP1_USED | RES_USED },
	/*  37 */	{ "POST_DEC", OP1_USED | RES_USED },
	/*  38 */	{ "ASSIGN", ALL_USED },
	/*  39 */	{ "ASSIGN_REF", SPECIAL },
	/*  40 */	{ "ECHO", OP1_USED },
	/*  41 */	{ "PRINT", RES_USED | OP1_USED },
	/*  42 */	{ "JMP", OP1_USED | OP1_OPLINE },
	/*  43 */	{ "JMPZ", OP1_USED | OP2_USED | OP2_OPLINE },
	/*  44 */	{ "JMPNZ", ALL_USED },
	/*  45 */	{ "JMPZNZ", SPECIAL },
	/*  46 */	{ "JMPZ_EX", ALL_USED | OP2_OPLINE },
	/*  47 */	{ "JMPNZ_EX", ALL_USED },
	/*  48 */	{ "CASE", ALL_USED },
	/*  49 */	{ "SWITCH_FREE", ALL_USED },
	/*  50 */	{ "BRK", ALL_USED },
	/*  51 */	{ "CONT", ALL_USED },
	/*  52 */	{ "BOOL", RES_USED | OP1_USED },
	/*  53 */	{ "INIT_STRING", RES_USED },
	/*  54 */	{ "ADD_CHAR", ALL_USED },
	/*  55 */	{ "ADD_STRING", ALL_USED },
	/*  56 */	{ "ADD_VAR", ALL_USED },
	/*  57 */	{ "BEGIN_SILENCE", ALL_USED },
	/*  58 */	{ "END_SILENCE", ALL_USED },
	/*  59 */	{ "INIT_FCALL_BY_NAME", SPECIAL },
	/*  60 */	{ "DO_FCALL", SPECIAL },
	/*  61 */	{ "DO_FCALL_BY_NAME", SPECIAL },
	/*  62 */	{ "RETURN", OP1_USED },
	/*  63 */	{ "RECV", RES_USED | OP1_USED },
	/*  64 */	{ "RECV_INIT", ALL_USED },
	/*  65 */	{ "SEND_VAL", OP1_USED },
	/*  66 */	{ "SEND_VAR", OP1_USED },
	/*  67 */	{ "SEND_REF", ALL_USED },
	/*  68 */	{ "NEW", RES_USED | OP1_USED },
	/*  69 */	{ "JMP_NO_CTOR", OP1_USED | OP2_USED | OP2_OPLINE },
	/*  70 */	{ "FREE", OP1_USED },
	/*  71 */	{ "INIT_ARRAY", ALL_USED },
	/*  72 */	{ "ADD_ARRAY_ELEMENT", ALL_USED },
	/*  73 */	{ "INCLUDE_OR_EVAL", ALL_USED },
	/*  74 */	{ "UNSET_VAR", ALL_USED },
	/*  75 */	{ "UNSET_DIM_OBJ", ALL_USED },
	/*  76 */	{ "ISSET_ISEMPTY", ALL_USED },
	/*  77 */	{ "FE_RESET", ALL_USED },
	/*  78 */	{ "FE_FETCH", ALL_USED },
	/*  79 */	{ "EXIT", ALL_USED },
	/*  80 */	{ "FETCH_R", RES_USED | OP1_USED | OP_FETCH },
	/*  81 */	{ "FETCH_DIM_R", ALL_USED },
	/*  82 */	{ "FETCH_OBJ_R", ALL_USED },
	/*  83 */	{ "FETCH_W", RES_USED | OP1_USED | OP_FETCH },
	/*  84 */	{ "FETCH_DIM_W", ALL_USED },
	/*  85 */	{ "FETCH_OBJ_W", ALL_USED },
	/*  86 */	{ "FETCH_RW", RES_USED | OP1_USED | OP_FETCH },
	/*  87 */	{ "FETCH_DIM_RW", ALL_USED },
	/*  88 */	{ "FETCH_OBJ_RW", ALL_USED },
	/*  89 */	{ "FETCH_IS", ALL_USED },
	/*  90 */	{ "FETCH_DIM_IS", ALL_USED },
	/*  91 */	{ "FETCH_OBJ_IS", ALL_USED },
	/*  92 */	{ "FETCH_FUNC_ARG", RES_USED | OP1_USED | OP_FETCH },
	/*  93 */	{ "FETCH_DIM_FUNC_ARG", ALL_USED },
	/*  94 */	{ "FETCH_OBJ_FUNC_ARG", ALL_USED },
	/*  95 */	{ "FETCH_UNSET", ALL_USED },
	/*  96 */	{ "FETCH_DIM_UNSET", ALL_USED },
	/*  97 */	{ "FETCH_OBJ_UNSET", ALL_USED },
	/*  98 */	{ "FETCH_DIM_TMP_VAR", ALL_USED },
	/*  99 */	{ "FETCH_CONSTANT", RES_USED | OP1_USED },
	/*  100 */	{ "DECLARE_FUNCTION_OR_CLASS", ALL_USED },
	/*  101 */	{ "EXT_STMT", ALL_USED },
	/*  102 */	{ "EXT_FCALL_BEGIN", ALL_USED },
	/*  103 */	{ "EXT_FCALL_END", ALL_USED },
	/*  104 */	{ "EXT_NOP", ALL_USED },
	/*  105 */	{ "TICKS", ALL_USED },
	/*  106 */	{ "SEND_VAR_NO_REF", ALL_USED },
#ifdef ZEND_ENGINE_2
	/*  107 */	{ "ZEND_CATCH", ALL_USED },
	/*  108 */	{ "ZEND_THROW", ALL_USED },
	
	/*  109 */	{ "ZEND_FETCH_CLASS", ALL_USED },
	
	/*  110 */	{ "ZEND_CLONE", ALL_USED },
	
	/*  111 */	{ "ZEND_INIT_CTOR_CALL", ALL_USED },
	/*  112 */	{ "ZEND_INIT_METHOD_CALL", ALL_USED },
	/*  113 */	{ "ZEND_INIT_STATIC_METHOD_CALL", ALL_USED },
	
	/*  114 */	{ "ZEND_ISSET_ISEMPTY_VAR", ALL_USED },
	/*  115 */	{ "ZEND_ISSET_ISEMPTY_DIM_OBJ", ALL_USED },
	
	/*  116 */	{ "ZEND_IMPORT_FUNCTION", ALL_USED },
	/*  117 */	{ "ZEND_IMPORT_CLASS", ALL_USED },
	/*  118 */	{ "ZEND_IMPORT_CONST", ALL_USED },
	
	/*  119 */	{ "119", ALL_USED },
	/*  120 */	{ "120", ALL_USED },
	
	/*  121 */	{ "ZEND_ASSIGN_ADD_OBJ", ALL_USED },
	/*  122 */	{ "ZEND_ASSIGN_SUB_OBJ", ALL_USED },
	/*  123 */	{ "ZEND_ASSIGN_MUL_OBJ", ALL_USED },
	/*  124 */	{ "ZEND_ASSIGN_DIV_OBJ", ALL_USED },
	/*  125 */	{ "ZEND_ASSIGN_MOD_OBJ", ALL_USED },
	/*  126 */	{ "ZEND_ASSIGN_SL_OBJ", ALL_USED },
	/*  127 */	{ "ZEND_ASSIGN_SR_OBJ", ALL_USED },
	/*  128 */	{ "ZEND_ASSIGN_CONCAT_OBJ", ALL_USED },
	/*  129 */	{ "ZEND_ASSIGN_BW_OR_OBJ", ALL_USED },
	/*  130 */	{ "ZEND_ASSIGN_BW_AND_OBJ", ALL_USED },
	/*  131 */	{ "ZEND_ASSIGN_BW_XOR_OBJ", ALL_USED },

	/*  132 */	{ "ZEND_PRE_INC_OBJ", ALL_USED },
	/*  133 */	{ "ZEND_PRE_DEC_OBJ", ALL_USED },
	/*  134 */	{ "ZEND_POST_INC_OBJ", ALL_USED },
	/*  135 */	{ "ZEND_POST_DEC_OBJ", ALL_USED },

	/*  136 */	{ "ZEND_ASSIGN_OBJ", ALL_USED },
	/*  137 */	{ "ZEND_OP_DATA", ALL_USED },
	
	/*  138 */	{ "ZEND_INSTANCEOF", ALL_USED },
	
	/*  139 */	{ "ZEND_DECLARE_CLASS", ALL_USED },
	/*  140 */	{ "ZEND_DECLARE_INHERITED_CLASS", ALL_USED },
	/*  141 */	{ "ZEND_DECLARE_FUNCTION", ALL_USED },
	
	/*  142 */	{ "ZEND_RAISE_ABSTRACT_ERROR", ALL_USED },
	
	/*  143 */	{ "ZEND_START_NAMESPACE", ALL_USED },
	
	/*  144 */	{ "ZEND_ADD_INTERFACE", ALL_USED },
	/*  145 */	{ "ZEND_VERIFY_INSTANCEOF", ALL_USED },
	/*  146 */	{ "ZEND_VERIFY_ABSTRACT_CLASS", ALL_USED },
#endif
};

inline void vld_dump_zval_null(zvalue_value value)
{
	zend_printf ("null");
}

inline void vld_dump_zval_long(zvalue_value value)
{
	zend_printf ("%ld", value.lval);
}

inline void vld_dump_zval_double(zvalue_value value)
{
}

inline void vld_dump_zval_string(zvalue_value value)
{
	char *new_str;
	int new_len;

	new_str = php_url_encode(value.str.val, value.str.len, &new_len);
	zend_printf ("'%s'", new_str);
	efree(new_str);
}

inline void vld_dump_zval_array(zvalue_value value)
{
}

inline void vld_dump_zval_object(zvalue_value value)
{
}

inline void vld_dump_zval_bool(zvalue_value value)
{
}

inline void vld_dump_zval_resource(zvalue_value value)
{
}

inline void vld_dump_zval_constant(zvalue_value value)
{
}

inline void vld_dump_zval_constant_array(zvalue_value value)
{
}


void vld_dump_zval (zval val)
{
	switch (val.type) {
		case IS_NULL:           vld_dump_zval_null (val.value);           break;
		case IS_LONG:           vld_dump_zval_long (val.value);           break;
		case IS_DOUBLE:         vld_dump_zval_double (val.value);         break;
		case IS_STRING:         vld_dump_zval_string (val.value);         break;
		case IS_ARRAY:          vld_dump_zval_array (val.value);          break;
		case IS_OBJECT:         vld_dump_zval_object (val.value);         break;
		case IS_BOOL:           vld_dump_zval_bool (val.value);           break;
		case IS_RESOURCE:       vld_dump_zval_resource (val.value);       break;
		case IS_CONSTANT:       vld_dump_zval_constant (val.value);       break;
		case IS_CONSTANT_ARRAY: vld_dump_zval_constant_array (val.value); break;
	}
}

int vld_dump_znode (znode node)
{
	switch (node.op_type) {
		case IS_CONST: /* 1 */
			vld_dump_zval (node.u.constant);
			break;
		case IS_TMP_VAR: /* 2 */
			zend_printf ("~%d", node.u.var);
			break;
		case IS_VAR: /* 4 */
			zend_printf ("$%d", node.u.var);
			break;
		case IS_UNUSED:
			zend_printf ("N/A", node.u.var);
			return 0;
		case VLD_IS_OPLINE:
			zend_printf ("->%d", node.u.opline_num);
			break;
	}
	return 1;
}

static zend_uchar vld_get_special_flags(zend_op *op)
{
	zend_uchar flags = 0;

	switch (op->opcode) {
		case ZEND_ASSIGN_REF:
			flags = OP1_USED | OP2_USED;
			if (op->result.op_type != IS_UNUSED) {
				flags |= RES_USED;
			}
			break;

		case ZEND_DO_FCALL_BY_NAME:
		case ZEND_DO_FCALL:
			flags = ALL_USED | EXT_VAL;
			op->op2.op_type = IS_CONST;
			op->op2.u.constant.type = IS_LONG;
			break;

		case ZEND_INIT_FCALL_BY_NAME:
			flags = OP2_USED;
			if (op->op1.op_type != IS_UNUSED) {
				flags |= OP1_USED;
			}
			break;

		case ZEND_JMPZNZ:
			flags = ALL_USED;
			op->result = op->op1;
			op->op1.op_type = VLD_IS_OPLINE;
			op->op1.u.opline_num = op->extended_value;
			op->op2.op_type = VLD_IS_OPLINE;
			break;
	}

	return flags;
}

#define NUM_KNOWN_OPCODES (sizeof(opcodes)/sizeof(opcodes[0]))

void vld_dump_op (int nr, zend_op op)
{
	static uint last_lineno = -1;
	int print_sep = 0;
	char *fetch_type = "";
	zend_uchar flags;
	
	if (op.opcode >= NUM_KNOWN_OPCODES) {
		flags = ALL_USED;
	} else {
		flags = opcodes[op.opcode].flags;
	}

	if (op.lineno == 0)
		return;

	if (flags == SPECIAL) {
		flags = vld_get_special_flags(&op);
	}

	if (flags & OP_FETCH) {
#ifdef ZEND_ENGINE_2
		switch (op.op2.u.EA.type) {
			case ZEND_FETCH_GLOBAL:
				fetch_type = "global";
				break;
			case ZEND_FETCH_LOCAL:
				fetch_type = "local";
				break;
			case ZEND_FETCH_STATIC:
				fetch_type = "static";
				break;
			case ZEND_FETCH_STATIC_MEMBER:
				fetch_type = "static member";
				break;
			default:
				fetch_type = "unknown";
				break;
		}
#else 
		if (op.op2.u.fetch_type == ZEND_FETCH_GLOBAL) {
			fetch_type = "global";
		} else if (op.op2.u.fetch_type == ZEND_FETCH_STATIC) {
			fetch_type = "static";
		}
#endif
	}

	if (op.lineno == last_lineno) {
		zend_printf("     ");
	} else {
		zend_printf("%4d ", op.lineno);
		last_lineno = op.lineno;
	}

	if (op.opcode >= NUM_KNOWN_OPCODES) {
		zend_printf("%5d  <%03d>%-23s %-14s ", nr, op.opcode, "", fetch_type);
	} else {
		zend_printf("%5d  %-28s %-14s ", nr, opcodes[op.opcode].name, fetch_type);
	}

	if (flags & EXT_VAL) {
		zend_printf("%3ld  ", op.extended_value);
	} else {
		zend_printf("     ");
	}

	if ((flags & RES_USED) &&
		!(op.result.u.EA.type & EXT_TYPE_UNUSED)) {
		vld_dump_znode (op.result);
		print_sep = 1;
	}
	if (flags & OP1_USED) {
		if (print_sep) zend_printf (", ");
		if (flags & OP1_OPLINE)
			op.op1.op_type = VLD_IS_OPLINE;
		vld_dump_znode (op.op1);
		print_sep = 1;
	}
	if (flags & OP2_USED) {
		if (print_sep) zend_printf (", ");
		if (flags & OP2_OPLINE)
			op.op2.op_type = VLD_IS_OPLINE;
		vld_dump_znode (op.op2);
	}
	zend_printf ("\n");
}

void vld_dump_oparray (zend_op_array *opa)
{
	int i;

	zend_printf ("filename:       %s\n", opa->filename);
	zend_printf ("function name:  %s\n", opa->function_name);
	zend_printf ("number of ops:  %d\n", opa->last);

    zend_printf("line     #  op                           fetch          ext  operands\n");
	zend_printf("-------------------------------------------------------------------------------\n");
	for (i = 0; i < opa->size; i++) {
		vld_dump_op (i, opa->opcodes[i]);
	}
	zend_printf("\n");
}

void opt_set_nop (zend_op_array *opa, int nr)
{
	opa->opcodes[nr].opcode = ZEND_NOP;
}
