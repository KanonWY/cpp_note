#include <iostream>
#include <chrono>

class Timer
{
public:
    Timer()
        :m_begin(std::chrono::high_resolution_clock::now())
    {}

    void reset()
    {
        m_begin = std::chrono::high_resolution_clock::now();
    }

    int64_t elapsed() const
    {
        return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now()- m_begin).count();
    }
    
    int64_t elapsed_ms() const 
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()- m_begin).count();
    }

    int64_t elapsed_us() const
    {
        return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()- m_begin).count();
    }

    int64_t elapsed_ns() const
    {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now()- m_begin).count();
    }

    int64_t elapsed_min() const
    {
        return std::chrono::duration_cast<std::chrono::minutes>(std::chrono::high_resolution_clock::now()- m_begin).count();
    }

    int64_t elapsed_hour() const
    {
        return std::chrono::duration_cast<std::chrono::hours>(std::chrono::high_resolution_clock::now()- m_begin).count();
    }


private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_begin;
};


int main()
{
    Timer t;

    for(int64_t i = 0; i < 100000000; ++i)
    {
        std::cout << "*";
    }
    std::cout << std::endl;
    std::cout << t.elapsed_hour()  << " hours" << std::endl;
    std::cout << t.elapsed_min() << " mins" << std::endl;
    std::cout << t.elapsed() << " seconds" << std::endl;
    std::cout << t.elapsed_ms() << " ms" << std::endl;
    std::cout << t.elapsed_us() << " us" <<  std::endl;
    std::cout << t.elapsed_ns() << " ns" << std::endl;
    return 0;
}
