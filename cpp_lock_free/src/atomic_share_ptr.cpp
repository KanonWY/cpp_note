#include <atomic>
#include <memory>
#include <iostream>
#include <thread>

void producer(std::atomic<std::shared_ptr<int>>& atomic_ptr)
{
    auto new_data = std::make_shared<int>(42);
    atomic_ptr.store(new_data);
    std::cout << "Produced: " << *new_data << std::endl;
}

void consumer(std::atomic<std::shared_ptr<int>>& atomic_ptr)
{
    std::shared_ptr<int> data = atomic_ptr.load();
    if (data)
    {
        std::cout << "Consumed: " << *data << std::endl;
    }
    else
    {
        std::cout << "No data available" << std::endl;
    }
}

int main()
{
    std::atomic<std::shared_ptr<int>> atomic_ptr;

    std::thread t1(producer, std::ref(atomic_ptr));
    std::thread t2(consumer, std::ref(atomic_ptr));

    t1.join();
    t2.join();

    return 0;
}
