## 继承与多态的介绍
### 1、继承
继承常见有三种方式:`public`,`private`,`protected`
#### 1.1 public继承
公有继承是`is-a`的关系。因此`Dervied is a Base`。
因为公共继承意味着`is-a`关系，所以该语言允许在指向同一层次结构中的不同类的引用和指针之间进行广泛的转换
```cpp
class Base {};
class Base: public Derived {};
```
下面这个转换总是有效的，因为派生类的实例也是基类的实例。
```cpp
Dervied *d = new Derived;
Base* b = d;        //自动隐式转换
```
下面的转换一是成立的，因为Base指向的实际是派生类；但是转换二确实错误的，它是一个未定义的行为，会导致错误。
```cpp
//转换一
Base* b = new Derived; // *b is really Derived
Derived* d = b; // 编译会报错
Derived* d = static_cast<Derived*>(b); // 必须显式转换 

//转换二
Base* b2 = new Base;
Drived* d2 = b2;    //error
Drived* d2 = b2;    //错误
```

#### 1.2 private继承
私有继承的时候，派生类不会扩展基类的公共接口，而是将公共接口隐藏，所有的基类方法在派生的类中都是私有的。所有的公共接口必须从派生类中创建。派生类从基类中得到的是实现细节——派生类都可以使用这些方法和数据成员来实现它自己的算法。因此私有继承实现了一种`has-a`的关系，即派生对象有一个包含的基类的实例。
通常来说，组合应优先于私有继承。 
#### 1.3 私有继承存在的意义
既然通常来说，组合应优先于私有继承。那么什么时候使用私有继承呢？主要有两点原因：  
（1）空类大小的优化 
如下代码，组合与继承在常规的情况下大小一致。
```cpp
class Base{
private:
    int a;
};

class Dervied
{
private:
    Base __base_;
    int a;
};

class Private_Dervied: private Base
{
private:
    int a;
};
std::cout  << sizeof(Dervied) << std::endl;
std::cout  << sizeof(Private_Dervied) << std::endl;
//x64(debug)下输出结果为
// 8
// 8
```
但是在模板编程等情况下空基类的情况十分常见。众所周知，一个空基类的实例`sizeof`大小为1，因为即使是空基类也需要在内存中唯一表示，因此占据了一个字节表示其唯一地址。这对于组合模式来说就存在问题了在组合类的成员对象即使是空类也会占据一个字节，默认内存对其的情况下可能会占据更多字节的消耗，这似乎是违反了C++零成本抽象的精髓。实例代码如下，使用私有继承就可以保证空基类对于不必要内存的消耗。
```cpp
class Empty{
};

class Empty_Comp
{
private:
    Empty __empty_;
    int a;
};

class Private_Empty: private Empty
{
private:
    int a;
};
std::cout  << sizeof(Empty_Comp) << std::endl;
std::cout  << sizeof(Private_Empty) << std::endl;
//x64(debug)下输出结果为
// 8
// 4
```
在STL源码中大量使用了空基类优化的特性，减少了内存消耗增加了性能。在`Boost`库中也有`boost/compressed_pair.hpp`用于对空pair对象处理。
```cpp
#include <boost/compressed_pair.hpp>
#include <iostream>
class Empty {
};

int main()
{
    boost::compressed_pair<int, Empty> m_pair;
    std::pair<int, Empty> m_pair2;
    std::cout << sizeof(m_pair) << std::endl;   
    std::cout << sizeof(m_pair2) << std::endl;
	return 0;
}
//输出 
//4
//8
```
（2）虚函数相关  
### 2、多态与虚函数


### 3、C++类型转换

#### 3.1 static_cast
用来强迫隐式转换如`non-const`对象转为`const`对象，编译时检查，用于非多态的转换，可以转换指针及其他，但没有运行时类型检查来保证转换的安全性。它主要有如下几种用法：
```cpp
static_cast< new_type >(expression)
// new_type为目标数据类型，expression为原始数据类型变量或者表达式。
```
常见用法：
- 用于类层次结构中基类（父类）和派生类（子类）之间指针或引用的转换。
- 用于基本数据类型之间的转换，如把int转换成char，把int转换成enum。
- 把空指针转换成目标类型的空指针。
- 把任何类型的表达式转换成void类型。  

用于基类与派生类之间的转换分为下面两类:  
1、进行上行转换（**把派生类的指针或引用转换成基类表示**）是安全的。  
2、进行下行转换（**把基类指针或引用转换成派生类表示**）时，由于没有动态类型检查，所以是不安全的。
```cpp
class Base {};

class Derived : public Base {}
Base* pB = new Base();

if(Derived* pD = static_cast<Derived*>(pB))
{}//下行转换是不安全的(坚决抵制这种方法)
Derived* pD = new Derived();
if(Base* pB = static_cast<Base*>(pD))
{}//上行转换(派生类转换为基类）是安全的
```
#### 3.2 const_cast

#### 3.3 dynamic_cast
`dynamic_cast`主要用于类层次间的上行转换和下行转换。在类层次间进行上行转换时，`dynamic_cast`和`static_cast`的效果是一样的；在进行下行转换时，`dynamic_cast`具有类型检查的功能，比`static_cast`更安全。
```cpp
dynamic_cast< type* >(e)
　type必须是一个类类型且必须是一个有效的指针
dynamic_cast< type& >(e)
type必须是一个类类型且必须是一个左值
dynamic_cast< type&& >(e)
type必须是一个类类型且必须是一个右值
```
1、当进行指针类型下行转换的时候  
Base为包含至少一个`虚函数`的基类，Derived是Base的共有派生类，如果有一个指向Base的指针bp，我们可以在运行时将它转换成指向Derived的指针。
```cpp
//bp->dp,下行转换
Derived *dp = dynamic_cast<Derived *>(bp);
如果转换失败：dp为nullptr。
如果成功：返回正确的指针。
```
2、 当进行引用类型的下行转换的时候  
#### 3.4