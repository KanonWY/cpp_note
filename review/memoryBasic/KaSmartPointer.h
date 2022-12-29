#include <cstddef>
#include <iostream>

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
