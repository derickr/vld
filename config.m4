dnl $Id: config.m4,v 1.2 2002-10-23 18:55:10 derick Exp $
dnl config.m4 for extension vld

PHP_ARG_ENABLE(vld, whether to enable vld support,
[  --enable-vld           Enable vld support])

if test "$PHP_VLD" != "no"; then
  PHP_EXTENSION(vld, $ext_shared)
fi
