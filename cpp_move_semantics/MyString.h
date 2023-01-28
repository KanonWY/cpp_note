#ifndef __KASTRING__H__
#define __KASTRING__H__

#include <cstddef>
#include <cstring>
#include <iostream>
#include <ostream>
#include <type_traits>

class KaString {

private:
    char*  _str = nullptr;
    size_t len  = 0;

public:
    KaString() {
        std::cout << "KaString Default" << std::endl;
    }

    KaString(const char* p) {
        std::cout << "KaString(const char*p): " << p << std::endl;
        len  = strlen(p) + 1;
        _str = new char[len];
        memcpy(_str, p, len);
        _str[len - 1] = 0;
    }

    // copy
    KaString(const KaString& rhs) {
        if (&rhs != this) {
            len  = rhs.len;
            _str = new char[len];
            memcpy(_str, rhs._str, len);
        }
        std::cout << "copy cs: " << _str << std::endl;
    }

    KaString& operator=(const KaString& rhs) {
        if (&rhs != this) {
            len  = rhs.len;
            _str = new char[len];
            memcpy(_str, rhs._str, len);
        }
        std::cout << "copy =: " << _str << std::endl;
        return *this;
    }

    // move
    KaString(KaString&& mhs) noexcept {
        if (&mhs != this) {
            if (_str) delete[] _str;
            _str     = mhs._str;
            len      = mhs.len;
            mhs.len  = 0;
            mhs._str = nullptr;
        }
        std::cout << "move cs: " << _str << std::endl;
    }

    KaString& operator=(KaString&& mhs) noexcept {
        if (&mhs != this) {
            if (_str)
                delete[] _str;
            _str     = mhs._str;
            len      = mhs.len;
            mhs._str = nullptr;
            mhs.len  = 0;
        }
        std::cout << "move =: " << _str << std::endl;
        return *this;
    }

    char* c_str() const {
        return _str;
    }

    char* c_str() {
        return _str;
    }

    size_t size() {
        return len;
    }

    size_t size() const {
        return len;
    }

    ~KaString() {

        if (_str) {
            std::cout << "~KaString: " << _str << std::endl;
            delete[] _str;
            len  = 0;
            _str = nullptr;
        }
        else {
            std::cout << "~KaString: " << std::endl;
        }
    }
};


class KaString_nomove {

private:
    char*  _str = nullptr;
    size_t len  = 0;

public:
    KaString_nomove() {
        std::cout << "KaString Default" << std::endl;
    }

    KaString_nomove(const char* p) {
        std::cout << "KaString(const char*p): " << p << std::endl;
        len  = strlen(p) + 1;
        _str = new char[len];
        memcpy(_str, p, len);
        _str[len - 1] = 0;
    }

    // copy
    KaString_nomove(const KaString_nomove& rhs) {
        if (&rhs != this) {
            len  = rhs.len;
            _str = new char[len];
            memcpy(_str, rhs._str, len);
        }
        std::cout << "copy cs: " << _str << std::endl;
    }

    KaString_nomove& operator=(const KaString_nomove& rhs) {
        if (&rhs != this) {
            len  = rhs.len;
            _str = new char[len];
            memcpy(_str, rhs._str, len);
        }
        std::cout << "copy =: " << _str << std::endl;
        return *this;
    }


    char* c_str() const {
        return _str;
    }

    char* c_str() {
        return _str;
    }

    ~KaString_nomove() {

        if (_str) {
            std::cout << "~KaString_nomove: " << _str << std::endl;
            delete[] _str;
            len  = 0;
            _str = nullptr;
        }
        else {
            std::cout << "~KaString_nomove: " << std::endl;
        }
    }
};


#endif