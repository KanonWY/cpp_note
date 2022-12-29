#include <iostream>
#include <limits>

template <typename T> class MyAllocator {
  public:
    using value_type = T;
    using pointer = T *;
    using const_pointer = const T *;
    using void_pointer = void *;
    using const_void_pointer = const void *;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;

    MyAllocator() = default;
    ~MyAllocator() = default;

  public:
    pointer allocate(size_type numObjs) {
        std::cout << "MyAllocator::allocate,内存分配:" << numObjs << std::endl;
        allocaCount += numObjs;
        return static_cast<pointer>(operator new(sizeof(T) * numObjs));
    }

    void deallocate(pointer p, size_type numObjs) {
        std::cout << "MyAllocator::deallocate,内存释放:" << numObjs
                  << std::endl;
        allocaCount -= numObjs;
        operator delete(p);
    }

    size_type max_size() const { return std::numeric_limits<size_type>::max(); }

    template <class U, class... Args> void construct(U *p, Args &&... args) {
        std::cout << "construct()" << std::endl;
        new (p) U(std::forward<Args>(args)...);
    }

    template <class U, class... Args> void destroy(U *p) { p->~U(); }

    template <class U> struct rebind { using other = MyAllocator<U>; };

    size_type get_allocations() const { return allocaCount; }

  private:
    size_type allocaCount;
};