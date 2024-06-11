## Factory
- 工厂方法：类的成员函数，用于创建对象，通常可以替代成员函数。
- 工厂：一个类，它知道如何创建工厂。如果传入一个可调用对象，这个可调用对象可以创建一个对象，那么这个可调用对象也称为工厂。
- 抽象工厂：抽象可以被具体的类继承，因而产生了一系列的工厂类族（应用较少).

### 1. 工厂类的优势
- 工厂可以拒绝创建对象（某种条件下）
- 工厂方法可以是多态的
- 工厂方法的命名不受约束规范
- 可以实现缓存或者其他存储优化
- 可以将对象不同的关注点内存封装

### 2. Factory Method
工厂方法，在类中嵌入静态方法，返回对象本身.
```c++
struct Point {
    float x, y;
    Point(float a, float b)
        :x(a), y(b)
    {}

    static Point NewCartesian(float x, float y)
    {
        return {x,y};
    }

    static Point NewPolar(float x, float ,y)
    {
        return {r * cos(theta), r * sin(theta)};
    }
};
```

### 3. Factory
将类的创建方法与类本身隔离开，使用一个友元类来创建对象.
```c++
struct Point {
    float x;
    float y;

    friend class PointFactory;
private:
    // 私有则自己无法创建
    Point(float x, float y) :x(x), y(y) {}
};
```

```c++
class PointFactory {
    static Point NewCartesian(float x, float y)
    {
        return {x,y};
    }

    static Point NewPolar(float x, float ,y)
    {
        return {r * cos(theta), r * sin(theta)};
    }
};
```

### 4. 内部 factory
其他没有友元函数或者友元类的语言，java、C# 使用内部工厂达到与 Factory 同样的效果.
显然 C++ 也可以这么做.  
将工厂类定义在对象类的内部，然后将工厂类定义为对象类的一个 static 成员函数即可。
```c++
class Point {
private:
    Point(float x, float y)
        :x(x), y(y)
    {}
    class Factory {
    private:
        Factory() = default;
    public:
        static Point NewCartesian(float x, float y)
        {
            return {x,y};
        }

        static Point NewPolar(float x, float ,y)
        {
            return {r * cos(theta), r * sin(theta)};
        }
    };
private:
    float x, float y;
    inline static Factory Factory = {};
};
```

### 5. 抽象工厂
前面的几种工厂，我们一直在研究单个对象的构造。有时，你可能会参与对象家族的创建。这实际上是一个非常罕见的情况，因此与工厂方法和普通的旧工厂模式不同，抽象工厂是一种只在复杂系统中弹出的模式.

```c++

/**
 * @brief 热饮抽象基类
 */
struct HotDrink {
    virtual void prepare(int volume) = 0;
};

struct Tea: HotDrink {
    void prepare(int volume) override {
        /// 茶的准备
    };
};


struct Coffe: HotDrink {
    void prepare(int volume) override {
        /// 咖啡的准备
    }
};

// TODO: 这里的 string 当然可以使用枚举来表示
std::unique_ptr<HotDrink> make_drink(std::string type, int volume)
{
    std::unique_ptr<HotDrink> drink;
    if(type = "tea")
    {
        drink = std::make_unique<Tea>(); 
    }
    else if (type = "coffe")
    {
        drink = std::make_unique<coffe>();
    }
    drink -> prepare(volume);
    return drink;
}
```
