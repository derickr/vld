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
/* $Id: vld.c,v 1.29 2007-08-01 07:12:20 derick Exp $ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_vld.h"
#include "srm_oparray.h"
#include "php_globals.h"

static zend_op_array* (*old_compile_file)(zend_file_handle* file_handle, int type TSRMLS_DC);
static zend_op_array* vld_compile_file(zend_file_handle*, int TSRMLS_DC);

static zend_op_array* (*old_compile_string)(zval *source_string, char *filename TSRMLS_DC);
static zend_op_array* vld_compile_string(zval *source_string, char *filename TSRMLS_DC);

static void (*old_execute)(zend_op_array *op_array TSRMLS_DC);
static void vld_execute(zend_op_array *op_array TSRMLS_DC);


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
	"0.8.0",
#endif
	STANDARD_MODULE_PROPERTIES
};


#ifdef COMPILE_DL_VLD
ZEND_GET_MODULE(vld)
#endif

ZEND_DECLARE_MODULE_GLOBALS(vld)

PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("vld.active",       "0", PHP_INI_SYSTEM, OnUpdateBool, active,       zend_vld_globals, vld_globals)
    STD_PHP_INI_ENTRY("vld.skip_prepend", "0", PHP_INI_SYSTEM, OnUpdateBool, skip_prepend, zend_vld_globals, vld_globals)
    STD_PHP_INI_ENTRY("vld.skip_append",  "0", PHP_INI_SYSTEM, OnUpdateBool, skip_append,  zend_vld_globals, vld_globals)
    STD_PHP_INI_ENTRY("vld.execute",      "1", PHP_INI_SYSTEM, OnUpdateBool, execute,      zend_vld_globals, vld_globals)
    STD_PHP_INI_ENTRY("vld.verbosity",    "1", PHP_INI_SYSTEM, OnUpdateBool, verbosity,    zend_vld_globals, vld_globals)
PHP_INI_END()
 
static void vld_init_globals(zend_vld_globals *vld_globals)
{
	vld_globals->active       = 0;
	vld_globals->skip_prepend = 0;
	vld_globals->skip_append  = 0;
	vld_globals->execute      = 1;
}


PHP_MINIT_FUNCTION(vld)
{
	ZEND_INIT_MODULE_GLOBALS(vld, vld_init_globals, NULL);
	REGISTER_INI_ENTRIES();

	return SUCCESS;
}


PHP_MSHUTDOWN_FUNCTION(vld)
{
	UNREGISTER_INI_ENTRIES();

	zend_compile_file   = old_compile_file;
#if (PHP_MAJOR_VERSION > 5) || (PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION >= 2)
	zend_compile_string = old_compile_string;
#endif
	zend_execute        = old_execute;

	return SUCCESS;
}



PHP_RINIT_FUNCTION(vld)
{
	old_compile_file = zend_compile_file;
#if (PHP_MAJOR_VERSION > 5) || (PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION >= 2)
	old_compile_string = zend_compile_string;
#endif
	old_execute = zend_execute;

	if (VLD_G(active)) {
		zend_compile_file = vld_compile_file;
#if (PHP_MAJOR_VERSION > 5) || (PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION >= 2)
		zend_compile_string = vld_compile_string;
#endif
		if (!VLD_G(execute)) {
			zend_execute = vld_execute;
		}
	}
	return SUCCESS;
}



PHP_RSHUTDOWN_FUNCTION(vld)
{
	zend_compile_file = old_compile_file;
	zend_execute      = old_execute;

	return SUCCESS;
}


PHP_MINFO_FUNCTION(vld)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "vld support", "enabled");
	php_info_print_table_end();

	DISPLAY_INI_ENTRIES();

}

int vld_printf(FILE *stream, const char* fmt, ...)
{
	char *message;
	int len;
	va_list args;
	
	va_start(args, fmt);
	len = vspprintf(&message, 0, fmt, args);
	va_end(args);

	fprintf(stream, "%s", message);
	efree(message);
	
	return len;
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
		vld_printf(stderr, "Function " ZSTRFMT ":\n", ZSTRCP(fe->function_name));
		vld_dump_oparray(fe TSRMLS_CC);
		vld_printf(stderr, "End of function " ZSTRFMT ".\n\n", ZSTRCP(fe->function_name));
	}

	return ZEND_HASH_APPLY_KEEP;
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

	if (ce->type != ZEND_INTERNAL_CLASS) {	
		zend_hash_apply_with_argument(&ce->function_table, (apply_func_arg_t) vld_check_fe, (void *)&have_fe TSRMLS_CC);
		if (have_fe) {
			vld_printf(stderr, "Class " ZSTRFMT ":\n", ZSTRCP(ce->name));
			zend_hash_apply(&ce->function_table, (apply_func_t) vld_dump_fe TSRMLS_CC);
			vld_printf(stderr, "End of class " ZSTRFMT ".\n\n", ZSTRCP(ce->name));
		} else {
			vld_printf(stderr, "Class " ZSTRFMT ": [no user functions]\n", ZSTRCP(ce->name));
		}
	}

	return ZEND_HASH_APPLY_KEEP;
}

/* {{{ zend_op_array vld_compile_file (file_handle, type)
 *    This function provides a hook for compilation */
static zend_op_array *vld_compile_file(zend_file_handle *file_handle, int type TSRMLS_DC)
{
	zend_op_array *op_array;

	if (!VLD_G(execute) &&
		((VLD_G(skip_prepend) && PG(auto_prepend_file) && PG(auto_prepend_file)[0] && PG(auto_prepend_file) == file_handle->filename) ||
	     (VLD_G(skip_append)  && PG(auto_append_file)  && PG(auto_append_file)[0]  && PG(auto_append_file)  == file_handle->filename)))
	{
		zval nop;
		ZVAL_STRINGL(&nop, "RETURN ;", 8, 0);
		return compile_string(&nop, "NOP" TSRMLS_CC);;
	}

	op_array = old_compile_file (file_handle, type TSRMLS_CC);

	if (op_array) {
		vld_dump_oparray (op_array TSRMLS_CC);
	}

	zend_hash_apply (CG(function_table), (apply_func_t) vld_dump_fe TSRMLS_CC);
	zend_hash_apply (CG(class_table), (apply_func_t) vld_dump_cle TSRMLS_CC);

	return op_array;
}
/* }}} */

/* {{{ zend_op_array vld_compile_string (source_string, filename)
 *    This function provides a hook for compilation */
static zend_op_array *vld_compile_string(zval *source_string, char *filename TSRMLS_DC)
{
	zend_op_array *op_array;

	op_array = old_compile_string (source_string, filename TSRMLS_CC);

	if (op_array) {
		vld_dump_oparray (op_array TSRMLS_CC);
	}

	zend_hash_apply (CG(function_table), (apply_func_t) vld_dump_fe TSRMLS_CC);
	zend_hash_apply (CG(class_table), (apply_func_t) vld_dump_cle TSRMLS_CC);

	return op_array;
}
/* }}} */

/* {{{ void vld_execute(zend_op_array *op_array TSRMLS_DC)
 *    This function provides a hook for execution */
static void vld_execute(zend_op_array *op_array TSRMLS_DC)
{
	// nothing to do
}
/* }}} */
