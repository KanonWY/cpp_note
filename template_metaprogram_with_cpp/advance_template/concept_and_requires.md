## concept and requires

### 概念的定义与理解
每个概念都是一个谓词，它在编译时求值(编译期谓词)，并在将之用作约束时成为模板接口的一部分。
```c++
template< 模板形参列表>
concept 概念名 属性(可选) = 约束表达式;
```
其中约束表达式只要是编译期谓词即可。

### require 子句
关键词 `requires` 用来引入 `requires 子句`，它指定对各模板实参，或对函数声明的约束。
requires 期待一个编译期谓词。
```c++
template<typename T>
concept add = requires(T t) {
    t + t;
};

tempalte<typename T>
requires std::is_same_v<T, int>
void foo(T a) {}

template<typename T>
requires add<T>    //因为概念本身就是一个编译器谓词所以 requires 可以使用概念来进行约束
void foo2(T a) {}

template<typename T>
void foo3(T a) requires requires(T t) { t + t;}   // 第一requires 是 requires 子句，第二个 requires 是约束表达式
{}

```
概念（concept）只需要写在 auto 之前即可约束 auto
```c++
decltype(auto) max(std::integral auto a, std::integral auto b) {
    return a > b ? a : b;
}
```
### 约束
约束是逻辑操作和操作数的序列，它指定了对模板实参的要求。它们可以在 requires 表达式（见下文）中出现，也可以直接作为概念的主体。



