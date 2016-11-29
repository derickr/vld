/*
   +----------------------------------------------------------------------+
   | Copyright (c) 1997-2016 Derick Rethans                               |
   +----------------------------------------------------------------------+
   | This source file is subject to the 2-Clause BSD license which is     |
   | available through the LICENSE file, or online at                     |
   | http://opensource.org/licenses/bsd-license.php                       |
   +----------------------------------------------------------------------+
   | Authors:  Derick Rethans <derick@derickrethans.nl>                   |
   |           Andrei Zmievski <andrei@gravitonic.com>                    |
   |           Marcus Börger <marcus.boerger@t-online.de>                 |
   +----------------------------------------------------------------------+
 */
/* $Id: srm_oparray.c,v 1.60 2009-11-25 12:55:40 derick Exp $ */

#include "php.h"
#include "zend_alloc.h"
#include "zend_compile.h"
#include "branchinfo.h"
#include "srm_oparray.h"
#include "ext/standard/url.h"
#include "set.h"
#include "php_vld.h"

ZEND_EXTERN_MODULE_GLOBALS(vld)

#if ZEND_USE_ABS_JMP_ADDR
# define VLD_ZNODE_JMP_LINE(node, opline, base)  (int32_t)(((long)((node).jmp_addr) - (long)(base_address)) / sizeof(zend_op))
#else
# define VLD_ZNODE_JMP_LINE(node, opline, base)  (int32_t)(((int32_t)((node).jmp_offset) / sizeof(zend_op)) + (opline))
#endif

/* Input zend_compile.h
 * And replace [^(...)(#define )([^ \t]+).*$]
 * BY     [/=*  \1 *=/  { "\3", ALL_USED },] REMEMBER to remove the two '=' signs
 */
static const op_usage opcodes[] = {
	/*  0 */	{ "NOP", NONE_USED },
	/*  1 */	{ "ADD", ALL_USED },
	/*  2 */	{ "SUB", ALL_USED },
	/*  3 */	{ "MUL", ALL_USED },
	/*  4 */	{ "DIV", ALL_USED },
	/*  5 */	{ "MOD", ALL_USED },
	/*  6 */	{ "SL", ALL_USED },
	/*  7 */	{ "SR", ALL_USED },
	/*  8 */	{ "CONCAT", ALL_USED },
	/*  9 */	{ "BW_OR", ALL_USED },
	/*  10 */	{ "BW_AND", ALL_USED },
	/*  11 */	{ "BW_XOR", ALL_USED },
	/*  12 */	{ "BW_NOT", RES_USED | OP1_USED },
	/*  13 */	{ "BOOL_NOT", RES_USED | OP1_USED },
	/*  14 */	{ "BOOL_XOR", ALL_USED },
	/*  15 */	{ "IS_IDENTICAL", ALL_USED },
	/*  16 */	{ "IS_NOT_IDENTICAL", ALL_USED },
	/*  17 */	{ "IS_EQUAL", ALL_USED },
	/*  18 */	{ "IS_NOT_EQUAL", ALL_USED },
	/*  19 */	{ "IS_SMALLER", ALL_USED },
	/*  20 */	{ "IS_SMALLER_OR_EQUAL", ALL_USED },
	/*  21 */	{ "CAST", ALL_USED | EXT_VAL },
	/*  22 */	{ "QM_ASSIGN", RES_USED | OP1_USED },
	/*  23 */	{ "ASSIGN_ADD", ALL_USED | EXT_VAL },
	/*  24 */	{ "ASSIGN_SUB", ALL_USED | EXT_VAL },
	/*  25 */	{ "ASSIGN_MUL", ALL_USED | EXT_VAL },
	/*  26 */	{ "ASSIGN_DIV", ALL_USED | EXT_VAL },
	/*  27 */	{ "ASSIGN_MOD", ALL_USED | EXT_VAL },
	/*  28 */	{ "ASSIGN_SL", ALL_USED | EXT_VAL },
	/*  29 */	{ "ASSIGN_SR", ALL_USED | EXT_VAL },
	/*  30 */	{ "ASSIGN_CONCAT", ALL_USED | EXT_VAL },
	/*  31 */	{ "ASSIGN_BW_OR", ALL_USED | EXT_VAL },
	/*  32 */	{ "ASSIGN_BW_AND", ALL_USED | EXT_VAL },
	/*  33 */	{ "ASSIGN_BW_XOR", ALL_USED | EXT_VAL },
	/*  34 */	{ "PRE_INC", OP1_USED | RES_USED },
	/*  35 */	{ "PRE_DEC", OP1_USED | RES_USED },
	/*  36 */	{ "POST_INC", OP1_USED | RES_USED },
	/*  37 */	{ "POST_DEC", OP1_USED | RES_USED },
	/*  38 */	{ "ASSIGN", ALL_USED },
	/*  39 */	{ "ASSIGN_REF", SPECIAL },
	/*  40 */	{ "ECHO", OP1_USED },
#if PHP_VERSION_ID < 70000
	/*  41 */	{ "PRINT", RES_USED | OP1_USED },
#elif PHP_VERSION_ID < 70100
	/*  41 */	{ "UNKNOWN", ALL_USED },
#else
	/*  41 */	{ "GENERATOR_CREATE", RES_USED },
#endif
	/*  42 */	{ "JMP", OP1_USED | OP1_OPLINE },
	/*  43 */	{ "JMPZ", OP1_USED | OP2_USED | OP2_OPLINE },
	/*  44 */	{ "JMPNZ", OP1_USED | OP2_USED | OP2_OPLINE },
	/*  45 */	{ "JMPZNZ", SPECIAL },
	/*  46 */	{ "JMPZ_EX", ALL_USED | OP2_OPLINE },
	/*  47 */	{ "JMPNZ_EX", ALL_USED | OP2_OPLINE },
	/*  48 */	{ "CASE", ALL_USED },
	/*  49 */	{ "SWITCH_FREE", RES_USED | OP1_USED },
	/*  50 */	{ "BRK", SPECIAL },
	/*  51 */	{ "CONT", ALL_USED },
	/*  52 */	{ "BOOL", RES_USED | OP1_USED },
#if defined(ZEND_ENGINE_3)
	/*  53 */	{ "FAST_CONCAT", ALL_USED },
	/*  54 */	{ "ROPE_INIT", ALL_USED | EXT_VAL },
	/*  55 */	{ "ROPE_ADD", ALL_USED | EXT_VAL },
	/*  56 */	{ "ROPE_END", ALL_USED | EXT_VAL },
#else
	/*  53 */	{ "INIT_STRING", RES_USED },
	/*  54 */	{ "ADD_CHAR", ALL_USED },
	/*  55 */	{ "ADD_STRING", ALL_USED },
	/*  56 */	{ "ADD_VAR", ALL_USED },
#endif
	/*  57 */	{ "BEGIN_SILENCE", ALL_USED },
	/*  58 */	{ "END_SILENCE", ALL_USED },
	/*  59 */	{ "INIT_FCALL_BY_NAME", SPECIAL },
	/*  60 */	{ "DO_FCALL", SPECIAL },
#if defined(ZEND_ENGINE_3)
	/*  61 */	{ "INIT_FCALL", ALL_USED },
#else
	/*  61 */	{ "DO_FCALL_BY_NAME", SPECIAL },
#endif
	/*  62 */	{ "RETURN", OP1_USED },
	/*  63 */	{ "RECV", RES_USED | OP1_USED },
	/*  64 */	{ "RECV_INIT", ALL_USED },
	/*  65 */	{ "SEND_VAL", OP1_USED },
#if defined(ZEND_ENGINE_3)
	/*  66 */	{ "SEND_VAR_EX", ALL_USED },
#else
	/*  66 */	{ "SEND_VAR", OP1_USED },
#endif
	/*  67 */	{ "SEND_REF", ALL_USED },
	/*  68 */	{ "NEW", SPECIAL },
#if (PHP_MAJOR_VERSION < 5) || (PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION < 1)
	/*  69 */	{ "JMP_NO_CTOR", SPECIAL },
#else
# if (PHP_MAJOR_VERSION > 5) || (PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION >= 3)
	/*  69 */	{ "INIT_NS_FCALL_BY_NAME", SPECIAL },
# else
	/*  69 */	{ "UNKNOWN", ALL_USED },
# endif
#endif
	/*  70 */	{ "FREE", OP1_USED },
	/*  71 */	{ "INIT_ARRAY", ALL_USED },
	/*  72 */	{ "ADD_ARRAY_ELEMENT", ALL_USED },
	/*  73 */	{ "INCLUDE_OR_EVAL", ALL_USED | OP2_INCLUDE },
	/*  74 */	{ "UNSET_VAR", ALL_USED },
#if defined(ZEND_ENGINE_2) || defined(ZEND_ENGINE_3)
	/*  75 */	{ "UNSET_DIM", ALL_USED },
	/*  76 */	{ "UNSET_OBJ", ALL_USED },
#else
	/*  75 */	{ "UNSET_DIM_OBJ", ALL_USED },
	/*  76 */	{ "ISSET_ISEMPTY", ALL_USED },
#endif
#if defined(ZEND_ENGINE_3)
	/*  77 */	{ "FE_RESET_R", SPECIAL },
	/*  78 */	{ "FE_FETCH_R", ALL_USED | EXT_VAL_JMP_REL },
#else
	/*  77 */	{ "FE_RESET", SPECIAL },
	/*  78 */	{ "FE_FETCH", ALL_USED | OP2_OPNUM },
#endif
	/*  79 */	{ "EXIT", ALL_USED },
	/*  80 */	{ "FETCH_R", RES_USED | OP1_USED | OP_FETCH },
	/*  81 */	{ "FETCH_DIM_R", ALL_USED },
	/*  82 */	{ "FETCH_OBJ_R", ALL_USED },
	/*  83 */	{ "FETCH_W", RES_USED | OP1_USED | OP_FETCH },
	/*  84 */	{ "FETCH_DIM_W", ALL_USED },
	/*  85 */	{ "FETCH_OBJ_W", ALL_USED },
	/*  86 */	{ "FETCH_RW", RES_USED | OP1_USED | OP_FETCH },
	/*  87 */	{ "FETCH_DIM_RW", ALL_USED },
	/*  88 */	{ "FETCH_OBJ_RW", ALL_USED },
	/*  89 */	{ "FETCH_IS", ALL_USED },
	/*  90 */	{ "FETCH_DIM_IS", ALL_USED },
	/*  91 */	{ "FETCH_OBJ_IS", ALL_USED },
	/*  92 */	{ "FETCH_FUNC_ARG", RES_USED | OP1_USED | OP_FETCH },
	/*  93 */	{ "FETCH_DIM_FUNC_ARG", ALL_USED },
	/*  94 */	{ "FETCH_OBJ_FUNC_ARG", ALL_USED },
	/*  95 */	{ "FETCH_UNSET", ALL_USED },
	/*  96 */	{ "FETCH_DIM_UNSET", ALL_USED },
	/*  97 */	{ "FETCH_OBJ_UNSET", ALL_USED },
#if defined(ZEND_ENGINE_3)
	/*  98 */	{ "FETCH_LIST", ALL_USED },
#else
	/*  98 */	{ "FETCH_DIM_TMP_VAR", ALL_USED },
#endif
	/*  99 */	{ "FETCH_CONSTANT", ALL_USED },
#if (PHP_MAJOR_VERSION < 5) || (PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION < 3)
	/*  100 */	{ "DECLARE_FUNCTION_OR_CLASS", ALL_USED },
#else
	/*  100 */	{ "GOTO", ALL_USED | OP1_OPLINE },
#endif
	/*  101 */	{ "EXT_STMT", ALL_USED },
	/*  102 */	{ "EXT_FCALL_BEGIN", ALL_USED },
	/*  103 */	{ "EXT_FCALL_END", ALL_USED },
	/*  104 */	{ "EXT_NOP", ALL_USED },
	/*  105 */	{ "TICKS", ALL_USED },
	/*  106 */	{ "SEND_VAR_NO_REF", ALL_USED | EXT_VAL },
#if PHP_VERSION_ID >= 50000
	/*  107 */	{ "CATCH", SPECIAL },
	/*  108 */	{ "THROW", ALL_USED | EXT_VAL },
	/*  109 */	{ "FETCH_CLASS", SPECIAL },
	/*  110 */	{ "CLONE", ALL_USED },
#if (PHP_MAJOR_VERSION < 5) || (PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION <= 2)
	/*  111 */	{ "INIT_CTOR_CALL", ALL_USED },
#else 
# if PHP_VERSION_ID >= 50400
	/*  111 */	{ "RETURN_BY_REF", OP1_USED },
# else 
	/*  111 */	{ "UNKNOWN", ALL_USED },
# endif
#endif
	/*  112 */	{ "INIT_METHOD_CALL", ALL_USED },
	/*  113 */	{ "INIT_STATIC_METHOD_CALL", ALL_USED },
	/*  114 */	{ "ISSET_ISEMPTY_VAR", ALL_USED | EXT_VAL },
	/*  115 */	{ "ISSET_ISEMPTY_DIM_OBJ", ALL_USED | EXT_VAL },
	
#if defined(ZEND_ENGINE_3)
	/*  116 */	{ "SEND_VAL_EX", ALL_USED },
	/*  117 */	{ "SEND_VAR", ALL_USED },
	/*  118 */	{ "INIT_USER_CALL", ALL_USED | EXT_VAL },
#else
	/*  116 */	{ "IMPORT_FUNCTION", ALL_USED },
	/*  117 */	{ "IMPORT_CLASS", ALL_USED },
	/*  118 */	{ "IMPORT_CONST", ALL_USED },
#endif
	
	/*  119 */	{ "UNKNOWN", ALL_USED },
	/*  120 */	{ "UNKNOWN", ALL_USED },
	
#if defined(ZEND_ENGINE_3)
	/*  121 */	{ "STRLEN", ALL_USED },
	/*  122 */	{ "DEFINED", ALL_USED },
	/*  123 */	{ "TYPE_CHECK", ALL_USED | EXT_VAL },
	/*  124 */	{ "VERIFY_RETURN_TYPE", ALL_USED },
	/*  125 */	{ "FE_RESET_RW", SPECIAL },
	/*  126 */	{ "FE_FETCH_RW", ALL_USED | EXT_VAL_JMP_REL },
	/*  127 */	{ "FE_FREE", ALL_USED },
	/*  128 */	{ "INIT_DYNAMIC_CALL", ALL_USED },
	/*  129 */	{ "DO_ICALL", ALL_USED },
	/*  130 */	{ "DO_UCALL", ALL_USED },
	/*  131 */	{ "DO_FCALL_BY_NAME", SPECIAL },
#else
	/*  121 */	{ "ASSIGN_ADD_OBJ", ALL_USED },
	/*  122 */	{ "ASSIGN_SUB_OBJ", ALL_USED },
	/*  123 */	{ "ASSIGN_MUL_OBJ", ALL_USED },
	/*  124 */	{ "ASSIGN_DIV_OBJ", ALL_USED },
	/*  125 */	{ "ASSIGN_MOD_OBJ", ALL_USED },
	/*  126 */	{ "ASSIGN_SL_OBJ", ALL_USED },
	/*  127 */	{ "ASSIGN_SR_OBJ", ALL_USED },
	/*  128 */	{ "ASSIGN_CONCAT_OBJ", ALL_USED },
	/*  129 */	{ "ASSIGN_BW_OR_OBJ", ALL_USED },
	/*  130 */	{ "ASSIGN_BW_AND_OBJ", ALL_USED },
	/*  131 */	{ "ASSIGN_BW_XOR_OBJ", ALL_USED },
#endif

	/*  132 */	{ "PRE_INC_OBJ", ALL_USED },
	/*  133 */	{ "PRE_DEC_OBJ", ALL_USED },
	/*  134 */	{ "POST_INC_OBJ", ALL_USED },
	/*  135 */	{ "POST_DEC_OBJ", ALL_USED },
	/*  136 */	{ "ASSIGN_OBJ", ALL_USED },
	/*  137 */	{ "OP_DATA", ALL_USED },
	/*  138 */	{ "INSTANCEOF", ALL_USED },
	/*  139 */	{ "DECLARE_CLASS", ALL_USED },
	/*  140 */	{ "DECLARE_INHERITED_CLASS", ALL_USED },
	/*  141 */	{ "DECLARE_FUNCTION", ALL_USED },
	/*  142 */	{ "RAISE_ABSTRACT_ERROR", ALL_USED },
	/*  143 */	{ "DECLARE_CONST", OP1_USED | OP2_USED },
	/*  144 */	{ "ADD_INTERFACE", ALL_USED },
	/*  145 */	{ "VERIFY_INSTANCEOF", ALL_USED },
	/*  146 */	{ "VERIFY_ABSTRACT_CLASS", ALL_USED },
	/*  147 */	{ "ASSIGN_DIM", ALL_USED },
	/*  148 */	{ "ISSET_ISEMPTY_PROP_OBJ", ALL_USED },
	/*  149 */	{ "HANDLE_EXCEPTION", NONE_USED },
	/*  150 */	{ "USER_OPCODE", ALL_USED },
#if defined(ZEND_ENGINE_3)
	/*  151 */	{ "ASSERT_CHECK", ALL_USED },
#else
	/*  151 */	{ "UNKNOWN", ALL_USED },
#endif
	/*  152 */	{ "JMP_SET", ALL_USED | OP2_OPLINE },
	/*  153 */	{ "DECLARE_LAMBDA_FUNCTION", OP1_USED },
	/*  154 */	{ "ADD_TRAIT", ALL_USED },
	/*  155 */	{ "BIND_TRAITS", OP1_USED },
	/*  156 */	{ "SEPARATE", OP1_USED | RES_USED },
#if defined(ZEND_ENGINE_3)
	/*  157 */	{ "FETCH_CLASS_NAME", ALL_USED },
#else
	/*  157 */	{ "QM_ASSIGN_VAR", OP1_USED | RES_USED },
#endif
	/*  158 */	{ "JMP_SET_VAR", OP1_USED | RES_USED },
	/*  159 */	{ "DISCARD_EXCEPTION", NONE_USED },
	/*  160 */	{ "YIELD", ALL_USED },
	/*  161 */	{ "GENERATOR_RETURN", NONE_USED },
	/*  162 */	{ "FAST_CALL", SPECIAL },
	/*  163 */	{ "FAST_RET", SPECIAL },
	/*  164 */	{ "RECV_VARIADIC", ALL_USED },
	/*  165 */	{ "SEND_UNPACK", ALL_USED },
	/*  166 */	{ "POW", ALL_USED },
	/*  167 */	{ "ASSIGN_POW", ALL_USED },
	/*  168 */	{ "BIND_GLOBAL", ALL_USED },
	/*  169 */	{ "COALESCE", ALL_USED },
	/*  170 */	{ "SPACESHIP", ALL_USED },
#endif
#if PHP_VERSION_ID >= 70100
    /*  171 */ { "DECLARE_ANON_CLASS", OP2_USED | RES_USED | RES_CLASS },
    /*  172 */ { "DECLARE_ANON_INHERITED_CLASS", OP2_USED | RES_USED | RES_CLASS },
    /*  173 */ { "FETCH_STATIC_PROP_R", RES_USED | OP1_USED | OP_FETCH },
    /*  174 */ { "FETCH_STATIC_PROP_W", RES_USED | OP1_USED | OP_FETCH },
    /*  175 */ { "FETCH_STATIC_PROP_RW", RES_USED | OP1_USED | OP_FETCH },
    /*  176 */ { "FETCH_STATIC_PROP_IS", ALL_USED },
    /*  177 */ { "FETCH_STATIC_PROP_FUNC_ARG", RES_USED | OP1_USED | OP_FETCH },
    /*  178 */ { "FETCH_STATIC_PROP_UNSET", ALL_USED },
    /*  179 */ { "UNSET_STATIC_PROP", ALL_USED },
    /*  180 */ { "ISSET_ISEMPTY_STATIC_PROP", RES_USED | OP1_USED },
    /*  181 */ { "FETCH_CLASS_CONSTANT", ALL_USED },
    /*  182 */ { "BIND_LEXICAL", ALL_USED },
    /*  183 */ { "BIND_STATIC", ALL_USED },
	/*  184 */	{ "FETCH_THIS", ALL_USED },
	/*  185 */	{ "UNKNOWN", ALL_USED },
	/*  186 */	{ "ISSET_ISEMPTY_THIS", ALL_USED },
#endif
};

#if PHP_VERSION_ID < 70000
zend_brk_cont_element* vld_find_brk_cont(int nest_levels, int array_offset, zend_op_array *op_array);
#endif

static inline int vld_dump_zval_null(ZVAL_VALUE_TYPE value)
{
	return vld_printf (stderr, "null");
}

static inline int vld_dump_zval_long(ZVAL_VALUE_TYPE value)
{
	return vld_printf (stderr, "%ld", value.lval);
}

static inline int vld_dump_zval_double(ZVAL_VALUE_TYPE value)
{
	return vld_printf (stderr, "%g", value.dval);
}

static inline int vld_dump_zval_string(ZVAL_VALUE_TYPE value)
{
	ZVAL_VALUE_STRING_TYPE *new_str;
#if PHP_VERSION_ID < 70000
	int new_len;
#endif
	int len;

	new_str = php_url_encode(ZVAL_STRING_VALUE(value), ZVAL_STRING_LEN(value) PHP_URLENCODE_NEW_LEN(new_len));
	len = vld_printf (stderr, "'%s'", ZSTRING_VALUE(new_str));
	efree(new_str);
	return len;
}

static inline int vld_dump_zval_array(ZVAL_VALUE_TYPE value)
{
	return vld_printf (stderr, "<array>");
}

static inline int vld_dump_zval_object(ZVAL_VALUE_TYPE value)
{
	return vld_printf (stderr, "<object>");
}

static inline int vld_dump_zval_bool(ZVAL_VALUE_TYPE value)
{
	return vld_printf (stderr, "<bool>");
}

static inline int vld_dump_zval_true(ZVAL_VALUE_TYPE value)
{
	return vld_printf (stderr, "<true>");
}

static inline int vld_dump_zval_false(ZVAL_VALUE_TYPE value)
{
	return vld_printf (stderr, "<false>");
}

static inline int vld_dump_zval_resource(ZVAL_VALUE_TYPE value)
{
	return vld_printf (stderr, "<resource>");
}

static inline int vld_dump_zval_constant(ZVAL_VALUE_TYPE value)
{
	return vld_printf (stderr, "<const:'%s'>", ZVAL_STRING_VALUE(value));
}

#if PHP_VERSION_ID >= 50600
static inline int vld_dump_zval_constant_ast(ZVAL_VALUE_TYPE value)
{
	return vld_printf (stderr, "<const ast>");
}
#else
static inline int vld_dump_zval_constant_array(ZVAL_VALUE_TYPE value)
{
	return vld_printf (stderr, "<const array>");
}
#endif

static inline int vld_dump_zval_undef(ZVAL_VALUE_TYPE value)
{
	return vld_printf (stderr, "<undef>");
}

static inline int vld_dump_zval_reference(ZVAL_VALUE_TYPE value)
{
	return vld_printf (stderr, "<reference>");
}

static inline int vld_dump_zval_callable(ZVAL_VALUE_TYPE value)
{
	return vld_printf (stderr, "<callable>");
}

static inline int vld_dump_zval_indirect(ZVAL_VALUE_TYPE value)
{
	return vld_printf (stderr, "<indirect>");
}

static inline int vld_dump_zval_ptr(ZVAL_VALUE_TYPE value)
{
	return vld_printf (stderr, "<ptr>");
}

int vld_dump_zval (zval val)
{
#if PHP_VERSION_ID >= 70000
	switch (val.u1.v.type) {
#else
	switch (val.type) {
#endif
		case IS_NULL:           return vld_dump_zval_null (val.value);
		case IS_LONG:           return vld_dump_zval_long (val.value);
		case IS_DOUBLE:         return vld_dump_zval_double (val.value);
		case IS_STRING:         return vld_dump_zval_string (val.value);
		case IS_ARRAY:          return vld_dump_zval_array (val.value);
		case IS_OBJECT:         return vld_dump_zval_object (val.value);
		case IS_RESOURCE:       return vld_dump_zval_resource (val.value);
		case IS_CONSTANT:       return vld_dump_zval_constant (val.value);
#if PHP_VERSION_ID >= 50600
		case IS_CONSTANT_AST:   return vld_dump_zval_constant_ast (val.value);
#else
		case IS_CONSTANT_ARRAY: return vld_dump_zval_constant_array (val.value);
#endif
#if PHP_VERSION_ID >= 70000
		case IS_UNDEF:          return vld_dump_zval_undef (val.value);
		case IS_FALSE:          return vld_dump_zval_false (val.value);
		case IS_TRUE:           return vld_dump_zval_true (val.value);
		case IS_REFERENCE:      return vld_dump_zval_reference (val.value);
		case IS_CALLABLE:       return vld_dump_zval_callable (val.value);
		case IS_INDIRECT:       return vld_dump_zval_indirect (val.value);
		case IS_PTR:            return vld_dump_zval_ptr (val.value);
#else
		case IS_BOOL:           return vld_dump_zval_bool (val.value);
#endif
	}
	return vld_printf(stderr, "<unknown>");
}


int vld_dump_znode (int *print_sep, unsigned int node_type, VLD_ZNODE node, unsigned int base_address, zend_op_array *op_array, int opline TSRMLS_DC)
{
	int len = 0;

	if (node_type != IS_UNUSED && print_sep) {
		if (*print_sep) {
			len += vld_printf (stderr, ", ");
		}
		*print_sep = 1;
	}
	switch (node_type) {
		case IS_UNUSED:
			VLD_PRINT(3, " IS_UNUSED ");
			break;
		case IS_CONST: /* 1 */
#if PHP_VERSION_ID >= 70000
			VLD_PRINT1(3, " IS_CONST (%d) ", VLD_ZNODE_ELEM(node, var) / sizeof(zval));
#else
			VLD_PRINT1(3, " IS_CONST (%d) ", VLD_ZNODE_ELEM(node, var) / sizeof(temp_variable));
#endif
#if PHP_VERSION_ID >= 70000
			vld_dump_zval(*RT_CONSTANT_EX(op_array->literals, node));
#else
# if PHP_VERSION_ID >= 50399
			vld_dump_zval(*node.zv);
# else
			vld_dump_zval(node.u.constant);
# endif
#endif
			break;
#if defined(ZEND_ENGINE_2) || defined(ZEND_ENGINE_3)
		case IS_TMP_VAR: /* 2 */
			VLD_PRINT(3, " IS_TMP_VAR ");
			len += vld_printf (stderr, "~%d", VAR_NUM(VLD_ZNODE_ELEM(node, var)));
			break;
		case IS_VAR: /* 4 */
			VLD_PRINT(3, " IS_VAR ");
			len += vld_printf (stderr, "$%d", VAR_NUM(VLD_ZNODE_ELEM(node, var)));
			break;
#if (PHP_MAJOR_VERSION > 5) || (PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION >= 1)
		case IS_CV:  /* 16 */
			VLD_PRINT(3, " IS_CV ");
#if PHP_VERSION_ID >= 70000
			len += vld_printf (stderr, "!%d", (VLD_ZNODE_ELEM(node, var)-sizeof(zend_execute_data)) / sizeof(zval));
#else
			len += vld_printf (stderr, "!%d", VLD_ZNODE_ELEM(node, var));
#endif
			break;
#endif
		case VLD_IS_OPNUM:
#if PHP_VERSION_ID >= 70000
			len += vld_printf (stderr, "->%d", VLD_ZNODE_JMP_LINE(node, opline, base_address));
#else
			len += vld_printf (stderr, "->%d", VLD_ZNODE_ELEM(node, opline_num));
#endif
			break;
		case VLD_IS_OPLINE:
#if PHP_VERSION_ID >= 70000
			len += vld_printf (stderr, "->%d", VLD_ZNODE_JMP_LINE(node, opline, base_address));
#else
			len += vld_printf (stderr, "->%d", (VLD_ZNODE_ELEM(node, opline_num) - base_address) / sizeof(zend_op));
#endif
			break;
		case VLD_IS_CLASS:
			len += vld_printf (stderr, ":%d", VAR_NUM(VLD_ZNODE_ELEM(node, var)));
			break;
#else /* ZEND_ENGINE_1 */
		case IS_TMP_VAR: /* 2 */
			len += vld_printf (stderr, "~%d", node.u.var);
			break;
		case IS_VAR: /* 4 */
			len += vld_printf (stderr, "$%d", node.u.var);
			break;           
		case VLD_IS_OPNUM:
		case VLD_IS_OPLINE:
			len += vld_printf (stderr, "->%d", node.u.opline_num);
			break;
		case VLD_IS_CLASS:
			len += vld_printf (stderr, ":%d", node.u.var);
			break;
#endif
		default:
			return 0;
	}
	return len;
}

static unsigned int vld_get_special_flags(const zend_op *op, unsigned int base_address)
{
	unsigned int flags = 0;

	switch (op->opcode) {
#if PHP_VERSION_ID >= 70000
		case ZEND_FE_RESET_R:
		case ZEND_FE_RESET_RW:
#else
		case ZEND_FE_RESET:
#endif
			flags = ALL_USED;
#if (PHP_MAJOR_VERSION > 5) || (PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION >= 1)
			flags |= OP2_OPNUM;
#elif (PHP_MAJOR_VERSION > 4) || (PHP_MAJOR_VERSION == 4 && PHP_MINOR_VERSION > 3) || (PHP_MAJOR_VERSION == 4 && PHP_MINOR_VERSION == 3 && PHP_RELEASE_VERSION >= 11)
			flags |= NOP2_OPNUM;
#endif
			break;

		case ZEND_ASSIGN_REF:
			flags = OP1_USED | OP2_USED;
			if (op->VLD_TYPE(result) != IS_UNUSED) {
				flags |= RES_USED;
			}
			break;

#if PHP_VERSION_ID < 70000
		case ZEND_DO_FCALL_BY_NAME:
#endif
		case ZEND_DO_FCALL:
			flags = OP1_USED | RES_USED | EXT_VAL;
			/*flags = ALL_USED | EXT_VAL;
			op->op2.op_type = IS_CONST;
			op->op2.u.constant.type = IS_LONG;*/
			break;

		case ZEND_INIT_FCALL_BY_NAME:
			flags = OP2_USED;
			if (op->VLD_TYPE(op1) != IS_UNUSED) {
				flags |= OP1_USED;
			}
			break;

		case ZEND_JMPZNZ:
			flags = OP1_USED | OP2_USED | EXT_VAL | OP2_OPNUM;
//			op->result = op->op1;
			break;

#if (PHP_MAJOR_VERSION < 5) || (PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION < 1)
		case ZEND_JMP_NO_CTOR:
			flags = OP2_USED | OP2_OPNUM;
			if (op->VLD_TYPE(op1) != IS_UNUSED) {
				flags |= OP1_USED;
			}
			break;
#endif

#if defined(ZEND_ENGINE_2) || defined(ZEND_ENGINE_3)
		case ZEND_FETCH_CLASS:
			flags = EXT_VAL|RES_USED|OP2_USED|RES_CLASS;
			break;
#endif

		case ZEND_NEW:
			flags = RES_USED|OP1_USED;
#if defined(ZEND_ENGINE_2) || defined(ZEND_ENGINE_3)
			flags |= OP1_CLASS;
#endif
			break;

		case ZEND_BRK:
		case ZEND_CONT:
			flags = OP2_USED|OP2_BRK_CONT;
			break;
#if PHP_VERSION_ID >= 50500
		case ZEND_FAST_CALL:
			flags = OP1_USED|OP1_OPLINE;
			if (op->extended_value) {
				flags |= OP2_USED|OP2_OPNUM|EXT_VAL;
			}
			break;
		case ZEND_FAST_RET:
			if (op->extended_value) {
				flags = OP2_USED|OP2_OPNUM|EXT_VAL;
			}
			break;
#endif
#if PHP_VERSION_ID >= 50000
		case ZEND_CATCH:
			flags = ALL_USED;
			if (!op->result.num) {
#if PHP_VERSION_ID >= 70100
				flags |= EXT_VAL_JMP_REL;
#else
				flags |= EXT_VAL_JMP_ABS;
#endif
			}
			break;
#endif
	}
	return flags;
}

#define NUM_KNOWN_OPCODES (sizeof(opcodes)/sizeof(opcodes[0]))

void vld_dump_op(int nr, zend_op * op_ptr, unsigned int base_address, int notdead, int entry, int start, int end, zend_op_array *opa TSRMLS_DC)
{
	static uint last_lineno = (uint) -1;
	int print_sep = 0, len;
	char *fetch_type = "";
	unsigned int flags, op1_type, op2_type, res_type;
	const zend_op op = op_ptr[nr];
	
	if (op.lineno == 0) {
		return;
	}

	if (op.opcode >= NUM_KNOWN_OPCODES) {
		flags = ALL_USED;
	} else {
		flags = opcodes[op.opcode].flags;
	}

	op1_type = op.VLD_TYPE(op1);
	op2_type = op.VLD_TYPE(op2);
	res_type = op.VLD_TYPE(result);

	if (flags == SPECIAL) {
		flags = vld_get_special_flags(&op, base_address);
	} 
	if (flags & OP1_OPLINE) {
		op1_type = VLD_IS_OPLINE;
	}
	if (flags & OP2_OPLINE) {
		op2_type = VLD_IS_OPLINE;
	}
	if (flags & OP1_OPNUM) {
		op1_type = VLD_IS_OPNUM;
	}
	if (flags & OP2_OPNUM) {
		op2_type = VLD_IS_OPNUM;
	}
	if (flags & OP1_CLASS) {
		op1_type = VLD_IS_CLASS;
	}
	if (flags & RES_CLASS) {
		res_type = VLD_IS_CLASS;
	}

#if PHP_VERSION_ID >= 50600 && PHP_VERSION_ID < 70100
	switch (op.opcode) {
		case ZEND_FAST_RET:
			if (op.extended_value == ZEND_FAST_RET_TO_FINALLY) {
				fetch_type = "to_finally";
			} else if (op.extended_value == ZEND_FAST_RET_TO_CATCH) {
				fetch_type = "to_catch";
			}
			break;
		case ZEND_FAST_CALL:
			if (op.extended_value == ZEND_FAST_CALL_FROM_FINALLY) {
				fetch_type = "from_finally";
			}
			break;
	}
#endif

	if (flags & OP_FETCH) {
#if defined(ZEND_ENGINE_2) || defined(ZEND_ENGINE_3)
		switch (op.VLD_EXTENDED_VALUE(op2)) {
			case ZEND_FETCH_GLOBAL:
				fetch_type = "global";
				break;
			case ZEND_FETCH_LOCAL:
				fetch_type = "local";
				break;
#if PHP_VERSION_ID < 70100
			case ZEND_FETCH_STATIC:
				fetch_type = "static";
				break;
			case ZEND_FETCH_STATIC_MEMBER:
				fetch_type = "static member";
				break;
#endif
#ifdef ZEND_FETCH_GLOBAL_LOCK
			case ZEND_FETCH_GLOBAL_LOCK:
				fetch_type = "global lock";
				break;
#endif
#ifdef ZEND_FETCH_AUTO_GLOBAL
			case ZEND_FETCH_AUTO_GLOBAL:
				fetch_type = "auto global";
				break;
#endif
			default:
				fetch_type = "unknown";
				break;
		}
#else /* ZEND_ENGINE_1 */
		if (op.op2.u.fetch_type == ZEND_FETCH_GLOBAL) {
			fetch_type = "global";
		} else if (op.op2.u.fetch_type == ZEND_FETCH_STATIC) {
			fetch_type = "static";
		}
#endif
	}

	if (op.lineno == last_lineno) {
		vld_printf(stderr, "     ");
	} else {
		vld_printf(stderr, "%4d ", op.lineno);
		last_lineno = op.lineno;
	}

	if (op.opcode >= NUM_KNOWN_OPCODES) {
		if (VLD_G(format)) {
			vld_printf(stderr, "%5d %s %c %c %c %c %s <%03d>%-23s %s %-14s ", nr, VLD_G(col_sep), notdead ? ' ' : '*', entry ? 'E' : ' ', start ? '>' : ' ', end ? '>' : ' ', VLD_G(col_sep), op.opcode, VLD_G(col_sep), fetch_type);
		} else {
			vld_printf(stderr, "%5d%c %c %c %c <%03d>%-23s %-14s ", nr, notdead ? ' ' : '*', entry ? 'E' : ' ', start ? '>' : ' ', end ? '>' : ' ', op.opcode, "", fetch_type);
		}
	} else {
		if (VLD_G(format)) {
			vld_printf(stderr, "%5d %s %c %c %c %c %s %-28s %s %-14s ", nr, VLD_G(col_sep), notdead ? ' ' : '*', entry ? 'E' : ' ', start ? '>' : ' ', end ? '>' : ' ', VLD_G(col_sep), opcodes[op.opcode].name, VLD_G(col_sep), fetch_type);
		} else {
			vld_printf(stderr, "%5d%c %c %c %c %-28s %-14s ", nr, notdead ? ' ' : '*', entry ? 'E' : ' ', start ? '>' : ' ', end ? '>' : ' ', opcodes[op.opcode].name, fetch_type);
		}
	}

	if (flags & EXT_VAL) {
		vld_printf(stderr, "%3d  ", op.extended_value);
	} else {
		vld_printf(stderr, "     ");
	}

#if PHP_VERSION_ID >= 70100
	if ((flags & RES_USED) && op.result_type != IS_UNUSED) {
#else
	if ((flags & RES_USED) && !(op.VLD_EXTENDED_VALUE(result) & EXT_TYPE_UNUSED)) {
#endif
		VLD_PRINT(3, " RES[ ");
		len = vld_dump_znode (NULL, res_type, op.result, base_address, opa, nr TSRMLS_CC);
		VLD_PRINT(3, " ]");
		if (VLD_G(format)) {
			if (len==0) {
				vld_printf(stderr, " ");
			}
		} else {
			vld_printf(stderr, "%*s", 8-len, " ");
		}
	} else {
		vld_printf(stderr, "        ");
	}

	if (flags & OP1_USED) {
		VLD_PRINT(3, " OP1[ ");
		vld_dump_znode (&print_sep, op1_type, op.op1, base_address, opa, nr TSRMLS_CC);
		VLD_PRINT(3, " ]");
	}
	if (flags & OP2_USED) {
		VLD_PRINT(3, " OP2[ ");
		if (flags & OP2_INCLUDE) {
			if (VLD_G(verbosity) < 3 && print_sep) {
				vld_printf(stderr, ", ");
			}
#if PHP_VERSION_ID >= 50399
			switch (op.extended_value) {
#else
			switch (Z_LVAL(op.op2.u.constant)) {
#endif
				case ZEND_INCLUDE_ONCE:
					vld_printf(stderr, "INCLUDE_ONCE");
					break;
				case ZEND_REQUIRE_ONCE:
					vld_printf(stderr, "REQUIRE_ONCE");
					break;
				case ZEND_INCLUDE:
					vld_printf(stderr, "INCLUDE");
					break;
				case ZEND_REQUIRE:
					vld_printf(stderr, "REQUIRE");
					break;
				case ZEND_EVAL:
					vld_printf(stderr, "EVAL");
					break;
				default:
					vld_printf(stderr, "!!ERROR!!");
					break;
			}
		} else {
			vld_dump_znode (&print_sep, op2_type, op.op2, base_address, opa, nr TSRMLS_CC);
		}
		VLD_PRINT(3, " ]");
	}
#if PHP_VERSION_ID < 70000
	if (flags & OP2_BRK_CONT) {
		long jmp;
		zend_brk_cont_element *el;

		VLD_PRINT(3, " BRK_CONT[ ");
#if PHP_VERSION_ID >= 50399
		el = vld_find_brk_cont(Z_LVAL_P(op.op2.zv), VLD_ZNODE_ELEM(op.op1, opline_num), opa);
#else
		el = vld_find_brk_cont(op.op2.u.constant.value.lval, VLD_ZNODE_ELEM(op.op1, opline_num), opa);
#endif
		jmp = op.opcode == ZEND_BRK ? el->brk : el->cont;
		vld_printf (stderr, ", ->%d", jmp);
		VLD_PRINT(3, " ]");
	}
#endif
	if (flags & EXT_VAL_JMP_ABS) {
		VLD_PRINT(3, " EXT_JMP_ABS[ ");
		vld_printf (stderr, ", ->%d", op.extended_value);
		VLD_PRINT(3, " ]");
	}
	if (flags & EXT_VAL_JMP_REL) {
		VLD_PRINT(3, " EXT_JMP_REL[ ");
#if PHP_VERSION_ID >= 70000
		vld_printf (stderr, ", ->%d", nr + ((int) op.extended_value / sizeof(zend_op)));
#else
		vld_printf (stderr, ", ->%d", nr + op.extended_value);
#endif
		VLD_PRINT(3, " ]");
	}
	if (flags & NOP2_OPNUM) {
		zend_op next_op = op_ptr[nr+1];
		vld_dump_znode (&print_sep, VLD_IS_OPNUM, next_op.op2, base_address, opa, nr TSRMLS_CC);
	}
	vld_printf (stderr, "\n");
}

void vld_analyse_oparray(zend_op_array *opa, vld_set *set, vld_branch_info *branch_info TSRMLS_DC);
void vld_analyse_branch(zend_op_array *opa, unsigned int position, vld_set *set, vld_branch_info *branch_info TSRMLS_DC);

void vld_dump_oparray(zend_op_array *opa TSRMLS_DC)
{
	unsigned int i;
	int          j;
	vld_set *set;
	vld_branch_info *branch_info;
	unsigned int base_address = (unsigned int)(zend_intptr_t)&(opa->opcodes[0]);

	set = vld_set_create(opa->last);
	branch_info = vld_branch_info_create(opa->last);

	if (VLD_G(dump_paths)) {
		vld_analyse_oparray(opa, set, branch_info TSRMLS_CC);
	}
	if (VLD_G(format)) {
		vld_printf (stderr, "filename:%s%s\n", VLD_G(col_sep), ZSTRING_VALUE(opa->filename));
		vld_printf (stderr, "function name:%s%s\n", VLD_G(col_sep), ZSTRING_VALUE(opa->function_name));
		vld_printf (stderr, "number of ops:%s%d\n", VLD_G(col_sep), opa->last);
	} else {
		vld_printf (stderr, "filename:       %s\n", ZSTRING_VALUE(opa->filename));
		vld_printf (stderr, "function name:  %s\n", ZSTRING_VALUE(opa->function_name));
		vld_printf (stderr, "number of ops:  %d\n", opa->last);
	}
#ifdef IS_CV /* PHP >= 5.1 */
	vld_printf (stderr, "compiled vars:  ");
	for (j = 0; j < opa->last_var; j++) {
		vld_printf (stderr, "!%d = $%s%s", j, OPARRAY_VAR_NAME(opa->vars[j]), ((j + 1) == opa->last_var) ? "\n" : ", ");
	}
	if (!opa->last_var) {
		vld_printf(stderr, "none\n");
	}
#endif
	if (VLD_G(format)) {
		vld_printf(stderr, "line%s# *%s%s%sop%sfetch%sext%sreturn%soperands\n",VLD_G(col_sep),VLD_G(col_sep),VLD_G(col_sep),VLD_G(col_sep),VLD_G(col_sep),VLD_G(col_sep),VLD_G(col_sep),VLD_G(col_sep));
	} else {
		vld_printf(stderr, "line     #* E I O op                           fetch          ext  return  operands\n");
		vld_printf(stderr, "-------------------------------------------------------------------------------------\n");
	}
	for (i = 0; i < opa->last; i++) {
		vld_dump_op(i, opa->opcodes, base_address, vld_set_in(set, i), vld_set_in(branch_info->entry_points, i), vld_set_in(branch_info->starts, i), vld_set_in(branch_info->ends, i), opa TSRMLS_CC);
	}
	vld_printf(stderr, "\n");

	if (VLD_G(dump_paths)) {
		vld_branch_post_process(opa, branch_info);
		vld_branch_find_paths(branch_info);
		vld_branch_info_dump(opa, branch_info TSRMLS_CC);
	}

	vld_set_free(set);
	vld_branch_info_free(branch_info);
}

void opt_set_nop (zend_op_array *opa, int nr)
{
	opa->opcodes[nr].opcode = ZEND_NOP;
}

#if PHP_VERSION_ID < 70000
zend_brk_cont_element* vld_find_brk_cont(int nest_levels, int array_offset, zend_op_array *op_array)
{
	zend_brk_cont_element *jmp_to;

	do {
		jmp_to = &op_array->brk_cont_array[array_offset];
		array_offset = jmp_to->parent;
	} while (--nest_levels > 0);
	return jmp_to;
}
#endif

int vld_find_jump(zend_op_array *opa, unsigned int position, long *jmp1, long *jmp2)
{
#if PHP_VERSION_ID < 70000 || ZEND_USE_ABS_JMP_ADDR
	zend_op *base_address = &(opa->opcodes[0]);
#endif

	zend_op opcode = opa->opcodes[position];
	if (opcode.opcode == ZEND_JMP) {
#if PHP_VERSION_ID >= 70000
		*jmp1 = VLD_ZNODE_JMP_LINE(opcode.op1, position, base_address);
#elif defined(ZEND_ENGINE_2)
		*jmp1 = ((long) VLD_ZNODE_ELEM(opcode.op1, jmp_addr) - (long) base_address) / sizeof(zend_op);
#else
		*jmp1 = opcode.op1.u.opline_num;
#endif
		return 1;
	} else if (
		opcode.opcode == ZEND_JMPZ ||
		opcode.opcode == ZEND_JMPNZ ||
		opcode.opcode == ZEND_JMPZ_EX ||
		opcode.opcode == ZEND_JMPNZ_EX
	) {
		*jmp1 = position + 1;
#if PHP_VERSION_ID >= 70000
		*jmp2 = VLD_ZNODE_JMP_LINE(opcode.op2, position, base_address);
#elif defined(ZEND_ENGINE_2)
		*jmp2 = ((long) VLD_ZNODE_ELEM(opcode.op2, jmp_addr) - (long) base_address) / sizeof(zend_op);
#else
		*jmp2 = opcode.op1.u.opline_num;
#endif
		return 1;
	} else if (opcode.opcode == ZEND_JMPZNZ) {
#if PHP_VERSION_ID >= 70000
		*jmp1 = VLD_ZNODE_JMP_LINE(opcode.op2, position, base_address);
		*jmp2 = (int32_t) (position + ((int32_t)opcode.extended_value / sizeof(zend_op)));
#else
		*jmp1 = VLD_ZNODE_ELEM(opcode.op2, opline_num);
		*jmp2 = opcode.extended_value;
#endif
		return 1;
#if PHP_VERSION_ID < 70000
	} else if (opcode.opcode == ZEND_BRK || opcode.opcode == ZEND_CONT) {
		zend_brk_cont_element *el;

		if (VLD_TYPE(opcode.op2) == IS_CONST
#ifdef ZEND_ENGINE_2
		    && VLD_ZNODE_ELEM(opcode.op1, jmp_addr) != (zend_op*) 0xFFFFFFFF
#endif
		) {
#if PHP_VERSION_ID >= 50399
			el = vld_find_brk_cont(Z_LVAL_P(opcode.op2.zv), VLD_ZNODE_ELEM(opcode.op1, opline_num), opa);
#else
			el = vld_find_brk_cont(opcode.op2.u.constant.value.lval, VLD_ZNODE_ELEM(opcode.op1, opline_num), opa);
#endif
			*jmp1 = opcode.opcode == ZEND_BRK ? el->brk : el->cont;
			return 1;
		}
#endif
#if PHP_VERSION_ID >= 70000
	} else if (opcode.opcode == ZEND_FE_FETCH_R || opcode.opcode == ZEND_FE_FETCH_RW) {
		*jmp1 = position + 1;
		*jmp2 = position + (opcode.extended_value / sizeof(zend_op));
		return 1;
	} else if (opcode.opcode == ZEND_FE_RESET_R || opcode.opcode == ZEND_FE_RESET_RW) {
#else
	} else if (opcode.opcode == ZEND_FE_RESET || opcode.opcode == ZEND_FE_FETCH) {
#endif
		*jmp1 = position + 1;
#if PHP_VERSION_ID >= 70000
		*jmp2 = VLD_ZNODE_JMP_LINE(opcode.op2, position, base_address);
#else
		*jmp2 = VLD_ZNODE_ELEM(opcode.op2, opline_num);
#endif
		return 1;
	} else if (opcode.opcode == ZEND_CATCH) {
		*jmp1 = position + 1;
#if PHP_VERSION_ID >= 50400
		if (!opcode.result.num) {
#else
		if (!opcode.op1.u.EA.type) {
#endif
#if PHP_VERSION_ID >= 70100
			*jmp2 = position + (opcode.extended_value / sizeof(zend_op));
#else
			*jmp2 = opcode.extended_value;
#endif
			if (*jmp2 == *jmp1) {
				*jmp2 = VLD_JMP_NOT_SET;
			}
		} else {
			*jmp2 = VLD_JMP_EXIT;
		}
		return 1;
#if PHP_VERSION_ID >= 50300
	} else if (opcode.opcode == ZEND_GOTO) {
#if PHP_VERSION_ID >= 70000
		*jmp1 = VLD_ZNODE_JMP_LINE(opcode.op1, position, base_address);
#else
		*jmp1 = ((long) VLD_ZNODE_ELEM(opcode.op1, jmp_addr) - (long) base_address) / sizeof(zend_op);
#endif
		return 1;
#endif

#if PHP_VERSION_ID >= 50500
	} else if (opcode.opcode == ZEND_FAST_CALL) {
#if PHP_VERSION_ID >= 70000
		*jmp1 = VLD_ZNODE_JMP_LINE(opcode.op1, position, base_address);
#else
		*jmp1 = ((long) VLD_ZNODE_ELEM(opcode.op1, jmp_addr) - (long) base_address) / sizeof(zend_op);
#endif
		*jmp2 = position + 1;
		return 1;
	} else if (opcode.opcode == ZEND_FAST_RET) {
		*jmp1 = VLD_JMP_EXIT;
		return 1;
#endif

	} else if (
#if PHP_VERSION_ID >= 50500
		opcode.opcode == ZEND_GENERATOR_RETURN ||
#endif
		opcode.opcode == ZEND_EXIT ||
		opcode.opcode == ZEND_THROW ||
		opcode.opcode == ZEND_RETURN
	) {
		*jmp1 = VLD_JMP_EXIT;
		return 1;
	}

	return 0;
}

void vld_analyse_oparray(zend_op_array *opa, vld_set *set, vld_branch_info *branch_info TSRMLS_DC)
{
	unsigned int position = 0;

	VLD_PRINT(1, "Finding entry points\n");
	while (position < opa->last) {
		if (position == 0) {
			vld_analyse_branch(opa, position, set, branch_info TSRMLS_CC);
			vld_set_add(branch_info->entry_points, position);
#if PHP_MAJOR_VERSION >= 5
		} else if (opa->opcodes[position].opcode == ZEND_CATCH) {
			if (VLD_G(format)) {
				VLD_PRINT2(1, "Found catch point at position:%s%d\n", VLD_G(col_sep),position);
			} else {
				VLD_PRINT1(1, "Found catch point at position: %d\n", position);
			}
			vld_analyse_branch(opa, position, set, branch_info TSRMLS_CC);
			vld_set_add(branch_info->entry_points, position);
#endif
		}
		position++;
	}
	vld_set_add(branch_info->ends, opa->last-1);
	branch_info->branches[opa->last-1].start_lineno = opa->opcodes[opa->last-1].lineno;
}

void vld_analyse_branch(zend_op_array *opa, unsigned int position, vld_set *set, vld_branch_info *branch_info TSRMLS_DC)
{
	long jump_pos1 = VLD_JMP_NOT_SET;
	long jump_pos2 = VLD_JMP_NOT_SET;

	if (VLD_G(format)) {
		VLD_PRINT2(1, "Branch analysis from position:%s%d\n", VLD_G(col_sep),position);
	} else {
		VLD_PRINT1(1, "Branch analysis from position: %d\n", position);
	}

	vld_set_add(branch_info->starts, position);
	branch_info->branches[position].start_lineno = opa->opcodes[position].lineno;

	/* First we see if the branch has been visited, if so we bail out. */
	if (vld_set_in(set, position)) {
		return;
	}
	/* Loop over the opcodes until the end of the array, or until a jump point has been found */
	VLD_PRINT1(2, "Add %d\n", position);
	vld_set_add(set, position);
	while (position < opa->last) {
		jump_pos1 = VLD_JMP_NOT_SET;
		jump_pos2 = VLD_JMP_NOT_SET;

		/* See if we have a jump instruction */
		if (vld_find_jump(opa, position, &jump_pos1, &jump_pos2)) {
			VLD_PRINT2(
				1, "Jump found. (Code = %d) Position 1 = %d",
				opa->opcodes[position].opcode,
				jump_pos1
			);
			if (jump_pos2 != VLD_JMP_NOT_SET) {
				VLD_PRINT1(1, ", Position 2 = %d\n", jump_pos2);
			} else {
				VLD_PRINT(1, "\n");
			}
			if (jump_pos1 == VLD_JMP_EXIT || jump_pos1 >= 0) {
				vld_branch_info_update(branch_info, position, opa->opcodes[position].lineno, 0, jump_pos1);
				if (jump_pos1 != VLD_JMP_EXIT) {
					vld_analyse_branch(opa, jump_pos1, set, branch_info TSRMLS_CC);
				}
			}
			if (jump_pos2 == VLD_JMP_EXIT || jump_pos2 >= 0) {
				vld_branch_info_update(branch_info, position, opa->opcodes[position].lineno, 1, jump_pos2);
				if (jump_pos2 != VLD_JMP_EXIT) {
					vld_analyse_branch(opa, jump_pos2, set, branch_info TSRMLS_CC);
				}
			}
			break;
		}
#if defined(ZEND_ENGINE_2) || defined(ZEND_ENGINE_3)
		/* See if we have a throw instruction */
		if (opa->opcodes[position].opcode == ZEND_THROW) {
			VLD_PRINT1(1, "Throw found at %d\n", position);
			vld_set_add(branch_info->ends, position);
			branch_info->branches[position].start_lineno = opa->opcodes[position].lineno;
			break;
		}
#endif
		/* See if we have an exit instruction */
		if (opa->opcodes[position].opcode == ZEND_EXIT) {
			VLD_PRINT(1, "Exit found\n");
			vld_set_add(branch_info->ends, position);
			branch_info->branches[position].start_lineno = opa->opcodes[position].lineno;
			break;
		}
		/* See if we have a return instruction */
		if (
			opa->opcodes[position].opcode == ZEND_RETURN
#if PHP_VERSION_ID >= 50400
			|| opa->opcodes[position].opcode == ZEND_RETURN_BY_REF
#endif
		) {
			VLD_PRINT(1, "Return found\n");
			vld_set_add(branch_info->ends, position);
			branch_info->branches[position].start_lineno = opa->opcodes[position].lineno;
			break;
		}

		position++;
		VLD_PRINT1(2, "Add %d\n", position);
		vld_set_add(set, position);
	}
}

