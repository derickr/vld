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
