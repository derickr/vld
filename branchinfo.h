/*
   +----------------------------------------------------------------------+
   | Copyright (c) 1997-2019 Derick Rethans                               |
   +----------------------------------------------------------------------+
   | This source file is subject to the 2-Clause BSD license which is     |
   | available through the LICENSE file, or online at                     |
   | http://opensource.org/licenses/bsd-license.php                       |
   +----------------------------------------------------------------------+
   | Authors:  Derick Rethans <derick@derickrethans.nl>                   |
   +----------------------------------------------------------------------+
 */

#ifndef __BRANCHINFO_H__
#define __BRANCHINFO_H__

#include "set.h"
#include "php_vld.h"
#include "zend_compile.h"

#if ZEND_USE_ABS_JMP_ADDR
# define VLD_ZNODE_JMP_LINE(node, opline, base)  (int32_t)(((long)((node).jmp_addr) - (long)(base_address)) / sizeof(zend_op))
#else
# define VLD_ZNODE_JMP_LINE(node, opline, base)  (int32_t)(((int32_t)((node).jmp_offset) / sizeof(zend_op)) + (opline))
#endif

#define VLD_JMP_NOT_SET -1
#define VLD_JMP_EXIT    -2

#define VLD_BRANCH_MAX_OUTS 32

typedef struct _vld_branch {
	unsigned int start_lineno;
	unsigned int end_lineno;
	unsigned int end_op;
	unsigned int outs_count;
	int          outs[VLD_BRANCH_MAX_OUTS];
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

void vld_branch_info_dump(zend_op_array *opa, vld_branch_info *branch_info);
void vld_branch_info_free(vld_branch_info *branch_info);

#endif
