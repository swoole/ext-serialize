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
    PHP_ADD_INCLUDE([$phpincludedir/ext/swoole])
    PHP_ADD_INCLUDE([$phpincludedir/ext/swoole/include])
    
    PHP_ADD_EXTENSION_DEP(swoole_serialize, swoole)

    PHP_NEW_EXTENSION(swoole_serialize, swoole_serialize.c , $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
