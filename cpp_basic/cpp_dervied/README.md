# CPP成员可见性与继承方式
## 1.1、CPP类成员可见性
- private成员：  
只有该类内部可以访问,`friend`可以访问，该类的子类也不能访问。
- protected成员：  
只有该类内部和它的子类可以访问和使用，其他都不可以。  
- public成员：  
都可以访问。
## 1.2、继承方式对可见性的影响
### 私有继承
私有继承情况下，公有类型，私有类型，受保护类型三种成员的访问权限如下：  
（1）基类的公有和受保护类型，在被派生类私有继承之后，都变为派生类的私有类型；即在类的类成员函数中可以访问，不能在类外访问。  
（2）而基类的私有成员，在派生类的类内和类外都不能访问。  

### 公有继承
在公有继承的情况下，权限如下：  
（1）基类中的公有成员，在派生类中仍为公有成员。都可以访问。  
（2）基类中的私有成员，在派生类和派生类对象中都不可以访问。  
（3）基类中的受保护成员，在派生类中仍然为受保护类型，在派生类中的函数中可以访问，但是派生类对象无法访问。  

### 保护继承
保护继承的情况下，权限如下：  
（1）基类的公有成员和保护成员在派生类中变为保护成员。  
（2）基类的私有成员在派生类中不能被直接访问。  

## 1.3 C++对象模型

### 1. 普通对象（不含虚函数）
```cpp
struct Base {
    Base() = default;
    ~Base() = default;

    void func() {}
    int a;
    int b;
};
```
对象内存布局如下：
```cpp
  |a|   
  |b|
```
### 2. 带虚函数的类对象布局
```cpp
struct Base {
    Base() = default;
    virtual ~Base() = default;
    void FuncA() {}
    virtual void FuncB() {
        std::cout <<"FuncB()" << std::endl;
    }
    int a;
    int b;
};
```
对象内存布局如下：

```cpp        
 Base                       ____vtable Base____
|vptr|  --------->         |  offset_to_top(x) |           
| a  |                     |      RTTI         |
| b  |                     | ~Base()[complete] |
                           | ~Base()[deleting] |
                           | Base::FuncB()     |
                            ————————————————————
```
**offset_to_top**：表示当前这个虚函数表地址距离对象顶部地址的偏移量，因为对象的头部就是虚函数表的指针，所以偏移量为0。  
**RTTI指针**：指向存储运行时类型信息(type_info)的地址，用于运行时类型识别，用于`typeid`和`dynamic_cast`。

### 3. 单继承下不含覆盖函数的类对象的布局
```cpp
struct Base {
    Base() = default;
    virtual ~Base() = default;
    void FuncA() {}
    virtual void FuncB() {
        std::cout <<"FuncB()" << std::endl;
    }
    int a;
    int b;
};

struct Derive: public Base {};
```

子对象内存布局如下：

```cpp
 Derive                     ____vtable Derive____
|Base vptr| --------->     |  offset_to_top(x)   |           
| a       |                |      RTTI           |
| b       |                | ~Derive()[complete] |
                           | ~Derive()[deleting] |
                           | Base::FuncB()       |
                            ————————————————————-
```

### 4. 单继承下含有覆盖函数的类对象的布局

```cpp
struct Base {
    Base() = default;
    virtual ~Base() = default;
    void FuncA() {}
    virtual void FuncB() {
        std::cout <<"FuncB()" << std::endl;
    }
    int a;
    int b;
};

struct Derive: public Base {
	
    void FuncB() override {
        std::cout << "Derive FuncB" << std::endl;
    }
};
```

子对象内存布局如下：

```cpp
 Derive                     ____vtable Derive____
|Base vptr| --------->     |  offset_to_top(x)   |           
| a       |                |      RTTI           |
| b       |                | ~Derive()[complete] |
                           | ~Derive()[deleting] |
                           | Derive::FuncB()     |
                            ————————————————————-
```

### 5. 多继承下不含覆盖函数的类对象的布局

多继承的情况下，继承了几个带虚函数的类就有几个虚表指针，并且子类也会继承基类的数据。

```cpp
struct BaseA {
    BaseA() = default;
    virtual ~BaseA()  = default;
	void FuncA() {}
    virtual void FuncB() {
        std::cout << "BaseA FuncB\n";
    }
    int a;
    int b;
};

struct BaseB {
    BaseB() = default;
    virtual ~BaseB() = default;
    
    void FuncA() {}
    virtual void FuncC() {
        std::cout << "BaseB FuncC\n";
    }
    int a;
    int b;
};

struct Derive: public BaseA, public BaseB {};
```

子对象内存布局如下：

```cpp
 Derive                     ____vtable Derive____
|BaseA vptr| --------->     |  offset_to_top(x)   |           
| a        |                |      RTTI BaseA     |
| b        |                | ~Derive()[complete] |
|BaseB vptr| --------->     | ~Derive()[complete] |
| a		   |                | BaseA::FuncB()      |
| b        |                | offset_to_top(-16)  |
    						|     RTTI  BaseB	  |
     						| ~Derive()[complete] |	(chunk)
      						| ~Derive()[complete] |	(chunk)
     						|	BaseB:FuncC()     |
     						 ————————————————————-
```

### 6. 多继承下含有覆盖函数的类成员的布局

```cpp
struct BaseA {
    BaseA() = default;
    virtual ~BaseA()  = default;
	void FuncA() {}
    virtual void FuncB() {
        std::cout << "BaseA FuncB\n";
    }
    int a;
    int b;
};

struct BaseB {
    BaseB() = default;
    virtual ~BaseB() = default;
    
    void FuncA() {}
    virtual void FuncC() {
        std::cout << "BaseB FuncC\n";
    }
    int a;
    int b;
};

struct Derive: public BaseA, public BaseB {
    void FuncB() override 
    {
        std::cout << "Derive::FuncB" << std::endl;
    }
    
    void Func() override
    {
        std::cout << "Derive::FuncC" << std::endl;
    }
};
```

内存布局如下：

```cpp
 Derive                     ____vtable Derive____
|BaseA vptr| --------->     |  offset_to_top(x)   |           
| a        |                |      RTTI BaseA     |
| b        |                | ~Derive()[complete] |
|BaseB vptr| --------->     | ~Derive()[complete] |
| a		   |                | Derive::FuncB()      |
| b        |                | Derive::FuncC()	  |
     						| offset_to_top(-16)  |
    						|     RTTI  BaseB	  |
     						| ~Derive()[complete] |	(chunk)
      						| ~Derive()[complete] |	(chunk)
     						|	Derive:FuncC()     |
     						 ————————————————————-
```

### 7. 继承顺序导致的类对象的布局的变化

```cpp
struct BaseA {
    BaseA() = default;
    virtual ~BaseA()  = default;
	void FuncA() {}
    virtual void FuncB() {
        std::cout << "BaseA FuncB\n";
    }
    int a;
    int b;
};

struct BaseB {
    BaseB() = default;
    virtual ~BaseB() = default;
    
    void FuncA() {}
    virtual void FuncC() {
        std::cout << "BaseB FuncC\n";
    }
    int a;
    int b;
};

struct Derive: public BaseA, public BaseB {
    void FuncB() override 
    {
        std::cout << "Derive::FuncB" << std::endl;
    }
    
    void Func() override
    {
        std::cout << "Derive::FuncC" << std::endl;
    }
};
```

内存布局如下：

```cpp
 Derive                     ____vtable Derive____
|BaseB vptr| --------->     |  offset_to_top(x)   |           
| a        |                |      RTTI BaseA     |
| b        |                | ~Derive()[complete] |
|BaseA vptr| --------->     | ~Derive()[complete] |
| a		   |                | Derive::FuncC()      |
| b        |                | Derive::FuncB()	  |
     						| offset_to_top(-16)  |
    						|     RTTI  BaseB	  |
     						| ~Derive()[complete] |	(chunk)
      						| ~Derive()[complete] |	(chunk)
     						|	Derive:FuncB()    |
     						 ————————————————————-
```

### 8. 虚继承的布局

```cpp
struct Base {
    Base() = default;
    virtual ~Base() = default;
    
    void FuncA() {}
    virtual void FuncB() {
        std::cout << "Base::FuncB" << std::endl;
    }
    int a;
    int b;
};

struct Derive: virtual public Base
{
    void FuncB() override {
        std::cout << "Derive FuncB" << std::endl;
    }
};
```

虚继承下，子类和虚基类分别有一个虚表地址指针。