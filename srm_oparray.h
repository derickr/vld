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
   | Authors:  Derick Rethans <derick@vl-srm.net>                         |
   +----------------------------------------------------------------------+
 */

#ifndef SRM_OPARRAY_H
#define SRM_OPARRAY_H

#include "php.h"

static const char* opcodes[] = {
	"NOP", /*  0 */
	"ADD", /*  1 */
	"SUB", /*  2 */
	"MUL", /*  3 */
	"DIV", /*  4 */
	"MOD", /*  5 */
	"SL", /*  6 */
	"SR", /*  7 */
	"CONCAT", /*  8 */
	"BW_OR", /*  9 */
	"BW_AND", /*  10 */
	"BW_XOR", /*  11 */
	"BW_NOT", /*  12 */
	"BOOL_NOT", /*  13 */
	"BOOL_XOR", /*  14 */
	"IS_IDENTICAL", /*  15 */
	"IS_NOT_IDENTICAL", /*  16 */
	"IS_EQUAL", /*  17 */
	"IS_NOT_EQUAL", /*  18 */
	"IS_SMALLER", /*  19 */
	"IS_SMALLER_OR_EQUAL", /*  20 */
	"CAST", /*  21 */
	"QM_ASSIGN", /*  22 */
	"ASSIGN_ADD", /*  23 */
	"ASSIGN_SUB", /*  24 */
	"ASSIGN_MUL", /*  25 */
	"ASSIGN_DIV", /*  26 */
	"ASSIGN_MOD", /*  27 */
	"ASSIGN_SL", /*  28 */
	"ASSIGN_SR", /*  29 */
	"ASSIGN_CONCAT", /*  30 */
	"ASSIGN_BW_OR", /*  31 */
	"ASSIGN_BW_AND", /*  32 */
	"ASSIGN_BW_XOR", /*  33 */
	"PRE_INC", /*  34 */
	"PRE_DEC", /*  35 */
	"POST_INC", /*  36 */
	"POST_DEC", /*  37 */
	"ASSIGN", /*  38 */
	"ASSIGN_REF", /*  39 */
	"ECHO", /*  40 */
	"PRINT", /*  41 */
	"JMP", /*  42 */
	"JMPZ", /*  43 */
	"JMPNZ", /*  44 */
	"JMPZNZ", /*  45 */
	"JMPZ_EX", /*  46 */
	"JMPNZ_EX", /*  47 */
	"CASE", /*  48 */
	"SWITCH_FREE", /*  49 */
	"BRK", /*  50 */
	"CONT", /*  51 */
	"BOOL", /*  52 */
	"INIT_STRING", /*  53 */
	"ADD_CHAR", /*  54 */
	"ADD_STRING", /*  55 */
	"ADD_VAR", /*  56 */
	"BEGIN_SILENCE", /*  57 */
	"END_SILENCE", /*  58 */
	"INIT_FCALL_BY_NAME", /*  59 */
	"DO_FCALL", /*  60 */
	"DO_FCALL_BY_NAME", /*  61 */
	"RETURN", /*  62 */
	"RECV", /*  63 */
	"RECV_INIT", /*  64 */
	"SEND_VAL", /*  65 */
	"SEND_VAR", /*  66 */
	"SEND_REF", /*  67 */
	"NEW", /*  68 */
	"JMP_NO_CTOR", /*  69 */
	"FREE", /*  70 */
	"INIT_ARRAY", /*  71 */
	"ADD_ARRAY_ELEMENT", /*  72 */
	"INCLUDE_OR_EVAL", /*  73 */
	"UNSET_VAR", /*  74 */
	"UNSET_DIM_OBJ", /*  75 */
	"ISSET_ISEMPTY", /*  76 */
	"FE_RESET", /*  77 */
	"FE_FETCH", /*  78 */
	"EXIT", /*  79 */
	"FETCH_R", /*  80 */
	"FETCH_DIM_R", /*  81 */
	"FETCH_OBJ_R", /*  82 */
	"FETCH_W", /*  83 */
	"FETCH_DIM_W", /*  84 */
	"FETCH_OBJ_W", /*  85 */
	"FETCH_RW", /*  86 */
	"FETCH_DIM_RW", /*  87 */
	"FETCH_OBJ_RW", /*  88 */
	"FETCH_IS", /*  89 */
	"FETCH_DIM_IS", /*  90 */
	"FETCH_OBJ_IS", /*  91 */
	"FETCH_FUNC_ARG", /*  92 */
	"FETCH_DIM_FUNC_ARG", /*  93 */
	"FETCH_OBJ_FUNC_ARG", /*  94 */
	"FETCH_UNSET", /*  95 */
	"FETCH_DIM_UNSET", /*  96 */
	"FETCH_OBJ_UNSET", /*  97 */
	"FETCH_DIM_TMP_VAR", /*  98 */
	"FETCH_CONSTANT", /*  99 */
	"DECLARE_FUNCTION_OR_CLASS", /*  100 */
	"EXT_STMT", /*  101 */
	"EXT_FCALL_BEGIN", /*  102 */
	"EXT_FCALL_END", /*  103 */
	"EXT_NOP", /*  104 */
	"TICKS", /*  105 */
	"SEND_VAR_NO_REF", /*  106 */
};

static const char* znode_type[] = {
	"",
	"cnst",
	"tvar",
	"",
	"var",
	"", "", "",
	"unsd",
};


void srm_dump_oparray (zend_op_array *opa);


#endif

