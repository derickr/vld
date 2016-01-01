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
/* $Id: set.c,v 1.1 2006-09-26 09:40:26 derick Exp $ */

#include <stdlib.h>
#include <math.h>
#include "set.h"

vld_set *vld_set_create(unsigned int size)
{
	vld_set *tmp;

	tmp = calloc(1, sizeof(vld_set));
	tmp->size = size;
	size = ceil((size + 7) / 8);
	tmp->setinfo = calloc(1, size);

	return tmp;
}

void vld_set_free(vld_set *set)
{
	free(set->setinfo);
	free(set);
}

void vld_set_add(vld_set *set, unsigned int position)
{
	unsigned char *byte;
	unsigned int   bit;

	byte = &(set->setinfo[position / 8]);
	bit  = position % 8;

	*byte = *byte | 1 << bit;
}

void vld_set_remove(vld_set *set, unsigned int position)
{
	unsigned char *byte;
	unsigned int   bit;

	byte = &(set->setinfo[position / 8]);
	bit  = position % 8;

	*byte = *byte & ~(1 << bit);
}

int vld_set_in_ex(vld_set *set, unsigned int position, int noisy)
{
	unsigned char *byte;
	unsigned int   bit;

	byte = &(set->setinfo[position / 8]);
	bit  = position % 8;

	return (*byte & (1 << bit));
}
