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
   +----------------------------------------------------------------------+
 */


#include "php.h"
#include "srm_oparray.h"

static const char* opcodes[] = {
	"NOP", /*  0 */
	"ADD", /*  1 */
	"SUB", /*  2 */
	"MUL", /*  3 */
	"DIV", /*  4 */
	"MOD", /*  5 */
	"SL", /*  6 */
	"SR", /*  7 */
	"CONCAT", /*  8 */
	"BW_OR", /*  9 */
	"BW_AND", /*  10 */
	"BW_XOR", /*  11 */
	"BW_NOT", /*  12 */
	"BOOL_NOT", /*  13 */
	"BOOL_XOR", /*  14 */
	"IS_IDENTICAL", /*  15 */
	"IS_NOT_IDENTICAL", /*  16 */
	"IS_EQUAL", /*  17 */
	"IS_NOT_EQUAL", /*  18 */
	"IS_SMALLER", /*  19 */
	"IS_SMALLER_OR_EQUAL", /*  20 */
	"CAST", /*  21 */
	"QM_ASSIGN", /*  22 */
	"ASSIGN_ADD", /*  23 */
	"ASSIGN_SUB", /*  24 */
	"ASSIGN_MUL", /*  25 */
	"ASSIGN_DIV", /*  26 */
	"ASSIGN_MOD", /*  27 */
	"ASSIGN_SL", /*  28 */
	"ASSIGN_SR", /*  29 */
	"ASSIGN_CONCAT", /*  30 */
	"ASSIGN_BW_OR", /*  31 */
	"ASSIGN_BW_AND", /*  32 */
	"ASSIGN_BW_XOR", /*  33 */
	"PRE_INC", /*  34 */
	"PRE_DEC", /*  35 */
	"POST_INC", /*  36 */
	"POST_DEC", /*  37 */
	"ASSIGN", /*  38 */
	"ASSIGN_REF", /*  39 */
	"ECHO", /*  40 */
	"PRINT", /*  41 */
	"JMP", /*  42 */
	"JMPZ", /*  43 */
	"JMPNZ", /*  44 */
	"JMPZNZ", /*  45 */
	"JMPZ_EX", /*  46 */
	"JMPNZ_EX", /*  47 */
	"CASE", /*  48 */
	"SWITCH_FREE", /*  49 */
	"BRK", /*  50 */
	"CONT", /*  51 */
	"BOOL", /*  52 */
	"INIT_STRING", /*  53 */
	"ADD_CHAR", /*  54 */
	"ADD_STRING", /*  55 */
	"ADD_VAR", /*  56 */
	"BEGIN_SILENCE", /*  57 */
	"END_SILENCE", /*  58 */
	"INIT_FCALL_BY_NAME", /*  59 */
	"DO_FCALL", /*  60 */
	"DO_FCALL_BY_NAME", /*  61 */
	"RETURN", /*  62 */
	"RECV", /*  63 */
	"RECV_INIT", /*  64 */
	"SEND_VAL", /*  65 */
	"SEND_VAR", /*  66 */
	"SEND_REF", /*  67 */
	"NEW", /*  68 */
	"JMP_NO_CTOR", /*  69 */
	"FREE", /*  70 */
	"INIT_ARRAY", /*  71 */
	"ADD_ARRAY_ELEMENT", /*  72 */
	"INCLUDE_OR_EVAL", /*  73 */
	"UNSET_VAR", /*  74 */
	"UNSET_DIM_OBJ", /*  75 */
	"ISSET_ISEMPTY", /*  76 */
	"FE_RESET", /*  77 */
	"FE_FETCH", /*  78 */
	"EXIT", /*  79 */
	"FETCH_R", /*  80 */
	"FETCH_DIM_R", /*  81 */
	"FETCH_OBJ_R", /*  82 */
	"FETCH_W", /*  83 */
	"FETCH_DIM_W", /*  84 */
	"FETCH_OBJ_W", /*  85 */
	"FETCH_RW", /*  86 */
	"FETCH_DIM_RW", /*  87 */
	"FETCH_OBJ_RW", /*  88 */
	"FETCH_IS", /*  89 */
	"FETCH_DIM_IS", /*  90 */
	"FETCH_OBJ_IS", /*  91 */
	"FETCH_FUNC_ARG", /*  92 */
	"FETCH_DIM_FUNC_ARG", /*  93 */
	"FETCH_OBJ_FUNC_ARG", /*  94 */
	"FETCH_UNSET", /*  95 */
	"FETCH_DIM_UNSET", /*  96 */
	"FETCH_OBJ_UNSET", /*  97 */
	"FETCH_DIM_TMP_VAR", /*  98 */
	"FETCH_CONSTANT", /*  99 */
	"DECLARE_FUNCTION_OR_CLASS", /*  100 */
	"EXT_STMT", /*  101 */
	"EXT_FCALL_BEGIN", /*  102 */
	"EXT_FCALL_END", /*  103 */
	"EXT_NOP", /*  104 */
	"TICKS", /*  105 */
	"SEND_VAR_NO_REF", /*  106 */
};

inline void srm_dump_zval_null(zvalue_value value)
{
}

inline void srm_dump_zval_long(zvalue_value value)
{
	zend_printf ("%ld", value.lval);
}

inline void srm_dump_zval_double(zvalue_value value)
{
}

inline void srm_dump_zval_string(zvalue_value value)
{
	zend_printf ("%s", value.str.val);
}

inline void srm_dump_zval_array(zvalue_value value)
{
}

inline void srm_dump_zval_object(zvalue_value value)
{
}

inline void srm_dump_zval_bool(zvalue_value value)
{
}

inline void srm_dump_zval_resource(zvalue_value value)
{
}

inline void srm_dump_zval_constant(zvalue_value value)
{
}

inline void srm_dump_zval_constant_array(zvalue_value value)
{
}


void srm_dump_zval (zval val)
{
	switch (val.type) {
		case IS_NULL:           srm_dump_zval_null (val.value);           break;
		case IS_LONG:           srm_dump_zval_long (val.value);           break;
		case IS_DOUBLE:         srm_dump_zval_double (val.value);         break;
		case IS_STRING:         srm_dump_zval_string (val.value);         break;
		case IS_ARRAY:          srm_dump_zval_array (val.value);          break;
		case IS_OBJECT:         srm_dump_zval_object (val.value);         break;
		case IS_BOOL:           srm_dump_zval_bool (val.value);           break;
		case IS_RESOURCE:       srm_dump_zval_resource (val.value);       break;
		case IS_CONSTANT:       srm_dump_zval_constant (val.value);       break;
		case IS_CONSTANT_ARRAY: srm_dump_zval_constant_array (val.value); break;
	}
}

void srm_dump_znode (znode node)
{
	switch (node.op_type) {
		case IS_CONST: /* 1 */
			zend_printf ("'");
			srm_dump_zval (node.u.constant);
			zend_printf ("'");
			break;
		case IS_TMP_VAR: /* 2 */
			zend_printf ("%%%d", node.u.var);
			break;
		case IS_VAR: /* 4 */
			zend_printf ("$%d", node.u.var);
			break;
		case IS_UNUSED: /* 4 */
			zend_printf ("%d", node.u.opline_num);
			break;
	}

}


void srm_dump_op (int nr, zend_op op)
{
	zend_printf ("%5d  %-20s", nr, opcodes[op.opcode]);
	srm_dump_znode (op.result);
//	if (op.op1.op_type != IS_UNUSED)
	{
		zend_printf (", ");
		srm_dump_znode (op.op1);
		
//		if (op.op2.op_type != IS_UNUSED)
		{
			zend_printf (", ");
			srm_dump_znode (op.op2);
		}
	}
	zend_printf ("\n");
}

void srm_dump_oparray (zend_op_array *opa)
{
	int i;

	zend_printf ("filename:       %s\n", opa->filename);
	zend_printf ("function name:  %s\n", opa->function_name);
	zend_printf ("number of ops:  %d\n", opa->size);

	for (i = 0; i < opa->size; i++) {
		srm_dump_op (i, opa->opcodes[i]);
	}
}

void opt_set_nop (zend_op_array *opa, int nr)
{
	opa->opcodes[nr].opcode = NOP;
}

void opt_concat_string (zend_op_array *opa, int dest, int src)
{
	int new_len;
	zval *zd = &(opa->opcodes[dest].op2.u.constant);
	zval *zs = &(opa->opcodes[src].op2.u.constant);

	new_len          = zd->value.str.len + zs->value.str.len;
	zd->value.str.val = erealloc (zd->value.str.val, new_len + 1);
	zd->value.str.len = new_len;
	strncat (zd->value.str.val, zs->value.str.val, zs->value.str.len);
	zd->value.str.val[new_len] = '\0';
}

void opt_concat_char (zend_op_array *opa, int dest, int src)
{
	zval *zd = &(opa->opcodes[dest].op2.u.constant);
	zval *zs = &(opa->opcodes[src].op2.u.constant);

	zd->value.str.val = erealloc (zd->value.str.val, zd->value.str.len + 2);
	zd->value.str.len++;
	zd->value.str.val[zd->value.str.len - 1] = zs->value.lval;
	zd->value.str.val[zd->value.str.len] = '\0';
}

void srm_concat_strings (zend_op_array **opa)
{
	int i;
	int last_add_string = -1;

	for (i = 0; i < (*opa)->size; i++) {
		if (((*opa)->opcodes[i].opcode == ADD_STRING) || 
			((*opa)->opcodes[i].opcode == ADD_CHAR)) {
			if (last_add_string == -1) {
				last_add_string = i;
			} else {
				if ((*opa)->opcodes[i].opcode == ADD_STRING) {
					opt_concat_string (*opa, last_add_string, i);
				} else {
					opt_concat_char (*opa, last_add_string, i);
				}
				opt_set_nop (*opa, i);
			}
		} else {
			last_add_string = -1;
		}
	}
}
