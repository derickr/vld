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

#ifndef PHP_VLE_H
#define PHP_VLE_H

#include "php.h"

extern zend_module_entry vle_module_entry;
#define phpext_vle_ptr &vle_module_entry

#ifdef PHP_WIN32
#define PHP_VLE_API __declspec(dllexport)
#else
#define PHP_VLE_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(vle);
PHP_MSHUTDOWN_FUNCTION(vle);
PHP_RINIT_FUNCTION(vle);
PHP_RSHUTDOWN_FUNCTION(vle);
PHP_MINFO_FUNCTION(vle);




#ifdef ZTS
#define VLE_G(v) TSRMG(vle_globals_id, zend_vle_globals *, v)
#else
#define VLE_G(v) (vle_globals.v)
#endif

#endif


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * indent-tabs-mode: t
 * End:
 */
