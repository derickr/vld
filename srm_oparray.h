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

#ifndef VLD_OPARRAY_H
#define VLD_OPARRAY_H

#include "php.h"

#define OP1_USED   1<<0
#define OP2_USED   1<<1
#define RES_USED   1<<2

#define NONE_USED  0
#define ALL_USED   0x7

#define OP1_OPLINE 1<<3
#define OP2_OPLINE 1<<4
#define OP_FETCH   1<<5

#define EXT_VAL    1<<6

#define SPECIAL    0xff

#define VLD_IS_OPLINE 1<<4

typedef struct _op_usage {
	char *name;
	zend_uchar flags;
} op_usage;

void vld_dump_oparray (zend_op_array *opa);

#endif

