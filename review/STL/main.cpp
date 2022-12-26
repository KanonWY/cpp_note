#include <algorithm>
#include <cstddef>
#include <cstring>
#include <iostream>
// #include <malloc/_malloc.h>
#include <memory>
#include <ostream>
#include <set>
#include <utility>
#include <vector>

class STLBasic {
  public:
    STLBasic(int x) : val(x) { std::cout << "STLBasic(int x)" << std::endl; }

    STLBasic(const STLBasic &rhs) {
        val = rhs.val;
        std::cout << "copy" << std::endl;
    }

    STLBasic(STLBasic &&obj) noexcept {
        val = std::move(obj.val);
        std::cout << "move" << std::endl;
    }

    ~STLBasic() { std::cout << "~ST:STLBasic" << std::endl; }

    int get_val() const { return val; }
    friend std::ostream &operator<<(std::ostream &os, const STLBasic &rhs);

  private:
    int val = 0;
};

std::ostream &operator<<(std::ostream &os, const STLBasic &rhs) {
    os << rhs.val;
    return os;
}

class KaString {
  public:
    KaString() {
        len = 0;
        _source = nullptr;
    }

    KaString(const char *str) {
        len = sizeof(str);
        _source = static_cast<char *>(malloc(len));
        memcpy(_source, str, len);
    }

    KaString(const KaString &obj) {
        std::cout << "copy" << std::endl;
        if (&obj != this) {
            len = obj.len;
            _source = static_cast<char *>(malloc(len));
            memcpy(_source, obj._source, len);
        }
    }

    KaString &operator=(const KaString &obj) {
        if (&obj != this) {
            len = obj.len;
            _source = static_cast<char *>(malloc(len));
            memcpy(_source, obj._source, len);
        }
        return *this;
    }

    KaString(KaString &&obj) noexcept {
        std::cout << "move" << std::endl;
        if (&obj != this) {
            len = obj.len;
            _source = obj._source;
            obj._source = nullptr;
            obj.len = 0;
        }
    }

    KaString &operator=(KaString &&obj) noexcept {
        if (&obj != this) {
            len = obj.len;
            _source = obj._source;
            obj._source = nullptr;
            obj.len = 0;
        }
        return *this;
    }

    char *c_str() { return _source; }

  private:
    size_t len = 0;
    char *_source;
};

class TestMove { // std::move转换的时候保留const
  public:
    TestMove(const KaString text) : _str(std::move(text)) {}

  private:
    KaString _str;
};

class Investment {};
class Stock : public Investment {};
class Bond : public Investment {};
class RealEstate : public Investment {};

void test() {
    auto f = [](STLBasic *p) {
        std::cout << "M:" << p->get_val() << std::endl;
        std::cout << "LLLLL" << std::endl;
    };
    std::shared_ptr<STLBasic> sp(new STLBasic(3999), f);
}

void test2() {
    auto f = [](STLBasic *p) {
        std::cout << "M:" << p->get_val() << std::endl;
        std::cout << "LLLLL" << std::endl;
    };

    std::unique_ptr<STLBasic, decltype(f)> usp(new STLBasic(4399), f);
}

int main2() {

    auto raw = new STLBasic(200);

    auto sp10 = std::make_unique<STLBasic>(3333);

    std::unique_ptr<STLBasic> sp12(new STLBasic(44444));

    std::cout << "sizeof(uq) = " << sizeof(sp12) << std::endl;

    std::shared_ptr<STLBasic> sp3(std::move(sp10)); //

    auto sp335 = std::make_shared<STLBasic>(3000);
    std::shared_ptr<STLBasic> sp1(raw);
    // std::shared_ptr<STLBasic> sp2(raw);

    std::weak_ptr<STLBasic> sw(sp1);

    std::cout << "weak size = " << sizeof(sw) << std::endl;

    return 0;
}

std::multiset<std::string> names;

template <typename T> void logAndAdd(T &&name) {
    std::cout << "1" << std::endl;
    names.emplace(std::forward<T>(name));
}

std::string nameFromIdx(int idx) { return "ppp"; }
void logAndAdd(int idx) {
    std::cout << "2" << std::endl;
    names.emplace(nameFromIdx(idx));
}

int main() {
    short a = 22;
    logAndAdd(a);
    return 0;
}
