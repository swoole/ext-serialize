dnl $Id$
dnl config.m4 for extension swoole_serialize

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(swoole_serialize, for swoole_serialize support,
dnl Make sure that the comment is aligned:
dnl [  --with-swoole_serialize             Include swoole_serialize support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(swoole_serialize, whether to enable swoole_serialize support,
Make sure that the comment is aligned:
[  --enable-swoole_serialize           Enable swoole_serialize support])

if test "$PHP_SWOOLE_SERIALIZE" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-swoole_serialize -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/swoole_serialize.h"  # you most likely want to change this
  dnl if test -r $PHP_SWOOLE_SERIALIZE/$SEARCH_FOR; then # path given as parameter
  dnl   SWOOLE_SERIALIZE_DIR=$PHP_SWOOLE_SERIALIZE
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for swoole_serialize files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       SWOOLE_SERIALIZE_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$SWOOLE_SERIALIZE_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the swoole_serialize distribution])
  dnl fi

  dnl # --with-swoole_serialize -> add include path
  dnl PHP_ADD_INCLUDE($SWOOLE_SERIALIZE_DIR/include)

  dnl # --with-swoole_serialize -> check for lib and symbol presence
  dnl LIBNAME=swoole_serialize # you may want to change this
  dnl LIBSYMBOL=swoole_serialize # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $SWOOLE_SERIALIZE_DIR/$PHP_LIBDIR, SWOOLE_SERIALIZE_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_SWOOLE_SERIALIZELIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong swoole_serialize lib version or lib not found])
  dnl ],[
  dnl   -L$SWOOLE_SERIALIZE_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(SWOOLE_SERIALIZE_SHARED_LIBADD)

  PHP_NEW_EXTENSION(swoole_serialize, swoole_serialize.c , $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
