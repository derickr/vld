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
   |                                                                      |
   +----------------------------------------------------------------------+
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_vle.h"
#include "srm_oparray.h"

#ifndef ZTS
#error VL-Encoder module is only useable in thread-safe mode
#endif

static int le_vle;
static ZEND_API zend_op_array* (*old_compile_file)(zend_file_handle* file_handle, int type TSRMLS_DC);
static ZEND_API zend_op_array* vle_compile_file(zend_file_handle*, int TSRMLS_DC);

function_entry vle_functions[] = {
	{NULL, NULL, NULL}
};


zend_module_entry vle_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"vle",
	vle_functions,
	PHP_MINIT(vle),
	PHP_MSHUTDOWN(vle),
	PHP_RINIT(vle),	
	PHP_RSHUTDOWN(vle),
	PHP_MINFO(vle),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1",
#endif
	STANDARD_MODULE_PROPERTIES
};


#ifdef COMPILE_DL_VLE
ZEND_GET_MODULE(vle)
#endif





PHP_MINIT_FUNCTION(vle)
{
	old_compile_file = zend_compile_file;
	zend_compile_file = vle_compile_file;

	return SUCCESS;
}


PHP_MSHUTDOWN_FUNCTION(vle)
{
	zend_compile_file = old_compile_file;

	return SUCCESS;
}



PHP_RINIT_FUNCTION(vle)
{
	return SUCCESS;
}



PHP_RSHUTDOWN_FUNCTION(vle)
{
	return SUCCESS;
}


PHP_MINFO_FUNCTION(vle)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "vle support", "enabled");
	php_info_print_table_end();

}


/* {{{ zend_op_array srm_compile_file (file_handle, type)
 *    This function provides a hook for the execution of bananas */
static ZEND_API zend_op_array *vle_compile_file(zend_file_handle *file_handle, int type TSRMLS_DC)
{
	zend_op_array * op_array;

	op_array = old_compile_file (file_handle, type TSRMLS_CC);

	srm_dump_oparray (op_array);

	return op_array;
}
/* }}} */
