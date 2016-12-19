--TEST--
Object test, __autoload
--SKIPIF--
--FILE--
<?php
if(!extension_loaded('swoole_serialize')) {
    dl('swoole_serialize.' . PHP_SHLIB_SUFFIX);
}
//  class Obj {
//        var $a;
//        var $b;
//
//        function __construct($a, $b) {
//            $this->a = $a;
//            $this->b = $b;
//        }
//    };
//    $f = new Obj(1,2);
//    $unserialized = swoole_serialize($f);
//    file_put_contents("/tmp/test", $unserialized);
    
function test($type, $test) {
    $serialized = file_get_contents("/tmp/test");
    $unserialized = swoole_unserialize($serialized);

    echo $type, PHP_EOL;
     
    var_dump($unserialized);
    echo $test || $unserialized->b == 2 ? 'OK' : 'ERROR', PHP_EOL;
}

function __autoload($classname) {
    class Obj {
        var $a;
        var $b;

        function __construct($a, $b) {
            $this->a = $a;
            $this->b = $b;
        }
    }
}

test('autoload', false);
?>
--EXPECTF--
autoload
object(Obj)#%d (2) {
  ["a"]=>
  int(1)
  ["b"]=>
  int(2)
}
OK
