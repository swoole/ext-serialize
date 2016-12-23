## swoole_serialize

## 要求

- PHP 7  (no zts)

## 安装

phpize=>./configure=>make install=>echo "extension=xx/swoole_serialize.so">php.ini


##技术特性:

- 追求的是极致的性能 是目前最快的序列化算法的2-3倍(参见bench.php)
- 适用于经常需要序列化反序列化的场景（例如我们需要把3000支股票的信息cache起来，每次请求都需要反序列化），或者是有local cache的场景。
- 支持常用的__sleep __wakeup __autoload等特性。

## 使用
```$str = swoole_serialize($arr);``` <br/>
```$arr = swoole_unserialize($str);```<br/>

or

```$str = swSerialize::pack($arr);```<br/>
```$arr = swSerialize::unpack($str);```<br/>

or

```$o = new swSerialize();```<br/>
```$str = $o->pack($arr);```<br/>
```$o->unpack($str);```<br/>

###### unpack args
```
class obj{
    
    
    public function __construct($a,$b){
        $this->a = $a; 
        $this->b = $b; 

  
  }

}

$o = new obj(1,2);
$ser = swSerialize::pack($o);
var_dump(swSerialize::unpack($ser,array(3,4)));

```

## contact us
- http://weibo.com/u/2661945152

## License

Apache License Version 2.0 see http://www.apache.org/licenses/LICENSE-2.0.html
