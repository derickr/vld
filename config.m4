dnl $Id: config.m4,v 1.2 2002-10-23 18:55:10 derick Exp $
dnl config.m4 for extension vld

PHP_ARG_ENABLE(vld, whether to enable vld support,
[  --enable-vld           Enable vld support])

if test "$PHP_VLD" != "no"; then
  AC_MSG_CHECKING([Check for supported PHP versions])
  PHP_XDEBUG_FOUND_VERSION=`${PHP_CONFIG} --version`
  PHP_XDEBUG_FOUND_VERNUM=`echo "${PHP_XDEBUG_FOUND_VERSION}" | $AWK 'BEGIN { FS = "."; } { printf "%d", ([$]1 * 100 + [$]2) * 100 + [$]3;}'`
  if test "$PHP_XDEBUG_FOUND_VERNUM" -lt "70000"; then
    AC_MSG_ERROR([not supported. Need a PHP version >= 7.0.0 (found $PHP_XDEBUG_FOUND_VERSION)])
  else
    AC_MSG_RESULT([supported ($PHP_XDEBUG_FOUND_VERSION)])
  fi
  PHP_EXTENSION(vld, $ext_shared)
fi
