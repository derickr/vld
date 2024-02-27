dnl $Id: config.m4,v 1.2 2002-10-23 18:55:10 derick Exp $
dnl config.m4 for extension vld

PHP_ARG_ENABLE(vld, whether to enable VLD support,
[  --enable-vld           Enable vld support])

PHP_ARG_ENABLE(vld-dev, whether to enable VLD developer build flags,
[  --enable-vld-dev          VLD: Enable developer flags],, no)

if test "$PHP_VLD" != "no"; then
  AC_MSG_CHECKING([Check for supported PHP versions])
  PHP_VLD_FOUND_VERSION=`${PHP_CONFIG} --version`
  PHP_VLD_FOUND_VERNUM=`echo "${PHP_VLD_FOUND_VERSION}" | $AWK 'BEGIN { FS = "."; } { printf "%d", ([$]1 * 100 + [$]2) * 100 + [$]3;}'`
  if test "$PHP_VLD_FOUND_VERNUM" -lt "70000"; then
    AC_MSG_ERROR([not supported. Need a PHP version >= 7.0.0 (found $PHP_VLD_FOUND_VERSION)])
  else
    AC_MSG_RESULT([supported ($PHP_VLD_FOUND_VERSION)])
  fi

  CPPFLAGS=$old_CPPFLAGS

  if test "$PHP_VLD_DEV" = "yes"; then
    AX_CHECK_COMPILE_FLAG([-Wbool-conversion],                [_MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wbool-conversion"])
    AX_CHECK_COMPILE_FLAG([-Wdeclaration-after-statement],    [_MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wdeclaration-after-statement"])
    AX_CHECK_COMPILE_FLAG([-Wdiscarded-qualifiers],           [_MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wdiscarded-qualifiers"])
    AX_CHECK_COMPILE_FLAG([-Wduplicate-enum],                 [_MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wduplicate-enum"])
    AX_CHECK_COMPILE_FLAG([-Wempty-body],                     [_MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wempty-body"])
    AX_CHECK_COMPILE_FLAG([-Wenum-compare],                   [_MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wenum-compare"])
    AX_CHECK_COMPILE_FLAG([-Werror],                          [_MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Werror"])
    AX_CHECK_COMPILE_FLAG([-Wextra],                          [_MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wextra"])
    AX_CHECK_COMPILE_FLAG([-Wformat-nonliteral],              [_MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wformat-nonliteral"])
    AX_CHECK_COMPILE_FLAG([-Wformat-security],                [_MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wformat-security"])
    AX_CHECK_COMPILE_FLAG([-Wheader-guard],                   [_MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wheader-guard"])
    AX_CHECK_COMPILE_FLAG([-Wincompatible-pointer-types-discards-qualifiers], [_MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wincompatible-pointer-types-discards-qualifiers"])
    AX_CHECK_COMPILE_FLAG([-Wimplicit-fallthrough],           [_MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wimplicit-fallthrough"])
    AX_CHECK_COMPILE_FLAG([-Winit-self],                      [_MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Winit-self"])
    AX_CHECK_COMPILE_FLAG([-Wlogical-not-parentheses],        [_MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wlogical-not-parentheses"])
    AX_CHECK_COMPILE_FLAG([-Wlogical-op],                     [_MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wlogical-op"])
    AX_CHECK_COMPILE_FLAG([-Wlogical-op-parentheses],         [_MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wlogical-op-parentheses"])
    AX_CHECK_COMPILE_FLAG([-Wloop-analysis],                  [_MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wloop-analysis"])
    AX_CHECK_COMPILE_FLAG([-Wmaybe-uninitialized],            [_MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wmaybe-uninitialized"])
    AX_CHECK_COMPILE_FLAG([-Wmissing-format-attribute],       [_MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wmissing-format-attribute"])
    AX_CHECK_COMPILE_FLAG([-Wno-missing-field-initializers],  [_MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wno-missing-field-initializers"])
    AX_CHECK_COMPILE_FLAG([-Wno-sign-compare],                [_MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wno-sign-compare"])
    AX_CHECK_COMPILE_FLAG([-Wno-unused-but-set-variable],     [_MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wno-unused-but-set-variable"])
    AX_CHECK_COMPILE_FLAG([-Wno-unused-parameter],            [_MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wno-unused-parameter"])
    AX_CHECK_COMPILE_FLAG([-Wno-variadic-macros],             [_MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wno-variadic-macros"])
    AX_CHECK_COMPILE_FLAG([-Wparentheses],                    [_MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wparentheses"])
    AX_CHECK_COMPILE_FLAG([-Wpointer-bool-conversion],        [_MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wpointer-bool-conversion"])
    AX_CHECK_COMPILE_FLAG([-Wsizeof-array-argument],          [_MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wsizeof-array-argument"])
    AX_CHECK_COMPILE_FLAG([-Wstring-conversion],              [_MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wstring-conversion"])
    AX_CHECK_COMPILE_FLAG([-Wwrite-strings],                  [_MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wwrite-strings"])
    AX_CHECK_COMPILE_FLAG([-fdiagnostics-show-option],        [_MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -fdiagnostics-show-option"])
    AX_CHECK_COMPILE_FLAG([-fno-exceptions],                  [_MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -fno-exceptions"])
    AX_CHECK_COMPILE_FLAG([-fno-omit-frame-pointer],          [_MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -fno-omit-frame-pointer"])
    AX_CHECK_COMPILE_FLAG([-fno-optimize-sibling-calls],      [_MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -fno-optimize-sibling-calls"])
    AX_CHECK_COMPILE_FLAG([-fsanitize-address],               [_MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -fsanitize-address"])
    AX_CHECK_COMPILE_FLAG([-fstack-protector],                [_MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -fstack-protector"])

    MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS"
    STD_CFLAGS="-g -O0 -Wall"
  fi

  PHP_VLD_CFLAGS="$STD_CFLAGS $MAINTAINER_CFLAGS"
  PHP_ADD_MAKEFILE_FRAGMENT($abs_srcdir/Makefile.frag, $abs_srcdir)
  PHP_NEW_EXTENSION(vld, vld.c srm_oparray.c set.c branchinfo.c, $ext_shared,,$PHP_VLD_CFLAGS)
fi
