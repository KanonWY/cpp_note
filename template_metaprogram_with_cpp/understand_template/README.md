## understand template



### 1、类模版函数的形式
常见的类模板函数的形式有三种：
- 非模板类成员函数模板
- 类模板的成员函数
- 类模板的成员函数模板

```c++
// 类模板的成员函数
template<typename T>
class Composition {
public:
    T add(T const a, T const b)
    {
        return a + b;
    }
};

Composition<int> c;
c.add(32,39);
```

```c++
// 类的成员函数模板
class Composition {
public:
    template<typename T>
    T add(T const a, T const b)
    {
        return a + b;
    }
};

Composition a;
a.add(2,3);
```

```c++
// 类模板的成员函数模板
template <typename T>
class Wrapper {
public:
  warpper(T const v)
    :value(v)
  {}

  T const& get() const { return value; }

  template<typename U>
  U as() const {
    return static_cast<U>(value);
  }
  
priavte:
  T value;
};

```

### 2、模板参数
模板参数可以是类型，也可以是值，可以存在默认值，也可以存在多个支持变参模板参数.
- 类型模板参数
- 非类型模板参数
- 双重模板参数
- 默认模板参数

#### 2.1 类型模板参数
这些参数表示模板实例化期间作为参数提供的类型，由 typename 或 class 关键字引 入。使用这两个关键字没什么区别。类型模板参数可以有一个默认值，即类型。这与为函数形参指 定默认值的方式相同:
```c++
template<typename T>
class Wrapper;

// 默认值为 int
template<typename T = int>
class Wrapper;

// 可变参数
template<typename... Ts>
class Wrapper;

// 概念约束
template<WrapperType T>
class Wrapper;

// 默认值
template<Wrapper T = int>
class Wrapper;

// 可变概念参数
template<WrapperType... Ts>
class Wrapper;
```
#### 2.2 非类型模板参数
模板参数并不总是必须表示类型，也可以是编译时表达式，例如常量、外部函数，或对象的地 址或静态类成员的地址。编译时表达式提供的参数称为非类型模板参数，这类参数只能有结构类型:
- 整数
- 浮点数（c++20）
- 枚举类型
- 指针类型
- 指向成员类型指针
- 左值引用类型
- 字面量类型
```c++

template<int V>
class foo{};

template<int V = 200>
class foo{};

template<int... Vs>
class foo{};
```
example:
```C++
template<typename T, std::size_t S>
class buffer {
public:
  constexpr T const* data() const {
    reurn m_data;
  }

  constexpr T& operator[](std::size_t index) 
  {
    return m_data[index];
  }

  constexpr const T& operator[](std::size_t index) 
  {
    return m_data[index];
  }

private:
  std::array<T, S> m_data;
};

```

#### 2.3 双重模板参数
指的是一类模板参数，这些参数本身就是模板。这些参数可以像类型 模板参数一样指定，带或不带名称，带或不带默认值，以及作为带或不带名称的参数包.

```c++
// 简单封装
template<typename T>
struct simple_wrapper {
    T value;
};


// funny 封装
template<typename T>
struct funny_wrapper {
    funny_wrapper(const T v)
      :value(v)
    {}
  
    T const& get() const {
        return value;
    }
    
    template<typename U>
    U as() const {
      return static_cast<U>(value);
    }

T value;
};

// 双重模板的模板
// W 是一个模板参数
template<typename T, typename U, template<typename> typename W = funny_wrapper>
struct wrapper_pair {
  wrapper_pair(T const a, U const b)
  :item1(a), item2(b)
  {}

W<T> item1;
W<U> item2;
};

```

### 3 模板的实例化

#### 3.1 隐式实例化
当编译器基于模板的使用生成定义，并且没有显式实例化时，就会触发隐式实例化。

#### 3.2 显式实例化
可以显式地告诉编译器实例化类模板或函数模板，这称为显式实例化。它有两种形式: 
- 显式实例化定义
- 显式实例化声明  
(1) 显式实例化定义:
显式实例化定义可以出现在程序中的任何地方，但不能出现在所引用的模板定义之后。
```c++
namespace ns {
  template<typename T>
  struct wrapper
  {
      T value;
  };
  template struct warpper<int>;
}
template
struct ns::wrapper<double>;

int main()
{
    return 0;
}

```

```c++
//////////////////////////////////////
// wrapper.h
//////////////////////////////////////
#pragma once

template<typename T>
struct wrapper {
    T data;
};

// 显式实例化声明
extern template wrapper<int>;

//////////////////////////////////////
// source1.cpp
//////////////////////////////////////

#include "wrapper.h"

template wrapper<int>;

void foo()
{
    wrapper<int> a{100};
    std::cout << a.data << std::endl;
}

//////////////////////////////////////
// source2.h
//////////////////////////////////////
#include "wrapper.h"
#include <iostream>

void g()
{
    wrapper<int> b {200};
    std::cout << b.data << std::endl;
}

//////////////////////////////////////
// main.cpp
//////////////////////////////////////
#include "wrapper.h"

int main()
{
    wrapper<int> c {3290};
    std::cout << c.data << std::endl;
    return 0;
}
```




