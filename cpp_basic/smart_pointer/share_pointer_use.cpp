#include <memory>
#include <string>
#include <iostream>


constexpr int POOLSIZE = 10;

class SourcePool {
public:
    SourcePool() {
        _pool_size = POOLSIZE;
        _pool      = new char[_pool_size];
    }

    // copy delete
    SourcePool(const SourcePool& rhs)            = delete;
    SourcePool& operator=(const SourcePool& rhs) = delete;
    // allow move
    SourcePool(SourcePool&& rhs) {
        if (&rhs != this) {
            _pool          = rhs._pool;
            rhs._pool      = nullptr;
            _pool_size     = rhs._pool_size;
            rhs._pool_size = 0;
        }
    }

    SourcePool& operator=(SourcePool&& rhs) {
        if (&rhs != this) {
            _pool          = rhs._pool;
            rhs._pool      = nullptr;
            _pool_size     = rhs._pool_size;
            rhs._pool_size = 0;
        }
        return *this;
    }

    void echo() {
        std::cout << "pool_size = " << _pool_size << std::endl;
    }

private:
    char*  _pool;
    size_t _pool_size = POOLSIZE;
};


int main() {

    auto sp = std::make_shared<SourcePool>();
    sp->echo();
    return 0;
}
