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
   | Authors:  Derick Rethans <derick@derickrethans.nl>                   |
   +----------------------------------------------------------------------+
 */
/* $Id: set.h,v 1.2 2006-11-12 13:59:53 helly Exp $ */

#ifndef __SET_H__
#define __SET_H__

typedef struct _vld_set {
	unsigned int size;
	unsigned char *setinfo;
} vld_set;

vld_set *vld_set_create(unsigned int size);
#ifndef ZEND_ENGINE_2
# define VLD_DEAD_CODE 107
#else
# define VLD_DEAD_CODE 150
#endif
void vld_set_add(vld_set *set, unsigned int position);
void vld_set_remove(vld_set *set, unsigned int position);
#define vld_set_in(x,y) vld_set_in_ex(x,y,1)
int vld_set_in_ex(vld_set *set, unsigned int position, int noisy);
void vld_set_dump(vld_set *set);
void vld_set_free(vld_set *set);

#endif
