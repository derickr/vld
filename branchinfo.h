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

#ifndef __BRANCHINFO_H__
#define __BRANCHINFO_H__

#include "set.h"
#include "php_vld.h"

typedef struct _vld_branch {
	unsigned int start_lineno;
	unsigned int end_lineno;
	unsigned int end_op;
	unsigned int out[2];
} vld_branch;

typedef struct _vld_path {
	unsigned int elements_count;
	unsigned int elements_size;
	unsigned int *elements;
} vld_path;

typedef struct _vld_branch_info {
	unsigned int  size;
	vld_set      *starts;
	vld_set      *ends;
	vld_branch   *branches;

	unsigned int  paths_count;
	unsigned int  paths_size;
	vld_path    **paths;
} vld_branch_info;

vld_branch_info *vld_branch_info_create(unsigned int size);

void vld_branch_info_update(vld_branch_info *branch_info, unsigned int pos, unsigned int lineno, unsigned int outidx, unsigned int jump_pos);
void vld_branch_post_process(vld_branch_info *branch_info);
void vld_branch_find_paths(vld_branch_info *branch_info);

void vld_branch_info_dump(zend_op_array *opa, vld_branch_info *branch_info TSRMLS_DC);
void vld_branch_info_free(vld_branch_info *branch_info);

#endif
