## chrono库处理时间
三种核心的类型，时间间隔`duration`,时钟`clocks`,时间点`time point`。  

### 1、duration
时间间隔表示一段时间间隔，用来记录时间长短，可以表示几秒钟，几分钟，几个小时。
```cpp
template <class Rep, class Period = std::ratio<1>>
class duration;
```
ratio表示一个时钟周期的秒数，`ratio<2>`表示一个时钟周期是2秒，`ratio<60>`表示一分钟,`ratio<60*60>`表示一个小时。
`ratio<1/1000>`表示1毫秒，`ratio<1/1000000>`表示1微秒，`ratio<1/1000000000>`表示1纳秒。  
为了方式显示，使用了以下重定义。
```cpp
std::chrono::nanoseconds    //纳秒
std::chrono::microseconds   //微秒
std::chrono::milliseconds   //毫秒
std::chrono::seconds        //秒
std::chrono::minutes        //分钟
std::chrono::hours          //小时
```
获取时间间隔的接口:
```cpp
constexpr rep count() const;
```
`duration`之间可以进行加减计算。
```cpp
#include <iostream>
#include <chrono>

int main()
{
    std::chrono::minutes t1(10);    //600s
    std::chrono::seconds t2(10);    //10s
    auto diff = t1 - t2;            //590s
    std::cout << diff.count() << " s" << std::endl;     // 590 s
    return 0;
}
```


### 2、time_point
表示时间点的类,它被实现为一个存储Duration类型自`Clock`的纪元开始的时间间隔的值。
```cpp
template <class Clock, class Duration = typename Clock::duration>
class time_point;
// Clock 时间点在此时钟上计时
// Duration 用于计算从时间点起的std::chrono::duration的值
```
### 3、clock
时钟，获取当前的系统时间的时钟类，包含三种：
- system_clock: 系统时钟。
- steady_clock: 固定的时钟，秒表或者计时器。
- high_resolution_clock: 固定的时钟。

##### system_clock
`system_clock`提供了对当前时间点的访问，将获取的时间点转换为`time_t`类型的时间对象，基于该对象获取时间信息。
```cpp

struct system_clock {

    //返回当前时间点
    static time_point now() noexcept
    {
        return time_point(duration(_Xtime_get_ticks()));
    }
    
    //将时间点转换为std::time_t类型 
    static __time64_t to_time_t(const time_point& _Time) noexcept
    {
        return duration_cast<seconds>(_Time.time_since_epoch()).count();
    }

    //将time_t类型转换为时间点
    static time_point from_time_t(__time64_t _Tm) noexcept
    {
        return time_point(seconds(_Tm));
    }

};
```












