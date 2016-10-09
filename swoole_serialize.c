/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2016 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:   woshiguo35@sina.com                                          |
  +----------------------------------------------------------------------+
 */

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_swoole_serialize.h"

static int le_swoole_serialize;

static CPINLINE int swoole_string_new(size_t size, seriaString *str, uint32_t type)
{
    int total = ZEND_MM_ALIGNED_SIZE(_STR_HEADER_SIZE + size + 1);
    str->total = total;
    //escape the header for later
    str->offset = _STR_HEADER_SIZE;
    //zend string addr
    str->buffer = emalloc(total);
    bzero(str->buffer,total);
    if (!str->buffer)
    {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "malloc Error: %s [%d]", strerror(errno), errno);
    }
    //    first 4 bytes is type
    *(uint32_t*) (str->buffer + str->offset) = type;
  //  SERIA_SET_TYPE(str->buffer, type);
    str->offset += sizeof (uint32_t);
    return 0;
}

static CPINLINE void swoole_string_cpy(seriaString *str, void *mem, size_t len)
{
    int new_size = len + str->offset;
    if (str->total < new_size)
    {//extend it

        //double size
        new_size = ZEND_MM_ALIGNED_SIZE(new_size + new_size);
        str->buffer = erealloc2(str->buffer, new_size, str->offset);
        if (!str->buffer)
        {
            php_error_docref(NULL TSRMLS_CC, E_ERROR, "realloc Error: %s [%d]", strerror(errno), errno);
        }
        str->total = new_size;
    }

    memcpy(str->buffer + str->offset, mem, len);
    str->offset += len;
}

/*
 * array
 */

static void* swoole_unserialize_arr(void *buffer, zval *zvalue)
{
    //Initialize zend array
    int len = 0;
    ZVAL_NEW_ARR(zvalue);
    memcpy(Z_ARR_P(zvalue), buffer, sizeof (zend_array));
    buffer += sizeof (zend_array);

    //Initialize buckets
    zend_array *ht = Z_ARR_P(zvalue);
    len = HT_SIZE(ht);
    void *arData = emalloc(len);
    memcpy(arData, buffer, len);
    HT_SET_DATA_ADDR(ht, arData);
    buffer += len;
    ht->pDestructor = ZVAL_PTR_DTOR;
    GC_REFCOUNT(ht) = 1;


    int idx;
    Bucket *p;
    for (idx = 0; idx < ht->nNumUsed; idx++)
    {
        p = ht->arData + idx;
        if (Z_TYPE(p->val) == IS_UNDEF) continue;

        /* Initialize key */
        if (p->key)
        {
            zend_string *str = (zend_string*) buffer;
            p->key = zend_string_dup(str, 0);
            len = ZEND_MM_ALIGNED_SIZE(_STR_HEADER_SIZE + str->len + 1);
            buffer += len;
        }
        if (Z_TYPE(p->val) == IS_STRING)
        {
            zend_string *str = (zend_string*) buffer;
            p->val.value.str = zend_string_dup(str, 0);
            len = ZEND_MM_ALIGNED_SIZE(_STR_HEADER_SIZE + str->len + 1);
            buffer += len;
        }
        else if (Z_TYPE(p->val) == IS_ARRAY)
        {
            buffer = swoole_unserialize_arr(buffer, &p->val);
        }
    }
    return buffer;

}

static void swoole_serialize_arr(seriaString *buffer, zval *zvalue)
{
    zval *data;
    zend_string *key;
    zend_ulong index;
    int len = 0;

  /*  seriaArray seriaArr;
    seriaArr.nNextFreeElement = zvalue->value.arr->nNextFreeElement;
    seriaArr.nTableSize = zvalue->value.arr->nTableSize;
    seriaArr.nNextFreeElement = zvalue->value.arr->nNextFreeElement;
    */
	swoole_string_cpy(buffer, zvalue->value.arr, sizeof (zend_array));


    void *arData = HT_GET_DATA_ADDR(Z_ARR_P(zvalue));
    len = HT_SIZE(Z_ARR_P(zvalue));
    swoole_string_cpy(buffer, arData, len);

    ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(zvalue), index, key, data)
    {
        if (key)
        {
            len = ZEND_MM_ALIGNED_SIZE(_STR_HEADER_SIZE + key->len + 1);
            swoole_string_cpy(buffer, key, len);
        }

        if (Z_TYPE_P(data) == IS_STRING)
        {
            //int            len3 = sizeof (zend_string) + Z_STRLEN_P(data);
            len = ZEND_MM_ALIGNED_SIZE(_STR_HEADER_SIZE + Z_STRLEN_P(data) + 1);
            swoole_string_cpy(buffer, Z_STR_P(data), len);
        }
        else if (Z_TYPE_P(data) == IS_ARRAY)
        {
            swoole_serialize_arr(buffer, data);
        }

    }
    ZEND_HASH_FOREACH_END();
}

/*
 * string
 */
static CPINLINE void swoole_serialize_string(seriaString *buffer, zval *zvalue)
{
    swoole_string_cpy(buffer, Z_STRVAL_P(zvalue), Z_STRLEN_P(zvalue));
}

static CPINLINE zend_string* swoole_unserialize_string(void *buffer, size_t len)
{
    return zend_string_init(buffer, len, 0);
}

/*
 * raw
 */

static CPINLINE void swoole_unserialize_raw(void *buffer, zval *zvalue)
{
    memcpy(&zvalue->value, buffer, sizeof (zend_value));
}

static CPINLINE void swoole_serialize_raw(seriaString *buffer, zval *zvalue)
{
    swoole_string_cpy(buffer, &zvalue->value, sizeof (zend_value));
}

/*
 * dispatch
 */

static CPINLINE void swoole_seria_dispatch(seriaString *buffer, zval *zvalue)
{
    switch (Z_TYPE_P(zvalue))
    {
        case IS_UNDEF:
        case IS_NULL:
        case IS_TRUE:
        case IS_FALSE:
        case IS_LONG:
        case IS_DOUBLE:
            swoole_serialize_raw(buffer, zvalue);
            break;
        case IS_STRING:
            swoole_serialize_string(buffer, zvalue);
            break;
        case IS_ARRAY:
        {
            swoole_serialize_arr(buffer, zvalue);
        }
            break;
        case IS_OBJECT:
        {
            php_error_docref(NULL TSRMLS_CC, E_ERROR, "swoole serialize not support obj now");
        }
            break;
        default:
            php_error_docref(NULL TSRMLS_CC, E_ERROR, "swoole serialize not support this type ");
            break;
    }
}

PHP_FUNCTION(swoole_serialize)
{
    zval *zvalue;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "z", &zvalue) == FAILURE)
    {
        return;
    }
    seriaString str;
    swoole_string_new(SERIA_SIZE, &str, Z_TYPE_P(zvalue));
    swoole_seria_dispatch(&str, zvalue); //serialize into a string
    zend_string *z_str = (zend_string *) str.buffer;

    z_str->val[str.offset] = '\0';
    z_str->len = str.offset + 1;
    z_str->h = 0;
    GC_REFCOUNT(z_str) = 1;
    GC_TYPE(z_str) = IS_STRING;
    GC_FLAGS(z_str) = 0;
    GC_INFO(z_str) = 0;

    RETURN_STR(z_str);
}

PHP_FUNCTION(swoole_unserialize)
{
    char *buffer = NULL;
    size_t arg_len;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &buffer, &arg_len) == FAILURE)
    {
        return;
    }
    uint32_t type = SERIA_GET_TYPE(buffer);
    buffer += sizeof (uint32_t);
    switch (type)
    {
        case IS_UNDEF:
        case IS_NULL:
        case IS_TRUE:
        case IS_FALSE:
        case IS_LONG:
        case IS_DOUBLE:
            swoole_unserialize_raw(buffer, return_value);
            Z_TYPE_INFO_P(return_value) = type;
            return;
        case IS_STRING:
            arg_len -= sizeof (uint32_t);
            zend_string *str = swoole_unserialize_string(buffer, arg_len);
            RETURN_STR(str);
        case IS_ARRAY:
            swoole_unserialize_arr(buffer, return_value);
            break;
        case IS_OBJECT:
        {
            php_error_docref(NULL TSRMLS_CC, E_ERROR, "swoole serialize not support obj now");
        }
            break;
        default:
            php_error_docref(NULL TSRMLS_CC, E_ERROR, "swoole serialize not support this type ");
            break;
    }

}

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(swoole_serialize)
{
    /* If you have INI entries, uncomment these lines
    REGISTER_INI_ENTRIES();
     */
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(swoole_serialize)
{
    /* uncomment this line if you have INI entries
    UNREGISTER_INI_ENTRIES();
     */
    return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */

/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(swoole_serialize)
{
#if defined(COMPILE_DL_SWOOLE_SERIALIZE) && defined(ZTS)
    ZEND_TSRMLS_CACHE_UPDATE();
#endif
    return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */

/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(swoole_serialize)
{
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(swoole_serialize)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "swoole_serialize support", "enabled");
    php_info_print_table_row(2, "Author", "郭新华");
    php_info_print_table_row(2, "email", "woshiguo35@sina.com");
    php_info_print_table_end();

    /* Remove comments if you have entries in php.ini
    DISPLAY_INI_ENTRIES();
     */
}
/* }}} */

/* {{{ swoole_serialize_functions[]
 *
 * Every user visible function must have an entry in swoole_serialize_functions[].
 */
const zend_function_entry swoole_serialize_functions[] = {
    PHP_FE(swoole_serialize, NULL)
    PHP_FE(swoole_unserialize, NULL)
    PHP_FE_END /* Must be the last line in swoole_serialize_functions[] */
};
/* }}} */

/* {{{ swoole_serialize_module_entry
 */
zend_module_entry swoole_serialize_module_entry = {
    STANDARD_MODULE_HEADER,
    "swoole_serialize",
    swoole_serialize_functions,
    PHP_MINIT(swoole_serialize),
    PHP_MSHUTDOWN(swoole_serialize),
    PHP_RINIT(swoole_serialize), /* Replace with NULL if there's nothing to do at request start */
    PHP_RSHUTDOWN(swoole_serialize), /* Replace with NULL if there's nothing to do at request end */
    PHP_MINFO(swoole_serialize),
    PHP_SWOOLE_SERIALIZE_VERSION,
    STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_SWOOLE_SERIALIZE
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(swoole_serialize)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
