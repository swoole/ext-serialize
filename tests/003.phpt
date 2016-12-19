--TEST--
Check for bool serialisation
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

test('bool true',  true);
test('bool false', false);
?>
--EXPECT--
bool true
bool(true)
OK
bool false
bool(false)
OK
