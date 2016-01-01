/*
   +----------------------------------------------------------------------+
   | Copyright (c) 1997-2016 Derick Rethans                               |
   +----------------------------------------------------------------------+
   | This source file is subject to the 2-Clause BSD license which is     |
   | available through the LICENSE file, or online at                     |
   | http://opensource.org/licenses/bsd-license.php                       |
   +----------------------------------------------------------------------+
   | Authors:  Derick Rethans <derick@derickrethans.nl>                   |
   |           Andrei Zmievski <andrei@gravitonic.com>                    |
   +----------------------------------------------------------------------+
 */
/* $Id: srm_oparray.h,v 1.17 2006-11-12 13:45:03 helly Exp $ */

#ifndef VLD_OPARRAY_H
#define VLD_OPARRAY_H

#include "php.h"


#if PHP_VERSION_ID >= 50399
# define VLD_ZNODE znode_op
# define VLD_ZNODE_ELEM(node,var) node.var
# define VLD_TYPE(t) t##_type
# define VLD_EXTENDED_VALUE(o) extended_value
#else
# define VLD_ZNODE znode
# define VLD_ZNODE_ELEM(node,var) node.u.var
# define VLD_TYPE(t) t.op_type
# define VLD_EXTENDED_VALUE(o) o.u.EA.type
#endif

#if PHP_VERSION_ID >= 50700
# define VAR_NUM(v) EX_VAR_TO_NUM(v)
#else
# if PHP_VERSION_ID >= 50500
#  define VAR_NUM(v) ((zend_uint)(EX_TMP_VAR_NUM(0, 0) - EX_TMP_VAR(0, v)))
# else
#  define VAR_NUM(v) ((v)/(sizeof(temp_variable)))
# endif
#endif

// flags used in the op array list
#define OP1_USED   1<<0
#define OP2_USED   1<<1
#define RES_USED   1<<2

#define NONE_USED  0
#define ALL_USED   0x7

#define OP1_OPLINE   1<<3
#define OP2_OPLINE   1<<4
#define OP1_OPNUM    1<<5
#define OP2_OPNUM    1<<6
#define OP_FETCH     1<<7
#define EXT_VAL      1<<8
#define NOP2_OPNUM   1<<9
#define OP2_BRK_CONT 1<<10
#define OP1_CLASS    1<<11
#define RES_CLASS    1<<12

#define SPECIAL    0xff

// special op-type flags
#define VLD_IS_OPLINE 1<<13
#define VLD_IS_OPNUM  1<<14
#define VLD_IS_CLASS  1<<15
#define OP2_INCLUDE   1<<16
#define EXT_VAL_JMP_REL   1<<17
#define EXT_VAL_JMP_ABS   1<<18

typedef struct _op_usage {
	char *name;
	unsigned int flags;
} op_usage;

void vld_dump_oparray (zend_op_array *opa TSRMLS_DC);
void vld_mark_dead_code (zend_op_array *opa);

#endif

