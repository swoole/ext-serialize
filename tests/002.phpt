--TEST--
Check for null serialisation
--SKIPIF--
--FILE--
<?php
if(!extension_loaded('swoole_serialize')) {
    dl('swoole_serialize.' . PHP_SHLIB_SUFFIX);
}

function test($type, $variable) {
    $serialized = swoole_serialize($variable);
    $unserialized = swoole_unserialize($serialized);

    echo $type, PHP_EOL;
    var_dump($unserialized);
    echo $unserialized === $variable ? 'OK' : 'ERROR', PHP_EOL;
}

test('null', null);
?>
--EXPECT--
null
NULL
OK
