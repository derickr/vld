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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_vld.h"
#include "srm_oparray.h"

static zend_op_array* (*old_compile_file)(zend_file_handle* file_handle, int type TSRMLS_DC);
static zend_op_array* vld_compile_file(zend_file_handle*, int TSRMLS_DC);

function_entry vld_functions[] = {
	{NULL, NULL, NULL}
};


zend_module_entry vld_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"vld",
	vld_functions,
	PHP_MINIT(vld),
	PHP_MSHUTDOWN(vld),
	PHP_RINIT(vld),	
	PHP_RSHUTDOWN(vld),
	PHP_MINFO(vld),
#if ZEND_MODULE_API_NO >= 20010901
	"0.6.0",
#endif
	STANDARD_MODULE_PROPERTIES
};


#ifdef COMPILE_DL_VLD
ZEND_GET_MODULE(vld)
#endif

ZEND_BEGIN_MODULE_GLOBALS(vld)
	int active;
ZEND_END_MODULE_GLOBALS(vld) 

ZEND_DECLARE_MODULE_GLOBALS(vld)

#ifdef ZTS
#define VLD_G(v) TSRMG(vld_globals_id, zend_vld_globals *, v)
#else
#define VLD_G(v) (vld_globals.v)
#endif 

PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("vld.active", "1", PHP_INI_SYSTEM, OnUpdateBool, active, zend_vld_globals, vld_globals)
PHP_INI_END()
 
static void vld_init_globals(zend_vld_globals *vld_globals)
{
	vld_globals->active = 0;
}


PHP_MINIT_FUNCTION(vld)
{
	ZEND_INIT_MODULE_GLOBALS(vld, vld_init_globals, NULL);
	REGISTER_INI_ENTRIES();
	old_compile_file = zend_compile_file;

	return SUCCESS;
}


PHP_MSHUTDOWN_FUNCTION(vld)
{
	zend_compile_file = old_compile_file;

	return SUCCESS;
}



PHP_RINIT_FUNCTION(vld)
{
	if (VLD_G(active)) {
		zend_compile_file = vld_compile_file;
	}
	return SUCCESS;
}



PHP_RSHUTDOWN_FUNCTION(vld)
{
	zend_compile_file = old_compile_file;

	return SUCCESS;
}


PHP_MINFO_FUNCTION(vld)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "vld support", "enabled");
	php_info_print_table_end();

}

static int vld_check_fe (zend_op_array *fe, zend_bool *have_fe TSRMLS_DC)
{
	if (fe->type == ZEND_USER_FUNCTION) {
		*have_fe = 1;
	}

	return 0;
}

static int vld_dump_fe (zend_op_array *fe TSRMLS_DC)
{
	if (fe->type == ZEND_USER_FUNCTION) {
		printf("Function %s:\n", fe->function_name);
		vld_dump_oparray(fe);
		printf("End of function %s.\n\n", fe->function_name);
	}

	return 0;
}

#ifdef ZEND_ENGINE_2
static int vld_dump_cle (zend_class_entry **class_entry TSRMLS_DC)
#else
static int vld_dump_cle (zend_class_entry *class_entry TSRMLS_DC)
#endif
{
	zend_class_entry *ce;
	zend_bool have_fe = 0;

#ifdef ZEND_ENGINE_2
	ce = *class_entry;
#else
	ce = class_entry;
#endif
	
	zend_hash_apply_with_argument(&ce->function_table, (apply_func_arg_t) vld_check_fe, (void *)&have_fe TSRMLS_CC);
	if (have_fe) {
		printf("Class %s:\n", ce->name);
		zend_hash_apply(&ce->function_table, (apply_func_t) vld_dump_fe TSRMLS_CC);
		printf("End of class %s.\n\n", ce->name);
	} else {
		printf("Class %s: [no user functions]\n", ce->name);
	}

	return 0;
}

/* {{{ zend_op_array vld_compile_file (file_handle, type)
 *    This function provides a hook for the execution of bananas */
static zend_op_array *vld_compile_file(zend_file_handle *file_handle, int type TSRMLS_DC)
{
	zend_op_array *op_array;

	op_array = old_compile_file (file_handle, type TSRMLS_CC);

	if (op_array) {
		vld_dump_oparray (op_array);
	}

	zend_hash_apply (CG(function_table), (apply_func_t) vld_dump_fe TSRMLS_CC);
	zend_hash_apply (CG(class_table), (apply_func_t) vld_dump_cle TSRMLS_CC);

	return op_array;
}
/* }}} */
