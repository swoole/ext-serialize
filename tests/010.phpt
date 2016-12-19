--TEST--
Array test
--SKIPIF--
--FILE--
<?php
if(!extension_loaded('swoole_serialize')) {
    dl('swoole_serialize.' . PHP_SHLIB_SUFFIX);
}

function test($type, $variable, $test) {
    $serialized = swoole_serialize($variable);
    $unserialized = swoole_unserialize($serialized);

    echo $type, PHP_EOL;
    var_dump($unserialized);
    echo $test || $unserialized == $variable ? 'OK' : 'ERROR', PHP_EOL;
}

$a = array(
    'a' => array(
        'b' => 'c',
        'd' => 'e'
    ),
    'f' => array(
        'g' => 'h'
    )
);

test('array', $a, false);
?>
--EXPECT--
array
array(2) {
  ["a"]=>
  array(2) {
    ["b"]=>
    string(1) "c"
    ["d"]=>
    string(1) "e"
  }
  ["f"]=>
  array(1) {
    ["g"]=>
    string(1) "h"
  }
}
OK
