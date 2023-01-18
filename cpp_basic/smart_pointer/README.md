# 智能指针

## 1、为什么使用智能指针

智能指针式为了解决长久以来C++堆内存释放的问题而发明的，采取了RAII的方式管理引用计数来实现堆内存的智能释放。常见的智能指针的类型有三种：1、独占式智能指针；2、共享式智能指针；3、弱引用智能指针

## 2、独占式智能指针

故名思考意义，独占式智能指针在作用域内只有一个一份，无法被拷贝，只能够被移动。

```cpp
#include <memory>
#include <iostream>

class USP {
public:
    ~USP() {
        std::cout << "~USP()" << std::endl;
    }
};


int example_operatpr_equal_nullptr()
{
  	auto sp  = std::unique_ptr<int>(new int(8888));
  	sp = nullptr;		//置空之后会调用析构
  	std::cout << "after = nullptr" << std::endl;
  	return 0;
}
//输出：  ～USP()


```

实现：

```cpp
template <class _Tp, class _Dp = default_delete<_Tp>>
class unique_ptr {
public:
  	typedef _Tp element_type;
    typedef _Dp deleter_type;
private:
  	__compressed_pair<pointer, deleter_type> __ptr_;		//压缩pair实现，空基类优化

public:
  	//析构函数
  	  ~unique_ptr() { reset(); }
    //reset函数
    void reset(pointer __p = pointer) noexcept {
        pointer __tmp = __ptr_.first();
        __ptr_.first() = __p;
        if(__tmp)
        {
            __ptr_.second()(__tmp);			//调用deletor函数
        }
    }
  
  	//置空操作
  	unique_ptr& operator(std::nullptr_t) noexcept {
      	reset();
      	return *this;
    }
  	
  	//* operator
  	typename add_lvalue_reference<_Tp>::type
    operator*() const {
      	return *__ptr_.first();
    }
  	pointer operator->() const {
      	return __ptr_.first();
    }
  	
  	//release
  	pointer release() noexcept {
      	pointer __t = __ptr_.first();
      	__ptr.first() = pointer();
      	return __t;
    }
  	//拷贝构造
  	
  	//移动拷贝
  	unique_ptr(unique_ptr&& __u) noexcept
    	:__ptr_(__u.release()), std::forward<deleter_type>(__u.get_deleter())
    {}
  	//移动赋值
  	unique_ptr& operator=(unique_ptr&& __u) noexcept{
      	reset(__u.release);
      	__ptr_.second() = std::forward<deleter_type>(__u.get_deleter());
      	return *this;
    }
};
```



## 3、共享式智能指针

共享式智能指针采用引用计数的方式实现对象内存的生命周期管理，单对象内存的引用计数为0时候，会释放内存。

```cpp
#include <memory>

class Test{};

int main()
{
  	auto sp = std::make_share<Test>();
 		
  	std::shared_ptr<Test> sp2(new Test());
}


```

#### 3.1 源码分析:

核心点：

共享式智能指针包含两个部分，1、指向资源的指针；2、指向控制块的指针。

- 什么时候生成控制块？
- enable_shared_from_this?

```cpp
template<class _Tp>
class shared_ptr
{
public:
  	typedef _Tp element_type;
private:
  	element_type* __ptr_;						//指向对象的指针		
  	__shared_weak_count* __cntrl_;		//控制块
  
public:
  	//核心函数
  	long use_count() const noexcept {
      	return __cntrl_ ? __cntrl_->use_count() : 0;
    }
  	bool unique() const noexcept {return use_count() == 1;}
  
  	//核心构造函数
  
  	//（1）空构造
  	shared_ptr() noexcept
    	:__ptr_(nullptr), __cntrl(nullptr)
    {}
  	
  	//（2）指针构造函数
  	explicit shared_ptr(_Yp* __p) :__ptr_(__p)
    {
      	// 创建独占式智能指针
				unique_ptr<_Yp> __hold(__p);		
      	// 删除器分配器
      	typedef typename __shared_ptr_default_allocator<_Yp>::type _AllocT;
      	// 控制块类型
      	typedef __shared_ptr_pointer<_Yp*, __shared_ptr_default_delete<_Tp,_Yp>, _AllocT> _CntrlBlk;
      	// 创建控制块的类型
      	__cntrl_ = new CntrlBlk(__p, __shared_ptr_default_delete<_Tp,_Yp>(),_AllocT());
      	// 清空独占式智能指针
      	__hold.release();
      	__enable_weak_this(__p, __p);
    }
  	
  	//（3）到删除函数的构造函数
  	template<class _Tp>
    template<class _Yp, class _Dp>
    shared_ptr(_Yp* __p, _Dp __d, typename     std::enable_if_t<__shared_ptr_deleter_ctor_reqs<_Dp,_Yp,element_type>::value,__nat>::type)
    :__ptr_(__p)
    {
      	try
        {
          	typedef typename __shared_ptr_default_allocator<_Yp>::type _AllocT;
        		typedef __shared_ptr_pointer<_Yp*, _Dp, _AllocT > _CntrlBlk;
        		__cntrl_ = new _CntrlBlk(__p, _VSTD::move(__d), _AllocT());
          	__enable_weak_this(__p, __p);
        }
      	catch(...)
        {
          	__d(__p);		//调用删除函数
          	throw;
        }
    }
		
};

// make_shared构造方式
template<class _Tp, class ..._Args, class = std::enable_if_t<!is_array<_Tp>::value>>
shared_ptr<_Tp> make_shared(_Args&& ...__args)
{
    return _VSTD::allocate_shared<_Tp>(allocator<_Tp>(), _VSTD::forward<_Args>(__args)...);
}

template<class _Tp, class _Alloc, class ...Args, class = std::enable_if_t<!is_array<_Tp>::value>>
shared_ptr<_Tp> allocate_shared(const _Alloc& __a, _Args&& ...__args)
{
    using _ControlBlock = __shared_ptr_emplace<_Tp, _Alloc>;
    using _ControlBlockAllocator = typename __allocator_traits_rebind<_Alloc, _ControlBlock>::type;
    __allocation_guard<_ControlBlockAllocator> __guard(__a, 1);
    ::new ((void*)_VSTD::addressof(*__guard.__get())) _ControlBlock(__a, _VSTD::forward<_Args>(__args)...);
    auto __control_block = __guard.__release_ptr();
    return shared_ptr<_Tp>::__create_with_control_block((*__control_block).__get_elem(), std::addressof(*__control_block));
}
```

控制块的创建规则：

（1）`std::make_shared`总是创建一个控制块。

上面源码中已有，

（2）从独占式智能指针(`unique_ptr<T>`)创建控制块。

由于独占式智能指针的只移动特性，因此只能使用右值引用传入，并且在内部调用了`release()`函数将独占式智能指针释放了，不影响内部资源的释放（不会调用内部指向对象的析构函数）。

```cpp
template<class _Tp>
template <class _Yp, class _Dp>
shared_ptr<_Tp>::shared_ptr(unqiue_ptr<_Yp,_Dp>&& __r, class = /**特化条件*/)
:__ptr_(__r.get())
{
  	if(__ptr_ == nullptr)
    {
      	__cntrl_ = nullptr;
    }
  	else
    {
      	typedef typename __shared_ptr_default_allocator<_Yp>::type _AllocT;
      	typedef __shared_ptr_pointer<typename unique_ptr<_Yp, _Dp>::pointer, _Dp, _AllocT > _CntrlBlk;
      	//创建控制块
      	 __cntrl_ = new _CntrlBlk(__r.get(), __r.get_deleter(), _AllocT());
      	//引用计数赋值
      	 __enable_weak_this(__r.get(), __r.get());
    }
  	__r.release();		//释放独占式智能指针
}
```

（3）从原始指针构造出控制块，但是从共享智能指针或者弱引用智能指针创建的时候不创建控制块。

上面源码中已经展示出该方式。需要注意的是，不能使用一个原始指针多个创建一个共享式的智能指针，这样会导致调用多次析构函数，产生异常。

#### 3.2、控制块实现

```cpp
class __shared_count
{
 
protected:
    long __shared_owners_;
    virtual ~__shared_count();
  
public:
  	//显式引用计数初始化。
    explicit __shared_count(long __refs = 0) _NOEXCEPT
        : __shared_owners_(__refs) {}
    
  	//增加共享引用计数
    void __add_shared() noexcept {
        __libcpp_atomic_refcount_increment(__shared_owners_);
      }
};

class __shared_weak_count : private __shared_count
{
  	long __shared_weak_owners_;
public:
  	//构造函数
		explicit __shared_weak_count(long __refs = 0) noexcept
      : __shared_count(refs)
     	  __shared_weak_owners_(refs)
    		{}
protected:
  	virtual ~__shared_weak_count();
  	//纯虚函数
 		virtual void __on_zero_shared() noexcept = 0;
public:
  	//增加共享引用计数
  	void __add_shared() noexcept
    {
      	__shared_count::__add_shared();		//原子增加__shared_owners_
    }
  	//增加弱引用计数
    void __add_weak() noexcept {
      __libcpp_atomic_refcount_increment(__shared_weak_owners_);		//原子增加__shared_weak_owners_
    }
};
```



## 4、弱引用智能指针

弱引用智能指针也有和共享式智能指针一样控制块。需要注意的是，弱引用智能指针只能使用共享式智能指针或者弱引用智能指针构造出来。

```cpp
template<class _Tp>
class weak_ptr
{
public:
    typedef _Tp element_type;
private:
    element_type*        __ptr_;
    __shared_weak_count* __cntrl_;
  	
  
  
public:
  	//构造函数
  	template<class _Yp>
    weak_ptr(shared_ptr<_Yp> const& __r, typename std::enable_if<is_convertible<_Yp*, _Tp*>::value, __nats*>::type) noexcept
    	: __ptr_(__r.__ptr_), __cntrl_(__r.__cntrl_)
    {
        if (__cntrl_)
        		__cntrl_->__add_weak();
    }
};
```

有两个引用计数，一个被共享智能指针使用，一个被弱引用指针使用。

## 5、智能指针的删除函数

主要讨论共享式智能指针的删除函数，和独占式智能指针的删除函数，讨论其使用区别与原理的区别。

共同注意点:

- make函数无法用于指定删除函数。

- 删除函数的参数必须为智能指针指向对象的指针类型。

#### 5.1 独占式智能指针的删除函数

(1)使用方式

考虑使用一个类来测试独占式智能指针。

```cpp
class TestA
{
private:
  	std::string;
};
//可以使用lambda函数来实现。
auto deletor_1 = [](TestA* obj) mutable ->void {
  	std::cout << "deletor_1()" << std::endl;
};
void foo(TestA* obj)
{
  	std::cout << "foo(TestA*)" << std::endl;
}

//使用lambda函数，可以使用decltype来推导lambda的类型。
std::unique_ptr<TestA,decltype(deletor_1)> sp1(new TestA, deletor_1);

//对于全局的函数，可以使用std::function来传入模板
std::unique_ptr<TestA, std::function<void(TestA*)>> sp2(new TestA, foo);
```

(2)实现原理

使用了`__compressed_pair`防止了空类的内存占用。

#### 5.2 共享式智能指针的删除函数

需要注意的是，共享式智能指针的模版类型与删除函数的形式无关，因为删除函数的信息都被控制块管理了。

注意点：

- 1、不管是lambda还是全局函数，或者是类的成员函数，函数的参数必须是智能指针所指向对象的指针类型。

```cpp
class SourcePool{};

//lambda函数
auto deletor = [](SourcePool * ptr) mutable ->void {};
//全局函数
void deletor_global(SourcePool* ptr)
{
  	
}

class A {
public:
  	//类成员函数
  	void deletor_class(SourcePool* ptr)	//与指定对象绑定的。
    {
      	
    }
  	static void deletor_class_static(SourcePool* ptr)	//所有对象共享的。
    {
      
    }
};
```

- 2、绑定方式简单,类成员函数可以使用`std::bind`封装或者使用`lambda表达式封装`。

建议使用`lambda表达式`而不是`std::bind`。

```cpp
//1. lambda函数
std::shared_ptr<SourcePool> sp(new SourcePool, deletor);
//2. 全局函数
std::shared_ptr<SourcePool> sp2(new SourcePool, deletor_global);
//3.类成员函数，可以为静态函数和非静态函数
//3.1 静态成员函数
std::shared_ptr<SourcePool> sp3(new SourcePool, std::bind(&A::deletor_class_static));
//3.2 非静态成员函数,必须捕获一个局部变量
A a;
std::shared_ptr<SourcePool> sp4(new SourcePool, std::bind(&A::deletor_class, a));
```

## 6、智能指针的消耗

#### 6.1 共享式智能指针的消耗

从表面来看，共享式智能指针包含两个指针变量，一个指向堆对象，一个指向控制块。因此这样看来共享式智能指针的消耗为：`16字节`或者`8字节`（不同平台)。

```cpp
std::shared_ptr<SourcePool> sp(new SourcePool);
int size = sizeof(sp);		//16
```

实际上消耗可能更大，这是因为控制块指向的一个堆内存没有被考虑。而控制块的大小取决于删除函数的大小。

因此共享式智能指针的消耗为`>16字节`。

#### 6.2 独占式智能指针的消耗

从源码来看，独占式智能指针的大小等于内部`__compressed_pair`对应的`__ptr_`的大小。其大小有以下特点：

- 1、对于没有指定删除函数的智能指针而言，其大小为`8字节`。
- 2、对于指定的删除函数没有捕获值，其大小为`8字节`。
- 3、对于指定删除函数的智能指针，其大小与捕获值的数量的大小相关。

因此独占式智能指针的消耗大小为`>8字节`，并且与捕获值的大小相关。
