#include <__memory/compressed_pair.h>
#include <cstddef>
#include <iostream>
#include <type_traits>

template <typename T> class KaUnique_ptr {
  public:
    using value = T;
    using pointer = T *;

    KaUnique_ptr(const KaUnique_ptr &rhs) = delete;

    KaUnique_ptr &operator=(const KaUnique_ptr &rhs) = delete;

    KaUnique_ptr() : __data_ptr_(nullptr){};

    KaUnique_ptr(pointer p) : __data_ptr_(p) {}

    KaUnique_ptr(KaUnique_ptr &&rhs) {
        __data_ptr_ = std::move(rhs.__data_ptr_);
        rhs.__data_ptr_ = nullptr;
    }

    KaUnique_ptr &operator=(KaUnique_ptr &&rhs) {
        __data_ptr_ = std::move(rhs.__data_ptr_);
        rhs.__data_ptr_ = nullptr;
    }

    value &operator*() { return *__data_ptr_; }

    pointer operator->() { return __data_ptr_; }

  private:
    value *__data_ptr_;
};

template <class T, class... Args>
KaUnique_ptr<T> make_kaUnique_ptr(Args... args) {
    return KaUnique_ptr<T>(new T(std::forward<Args>(args)...));
}

// 默认删除函数
template <class T> struct ka_default_delete {
    static_assert(!std::is_function<T>::value,
                  "default_delete cannot be instantiated for function types");

    constexpr ka_default_delete() noexcept = default;

    template <class _Up>
    ka_default_delete(
        const ka_default_delete<_Up> &,
        typename std::enable_if<std::is_convertible<_Up *, T *>::value>::type
            * = 0) noexcept {}

    void operator()(T *__ptr) const noexcept {
        static_assert(sizeof(T) > 0,
                      "default_delete can not delete incomplete type");
        static_assert(!std::is_void<T>::value,
                      "default_delete can not delete incomplete type");
        delete __ptr;
    }
};

template <class _Tp, class = void> struct __has_pointer : std::false_type {};
template <class _Tp>
struct __has_pointer<_Tp, typename std::__void_t<typename _Tp::pointer>::type>
    : std::true_type {};

template <class _Tp, class _Alloc,
          class _RawAlloc = typename std::remove_reference<_Alloc>::type,
          bool = __has_pointer<_RawAlloc>::value>
struct __pointer {
    using type = typename _RawAlloc::pointer;
};

template <class _Tp, class _Dp = ka_default_delete<_Tp>> class ka_unique_ptr {
  public:
    typedef _Tp element_type;
    typedef _Dp deleter_type;
    typedef typename __pointer<_Tp, deleter_type>::type pointer;

    static_assert(!std::is_rvalue_reference<deleter_type>::value,
                  "the specified deleter type cannot be an rvalue reference");

  private:
    std::__compressed_pair<pointer, deleter_type> __ptr_;
    struct __nat {
        int __for_bool_;
    };
};