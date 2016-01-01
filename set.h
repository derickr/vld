/*
   +----------------------------------------------------------------------+
   | Copyright (c) 1997-2016 Derick Rethans                               |
   +----------------------------------------------------------------------+
   | This source file is subject to the 2-Clause BSD license which is     |
   | available through the LICENSE file, or online at                     |
   | http://opensource.org/licenses/bsd-license.php                       |
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
#if defined(ZEND_ENGINE_2) || defined(ZEND_ENGINE_3)
# define VLD_DEAD_CODE 150
#else
# define VLD_DEAD_CODE 107
#endif
void vld_set_add(vld_set *set, unsigned int position);
void vld_set_remove(vld_set *set, unsigned int position);
#define vld_set_in(x,y) vld_set_in_ex(x,y,1)
int vld_set_in_ex(vld_set *set, unsigned int position, int noisy);
void vld_set_dump(vld_set *set);
void vld_set_free(vld_set *set);

#endif
