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
/* $Id: php_vld.h,v 1.9 2009-03-30 18:22:15 derick Exp $ */

#ifndef PHP_VLD_H
#define PHP_VLD_H

#include "php.h"

extern zend_module_entry vld_module_entry;
#define phpext_vld_ptr &vld_module_entry

#ifdef PHP_WIN32
#define PHP_VLD_API __declspec(dllexport)
#else
#define PHP_VLD_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(vld);
PHP_MSHUTDOWN_FUNCTION(vld);
PHP_RINIT_FUNCTION(vld);
PHP_RSHUTDOWN_FUNCTION(vld);
PHP_MINFO_FUNCTION(vld);

ZEND_BEGIN_MODULE_GLOBALS(vld)
	int active;
	int skip_prepend;
	int skip_append;
	int execute;
	int verbosity;
	int format;
	char *col_sep;
	int save_paths;
	char *save_dir;
	FILE *path_dump_file;
	int dump_paths;
ZEND_END_MODULE_GLOBALS(vld) 

int vld_printf(FILE *stream, const char* fmt, ...);

#ifdef ZTS
#define VLD_G(v) TSRMG(vld_globals_id, zend_vld_globals *, v)
#else
#define VLD_G(v) (vld_globals.v)
#endif
#define VLD_PRINT(v,args) if (VLD_G(verbosity) >= (v)) { vld_printf(stderr, args); }
#define VLD_PRINT1(v,args,x) if (VLD_G(verbosity) >= (v)) { vld_printf(stderr, args, (x)); }
#define VLD_PRINT2(v,args,x,y) if (VLD_G(verbosity) >= (v)) { vld_printf(stderr, args, (x), (y)); }

#if PHP_VERSION_ID >= 60000
#define ZSTRCP(str)  ((str).v)
#define ZSTRKEY(key) ((key).s)
#define ZSTRFMT      "%v"
#else
#define ZSTRCP(str)  (str)
#define ZSTRKEY(key) (key)
#define ZSTRFMT      "%s"
#endif

#endif


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * indent-tabs-mode: t
 * End:
 */
