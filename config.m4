dnl $Id: config.m4,v 1.1.1.1 2002-01-02 11:35:33 derick Exp $
dnl config.m4 for extension vle

PHP_ARG_ENABLE(vle, whether to enable vle support,
Make sure that the comment is aligned:
[  --enable-vle           Enable vle support])

if test "$PHP_VLE" != "no"; then
  PHP_EXTENSION(vle, $ext_shared)
fi
