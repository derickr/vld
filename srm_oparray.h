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

#ifndef SRM_OPARRAY_H
#define SRM_OPARRAY_H

#include "php.h"

#define NOP  0 
#define ADD  1 
#define SUB  2 
#define MUL  3 
#define DIV  4 
#define MOD  5 
#define SL  6 
#define SR  7 
#define CONCAT  8 
#define BW_OR  9 
#define BW_AND  10 
#define BW_XOR  11 
#define BW_NOT  12 
#define BOOL_NOT  13 
#define BOOL_XOR  14 
#define IS_IDENTICAL  15 
#define IS_NOT_IDENTICAL  16 
#define IS_EQUAL  17 
#define IS_NOT_EQUAL  18 
#define IS_SMALLER  19 
#define IS_SMALLER_OR_EQUAL  20 
#define CAST  21 
#define QM_ASSIGN  22 
#define ASSIGN_ADD  23 
#define ASSIGN_SUB  24 
#define ASSIGN_MUL  25 
#define ASSIGN_DIV  26 
#define ASSIGN_MOD  27 
#define ASSIGN_SL  28 
#define ASSIGN_SR  29 
#define ASSIGN_CONCAT  30 
#define ASSIGN_BW_OR  31 
#define ASSIGN_BW_AND  32 
#define ASSIGN_BW_XOR  33 
#define PRE_INC  34 
#define PRE_DEC  35 
#define POST_INC  36 
#define POST_DEC  37 
#define ASSIGN  38 
#define ASSIGN_REF  39 
#define ECHO  40 
#define PRINT  41 
#define JMP  42 
#define JMPZ  43 
#define JMPNZ  44 
#define JMPZNZ  45 
#define JMPZ_EX  46 
#define JMPNZ_EX  47 
#define CASE  48 
#define SWITCH_FREE  49 
#define BRK  50 
#define CONT  51 
#define BOOL  52 
#define INIT_STRING  53 
#define ADD_CHAR  54 
#define ADD_STRING  55 
#define ADD_VAR  56 
#define BEGIN_SILENCE  57 
#define END_SILENCE  58 
#define INIT_FCALL_BY_NAME  59 
#define DO_FCALL  60 
#define DO_FCALL_BY_NAME  61 
#define RETURN  62 
#define RECV  63 
#define RECV_INIT  64 
#define SEND_VAL  65 
#define SEND_VAR  66 
#define SEND_REF  67 
#define NEW  68 
#define JMP_NO_CTOR  69 
#define FREE  70 
#define INIT_ARRAY  71 
#define ADD_ARRAY_ELEMENT  72 
#define INCLUDE_OR_EVAL  73 
#define UNSET_VAR  74 
#define UNSET_DIM_OBJ  75 
#define ISSET_ISEMPTY  76 
#define FE_RESET  77 
#define FE_FETCH  78 
#define EXIT  79 
#define FETCH_R  80 
#define FETCH_DIM_R  81 
#define FETCH_OBJ_R  82 
#define FETCH_W  83 
#define FETCH_DIM_W  84 
#define FETCH_OBJ_W  85 
#define FETCH_RW  86 
#define FETCH_DIM_RW  87 
#define FETCH_OBJ_RW  88 
#define FETCH_IS  89 
#define FETCH_DIM_IS  90 
#define FETCH_OBJ_IS  91 
#define FETCH_FUNC_ARG  92 
#define FETCH_DIM_FUNC_ARG  93 
#define FETCH_OBJ_FUNC_ARG  94 
#define FETCH_UNSET  95 
#define FETCH_DIM_UNSET  96 
#define FETCH_OBJ_UNSET  97 
#define FETCH_DIM_TMP_VAR  98 
#define FETCH_CONSTANT  99 
#define DECLARE_FUNCTION_OR_CLASS  100 
#define EXT_STMT  101 
#define EXT_FCALL_BEGIN  102 
#define EXT_FCALL_END  103 
#define EXT_NOP  104 
#define TICKS  105 
#define SEND_VAR_NO_REF  106 

void srm_dump_oparray (zend_op_array *opa);
void srm_concat_strings (zend_op_array **opa);

#endif

