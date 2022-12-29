#include <iostream>
#include <memory>
#include <string>
#include <type_traits>

// DOC https://cplusplus.com/reference/type_traits/enable_if/

// 限制模板函数的参数类型
// 在某些场景下，我们需要实现只有特定类型可以调用的模板函数。
template <typename T>
typename std::enable_if<std::is_unsigned<T>::value, std::string>::type
DecimalToBinary(T t) {
    std::string str{"unsigned"};
    return str;
}

template <typename T>
typename std::enable_if<!std::is_unsigned<T>::value, std::string>::type
DecimalToBinary(T t) {
    std::string str{"signed"};
    return str;
}

// make sure the inergral's T can call example function!
// template <typename T,
//           typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
// void example(T t) {
//     std::cout << "T is intergral" << std::endl;
// }
template <typename T, std::enable_if_t<std::is_integral<T>::value, int> = 0>
void example(T t) {
    std::cout << "T is integral" << std::endl;
}

/**
 *
 *  template <bool Cond, class T, class F> struct conditional;
 * **/

int testEnable_if() {
    unsigned int a = 200;
    int b = 399;
    std::cout << DecimalToBinary(a) << std::endl;
    std::cout << DecimalToBinary(b) << std::endl;
    return 0;
}

// 在使用模板编程时，可以利用std::enable_if的特性根据模板参数的不同特性进行不同的类型选择。
template <typename T>
struct is_smart_pointer_helper : public std::false_type {};

template <typename T>
struct is_smart_pointer_helper<std::shared_ptr<T>> : public std::true_type {};

template <typename T>
struct is_smart_pointer_helper<std::weak_ptr<T>> : public std::true_type {};

template <typename T>
struct is_smart_pointer_helper<std::unique_ptr<T>> : public std::true_type {};

template <typename T>
struct is_smart_pointer
    : public is_smart_pointer_helper<typename std::remove_cv<T>::type> {};

template <typename T>
typename std::enable_if<is_smart_pointer<T>::value, void>::type
check_smart_pointer(const T &t) {
    std::cout << "is smart pointer" << std::endl;
}

template <typename T>
typename std::enable_if<!is_smart_pointer<T>::value, void>::type
check_smart_pointer(const T &t) {
    std::cout << "is not smart pointer" << std::endl;
}

void testSmartPointer() {
    std::unique_ptr<int> usp = std::make_unique<int>(199);
    std::shared_ptr<int> sp_2(std::move(usp));
    int *p = new int(1000);
    check_smart_pointer(p);
    check_smart_pointer(sp_2);
    std::unique_ptr<int> sp3(new int(2000));
}

int main() {
    testSmartPointer();
    return 0;
}