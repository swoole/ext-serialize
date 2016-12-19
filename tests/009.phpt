--TEST--
Check for reference serialization
--SKIPIF--
<?php
if ((version_compare(PHP_VERSION, '5.2.13') <= 0) ||
    (version_compare(PHP_VERSION, '5.3.0') >= 0 &&
     version_compare(PHP_VERSION, '5.3.2') <= 0)) {
    echo "skip tests in PHP 5.2.14/5.3.3 or newer";
}
--FILE--
<?php
ini_set("display_errors", "Off");
if(!extension_loaded('swoole_serialize')) {
    dl('swoole_serialize.' . PHP_SHLIB_SUFFIX);
}

function test($type, $variable, $test) {
    $serialized = @swoole_serialize($variable);
    $unserialized = swoole_unserialize($serialized);

    echo $type, PHP_EOL;
    var_dump($unserialized);
    echo $test || $unserialized == $variable ? 'OK' : 'ERROR', PHP_EOL;
}

$a = array('foo');

test('array($a, $a)', array($a, $a), false);
test('array(&$a, &$a)', array(&$a, &$a), false);

$a = array(null);
$b = array(&$a);
$a[0] = &$b;

test('cyclic', $a, true);

--EXPECT--
array($a, $a)
array(2) {
  [0]=>
  array(1) {
    [0]=>
    string(3) "foo"
  }
  [1]=>
  array(1) {
    [0]=>
    string(3) "foo"
  }
}
OK
array(&$a, &$a)
array(2) {
  [0]=>
  array(1) {
    [0]=>
    string(3) "foo"
  }
  [1]=>
  array(1) {
    [0]=>
    string(3) "foo"
  }
}
OK
cyclic
array(1) {
  [0]=>
  array(1) {
    [0]=>
    array(1) {
      [0]=>
      array(1) {
        [0]=>
        array(1) {
          [0]=>
          array(0) {
          }
        }
      }
    }
  }
}
OK