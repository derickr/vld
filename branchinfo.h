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

#ifndef __BRANCHINFO_H__
#define __BRANCHINFO_H__

#include "set.h"
#include "php_vld.h"

#define VLD_JMP_NOT_SET -1
#define VLD_JMP_EXIT    -2

typedef struct _vld_branch {
	unsigned int start_lineno;
	unsigned int end_lineno;
	unsigned int end_op;
	int          out[2];
} vld_branch;

typedef struct _vld_path {
	unsigned int elements_count;
	unsigned int elements_size;
	unsigned int *elements;
} vld_path;

typedef struct _vld_branch_info {
	unsigned int  size;
	vld_set      *entry_points;
	vld_set      *starts;
	vld_set      *ends;
	vld_branch   *branches;

	unsigned int  paths_count;
	unsigned int  paths_size;
	vld_path    **paths;
} vld_branch_info;

vld_branch_info *vld_branch_info_create(unsigned int size);

void vld_branch_info_update(vld_branch_info *branch_info, unsigned int pos, unsigned int lineno, unsigned int outidx, unsigned int jump_pos);
void vld_branch_post_process(zend_op_array *opa, vld_branch_info *branch_info);
void vld_branch_find_paths(vld_branch_info *branch_info);

void vld_branch_info_dump(zend_op_array *opa, vld_branch_info *branch_info TSRMLS_DC);
void vld_branch_info_free(vld_branch_info *branch_info);

#endif
