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
/* $Id: branch_info.c,v 1.1 2006-09-26 09:40:26 derick Exp $ */

#include <stdlib.h>
#include <math.h>
#include "branchinfo.h"

ZEND_EXTERN_MODULE_GLOBALS(vld)

vld_branch_info *vld_branch_info_create(unsigned int size)
{
	vld_branch_info *tmp;

	tmp = calloc(1, sizeof(vld_branch_info));
	tmp->size = size;
	tmp->branches = calloc(size, sizeof(vld_branch));
	tmp->starts = vld_set_create(size);
	tmp->ends   = vld_set_create(size);

	tmp->paths_count = 0;
	tmp->paths_size  = 0;
	tmp->paths = NULL;

	return tmp;
}

void vld_branch_info_free(vld_branch_info *branch_info)
{
	free(branch_info->branches);
	free(branch_info->starts);
	free(branch_info->ends);
	free(branch_info);
}

void vld_branch_info_update(vld_branch_info *branch_info, unsigned int pos, unsigned int lineno, unsigned int outidx, unsigned int jump_pos)
{
	vld_set_add(branch_info->ends, pos);
	branch_info->branches[pos].out[outidx] = jump_pos;
	branch_info->branches[pos].start_lineno = lineno;
}

void vld_branch_post_process(vld_branch_info *branch_info)
{
	int i, in_branch = 0, last_start = -1;

	for (i = 0; i < branch_info->starts->size; i++) {
		if (vld_set_in(branch_info->starts, i)) {
			if (in_branch) {
				branch_info->branches[last_start].out[0] = i;
				branch_info->branches[last_start].end_op = i-1;
				branch_info->branches[last_start].end_lineno = branch_info->branches[i].start_lineno;
			}
			last_start = i;
			in_branch = 1;
		}
		if (vld_set_in(branch_info->ends, i)) {
			branch_info->branches[last_start].out[0] = branch_info->branches[i].out[0];
			branch_info->branches[last_start].out[1] = branch_info->branches[i].out[1];
			branch_info->branches[last_start].end_op = i;
			branch_info->branches[last_start].end_lineno = branch_info->branches[i].start_lineno;
			in_branch = 0;
		}
	}
}

static void vld_path_add(vld_path *path, unsigned int nr)
{
	if (path->elements_count == path->elements_size) {
		path->elements_size += 32;
		path->elements = realloc(path->elements, sizeof(unsigned int) * path->elements_size);
	}
	path->elements[path->elements_count] = nr;
	path->elements_count++;
}

static void vld_branch_info_add_path(vld_branch_info *branch_info, vld_path *path)
{
	if (branch_info->paths_count == branch_info->paths_size) {
		branch_info->paths_size += 32;
		branch_info->paths = realloc(branch_info->paths, sizeof(vld_path*) * branch_info->paths_size);
	}
	branch_info->paths[branch_info->paths_count] = path;
	branch_info->paths_count++;
}

static vld_path *vld_path_new(vld_path *old_path)
{
	vld_path *tmp;
	tmp = calloc(1, sizeof(vld_path));

	if (old_path) {
		unsigned i;

		for (i = 0; i < old_path->elements_count; i++) {
			vld_path_add(tmp, old_path->elements[i]);
		}
	}
	return tmp;
}

static void vld_path_free(vld_path *path)
{
	if (path->elements) {
		free(path->elements);
	}
	free(path);
}

static unsigned int vld_branch_find_last_element(vld_path *path)
{
	return path->elements[path->elements_count-1];
}

static int vld_path_exists(vld_path *path, unsigned int elem1, unsigned int elem2)
{
	int i;

	for (i = 0; i < path->elements_count - 1; i++) {
		if (path->elements[i] == elem1 && path->elements[i + 1] == elem2) {
			return 1;
		}
	}
	return 0;
}

static void vld_branch_find_path(unsigned int nr, vld_branch_info *branch_info, vld_path *prev_path)
{
	unsigned int out0, out1, last;
	vld_path *new_path;
	int found = 0;

	if (branch_info->paths_count > 65535) {
		return;
	}

	new_path = vld_path_new(prev_path);
	vld_path_add(new_path, nr);
	out0 = branch_info->branches[nr].out[0];
	out1 = branch_info->branches[nr].out[1];

	last = vld_branch_find_last_element(new_path);

	if (out0 != 0 && !vld_path_exists(new_path, last, out0)) {
		vld_branch_find_path(out0, branch_info, new_path);
		found = 1;
	}
	if (out1 != 0 && !vld_path_exists(new_path, last, out1)) {
		vld_branch_find_path(out1, branch_info, new_path);
		found = 1;
	}
	if (!found) {
		vld_branch_info_add_path(branch_info, new_path);
	} else {
		vld_path_free(new_path);
	}
}

void vld_branch_find_paths(vld_branch_info *branch_info)
{
	vld_branch_find_path(0, branch_info, NULL);
}

void vld_branch_info_dump(zend_op_array *opa, vld_branch_info *branch_info TSRMLS_DC)
{
	int i, j;
	char *fname = opa->function_name ? opa->function_name : "__main";

	if (VLD_G(path_dump_file)) {
		fprintf(VLD_G(path_dump_file), "subgraph cluster_%08x {\n\tlabel=\"%s\";\n\tgraph [rankdir=\"LR\"];\n\tnode [shape = record];\n", opa, fname, fname);

		for (i = 0; i < branch_info->starts->size; i++) {
			if (vld_set_in(branch_info->starts, i)) {
				fprintf(
					VLD_G(path_dump_file), 
					"\t\"%s_%d\" [ label = \"{ op #%d | line %d-%d }\" ];\n", 
					fname, i, i, 
					branch_info->branches[i].start_lineno,
					branch_info->branches[i].end_lineno
				);
				if (branch_info->branches[i].out[0]) {
					fprintf(VLD_G(path_dump_file), "\t%s_%d -> %s_%d;\n", fname, i, fname, branch_info->branches[i].out[0]);
				}
				if (branch_info->branches[i].out[1]) {
					fprintf(VLD_G(path_dump_file), "\t%s_%d -> %s_%d;\n", fname, i, fname, branch_info->branches[i].out[1]);
				}
			}
		}
		fprintf(VLD_G(path_dump_file), "}\n");
	}

	for (i = 0; i < branch_info->starts->size; i++) {
		if (vld_set_in(branch_info->starts, i)) {
			printf("branch: #%3d; line: %5d-%5d; sop: %5d; eop: %5d",
				i,
				branch_info->branches[i].start_lineno,
				branch_info->branches[i].end_lineno,
				i,
				branch_info->branches[i].end_op
			);
			if (branch_info->branches[i].out[0]) {
				printf("; out1: %3d", branch_info->branches[i].out[0]);
			}
			if (branch_info->branches[i].out[1]) {
				printf("; out2: %3d", branch_info->branches[i].out[1]);
			}
			printf("\n");
		}
	}

	for (i = 0; i < branch_info->paths_count; i++) {
		printf("path #%d: ", i + 1);
		for (j = 0; j < branch_info->paths[i]->elements_count; j++) {
			printf("%d, ", branch_info->paths[i]->elements[j]);
		}
		printf("\n");
	}
}
