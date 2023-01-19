## 模板编程基础
[基础学习](https://blog.csdn.net/baidu_41388533/category_9978863.html)
[EBO与私有继承](https://www.jb51.net/article/259368.htm)
- EBO （Empty Base Optimization) 
- type_traite
- fixed traits
- decay
- SFINAE
- CRTP
- Mixins

### 1、`enable_if`的作用
- 1.1 在某些场景下，我们需要实现只有特定类型可以调用的模板函数。
```cpp
//假设一个函数只在接收unsigned的参数的时候才生效.
template<typename T>
typename std::enable_if<std::is_unsigned<T>::value, std::string>::type
foo(T t)
{
    std::string str("Unsigned var");
    return str;
}

int a = 200;
unsigned int b = 300;
foo(a);         //编译失败没有合适的函数.
foo(b);         //编译成功，输出Unsigned var.

//假设一个函数只接收整型参数
template<typename T,std::enable_if<std::is_integral<T>::value,int>::type = 0>
void example(T t)
{
    std::cout << "is_integral" << std::endl;
}

int n = 200;
int m = 3999;
std::string sg("lplpd");

example(n);     //ok，is_integral
example(m);     //ok，is_integral
example(sg);    //error，无法编译通过

```
- 1.2 在使用模板编程时，可以利用std::enable_if的特性根据模板参数的不同特性进行不同的类型选择。
```cpp
//使用模板判断一个指针是否是智能指针
template<typename T>
struct is_smart_pointer_helper: public std::false_type;

template<typename T>
struct is_smart_pointer_helper<std::shared_ptr<T>>:
    public std::true_type;

template<typename T>
struct is_smart_pointer_helper<std::weak_ptr<T>>:
    public std::true_type;

template<typename T>
struct is_smart_pointer_helper<std::unique_ptr<T>>:
    public std::true_type;

template<typename T>
typename std::enable_if<is_smart_pointer_helper<T>::value, std::string>::type
check_smart_pointer(const T& t)
{
    std::string res("is smart pointer!");
    std::cout << res.c_str() << std::endl;
    return res;
}

template<typename T>
typename std::enable_if<!is_smart_pointer_helper<T>::value, std::string>::type
check_smart_pointer(const T& t)
{
    std::string res("is not smart pointer!");
    std::cout << res.c_str() << std::endl;
    return res;    
}

void test_smart_pointer_type()
{
    auto sp = std::make_shared<int>(2000);
    int *raw_p = new int(233);
    check_smart_pointer(sp);
    check_smart_pointer(raw_p);
    delete p;
}
```

