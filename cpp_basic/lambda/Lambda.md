## C++之lambda表达式

### 1、概述

Lamdba表示式在C++11标准中被正式引入，其实质为一种语法糖。Lambda表示式由`编译器`在`编译期间`产生一个`闭包类`，在运行时由这个闭包类产生一个`闭包对象`。因此所谓的闭包可以理解为一个`匿名`其包含`定义时作用域上下文`的函数对象。

**可以使用[cppinsights](https://cppinsights.io/)这个网站来查看lambda表达式的实现细节。**

#### 1.1 基本语法

lambda表示式的定义十分简洁。

```cpp
[capture_list](params) specifiers exception -> ret { body }
```

- 捕获表列: 它可以捕获当前函数作用域的零个或者多个变量，变量之间使用逗号分隔。捕获了的变量可以在`body`内使用。捕获方式有两种将在下文进行描述。
- 参数：与普通函数一致，可以不存在。
- 可选限定符:在C++11中，按值捕获的变量无法改变其内容，但是加上`mutable`之后可以改变，另外也可以使得在`body`内部调用非const成员函数。
- 可选异常说明符:C++11中可以使用`noexcept`来表示该lambda表达式不会抛出异常。

- 返回值：可以后置返回值，也可以在有返回值的时候不指定来让编译器自动推导出一个返回类型。
- 函数体

#### 1.2 捕获列表

lambda表达式的作用域分为**定义时的作用域**和**函数体的作用域**。C++标准规定，能够捕获的对象是一个自动存储变量，简单的说就是非晶态局部变量。

- 全局变量和静态局部变量无法被捕获。
- 尽管全局变量和静态局部变量无法被捕获，但是可以直接在函数体内部使用。
- 全局作用域的lambda表达式无法捕获任何变量。

#### 1.3 捕获方式

在Lmadba表达式中的捕获方式分为：捕获引用，捕获值。

（1）捕获值：

捕获值将函数作用域内的x和y值复制到函数表达式内部。但是在不加`mutable`限制修饰的情况下，无法修改值捕获的对象的内存。

```cpp
foo()
{
  	int a = 100;
  	auto foo = [a](){
        a++;		//error
    }
  	auto Moo = [a]() mutable{
      	a++; //yes, but a b
    }
}
```

（2）捕获引用：

捕获引用在捕获值变量的前面加上`&`即可。值捕获可以修改捕获值，而不必添加`mutable`。引用捕获会影响被捕获的值。

```cpp
int main()
{
  int x = 5,y = 8;
  auto foo = [x, &y]() mutable {
  	x += 1;
    y += 2;
  	return x * y;
  };
 
  foo();
  // x = 5, y = 10
  foo();
  // x = 5, y = 12
}
```

总结：

- 捕获的变量默认为常量。
- lambda是一个常量函数（类似于常量成员变量）。
- 引用捕获之所以能够改变变量是因为，引用值改变的不是引用本身，而是引用指向的对象，因此可以改变。
- mutable限制词可以改变lambda表达式的const特性，但是有mutable限制符必须有形参表列，即使它为空。

```cpp
int x = 200;
int u = 3000;
auto foo = [x, u]{
  	return x + u;
}
auto mmp = [x, u]() mutable {
  	x = x + 1;
  	return x + u;
}
auto mmp2= [x, u] mutable {		//错误，缺少了形参表列。
  	x = x + 1;
  	return x + u;
}
```

#### 1.4 特殊的捕获方式

- [this] 捕获this指针，可以使用this指针访问this指针指向的成员与函数。
- [=] 捕获lambda表达式定义作用域的全部变量的值，包括this指针。
- [&] 捕获lambda表达式定义作用域的全部变量的引用，包括this指针。

捕获this指针:实际是捕获了指向对象的指针。

```cpp
class A
{
public:
  	void print()
    {
      	std::cout << "Class A" << std::endl;
    }
  	void test()
    {
      	auto foo = [this] {
          	print();
          	x = 5;
        };
      	foo();
    }
private:
  	int x;
};
```

由于捕获了this指针，于是可以调用函数，和使用成员变量。

[=]捕获

```cpp
class Test{
public:
  	void echo()
    {
      std::cout << "Test" << std::endl;
    }
};

int main()
{
  	int x = 200;
  	int y = 209999;
  	Test ts;
  	auto foo = [=]() mutable {	//不加mutable， echo必须是const函数才可以访问。
    	return x + y;
      	ts.echo();
    };
    foo();
}
```

### 2、lambda表达式实现原理

#### 2.1 lambda表达式和函数对象区别

lambda表达式与函数对象的实现十分类似。

```cpp
class Base
{
public:
  	int operator()()
    {
      	std::cout << "Base()" << std::endl;
    }
};

```



优点：

- 使用lambda表达不需要显式的定义一个类，书写更加快捷，但是函数对象可以更加细节的控制操作。
- 使用函数对象可以在初始化的时候使用更加丰富的操作，比如说可以使用直到C++14才出现的`捕获表达式结果`。

- lambda表达式实际是基于函数对象实现的。

#### 2.2 广义捕获

C++14标准中定义了广义捕获，广义捕获是两种捕获方式，第一种称为简单捕获（C++11中的捕获方式)；第二种为初始化捕获，初始化捕获解决了**只能捕获lambda表达式定义上下文的变量，而无法捕获表达式结果以及自定义捕获变量名**。
```cpp

//c++11,无法执行移动捕获，将str的所有权转移到lambda闭包类中
//但是在C++14中可以进行如下操作：将str的值转移到闭包的st变量中，此后使用的str已经被闭包移动捕获了，因此再次使用有问题。
std::string str("llpdlapdl");
auto foo = [st = std::move(str)]()->std::string {
	st.append("]]]]]]]");
	return st;
};
foo();					//return value = llpdlapdl]]]]]]]；
std::cout << "str size = " << str.size() << std::endl;		//str size = 0;//因为被闭包捕获了，所以为空。
```
### 3、lambda实现原理

#### 3.1 `lambda`基本实现

创建了一个简单的`lambda`表达式，捕获值a，捕获引用b，然后解析其大概实现。编译器为每一个`lambda`表达式创建了一个匿名闭包类。然后根据此类创建出了一个闭包对象。对于值捕获而言，相当于在闭包类内部有一个`值成员变量`；对于引用捕获，相当于在其内部有一个`引用类型的成员变量`。然后捕获作用域内的变量（这里是a,b）作为该类的构造函数参数，然后创建出闭包成员变量。

调用`lambda`函数相当于调用`operator()`的重载函数。需要注意的是没有加`mutable`时，`operator()`的重载函数是const类型的成员函数。

```cpp
// 
int a = 200;
int b = 3333;
auto foo = [a, &b]()->int{		//捕获值a，捕获引用b
    return a + b;
};
foo();

//简单解析
int main()
{
  int a = 200;
  int b = 3333;
  
  //编译器创建了一个匿名闭包类
  class __lambda_5_12
  {
    public: 
    inline int operator()() const
    {
      return a + b;
    }
    
    private: 
    int a;
    int & b;
    public: 
    // inline /*constexpr */ __lambda_5_12(__lambda_5_12 &&) noexcept = default;
    __lambda_5_12(int & _a, int & _b)
    : a{_a}
    , b{_b}
    {}
    
  };
  //使用闭包类创建了一个闭包对象。
  __lambda_5_12 foo = __lambda_5_12(__lambda_5_12{a, b});
  foo.operator()();
  return 0;
}

```

#### 3.2`mutable`的影响
对于没有添加`mutable`的`lambda`表达式，本质上是一个const函数。因此捕获的对象默认无法修改的，假如捕获的是类对象，那么只能调用类对象的`const`函数。假设有`Test`类中有`echo_const()`和`echo_no_const()`。

```cpp
class Test {

  public:
    Test(const std::string str) {
        m_str = str;
        std::cout << "string cs" << std::endl;
    }
    Test(const Test& rhs) {
        m_str = rhs.m_str;
        std::cout << "copy" << std::endl;
    }
    Test& operator=(const Test& rhs) {
        m_str = rhs.m_str;
        std::cout << "copy =" << std::endl;
        return *this;
    }

    Test(Test&& rhs) noexcept {
        m_str = std::move(rhs.m_str);
        std::cout << "move" << std::endl;
    }

    Test& operator=(Test&& rhs) noexcept {
        m_str = std::move(rhs.m_str);
        std::cout << "move = " << std::endl;
        return *this;
    }

    void echo_const() const {
        std::cout << "str = " << m_str.c_str() << std::endl;
    }

    void echo_no_const() {
        std::cout << "str = " << m_str.c_str() << std::endl;
    }

  private:
    std::string m_str;
};
```

- 无`mutable`时无法修改捕获值的变量，无法调用捕获值对象的非`const`成员函数，但是可以修改捕获引用的变量，可以调用捕获引用对象的所有成员函数。

```cpp
int a = 200;
int b = 200;
auto fun1 = [a, &b]()->void{ 
	a++;		//error
	b++;		//ok,因为是引用捕获，引用不能修改，引用对应的值可以修改。
};

Test test("MMMM");
Test test2("lkdka");
auto fun2 = [test, &test2]()->void{
    test.echo_const();		//yes, 可以调用const成员函数。
    test.echo_no_const();	//no, 不能调用非const成员函数。
    
    test2.echo_const();		//yes
    test2.echo_no_const();	//yes
};
```

- 有`mutable`时，可以修改捕获值的变量，可以调用捕获值对象的任何函数。

```cpp
int a = 200;
int b = 200;
auto fun1 = [a, &b] mutable ()->void{ 
	a++;		//ok, 因为加了mutable，使得可以修改捕获值对象。
	b++;		//ok,因为是引用捕获，引用不能修改，引用对应的值可以修改。
};

Test test("MMMM");
Test test2("lkdka");
auto fun2 = [test, &test2] mutable ()->void{
    test.echo_const();		//yes, 可以调用const成员函数。
    test.echo_no_const();	//ok， 因为加了mutable，使得可以调用捕获值对象的任何函数。
};
```

