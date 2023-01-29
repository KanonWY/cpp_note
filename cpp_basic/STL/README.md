## STL基础

本文档的STL代码是基于`MaxOS13.0.SDK`。不同平台编译器的实现似乎都不太一样。

- clang/llvm
- Gnu/gcc
- Msvc

经常使用的容器有，`vector`,`map`,`unorder_map`,`set`,`unorder_set`,`list`。

### 1、vector

vector底层实现为动态数组，会根据插入数据的容量进行动态变化。

#### 1.1 vector构造函数

支持构造N个元素的vector。

```cpp
template <class _Tp, class _Allocator>
vector<_Tp, _Allocator>::vector(size_type __n, const value_type& __x)
{
    if (__n > 0)
    {
        __vallocate(__n);
        __construct_at_end(__n, __x);
    }
}

std::vector<int> m_vec(10, 2);
//m_vec有十个元素，每一个元素都是2
```

#### 1.2 容量影响函数

- resize
- reserve

- shrink_to_fit
- erase
- insert

当容器发生扩容的时候，会根据元素的本身的构造函数的存在型进行不同的操作。在C++11之前，不存在移动构造语句，在该情况下，会导致元素的批量复制操作，产生大量不必要的消耗。在C++11之后，对于有移动语义的元素，会调用移动语句，减少不必要的消耗。需要注意的是，元素的移动语义必须是`noexcept`的。

#### 1.3 `resize函数`

`resize`函数，该函数改变容器的`size`大小(**可以扩容，但是不会缩容**)。

```cpp
template <class _Tp, class _Allocator>
void
vector<_Tp, _Allocator>::resize(size_type __sz, const_reference __x)
{
    size_type __cs = size();
    if (__cs < __sz)
        this->__append(__sz - __cs, __x);
    else if (__cs > __sz)
        this->__destruct_at_end(this->__begin_ + __sz);
}
```

| resize(size)函数的作用是修改size的大小 |                                                            |
| -------------------------------------- | ---------------------------------------------------------- |
| 如果 size > sz, 且 size < cap          | 不进行扩容，将size - sz个数据在容器中填充，修改sz = size。 |
| 如果 size < sz, 且 size > cap          | 此操作会进行扩容操作，然后进行移动，然后构造新增填充。     |
| 如果 sz > size                         | 将sz - size个元素析构掉。                                  |
| 如果原本的cap = size                   | 则不会做任何操作。                                         |

#### 1.4 `reserve函数`

`reserve`函数，该函数改变容器的`cap`大小。

```cpp
template <class _Tp, class _Allocator>
void
vector<_Tp, _Allocator>::reserve(size_type __n)
{
    if (__n > capacity())
    {
        allocator_type& __a = this->__alloc();
        __split_buffer<value_type, allocator_type&> __v(__n, size(), __a);
        __swap_out_circular_buffer(__v);
    }
}
```

| reserve(size_type)函数的作用是修改cap的大小 |                                            |
| ------------------------------------------- | ------------------------------------------ |
| 如果size > cap                              | 容器会进行扩容操作，然后将原来的数据移动。 |
| 如果size <= cap                             | 容器不进行任何操作。                       |

#### 1.5 `shrink_to_fit函数`

对容器的容量进行修改，只有在`cap`大于`size`的时候才会生效，将容器的容量与size调整为一样的大小。

```cpp
template <class _Tp, class _Allocator>
void
vector<_Tp, _Allocator>::shrink_to_fit() _NOEXCEPT
{
    if (capacity() > size())		//容量大大于size的时候才会生效。
    {
#ifndef _LIBCPP_NO_EXCEPTIONS
        try
        {
#endif // _LIBCPP_NO_EXCEPTIONS
            allocator_type& __a = this->__alloc();
            __split_buffer<value_type, allocator_type&> __v(size(), size(), __a);
            __swap_out_circular_buffer(__v);
#ifndef _LIBCPP_NO_EXCEPTIONS
        }
        catch (...)
        {
        }
#endif // _LIBCPP_NO_EXCEPTIONS
    }
}
```

#### 1.6 `clear函数`

 `clear()`函数的作用是清空元素的内容并且改变`size`的大小，但是不改变容器的容量。

```cpp
void clear() _NOEXCEPT
{
    size_type __old_size = size();
    __base::clear();
    __annotate_shrink(__old_size);
    __invalidate_all_iterators();
}
```

### 2、map

map的底层数据结构为红黑树，因此其插入、删除、查找的时间复杂读为`logN`。并且基础的map的key不允许
重复。

`map`中常见的操作为：

（1）查找指定key的元素。

（2）插入键值对。

（3）删除某一个迭代器或者指定key的值。

（4）修改某一个键对应的值。

（5）检查某一个key是否存在。

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
  
    __base __tree_;     //底层的红黑树数据结构
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
    iterator __p = __lower_bound(__v, __root(), __end_node());      //查找返回一个迭代器
    if (__p != end() && !value_comp()(__v, *__p))
        return __p;     //找到了返回该迭代器
    return end();           //找不到返回一个end()迭代器
}
```

#### 2.3 插入操作

- insert插入

`insert`一个键值对，会返回一个迭代器与`bool`组合的`pair`，其中`bool`表示成功插入。

如果元素在容器中存在，返回`<iterator, false>`。

如果元素不存在，就插入，然后返回`<iterator, true>`。

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
    __node_base_pointer& __child = __find_equal(__parent, __k);     //是否找到key等的node
    __node_pointer __r = static_cast<__node_pointer>(__child);
    bool __inserted = false;
    if (__child == nullptr) //没找到则新插入
    {
        __node_holder __h = __construct_node(_VSTD::forward<_Args>(__args)...);
        __insert_node_at(__parent, __child, static_cast<__node_base_pointer>(__h.get()));
        __r = __h.release();
        __inserted = true;  //新插入
    }
    return pair<iterator, bool>(iterator(__r), __inserted);
}
```

- operation []插入

注意[]与`insert`的区别:本质是修改，但是它**默认有插入**的语义。

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

有两种方式：传入参数为迭代器或者指定的key值。

当删除指定迭代器时，返回下一个迭代器值。

当删除指定key值时候，返回删除节点的数量，如果没有该值返回0，存在该值返回删除值的数量。

（1）遍历迭代器，删除指定key的迭代器，返回下一个迭代器。

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
        it = m_map.erase(it);       //删除节点
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
    __node_pointer __np = __p.__get_np();               //获取节点指针
    iterator __r = __remove_node_pointer(__np); //删除np节点，返回一下一个节点
    __node_allocator& __na = __node_alloc();
    __node_traits::destroy(__na, _NodeTypes::__get_ptr(
        const_cast<__node_value_type&>(*__p)));
    __node_traits::deallocate(__na, __np, 1);
    return __r; //返回下一个迭代器
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
    if (__i == end())           //没找到
        return 0;
    erase(__i);                     //找到删除
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
int res = m_map.count(key);     //查看key=4是否存在

//count(key_type key)内部实现
size_type count(const key_type& __k) const
    {return __tree_.__count_unique(__k);}

template <class _Tp, class _Compare, class _Allocator>
template <class _Key>
typename __tree<_Tp, _Compare, _Allocator>::size_type
__tree<_Tp, _Compare, _Allocator>::__count_unique(const _Key& __k) const
{
    __node_pointer __rt = __root(); //获取根节点
    while (__rt != nullptr)
    {
        if (value_comp()(__k, __rt->__value_))      //左比较
        {
            __rt = static_cast<__node_pointer>(__rt->__left_);
        }
        else if (value_comp()(__rt->__value_, __k))     //右比较
            __rt = static_cast<__node_pointer>(__rt->__right_);
        else
            return 1;   //找到了
    }
    return 0;       //没找到
}
```

### 3、unordered_map

`unordered_map`的底层数据结构为哈希表（拉链实现），其查找的时间复杂度为`O(1)`，使用此数据结构是为了使用其优秀的查找能力。哈希表的内存消耗是大于红黑树的。

哈希表的核心操作有以下：

（1）查找某一个key是否存在，返回其迭代器；使用`[]`获取元素。

（2）插入一个键-值对，返回`<iterator, bool>`键值对。

（3）修改某一个key的值，使用`[]操作符来修改对应key的值`。

（4）查看某一个key是否存在。

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

    __table __table_;       //底层的哈希表
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
    size_type __bc = bucket_count();             //获取哈希桶的数量
    if (__bc != 0)
    {
        size_t __chash = __constrain_hash(__hash, __bc);            //找到桶下标
        __next_pointer __nd = __bucket_list_[__chash];            //找到对应的拉链
        if (__nd != nullptr)
        {
            for (__nd = __nd->__next_; __nd != nullptr &&
                (__nd->__hash() == __hash
                  || __constrain_hash(__nd->__hash(), __bc) == __chash);
                                                           __nd = __nd->__next_)
            {
                if ((__nd->__hash() == __hash)
                    && key_eq()(__nd->__upcast()->__value_, __k))   //如果拉链中的node的key与我们find等
#if _LIBCPP_DEBUG_LEVEL == 2
                    return iterator(__nd, this);
#else
                    return iterator(__nd);      //返回该node
#endif
            }
        }
    }
    return end();       //返货end()
}
```

#### 3.3 插入操作

插入一个键值对，然后返回一个`<iterator, bool>`pair对象。第一个参数是插键值对应的迭代器，第二个参数是是否参入成功，如果插入成功，返回true，如果该key已经存在则返回该key迭代器与false。因此**插入操作并不会修改原有的值**。

#### 3.4 删除操作

与`map`的接口类似，也有两种删除方式。

(1)指定某一个key值删除，然后删除的值的个数，成功返回1，失败返回0。

(2)删除指定的迭代器，删除成功返回下一个迭代器。

### 4、list

list底层的数据结构是双端链表。头部、尾部插入的时间复杂度为O(1),查找的时间复杂为O(n)。使用`list`是为了获取其优秀的节点增删能力，节点的增删不会对其他节点产生影响。

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
};
```

#### 4.2 插入操作

- 在头部和尾部插入。

`push_front()`,`push_back()`

- 在指定迭代器位置之后插入。

`insert(iterator, value_type)`

#### 4.2 查找操作

需要注意的是，查找查找的时间复杂度较高(`O(n)`)，需要遍历整个链表。

list本身没有内置find函数，可以自己使用迭代器遍历，也可以使用标准库的find函数。

```cpp
std::list<int> l;
for(int i = 0; i < 10; ++i)
{
  	l.push_back(i+1);
}
//使用std::find
auto it = std::find(l.begin(), l.end(), 8);
//自己遍历
for(auto item = l.begin(); item != l.end(); ++item)
{
  	if(*item == 8)
    {
      	it = item;
      	break;
    }
}
```

#### 4.3 删除操作

list给定的接口只能删除某一个迭代器,返回接下来的一个迭代器。

```cpp
template <class _Tp, class _Alloc>
typename list<_Tp, _Alloc>::iterator
list<_Tp, _Alloc>::erase(const_iterator __p)
{};
```

因此如果需要删除某一个值，需要找到该值对应的迭代器，然后删除该迭代器。

```cpp
//删除值为8的
std::list<int> l;
for(int i = 0; i < 10; ++i)
{
  	l.push_back(i+1);
}
//使用std::find
auto it = std::find(l.begin(), l.end(), 8);
it = l.erase(it);		//删除之后返回下一个迭代器。
```