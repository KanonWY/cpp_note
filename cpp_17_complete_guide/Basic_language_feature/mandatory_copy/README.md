## 强制复制省略或传递非物质化对象

从技术上讲，C++17引入了在某些情况下强制复制的新规则：以前消除复制临时对象的选项，在按值传递或返回时，现在成为强制性的。

### 1. 强制复制的动机
在C++17之前，C++允许在某些情况下省略拷贝操作（称为拷贝省略，copy elision），即使这样做可能会影响程序的行为，因为不会调用拷贝构造函数。常见的情况是用临时对象初始化新对象时，例如临时对象按值传递给函数或从函数按值返回时。以下是一个示例：
```c++
class MyClass{};


MyClass getClass() {
  return MyClass();
}

void foo(MyClass o);

int main()
{
    foo(getClass());         //用返回的临时对象初始化参数
    foo(MyClass());          // 用临时对象初始化参数
    MyClass obj = getClass() // 用返回的临时对象初始化 obj
    return 0;
}
```
在C++17之前，这些优化并不是强制性的，因此必须提供`隐式或显式的拷贝或移动构造函数，以使拷贝操作成为可能`。尽管通常`不会调用拷贝/移动构造函数`，但它们必须存在。如果没有定义拷贝/移动构造函数，这样的代码是无法编译的。例如：
```c++
// 删除拷贝移动函数
```c++
class MyClass {
public:
    // ...
    MyClass(const MyClass&) = delete;
    MyClass(MyClass&&) = delete;
    // ...
};
```
从C++17开始，初始化临时对象的拷贝省略是`强制性`的。这意味着，即使定义的类MyClass不允许任何拷贝操作，上述示例代码也能够编译通过。实际上，这意味着`传递一个值作为初始化参数或返回值，将用于构建一个新对象`。

### 2、限制
```c++
class MyClass{};

// 返回一个局部对象
MyClass foo() {
    MyClass obj;
    // ...
    return obj; // 仍然需要拷贝/移动支持
}

// 返回一个局部对象
MyClass bar(MyClass obj) {
    // ...
    return obj; // 仍然需要拷贝/移动支持
}
```
### 3、优势与影响
(1) 从语义层面提供了临时值的优化  
```c++
class CopyOnly {
public:
    CopyOnly() {}
    CopyOnly(int) {}
    CopyOnly(const CopyOnly&) = default;
    CopyOnly(CopyOnly&&) = delete; // 显式删除
};

CopyOnly ret() {
    return CopyOnly{}; // C++17起可以
}

CopyOnly x = 42; // C++17起可以
```
(2) 简化代码，现在构建一个通用工厂类更加简单,无需考虑产品的构造和移动性  
```c++
#include <utility>

template <typename T, typename... Args>
T EasyFactory(Args&&... args) {
    return T{std::forward<Args>(args)...};
}
```





