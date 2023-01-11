#include <iostream>
#include <string>

// 模板参数特殊化，也称为模板参数具体化。指的是使用指定的类型替换掉模板中的泛型参数。
// 有时候，我们需要对特殊的类型做特殊的处理，就需要判断如果是某一个类型的时候，走另外的分支。

// （1）类模板特化
// 首先在类型名后面加<>,在其中添加特化的参数
// 便特化是一个泛化的主题模板，不能只写泛化模板。

template <class K, class V>
class MyMap
{
public:
    MyMap() { std::cout << "泛化" << std::endl; }
};

template <class V>
class MyMap<std::string, V>
{
public:
    MyMap() { std::cout << "部分特化 <std::string, V>" << std::endl; }
};

template <>
class MyMap<std::string, int>
{
public:
    MyMap() { std::cout << "全特化 <std::string, int>" << std::endl; }
};

void testClassSpecial()
{
    MyMap<int, double>();
    MyMap<std::string, int>();
    MyMap<std::string, double>();
    MyMap<std::string, std::string>();
}

// (2)函数模板的特化与重载及其次序
//  注意点：函数模板不支持部分特化
//
//  函数模板与非函数模板都可以正确匹配某一个调用，优先匹配非函数模板。
//  除非指定了参数模板

template <typename T, typename U>
void add(T t, U u)
{
    std::cout << "泛化" << std::endl;
}

template <>
void add<double, double>(double t, double u)
{
    std::cout << "全特化" << std::endl;
}

template <typename T>
void add(T t, int u)
{
    std::cout << "add(T t, int u)" << std::endl;
}

void add(int a, int b) { std::cout << "add(int, int)" << std::endl; }

void testFunctionSpecial()
{
    add(std::string("00"), std::string("kk")); // 泛化
    add(std::string("kjj"), 22);               // add(T t, int u)
    add(10, 10);                               // add(int, int)
    add(12.9, 299.3);                          // 全特化
}

int main()
{
    testFunctionSpecial();
    return 0;
}