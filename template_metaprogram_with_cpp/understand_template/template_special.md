## template special
模板特化是从模板实例化创建的定义，进行特化的模板称为`主模板`。可以为给定的模板参数集提供显式的特化定义，从而`覆盖编译器将生成的隐式代码`。这种技术支持诸如`类型特征`和`条件编译`等特性。

### 1、full special
显式特化 (也称为全特化) 发生在，使用完整的模板参数集为模板实例化提供定义时。以下可以 全特化的模板:
- 函数模板
- 类模板
- 变量模板
- 类模板的成员函数、类和枚举
- 类或者类模板的成员函数模板或者类模板
- 类模板的静态数据成员

```c++
// 主模板
template<typename T>
struct is_float_pointer
{
    constexpr static bool value = false;
};

// float 特化
template<>
struct is_float_pointer<float>
{
    constexpr static bool value = true;
};

// double 特化
template<>
struct is_float_pointer<double>
{
    constexpr static bool value = true;
};

// long double 特化
template<>
struct is_float_pointer<long double>
{
   constexpr static bool value = true;
};
```
`全特化必须出现在主模板声明之后，不需要在全特化之前对主模板进行定义`，以下例子对此进行了说明:
```c++
// 主模板声明
template<typename T>
struct is_float_pointer;

// 全特化 float 版本
template<>
struct is_float_pointer<float>
{
    constexpr static bool value = true;
};

// 主模板定义
template<typename T>
struct is_float_pointer
{
    constexpr static bool value = false;
};
```
### 2、partial special
偏特化发生在特化一个主模板但只指定一些模板参数时，所以偏特化同时具有模板形参列表 (跟在模板关键字后面) 和模板实参列表 (跟在模板名称后面)，`只有类模板可以偏特化，函数模板不能偏特化只能重载`。
```c++
template<typename T, int S>
struct collection
{
    void operator()()
    { std::cout << "primary template" << std::endl; }
};

template<typename T>
struct collection<T, 10>
{
    void operator()()
    { std::cout << "partial special: <T, 10>" << std::endl; }
};

template<int S>
struct collection<int S>
{
    void operator()()
    { std::cout << "partial special: <int, S>" << std::endl; }
};

// 偏特化类型指针
template<typename T, int S>
struct collection<T*, S>
{
    void operator()()
    { std::cout << "partial special: <T*, S>" << std::endl; }
};
```


