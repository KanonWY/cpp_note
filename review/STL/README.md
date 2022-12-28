## STL基础

本文档的STL代码是基于`MaxOS13.0.SDK`。不同平台编译器的实现似乎都不太一样。

- clang/llvm
- Gnu/gcc
- Msvc


### 1、vector


### 2、map
map的底层数据结构为红黑树，因此其插入、删除、查找的时间复杂读为`logN`。并且基础的map的key不允许
重复。

#### 2.1 基本数据结构

```cpp
template <class _Key, class _Tp, class _Compare = less<_Key>,
          class _Allocator = allocator<pair<const _Key, _Tp> > >
class  map {
public:
  	typedef _Key                                     key_type;
    typedef _Tp                                      mapped_type;
    typedef pair<const key_type, mapped_type>        value_type;
    typedef __identity_t<_Compare>                   key_compare;
    typedef __identity_t<_Allocator>                 allocator_type;
    typedef value_type&                              reference;
    typedef const value_type&                        const_reference;
  
private:
    typedef __tree<__value_type, __vc, __allocator_type>   __base;
  
    __base __tree_;		//底层的红黑树数据结构
};
```

#### 2.2 查找操作

根据输入的KEY在红黑树上查找元素，如果存在返回对应的迭代器，如果失败的话就返回`end()`迭代器。	

```cpp
//输入为一个key
//返回为一个迭代器
iterator find(const key_type& __k)             {return __tree_.find(__k);}

//底层实现
typename __tree<_Tp, _Compare, _Allocator>::iterator
__tree<_Tp, _Compare, _Allocator>::find(const _Key& __v)
{
    iterator __p = __lower_bound(__v, __root(), __end_node());		//查找返回一个迭代器
    if (__p != end() && !value_comp()(__v, *__p))
        return __p;		//找到了返回该迭代器
    return end();			//找不到返回一个end()迭代器
}
```

#### 2.3 插入操作

- insert插入

`insert`一个键值对，会返回一个迭代器与`bool`组合的`pair`，其中`bool`表示成功插入。

```cpp
std::map<int,int> m_map;
m_map.insert({1,2});

//插入一个键值对，返回一个迭代器和bool的pair
std::pair<iterator, bool>
    insert(value_type&& __v) {return __tree_.__insert_unique(std::move(__v));}

//走参数为右值这个特化路径
pair<iterator, bool> __insert_unique(__container_value_type&& __v) {
        return __emplace_unique_key_args(_NodeTypes::__get_key(__v), _VSTD::move(__v));
}
```

`__emplace_unique_key_args`底层会去查找是否有与键`__k`相等的节点，如果存在返回该节点，然后说明原key已经存在了,即使新插入的`value`不相等也不会修改原本的值。

`__emplace_unique_key_args`函数

```cpp
template <class _Tp, class _Compare, class _Allocator>
template <class _Key, class... _Args>
pair<typename __tree<_Tp, _Compare, _Allocator>::iterator, bool>
__tree<_Tp, _Compare, _Allocator>::__emplace_unique_key_args(_Key const& __k, _Args&&... __args)
{
    __parent_pointer __parent;
    __node_base_pointer& __child = __find_equal(__parent, __k);		//是否找到key等的node
    __node_pointer __r = static_cast<__node_pointer>(__child);
    bool __inserted = false;
    if (__child == nullptr)	//没找到则新插入
    {
        __node_holder __h = __construct_node(_VSTD::forward<_Args>(__args)...);
        __insert_node_at(__parent, __child, static_cast<__node_base_pointer>(__h.get()));
        __r = __h.release();
        __inserted = true;	//新插入
    }
    return pair<iterator, bool>(iterator(__r), __inserted);
}
```

- operation []插入

```cpp
template <class _Key, class _Tp, class _Compare, class _Allocator>
_Tp&
map<_Key, _Tp, _Compare, _Allocator>::operator[](const key_type& __k)
{
    return __tree_.__emplace_unique_key_args(__k,
        _VSTD::piecewise_construct,
        _VSTD::forward_as_tuple(__k),
        _VSTD::forward_as_tuple()).first->__get_value().second;
}
```

`__emplace_unique_key_args`函数在操作符号重载的情况下还是调用了。

#### 2.4 删除操作

有两种方式：

（1）遍历迭代器，删除指定key的node

```cpp
//（1）
std::map<int, int> m_map;
for(int i = 0; i < 10; ++i)
{
  	m_map.insert({i, pow(2,i)});
}
int deleteKey = 4;
for(auto it = m_map.begin(); it != m_map.end();)
{
  	if(it->first == deleteKey){
      	it = m_map.erase(it);		//删除节点
    } else {
      	it++;
    }
}
//底层对应实现为：
iterator erase(iterator __p)       {return __tree_.erase(__p.__i_);}

template <class _Tp, class _Compare, class _Allocator>
typename __tree<_Tp, _Compare, _Allocator>::iterator
__tree<_Tp, _Compare, _Allocator>::erase(const_iterator __p)
{
    __node_pointer __np = __p.__get_np();				//获取节点指针
    iterator __r = __remove_node_pointer(__np);	//删除np节点，返回一下一个节点
    __node_allocator& __na = __node_alloc();
    __node_traits::destroy(__na, _NodeTypes::__get_ptr(
        const_cast<__node_value_type&>(*__p)));
    __node_traits::deallocate(__na, __np, 1);
    return __r;	//返回下一个迭代器
}
```

（2）根据指定的key，删除该node。然后返回0或者1。

```cpp
std::map<int, int> m_map;
for(int i = 0; i < 10; ++i)
{
  	m_map.insert({i, pow(2,i)});
}
int deleteKey = 4;
int res = m_map.erase(deleteKey);

//底层实现为：根据deleKey找到对应迭代器，如果找到删除该迭代器，然后返回1，如果没找到返回0
size_type erase(const key_type& __k)
		{return __tree_.__erase_unique(__k);}

template <class _Tp, class _Compare, class _Allocator>
template <class _Key>
typename __tree<_Tp, _Compare, _Allocator>::size_type
__tree<_Tp, _Compare, _Allocator>::__erase_unique(const _Key& __k)
{
    iterator __i = find(__k);
    if (__i == end())			//没找到
        return 0;
    erase(__i);						//找到删除
    return 1;		
}
```

#### 2.5 修改操作

使用`operator[]`即可实现。

#### 2.6 查看某一个key是否存在

使用`count(key)`函数，如果返回0表示不存在，返回1表示存在。

```cpp
std::map<int, int> m_map;
for(int i = 0; i < 10; ++i)
{
  	m_map.insert({i, pow(2,i)});
}
int key = 4;
int res = m_map.count(key);		//查看key=4是否存在

//count(key_type key)内部实现
size_type count(const key_type& __k) const
	{return __tree_.__count_unique(__k);}

template <class _Tp, class _Compare, class _Allocator>
template <class _Key>
typename __tree<_Tp, _Compare, _Allocator>::size_type
__tree<_Tp, _Compare, _Allocator>::__count_unique(const _Key& __k) const
{
    __node_pointer __rt = __root();	//获取根节点
    while (__rt != nullptr)
    {
        if (value_comp()(__k, __rt->__value_))		//左比较
        {
            __rt = static_cast<__node_pointer>(__rt->__left_);
        }
        else if (value_comp()(__rt->__value_, __k))		//右比较
            __rt = static_cast<__node_pointer>(__rt->__right_);
        else
            return 1;	//找到了
    }
    return 0;		//没找到
}
```

### 3、unordered_map

`unordered_map`的底层数据结构为哈希表（拉链实现），其查找的时间复杂度为logN。

#### 3.1 基本数据结构

```cpp
template <class _Key, class _Tp, class _Hash = hash<_Key>, class _Pred = equal_to<_Key>,
          class _Alloc = allocator<pair<const _Key, _Tp> > >
class _LIBCPP_TEMPLATE_VIS unordered_map
{
public:
    // types
    typedef _Key                                           key_type;
    typedef _Tp                                            mapped_type;
    typedef __identity_t<_Hash>                            hasher;
    typedef __identity_t<_Pred>                            key_equal;
    typedef __identity_t<_Alloc>                           allocator_type;
    typedef pair<const key_type, mapped_type>              value_type;
    typedef value_type&                                    reference;
    typedef const value_type&                              const_reference;
  

    typedef __hash_table<__value_type, __hasher,
                         __key_equal,  __allocator_type>   __table;

    __table __table_;		//底层的哈希表
};
```

#### 3.2 查找操作

`find(key_type key)`函数，查找指定的key的迭代器，返回该迭代器。如果没找到返回end()。

```cpp
iterator       find(const key_type& __k)       {return __table_.find(__k);}

template <class _Tp, class _Hash, class _Equal, class _Alloc>
template <class _Key>
typename __hash_table<_Tp, _Hash, _Equal, _Alloc>::iterator
__hash_table<_Tp, _Hash, _Equal, _Alloc>::find(const _Key& __k)
{
    size_t __hash = hash_function()(__k);  //计算出哈希值
    size_type __bc = bucket_count();			 //获取哈希桶的数量
    if (__bc != 0)
    {
        size_t __chash = __constrain_hash(__hash, __bc);			//找到桶下标
        __next_pointer __nd = __bucket_list_[__chash];			  //找到对应的拉链
        if (__nd != nullptr)
        {
            for (__nd = __nd->__next_; __nd != nullptr &&
                (__nd->__hash() == __hash
                  || __constrain_hash(__nd->__hash(), __bc) == __chash);
                                                           __nd = __nd->__next_)
            {
                if ((__nd->__hash() == __hash)
                    && key_eq()(__nd->__upcast()->__value_, __k))	//如果拉链中的node的key与我们find等
#if _LIBCPP_DEBUG_LEVEL == 2
                    return iterator(__nd, this);
#else
                    return iterator(__nd);		//返回该node
#endif
            }
        }
    }
    return end();		//返货end()
}
```

#### 3.3 插入操作



### 4、list

list底层的数据结构是双端链表。头部、尾部插入的时间复杂度为O(1),查找的时间复杂为O(n)。

#### 4.1 基本数据结构

```cpp
template <class _Tp, class _Alloc /*= allocator<_Tp>*/>
class _LIBCPP_TEMPLATE_VIS list
    : private __list_imp<_Tp, _Alloc>
{
    typedef __list_imp<_Tp, _Alloc> base;
    typedef typename base::__node              __node;
    typedef typename base::__node_allocator    __node_allocator;
    typedef typename base::__node_pointer      __node_pointer;
    typedef typename base::__node_alloc_traits __node_alloc_traits;
    typedef typename base::__node_base         __node_base;
    typedef typename base::__node_base_pointer __node_base_pointer;
    typedef typename base::__link_pointer __link_pointer;
    
};

template <class _Tp, class _Alloc>
class __list_imp {

};
```



#### 4.2 插入操作

#### 4.2 查找操作

#### 4.3 删除操作

