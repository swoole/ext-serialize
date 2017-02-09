--TEST--
Check for msgpack presence
--SKIPIF--
<?php if (!extension_loaded("swoole_serialize")) print "skip"; ?>
--FILE--
<?php 
echo "swoole_serialize extension is available";
?>
--EXPECT--
swoole_serialize extension is available
