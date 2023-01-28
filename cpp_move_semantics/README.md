##the power of move semantics
### 1.1 Why move semantics?
```cpp
std::vector<std::string> createAndInsert()
{
    std::vector<std::string> coll;
    coll.reserve(3);
    std::string s = "data";
    coll.push_back(s);
    coll.push_back(s + s);      //将临时对象插入到容器中
    coll.push_back(s);          //对象插入容器之后不再使用
    return coll;                //返回一个临时的有名对象
}

int main()
{
    std::vector<std::string> v;
    v = createAndInsert();      //将一个临时vector赋值给其他对象
}
```
在C++11以前以上的内存分配存在一些不必要的内存分配：
- 将临时对象插入到容器中
- 对象插入到容器中之后不再使用
- 将一个临时的vector赋值给其他对象
在C98可以使用以下方式对其中部分进行优化:
```cpp
// 将vector作为out-ref
void createAndInsert(std::vector<std::string>& rhs)
{

}
// swap函数
createAndInsert().swap(v);
//编译器层面的返回值优化，在编译可以执行的才可以优化。
 RVO
 NRVO
```
C++11之后使用移动语句对以上情况进行优化:
```cpp
#include <string>
#include <vector>

std::vector<std::string> createAndInsert()
{
    std::string<std::string> coll;
    coll.reserve(3);
    std::string s = "data";
    coll.push_back(s);                  //插入一个左值引用，会调用拷贝构造函数
    coll.push_back(s + s);              //插入一个临时对象，会调用移动构造函数，减少一次拷贝。
    coll.push_back(std::move(s));       //插入一个被std::move转换的左值，也会调用移动构造函数，减少一次拷贝构造。
}

int main()
{
    std::vector<std::string> v;
    v = createAndInsert();              //如果有编译器层面的返回值优化会使用返回值优化，如果没有的话，会调用启动移动构造，减少一次拷贝。
    return 0;
}
```
### 1.2 实现移动语句
在C++11以前，vector`vector`的`push_back`函数只有一个实现,此实现是接收所有的类型数据，然后进行拷贝。其根本语义是拷贝。
```cpp
void push_back(cosnt T& elem);
```
但是在C++11之后，`push_back`函数有了重载，可以接收一个右值引用，表明可以允许对传入参数的内存进行窃取。
```cpp
// insert elem when the value of elem is no longer needed
void push_back(T&& elem);
```
假设`push_back`的对象是简单的`string`:
```cpp
class string
{
private:
    int len;
    char* data;
public:
    string(const string& s)
    {
        len = s.len;
        if(len > 0)
        {
            data = new char[len + 1];       //分配新内存
            memcpy(data, s.data, len+1);    //拷贝
        }
    }

    string(string&& rhs)
    {
        data = rhs.data;            //仅仅复制了指针
        len = rhs.len;              //复制了长度
        rhs.data = nullptr;
        rhs.len = 0;
    }
};
```
### 1.3 复制是一种应急方式
使用移动语句用于移动临时对象或者使用`std::move`修改类型的对象，该操作可以通过类似`浅拷贝`的方式来优化值的复制，减少不必要的内存重新分配。  
对于临时对象或者使用`std::moev`转换的对象，首先会调用的是**形参声明为右值引用**的函数。如果不存在这样的函数，则将会使用复制语义。这样就可以确保优化调用不需要知道是否优化确实存在。  
优化可能不存在的原因：  
- 1、函数或者类是在移动语义支持之前实现的，或者实现时没有考虑移动语义。
- 2、没有什么需要优化的。（只有基本数据类型的类）。
注意：使用`std::move`标记的基本数据类型对象，比如int、int*等,仍然会使用复制语义。
```cpp
std::vector<int> coll;
int x{999};
coll.push_back(std::move(x));       //move no effect,因为没有优化空间，因为复制操作性能已经足够。
```
### 1.4 const对象的移动语义
不能移动const声明的对象，因为任何优化的实现要求可以修改传递的实参，如果不允许修改，就修改指针的指向。
```cpp
std::vector<std::string> coll;
const std::string s{"data"};
// OK，但是实际调用的是push_back(const std::string&);
coll.push_back(std::move(s));
```
总之就是const对象的`std::move`不起作用。
```cpp
  template<typename _Tp>
    constexpr typename std::remove_reference<_Tp>::type&&
    move(_Tp&& __t) noexcept
    { return static_cast<typename std::remove_reference<_Tp>::type&&>(__t); }
```
从`std::move`的源码中可以看出原因,`std::move`仅仅是对引用的处理但是没有对限定值(比如const)等进行消除。
```cpp
const std::string str{"lll"};
auto it = std::move(str);  //auto --> cosnt rvalue ref
```
- const 返回值
```cpp
const std::string getValue()
std::vector<std::string> coll;
coll.push_back(getValue());  //copy因为返回值是const的
```

## 3、类中的移动语句



## 4、如何从移动语句中获益


## 5、完美转发
完美地转发移动语义: 对于任何通过移动语义传递的参数，保持移动语义。当遇到没有移动语义的参数时，不添加移动语义。
`std::forward`实际上是一个条件性的`std::move`。
完美转发支持转发一个或者多个参数，当然也可以是变参。