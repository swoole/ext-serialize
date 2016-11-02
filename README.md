## swoole_serialize

## 要求

- PHP 7  (no zts)

## 安装

phpize=>./configure=>make install=>echo "extension=xx/swoole_serialize.so">php.ini


##技术特性:

- 追求的是极致的性能 是目前最快的序列化算法的2-3倍(参见bench.php)
- 适用于经常需要序列化反序列化的场景（例如我们需要把3000支股票的信息cache起来，每次请求都需要反序列化），或者是有local cache的场景。


## 使用
$str = swoole_serialize($arr);
$arr = swoole_unserialize($str);

## contact us
- http://weibo.com/u/2661945152

## License

Apache License Version 2.0 see http://www.apache.org/licenses/LICENSE-2.0.html

## todo
检测循环引用
完善对象的支持