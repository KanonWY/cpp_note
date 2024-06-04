## SFINAE

### 1、SFINAE
在函数模板的重载决议中会应用此规则：当模板形参在替换成显式指定的类型或推导出的类型失败时，从重载集中丢弃这个特化，而非导致编译失败。

example1: 写一个add 模板函数，要传入的对象支持 operator+.
```c++
// 使用 SFINAE
template<typename T>
auto sf_add(const T& t1, const T& t2) -> decltype(t1 + t2){   // C++11 后置返回类型，在返回类型中运用 SFINAE
    std::puts("SFINAE +");
    return t1 + t2;
}

// 实例化阶段报错
template<typename T>
auto ins_add(const T& t1, const T& t2) {
    return t1 + t2;
}
```

- 这样有啥好处吗？使用了 SFINAE 看起来还变复杂了。我就算不用这写法，如果对象没有 operator+ 不是一样会编译错误吗？

- 虽然前面说了 SFINAE 可以影响重载决议，我知道这个很有用，但是我这个函数根本没有别的重载，这样写还是有必要的吗？

原因：
(1)如果就是简单写一个 add 函数模板不使用 SFINAE，那么编译器在编译的时候，会尝试模板实例化，生成函数定义，发现你这类型根本没有 operator+，于是实例化模板错误。  

(2)如果按照我们上面的写法使用 SFINAE，根据“代换失败不是错误”的规则，从重载集中丢弃这个特化 add，然而又没有其他的 add 重载，所以这里的错误是“未找到匹配的重载函数”。  
```bash
// 不同的报错:
sfinae.cpp:12:14: error: invalid operands to binary expression ('const char[2]' and 'const char[2]')
    return a + b;
           ~ ^ ~
sfinae.cpp:17:5: note: in instantiation of function template specialization 'add<char[2]>' requested here
    add("x", "y");
    ^
sfinae.cpp:18:5: error: no matching function for call to 'm_add'
    m_add("a", "b");
    ^~~~~
sfinae.cpp:4:6: note: candidate template ignored: substitution failure [with T = char[2]]: invalid operands to binary expression ('const char[2]' and 'const char[2]')
auto m_add(const T& a, const T& b) -> decltype(a + b)
     ^
```
从上面可以看出，没有使用`decltype(a + b)`来 SFINAE 的会在实例化节点发现`operator + `操作符无效。  
是模板实例化，能不要实例化就不要实例化，我们当前的示例只是因为 add 函数模板非常的简单，即使实例化错误，编译器依然可以很轻松的报错告诉你，是因为没有 operator+。但是很多模板是非常复杂的，编译器实例化模板经常会产生一些完全不可读的报错；如果我们使用 SFINAE，编译器就是直接告诉我：“未找到匹配的重载函数”，我们自然知道就是传入的参数没有满足要求。而且实例化模板也是有开销的，很多时候甚至很大。


### 2、常用的 SFINAE 相关使用
#### 2.1 enable_if
使用 enable_if 帮助筛选重载函数
```c++
template<bool B, class T = void>
struct enable_if {};
 
template<class T> // 类模板偏特化
struct enable_if<true, T> { using type =  T; };     // 只有 B 为 true，才有 type，即 ::type 才合法

template< bool B, class T = void >
using enable_if_t = typename enable_if<B,T>::type; // C++14 引入
```

```c++
// T 类型为 int 才可以生效
template<typename T,typename SFINAE = 
    std::enable_if_t<std::is_same_v<T,int>>>
void f(T){}
```
std::enable_if_t<std::is_same_v<T,int>>> 如果 T 不是 int，那么 std::is_same_v 就会返回一个 false，也就是说 std::enable_if_t<false> ，带入：
```c++
using enable_if_t = typename enable_if<false,void>::type; // void 是默认模板实参
// false 没有 type，因此不会选择该函数重载
```
#### 2.3 std::void_t
用不帮助简化 enable_if_t.
```c++
template< class... >
using void_t = void;
```
- 将任意类型的序列映射到类型`void`的工具元函数
- 模板元编程中，用此元函数检测`SFINAE`语境中的非良构类型  
example:
```c++

#include <iostream>
#include <type_traist>

template<typename T,
        typename SFINAE = std::void_t<
        decltype(T{} + T{}), typename T::type, decltype(&T::value), decltype(&T::f)
        >>
auto add(const T1& t1, const T2& t2)
{
    return t1 + t2;
}
```

#### 2.4 std::declval
将任意类型 T 转换成引用类型，使得在 decltype 说明符的操作数中不必经过构造函数就能使用成员函数。
- std::declval 只能用于 不求值语境，且不要求有定义
- 它不能被实际调用，因此不会返回值，返回类型是 T&&

example:
```c++
template<typename T, typename SFINAE = std::void_t< 
          decltype(T{} + T{})
        >>
auto add(const T& x, const T& y)
{
    return x + y;
}

// 存在默认构造函数
struct X{
    int operator+(const X&)const{
        return 0;
    }
};

// 不存在默认构造函数
struct X2 {
    X2(int){}   // 有参构造，没有默认构造函数
    int operator+(const X2&)const {
        return 0;
    }
};

int main(){
    X x1, x2;
    add(x1, x2);          // OK

    X2 x3{ 0 }, x4{ 0 };
    add(x3,x4);           // 未找到匹配的重载函数
}
```
错误的原因很简单，decltype(T{} + T{}) 这个表达式中，同时要求了 T 类型支持默认构造（虽然这不是我们的本意），然而我们的 X2 类型没有默认构造，自然而然 T{} 不是合法表达式，代换失败。其实我们之前也有类似的写法，我们在本节进行纠正，使用 std::declval。
```c++

template<typename T, typename SFINAE = std::void_t<
          decltype(declval<T>() + declval<T>())
>>
auto add(const T& t1, const T& t2)
{
    return t1 + t2;
}
```
把 `T{}` 改成 `std::declval<T>()` 即可，`decltype`是不求值语境，没有问题。  
当然也可以使用 `std::declval<T>()` 来判断 `T` 的某一个成员函数是否存在。

```c++
template<typename T, typename SFINAE = decltype(std::declval<T>().f(1))>
auto add(const T& a, const T& b)
{
    std::cout << "int" << std::endl;
    return a + b;
}

template<typename T, typename SFINAE = decltype(std::declval<T>().f(2.2))>
auto add(const T& a, const T& b)
{
    std::cout << "float" << std::endl;
    return a + b;
}

```

### 3、类型特征
使用类模板偏特化实现类型特征
```c++
template<typename T>
struct CanWrite {
    constexpr static bool value = false;
};

template<>
struct CanWrite<Widget>
{
    constexpr static bool value = true;
};

template<typename T>
inline constexpr bool can_write_t = CanWrite<T>::value;
```




