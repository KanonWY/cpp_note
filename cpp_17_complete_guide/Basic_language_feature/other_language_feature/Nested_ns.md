## Nested Namespaces
嵌套命名空间（Nested Namespaces）在C++17中引入。这个特性使得在定义嵌套命名空间时更加简洁和直观。  
在C++17之前，如果要定义嵌套命名空间，通常需要像这样逐层嵌套：
```c++
namespace A {
    namespace B {
        namespace C {
            // 代码
        }
    }
}

```

在C++17中，可以通过嵌套命名空间语法将上述定义简化为一行：
```c++
namespace A::B::C {
    // 代码
}
```
这个特性还是很好用的
