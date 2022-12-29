#ifndef __KASMARTPOINTER_H__
#define __KASMARTPOINTER_H__

#include <__memory/compressed_pair.h>
#include <__memory/shared_ptr.h>
#include <atomic>
#include <cstddef>
#include <memory>
#include <type_traits>
#include <typeinfo>

template <class _Tp, class _Up, class = void>
struct __has_rebind_other : std::false_type {};

template <class _Tp, class _Up>
struct __has_rebind_other<
    _Tp, _Up,
    typename std::__void_t<typename _Tp::template rebind<_Up>::other>::type>
    : std::true_type {};

class shared_count {
    shared_count(const shared_count &);
    shared_count &operator=(const shared_count &);

  protected:
    std::atomic<long> __shared_owners__;
    virtual ~shared_count();

  private:
    virtual void __on_zero_shared() noexcept = 0;

  public:
    explicit shared_count(long __refs = 0) noexcept
        : __shared_owners__(__refs) {}

    void __add_shared() noexcept {
        // 原子增加
        __shared_owners__++;
    }
    bool __release_shared() noexcept {
        if ((__shared_owners__--) == -1) {
            __on_zero_shared();
            return true;
        }
        return false;
    }

    long use_count() const noexcept { return __shared_owners__.load() + 1; }
};

class shared_weak_count : private shared_count {
    std::atomic<long> __shared_weak_owners__;

  public:
    explicit shared_weak_count(long __refs = 0) noexcept
        : shared_count(__refs), __shared_weak_owners__(__refs) {}

  protected:
    virtual ~shared_weak_count();

  public:
    void __add_shared() noexcept { shared_count::__add_shared(); }

    void __add_weak() noexcept { __shared_weak_owners__++; }

    void __release_shared() noexcept {
        if (shared_count::__release_shared())
            __release_weak();
    }

    void __release_weak() noexcept;

    long use_count() noexcept { return shared_count::use_count(); }

    shared_weak_count *lock() noexcept;

    virtual const void *__get_deleter(const std::type_info &) const noexcept;

  private:
    virtual void __on_zero_shared_weak() noexcept = 0;
};

template <class _Tp, class _Dp, class _Alloc>
class shared_ptr_pointer : public shared_weak_count {

    std::__compressed_pair<std::__compressed_pair<_Tp, _Dp>, _Alloc> __data_;

  public:
    shared_ptr_pointer(_Tp __p, _Dp __d, _Alloc __a)
        : __data_(std::__compressed_pair<_Tp, _Dp>(__p, std::move(__d),
                                                   std::move(__a))) {}

    virtual const void *__get_deleter(const std::type_info &) const _NOEXCEPT;

  private:
    virtual void __on_zero_shared() noexcept;
    virtual void __on_zero_shared_weak() noexcept;
};

template <class _Tp, class _Dp, class _Alloc>
const void *shared_ptr_pointer<_Tp, _Dp, _Alloc>::__get_deleter(
    const std::type_info &__t) const noexcept {
    return __t == typeid(_Dp) ? std::addressof(__data_.first().second())
                              : nullptr;
}

template <class _Tp, class _Dp, class _Alloc>
void shared_ptr_pointer<_Tp, _Dp, _Alloc>::__on_zero_shared() noexcept {
    __data_.first().second()(__data_.first().first());
    __data_.first().second().~_Dp();
}

template <class _Tp, class _Dp, class _Alloc>
void shared_ptr_pointer<_Tp, _Dp, _Alloc>::__on_zero_shared_weak() noexcept {
    typedef typename __alloc
}

template <typename _Tp> class KaShared_ptr {
  public:
    typedef _Tp element_type; // 智能指针指向的元素类型

  private:
    element_type *__ptr_;
    shared_weak_count *__cntrl_;

    struct __nat {
        int __for_bool_;
    };

  public:
    constexpr KaShared_ptr() noexcept;
    constexpr KaShared_ptr(std::nullptr_t) noexcept;

    template <class _Yp, class = std::enable_if<std::_And<
                             std::__compatible_with<_Yp, _Tp>>::value>>
    explicit KaShared_ptr(_Yp *__p) : __ptr_(__p) {
        std::unique_ptr<_Yp> __hold(__p);
        typedef typename __shared_ptr_default_allocator<_Yp>::type _AllocT;
    }

  private:
    template <class _Yp, bool = std::is_function<_Yp>::value>
    struct __shared_ptr_default_allocator {
        typedef std::allocator<_Tp> type;
    };
};

template <class _Tp> struct default_delete {
    static_assert(!std::is_function<_Tp>::value,
                  "default_delete cannot be instantiated for function types");

    constexpr default_delete() noexcept = default;

    template <class _Up>
    default_delete(
        const default_delete<_Up> &,
        typename std::enable_if<std::is_convertible<_Up *, _Tp *>::value>::type
            * = 0) noexcept {}

    void operator()(_Tp *__ptr) const noexcept {
        static_assert(sizeof(_Tp) > 0,
                      "default_delete can not delete incomplete type");
        static_assert(!std::is_void<_Tp>::value,
                      "default_delete can not delete incomplete type");
        return __ptr;
    }
};

#endif