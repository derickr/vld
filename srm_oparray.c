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
   | Authors:  Derick Rethans <derick@vl-srm.net>                         |
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
		case IS_CONST:
			zend_printf ("'");
			srm_dump_zval (node.u.constant);
			zend_printf ("'");
			break;
		case IS_TMP_VAR: zend_printf ("%%"); break;
		case IS_VAR:     zend_printf ("$"); break;
	}

}


void srm_dump_op (int nr, zend_op op)
{
	zend_printf ("%5d  %-20s", nr, opcodes[op.opcode]);
	srm_dump_znode (op.result);
	if (op.op1.op_type != IS_UNUSED) {
		zend_printf (", ");
		srm_dump_znode (op.op1);
		
		if (op.op2.op_type != IS_UNUSED) {
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

