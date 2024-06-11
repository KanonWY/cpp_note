#include <algorithm>
#include <chrono>
#include <set>
#include <string>
#include <iostream>
#include <thread>

/**
 * @brief 订阅器
 */
template <typename T>
struct Observer
{
    // 纯虚接口
    virtual void filed_changed(T& obj, const std::string& filed_name) = 0;
};

/**
 * @brief 可被观察者
 */
template <typename T>
struct Observable
{
private:
    std::set<Observer<T>*> m_obervers;

public:
    void notify(T& source, const std::string& name)
    {
        for (auto obs : m_obervers)
        {
            obs->filed_changed(source, name);
        }
    }
    void subscribe(Observer<T>* f) { m_obervers.insert(f); }
    void unsubscriber(Observer<T>* f)
    {
        m_obervers.erase(std::remove(m_obervers.begin(), m_obervers.end(), f),
                         m_obervers.end());
    }
};

/**
 * @brief 实际的被观测者对象需要 CRTP 于 Observabl, 然后在属性的 set
 * 函数中决定触发属性设置.
 */
struct Person : Observable<Person>
{
    void set_age(int a)
    {
        if (this->age == a) return;
        this->age = a;
        notify(*this, "age");
    }

    int get_age() { return age; }

    int age = 0;
};

/**
 * @brief 实际的观测者
 */
struct ConsolPersonObserver : Observer<Person>
{
    void filed_changed(Person& obj, const std::string& filed_name) override
    {
        std::cout << "Person's " << filed_name << " has changed to "
                  << obj.get_age() << std::endl;
    }
};

int main()
{
    Person p1;

    ConsolPersonObserver printer;

    p1.subscribe(&printer);

    for (int i = 1; i < 11; ++i)
    {
        p1.set_age(i);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
