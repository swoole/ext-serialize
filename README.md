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
- support pack and fastPack two function.
- support __sleep __wakeup __autoload etc.

## use
```php
$str = swoole_pack($arr);
$arr = swoole_unpack($str);


$str = swoole_fast_pack($arr);
$arr = swoole_unpack($str);
```

or

```php
$str = swSerialize::pack($arr);
$arr = swSerialize::unpack($str);


$str = swSerialize::fastPack($arr);
$arr = swSerialize::unpack($str);
```

or

```php
$o = new swSerialize();
$str = $o->pack($arr);
$o->unpack($str);


$o = new swSerialize();
$str = $o->fastPack($arr);
$o->unpack($str);
```


## contact us
- http://weibo.com/u/2661945152

## License

Apache License Version 2.0 see http://www.apache.org/licenses/LICENSE-2.0.html


