## swoole_serialize

## require

- PHP 7+

## install

```bash
phpize
./configure
make
make install
echo "extension=/path/to/swoole_serialize.so" >> php.ini
```

## features

- the fastest serialize function for php7+ (see bench.php,or you can bench it use you data,trust me it is cool!).
- support __sleep __wakeup __autoload etc.

## use

```php
$str = swoole_serialize::pack($arr);
$arr = swoole_serialize::unpack($str);

```




## contact us
- http://weibo.com/u/2661945152

## License

Apache License Version 2.0 see http://www.apache.org/licenses/LICENSE-2.0.html


