## swoole_serialize

## 要求

- PHP 7  (no zts)

## 安装

phpize=>./configure=>make install=>echo "extension=xx/swoole_serialize.so">php.ini


##技术特性:

- 序列化利用php7 的zval的紧凑的数据结构，直接将zval内存拷贝到buffer中，反序列化类似
- 追求的是极致的性能（是目前最快的msgpack的2倍）但是序列化的数据会非常大，是空间换时间的做法
- 适用于数据量比较少 但是经常需要序列化反序列化的场景（例如我们需要把3000支股票的信息cache起来，每次请求都需要反序列化），或者是有local cache的场景。


## 使用
$str = swoole_serialize($arr);
$arr = swoole_unserialize($str);

##todo
- 在不影响性能的情况下尽量压缩序列化大小
- 支持object和引用类型