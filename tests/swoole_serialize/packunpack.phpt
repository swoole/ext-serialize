--TEST--
swoole_serialize: pack & unpack
--SKIPIF--
<?php
//require __DIR__ . '/../include/skipif.inc';
//skip_if_class_not_exist('swoole_serialize');
?>
--FILE--
<?php
//require __DIR__ . '/../include/bootstrap.php';

// int
$int_data = mt_rand(100, 999);
$data = swoole_serialize::pack($int_data);
$un_data = swoole_serialize::unpack($data);
var_dump($int_data==$un_data);

// long
$long_data = mt_rand(100000000000, 999999999999);
$data = swoole_serialize::pack($long_data);
$un_data = swoole_serialize::unpack($data);
var_dump($long_data==$un_data);

// string
$str_data = str_repeat('bcy', 10);
$data = swoole_serialize::pack($str_data);
$un_data = swoole_serialize::unpack($data);
var_dump($str_data== $un_data);

// array
$arr_data = array_pad([], 32, '0123456789abcdefghijklmnopqrstuvwxyz');
$data = swoole_serialize::pack($arr_data);
$un_data = swoole_serialize::unpack($data);
var_dump($arr_data== $un_data);

// large array
$large_arr_data = array_pad([], 4096, '0123456789abcdefghijklmnopqrstuvwxyz');
$data = swoole_serialize::pack($large_arr_data);
$un_data = swoole_serialize::unpack($data);
var_dump($large_arr_data== $un_data);


// pack array
$aaaa = [11,3,12,2,3,5,6];
$a = swoole_serialize::pack($aaaa);
$c = swoole_serialize::unpack($a);
var_dump(3== $c[1]);


//bad pack array
$aaaa =  [1=>11,2=>3];
$a = swoole_serialize::pack($aaaa);
$c = swoole_serialize::unpack($a);
var_dump(3== $c[2]);


// error array data
$data_out = substr($data, 0, 8192);
$err_data = @swoole_serialize::unpack($data_out);
var_dump($err_data);
?>
DONE
--EXPECTF--
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(false)
DONE
