## declval
std::declval 是一个工具类型的操作函数，与 std::move 和 std::forward 等函数属于同一类别，其功能非常简单: 将右值引用添加到类型模板参数中。
```c++
template<typename T>
typename std::add_rvalue_reference<T>::type declval() noexcept;
```

### 作用
这个函数没有定义，因此不能直接调用，只能在未求值的上下文中使用——decltype、sizeof、 typeid 和 noexcept。这些是仅在编译时执行的上下文，在运行时不会进行计算。std::declval 的目的是 帮助对没有默认构造函数或有默认构造函数，但由于是 private 或 protected 而不能访问的类型，进 行依赖类型求值.下面是一个使用例子：
```c++

template<typename T, typenemae U>
struct composition {
    using result_type = decltype(???);
};

template<typename T, typenemae U>
struct composition {
    using result_type = decltype(std::declval<T>() + std::declval<U>());
};

```

