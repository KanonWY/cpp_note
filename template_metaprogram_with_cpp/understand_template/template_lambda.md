## template lambda
### 1、引入历史
Lambda 表达式是在 C++11 中引入的，该标准的后续版本中进行了多次更新.
- 泛型 Lambda:在 C++14 中引入，其允许我们使用 auto，而不是显式地指定类型。这将生成的 函数对象转换为，具有模板函数调用操作符的函数对象
- 模板 Lambda:在 C++20 中引入，模板 Lambda 可以使用模板语法显式指定模板化的函数调用 操作符
```c++

// C++11 常规 lambda
auto c_11 = [](int a) { return a + a; };

// C++14 范型 lambda
auto c_14 = [](auto a) { return a + a; };

// C++20 模板 lambda
auto c_20 = []<template T>(T a) { return a + b ;};

```

### 2、范型 lambda 与 模板 lambda 的比较


### 3、何时使用 模板 lambda
