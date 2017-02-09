## swoole_serialize

## require

- PHP 7+

## install
pecl install swoole_serialize <br/>

or

phpize=>./configure=>make install=>echo "extension=xx/swoole_serialize.so">>php.ini


##features:

- the fastest serialize function for php7（see bench.php,or you can bench it use you data,trust me it is cool!）。
- support pack and fastPack two function.
- support __sleep __wakeup __autoload etc。

## use

$str = swoole_pack($arr);<br/>
$arr = swoole_unpack($str);<br/>
<br/>

$str = swoole_fast_pack($arr);<br/>
$arr = swoole_unpack($str);<br/>

or

$str = swSerialize::pack($arr);<br/>
$arr = swSerialize::unpack($str);<br/>
<br/>

$str = swSerialize::fastPack($arr);<br/>
$arr = swSerialize::unpack($str);<br/>

or

$o = new swSerialize();<br/>
$str = $o->pack($arr);<br/>
$o->unpack($str);<br/>

<br/>

$o = new swSerialize();<br/>
$str = $o->fastPack($arr);<br/>
$o->unpack($str);<br/>


## contact us
- http://weibo.com/u/2661945152

## License

Apache License Version 2.0 see http://www.apache.org/licenses/LICENSE-2.0.html
