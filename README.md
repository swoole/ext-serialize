## swoole_serialize

## 要求

- PHP 7  (no zts)

## 安装

phpize=>./configure=>make install=>echo "extension=xx/swoole_serialize.so">>php.ini


##技术特性:

- php7下最快的序列化/反序列化方法。
- 支持pack和fastPack两种方法。pack方法在保证性能的同时尽量压缩序列化后大小，fastPack比pack更快，但是尺寸稍大。(参见bench.php).
- 支持常用的__sleep __wakeup __autoload等特性。

## 使用

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
