#include <spdlog/logger.h>
#include <cstddef>
#include <memory>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>

#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/spdlog.h"

class KaString
{
public:
    KaString() = default;

    KaString(const char* str)
    {
        auto len = std::strlen(str);
        __ptr_ = static_cast<char*>(calloc(len + 1, sizeof(char)));
        __len_ = len;
        __cap_ = len + 1;
        memcpy(__ptr_, str, len);
    }

    KaString(const KaString& rhs)
    {
        std::cout << "KaString copy" << std::endl;
        if (&rhs != this)
        {
            this->__len_ = rhs.__len_;
            this->__ptr_ = static_cast<char*>(malloc(this->__len_));
            memcpy(this->__ptr_, rhs.__ptr_, this->__len_);
        }
    }

    KaString& operator=(const KaString& rhs)
    {
        std::cout << "KaString copy s" << std::endl;
        if (&rhs != this)
        {
            this->__len_ = rhs.__len_;
            this->__ptr_ = static_cast<char*>(malloc(this->__len_));
            memcpy(this->__ptr_, rhs.__ptr_, this->__len_);
        }
        return *this;
    }

    virtual ~KaString()
    {
        if (this->__ptr_ != nullptr)
        {
            free(this->__ptr_);
            this->__ptr_ = nullptr;
            this->__len_ = 0;
        }
    }

    KaString(KaString&& rhs) noexcept
    {
        std::cout << "KaString move" << std::endl;
        if (&rhs != this)
        {
            this->__ptr_ = std::move(rhs.__ptr_);
            this->__len_ = rhs.__len_;
            rhs.__ptr_ = nullptr;
            rhs.__len_ = 0;
        }
    }

    KaString& operator=(KaString&& rhs) noexcept
    {
        std::cout << "KaString move s" << std::endl;
        if (&rhs != this)
        {
            this->__ptr_ = std::move(rhs.__ptr_);
            this->__len_ = rhs.__len_;
            rhs.__ptr_ = nullptr;
            rhs.__len_ = 0;
        }
        return *this;
    }

    size_t size() { return __len_; }

    size_t size() const { return __len_; }

    const char* c_str() { return __ptr_; }

    char* c_str() const { return __ptr_; }

private:
    char* __ptr_ = nullptr;
    size_t __len_ = 0;
    size_t __cap_ = 0;
};

class Base
{
public:
    Base() = default;

    Base(const char* str) : m_str(str)
    {
        std::cout << "Base(const char* str)" << std::endl;
    }

    Base(std::string rhs) : m_str(std::move(rhs))
    {
        std::cout << "Base(std::string rhs)" << std::endl;
    }

    Base(const Base& rhs) : m_str(rhs.m_str)
    {
        std::cout << "copy" << std::endl;
    }
    Base& operator=(const Base& rhs)
    {
        std::cout << "copy s" << std::endl;
        m_str = rhs.m_str;
        return *this;
    }
    virtual ~Base() {}

    Base(Base&& rhs)
    {
        std::cout << "move" << std::endl;
        if (&rhs != this)
        {
            rhs.m_str = std::move(rhs.m_str);
        }
    }

    Base& operator=(Base&& rhs)
    {
        std::cout << "move s" << std::endl;
        if (&rhs != this)
        {
            m_str = std::move(rhs.m_str);
        }
        return *this;
    }

    void print() { std::cout << "str = " << m_str.c_str() << std::endl; }

private:
    std::string m_str;
};

auto sink = std::make_shared<spdlog::sinks::stdout_sink_st>();
auto mylogger = std::make_shared<spdlog::logger>("mylogger", sink);

int main()
{
    mylogger->info("---------------->");
    mylogger->error("--------------->");
    return 0;
}