## Inline Variables

### 1、引入原因
在 C++ 17 之前不允许在结构体或者类的内部初始化`非 const 静态变量`。
```c++
struct MyClass{
  static std::string myName = "xxx"; // error.
  static const std::string myName2 = "yy"; //OK.
};
```
在头文件中初始化类静态变量或者全局变量会违反 ODR 规则（一个变量或者实体只能定义在转译单元中定义一次)
```c++
#prama once
#include <string>

struct Test{
static std::string name;
};

std::string Test::name = "xxxx";

Test myGlobalTest{};
```
上述头文件如果被多个cpp文件include会导致多次定义！

### 2、inline variable
在 C++17 之后，使用内联变量，不仅可以在类或者结构体中初始化非const static 变量，也可以在头文件中定义全局变量，即使这个头文件会被多个
cpp 文件引入。  
```c++

struct Kanon
{
  inline static std::string name = "xxxx";
};

inline Kanon MyGlobalKanon;
```

### 3、constexpr 有隐含 inline 的语义
C++17 以前
```c++
struct D{
    static constexpr int n = 100;  // 仅仅是声明
};
```
需要在某一个 CPP 源文件中定义它
```c++
constexpr int D::n;
```
这个额外的定义是为了满足一个定义规则（ODR），当D::n按引用传递或者取其地址时，需要这个定义。  
C++17 之后
```c++
struct D {
    static constexpr int n = 200; // 这是一个定义, constexpr 隐含有 inline 语义
};
```
C++11/C++14: 必须在类外单独定义static constexpr数据成员。  
C++17及以后: 在类内声明的static constexpr数据成员已经是定义，不需要在类外单独定义，旧的定义方式虽然有效但被弃用。  
通过这个改变，C++17简化了代码管理，减少了因缺少定义而导致的错误。

