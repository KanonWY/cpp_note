## lambda 表达式的扩展
lambda 表达式在 C++11引入，c++14 增加了其泛型的能力，c++17 进一步增强。

### 1、constexpr lambda
自C++17以来，如果可能的话，`lambdas是隐式constexpr`。也就是说，任何lambda都可以在编译时上下文中使用，前提是它使用的功能对编译时上下文有效（例如，只有文字类型，没有静态变量，没有虚拟，没有try/catch，没有new/delete.
```c++
auto test_lam = [](int x) { return x * x;};
std::array<int, test_lam(5)>;  //=> std::array<int, 25>;
```
在 constexpr 不生效的时候，会自动变为运行时
```c++
auto test_lam_2 = [](int x) {
  static int call = 0;
  return x * x;
};

std::array<int, test_lam2(5)>; //error: runtime
std::cout << test_lam2(5) << std::endl; // OK
```
如果想使用 constexpr lambada 可以 在后面添加 constexpr 让编译器帮你检查是否是 constexpr 的lambda，以便提早暴露出错误。
```c++
auto test_lambda3 = [](int x) constexpr -> int {
  static int call = 0;  // error. 会在编译期报错
  return x * x;
}
```
### 2、捕获 this 指针的拷贝
在C++17之前的C++版本中，使用Lambda表达式时，如果需要访问当前对象的成员，必须显式地捕获this指针。但这种捕获方式有一定的局限性和潜在的风险。C++17对Lambda捕获this进行了改进，使得编写代码更加安全和简洁。  
(1) c++17 之前的方式：
```c++
class C {
private:
    std::string name;
public:
    void foo() {
        auto l1 = [this] { std::cout << name << '\n'; }; // OK
        auto l2 = [=] { std::cout << name << '\n'; }; // OK
        auto l3 = [&] { std::cout << name << '\n'; }; // OK
    }
};
```
上述实际捕获的都是 this 指针，如果lambda运行的声明周期超过了对象时，this 指针将会失效。  

(2) 捕获 *this 
C++17引入了一种新的捕获方式，可以捕获当前对象的副本，而不是捕获this指针。这通过捕获*this来实现：
```c++
class C {
private:
    std::string name;
public:
    void foo() {
        // 捕获的是 this 指针的拷贝
        auto l1 = [*this] { std::cout << name << '\n'; }; // C++17
    }
};
```
(3) 捕获结合
```c++
auto l2 = [&, *this] { ... }; // OK
auto l3 = [this, *this] { ... }; // ERROR
```
上面第一个例子是合法的，因为它捕获了当前对象的副本，同时按引用捕获其他变量。但第二个例子是非法的，因为它同时试图捕获this指针和*this副本，产生了冲突。  
(4) 捕获this的demo
```c++
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

class Data {
private:
    std::string name;
public:
    Data(const std::string& s) : name(s) {}
    
    auto startThreadWithCopyOfThis() const {
        using namespace std::literals;
        std::thread t([*this] { // 此处如果不捕获*this 将会导致 this 指针失效
            std::this_thread::sleep_for(3s);
            std::cout << name << '\n';
        });
        return t;
    }
};

int main() {
    std::thread t;
    {
        Data d{"c1"};
        t = d.startThreadWithCopyOfThis();
    } // d对象在此处被销毁
    t.join(); // 等待线程结束
}
```






