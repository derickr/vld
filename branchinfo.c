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
	tmp->entry_points = vld_set_create(size);
	tmp->starts       = vld_set_create(size);
	tmp->ends         = vld_set_create(size);

	tmp->paths_count = 0;
	tmp->paths_size  = 0;
	tmp->paths = NULL;

	return tmp;
}

void vld_branch_info_free(vld_branch_info *branch_info)
{
	unsigned int i;

	for (i = 0; i < branch_info->paths_count; i++) {
		free(branch_info->paths[i]->elements);
		free(branch_info->paths[i]);
	}
	free(branch_info->paths);
	free(branch_info->branches);
	vld_set_free(branch_info->entry_points);
	vld_set_free(branch_info->starts);
	vld_set_free(branch_info->ends);
	free(branch_info);
}

void vld_branch_info_update(vld_branch_info *branch_info, unsigned int pos, unsigned int lineno, unsigned int outidx, unsigned int jump_pos)
{
	vld_set_add(branch_info->ends, pos);
	branch_info->branches[pos].out[outidx] = jump_pos;
	branch_info->branches[pos].start_lineno = lineno;
}

void vld_only_leave_first_catch(zend_op_array *opa, vld_branch_info *branch_info, int position)
{
	unsigned int exit_jmp;

	if (opa->opcodes[position].opcode == ZEND_FETCH_CLASS) {
		position++;
	}

#if PHP_VERSION_ID >= 70100
	exit_jmp = position + ((signed int) opa->opcodes[position].extended_value / sizeof(zend_op));
#else
	exit_jmp = opa->opcodes[position].extended_value;
#endif

	if (opa->opcodes[position].opcode != ZEND_CATCH) {
		return;
	}

	if (opa->opcodes[exit_jmp].opcode == ZEND_FETCH_CLASS) {
		exit_jmp++;
	}
	if (opa->opcodes[exit_jmp].opcode == ZEND_CATCH) {
		vld_only_leave_first_catch(opa, branch_info, exit_jmp);
	}

	vld_set_remove(branch_info->entry_points, position);
}

void vld_branch_post_process(zend_op_array *opa, vld_branch_info *branch_info)
{
	unsigned int i;
	int in_branch = 0, last_start = VLD_JMP_NOT_SET;

	/* Figure out which CATCHes are chained, and hence which ones should be
	 * considered entry points */
	for (i = 0; i < branch_info->entry_points->size; i++) {
		if (vld_set_in(branch_info->entry_points, i) && opa->opcodes[i].opcode == ZEND_CATCH) {
#if PHP_VERSION_ID >= 70100
			vld_only_leave_first_catch(opa, branch_info, i + ((signed int) opa->opcodes[i].extended_value / sizeof(zend_op)));
#else
			vld_only_leave_first_catch(opa, branch_info, opa->opcodes[i].extended_value);
#endif
		}
	}

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
	unsigned int i;

	for (i = 0; i < path->elements_count - 1; i++) {
		if (path->elements[i] == elem1 && path->elements[i + 1] == elem2) {
			return 1;
		}
	}
	return 0;
}

static void vld_branch_find_path(unsigned int nr, vld_branch_info *branch_info, vld_path *prev_path)
{
	int out0, out1;
	unsigned int last;
	vld_path *new_path;
	int found = 0;

	if (branch_info->paths_count > 255/*65535*/) {
		return;
	}

	new_path = vld_path_new(prev_path);
	vld_path_add(new_path, nr);
	out0 = branch_info->branches[nr].out[0];
	out1 = branch_info->branches[nr].out[1];

	last = vld_branch_find_last_element(new_path);

	if (out0 != 0 && out0 != VLD_JMP_EXIT && !vld_path_exists(new_path, last, out0)) {
		vld_branch_find_path(out0, branch_info, new_path);
		found = 1;
	}
	if (out1 != 0 && out1 != VLD_JMP_EXIT && !vld_path_exists(new_path, last, out1)) {
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
	unsigned int i;

	for (i = 0; i < branch_info->entry_points->size; i++) {
		if (vld_set_in(branch_info->entry_points, i)) {
			vld_branch_find_path(i, branch_info, NULL);
		}
	}
}

void vld_branch_info_dump(zend_op_array *opa, vld_branch_info *branch_info TSRMLS_DC)
{
	unsigned int i, j;
	const char *fname = opa->function_name ? ZSTRING_VALUE(opa->function_name) : "__main";

	if (VLD_G(path_dump_file)) {
		fprintf(VLD_G(path_dump_file), "subgraph cluster_%p {\n\tlabel=\"%s\";\n\tgraph [rankdir=\"LR\"];\n\tnode [shape = record];\n", opa, fname);

		for (i = 0; i < branch_info->starts->size; i++) {
			if (vld_set_in(branch_info->starts, i)) {
				fprintf(
					VLD_G(path_dump_file), 
					"\t\"%s_%d\" [ label = \"{ op #%d-%d | line %d-%d }\" ];\n", 
					fname, i, i, 
					branch_info->branches[i].end_op,
					branch_info->branches[i].start_lineno,
					branch_info->branches[i].end_lineno
				);
				if (vld_set_in(branch_info->entry_points, i)) {
					fprintf(VLD_G(path_dump_file), "\t%s_ENTRY -> %s_%d\n", fname, fname, i);
				}
				if (branch_info->branches[i].out[0]) {
					if (branch_info->branches[i].out[0] == VLD_JMP_EXIT) {
						fprintf(VLD_G(path_dump_file), "\t%s_%d -> %s_EXIT;\n", fname, i, fname);
					} else {
						fprintf(VLD_G(path_dump_file), "\t%s_%d -> %s_%d;\n", fname, i, fname, branch_info->branches[i].out[0]);
					}
				}
				if (branch_info->branches[i].out[1]) {
					if (branch_info->branches[i].out[1] == VLD_JMP_EXIT) {
						fprintf(VLD_G(path_dump_file), "\t%s_%d -> %s_EXIT;\n", fname, i, fname);
					} else {
						fprintf(VLD_G(path_dump_file), "\t%s_%d -> %s_%d;\n", fname, i, fname, branch_info->branches[i].out[1]);
					}
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
