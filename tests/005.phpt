--TEST--
Check for double serialisation
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

test('double: 123.456', 123.456);
?>
--EXPECT--
double: 123.456
float(123.456)
OK
