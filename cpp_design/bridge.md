## Bridge

### 1. 原理
桥接模式的核心思想是将抽象和实现分离，具体实现这一分离的步骤如下：
- 1、定义抽象接口和实现接口：  
抽象接口定义了高层的业务逻辑，而实现接口定义了具体的实现细节。
- 2、实现具体的抽象和实现类：  
具体的抽象类扩展抽象接口，并持有实现接口的引用。  
具体的实现类实现实现接口，提供具体的实现细节。  
- 3、通过组合来桥接抽象和实现：  
抽象类通过组合的方式持有实现接口的引用，从而将实现细节委托给具体的实现类。

典型类图:
```bash
            +---------------+                      +-------------------+
            |   Abstraction  |<------------------- |    Implementor    |
            |  +operation()  |                     |  +operationImpl() |
            +-------+-------+                      +---------+---------+
                    |                                     |
                    |                                     |
           +-------------+---------+                +--------------|-----------+
           |   Refined Abstraction  |               |    Concrete Implementor   |
           |  +operation()          |               |  +operationImpl()         |
           +-----------------------+                 +-------------------------+

```
### 2. 抽象接口和实现接口
抽象接口（Abstraction）：定义了高层业务逻辑。  
实现接口（Implementor）：定义了具体的实现细节。  
```c++
/**
 * @brief 实现接口
 */
class Implementor {
public:
    virtual void operationImpl() = 0;
    virtual ~Implementor() = default;
};

/**
 * @brief 抽象接口
 */
class Abstraction {
protected:
    // 持有实现接口
    Implementor* implementor;
public:
    Abstraction(Implementor* impl) : implementor(impl) {}
    virtual void operation() = 0;
    virtual ~Abstraction() = default;
};
```
### 3. 具体实现类
具体实现类（ConcreteImplementor）：实现了实现接口，提供具体的实现。
```c++
/**
 * @brief 具体实现A
 */
class ConcreteImplementorA : public Implementor {
public:
    void operationImpl() override {
        std::cout << "ConcreteImplementorA operation implementation" << std::endl;
    }
};

/**
 * @brief 具体实现B
 */
class ConcreteImplementorB : public Implementor {
public:
    void operationImpl() override {
        std::cout << "ConcreteImplementorB operation implementation" << std::endl;
    }
};

```
### 3. 扩展抽象类(桥接类)
扩展抽象类（RefinedAbstraction）：扩展了抽象接口，具体实现了高层业务逻辑。
```c++
class RefinedAbstraction : public Abstraction {
public:
    RefinedAbstraction(Implementor* impl) : Abstraction(impl) {}
    void operation() override {
        implementor->operationImpl();
    }
};

```
### 4. 使用代码
```c++
int main() {
    Implementor* implA = new ConcreteImplementorA();
    Implementor* implB = new ConcreteImplementorB();

    Abstraction* absA = new RefinedAbstraction(implA);
    Abstraction* absB = new RefinedAbstraction(implB);

    absA->operation(); // 输出: ConcreteImplementorA operation implementation
    absB->operation(); // 输出: ConcreteImplementorB operation implementation

    delete absA;
    delete absB;
    delete implA;
    delete implB;

    return 0;
}
```
### 5. 解释
在桥接模式中，桥接类（Bridge Class）实际上是抽象类（Abstraction）和实现接口（Implementor）之间的组合。桥接模式的关键在于通过组合而不是继承来实现两个独立的层次：一个是抽象部分，一个是实现部分。  
在具体实现中，抽象类和其子类共同作为桥接类的一部分，而实现接口及其实现类构成了另一部分。通过这种设计，抽象类和实现类可以独立变化，而不会相互影响。  
桥接模式中的类角色  
- Abstraction（抽象类）：定义高层次的操作，维护对实现部分的引用。
- Refined Abstraction（扩展抽象类）：扩展抽象类，增加额外的操作。
- Implementor（实现接口）：定义实现类的接口。
- Concrete Implementor（具体实现类）：实现实现接口，提供具体的实现细节。

### 6. 一些示例
操作系统跨平台操作
```c++

// 定义实现接口
class OperatingSystem {
public:
    virtual void start() = 0;
    virtual void shutdown() = 0;
    virtual ~OperatingSystem() = default;
};


// 定义具体实现类
class WindowsOS : public OperatingSystem {
public:
    void start() override {
        std::cout << "Windows OS started." << std::endl;
    }

    void shutdown() override {
        std::cout << "Windows OS shutdown." << std::endl;
    }
};

class LinuxOS : public OperatingSystem {
public:
    void start() override {
        std::cout << "Linux OS started." << std::endl;
    }

    void shutdown() override {
        std::cout << "Linux OS shutdown." << std::endl;
    }
};

// 定义抽象接口, 持有抽象实现接口
class Computer {
protected:
    OperatingSystem* os;
public:
    Computer(OperatingSystem* os) : os(os) {}

    virtual void start() = 0;
    virtual void shutdown() = 0;
    virtual ~Computer() {
        delete os;
    }
};

// 扩展抽象类
class Desktop : public Computer {
public:
    Desktop(OperatingSystem* os) : Computer(os) {}

    void start() override {
        std::cout << "Desktop computer started." << std::endl;
        os->start();
    }

    void shutdown() override {
        std::cout << "Desktop computer shutdown." << std::endl;
        os->shutdown();
    }
};

class Laptop : public Computer {
public:
    Laptop(OperatingSystem* os) : Computer(os) {}

    void start() override {
        std::cout << "Laptop started." << std::endl;
        os->start();
    }

    void shutdown() override {
        std::cout << "Laptop shutdown." << std::endl;
        os->shutdown();
    }
};

// 简单使用
int main() {
    // windows使用
    OperatingSystem* windows = new WindowsOS();
    // 抽象类实现
    Computer* desktop = new Desktop(windows);
    desktop->start();
    desktop->shutdown();

    OperatingSystem* linux = new LinuxOS();
    Computer* laptop = new Laptop(linux);
    laptop->start();
    laptop->shutdown();

    delete desktop;
    delete laptop;

    return 0;
}

```
桥接实现不同类型的日志器，使之输出到不同的地方
```c++
#include <iostream>
#include <string>
#include <memory>

// 抽象日志记录器实现
class LoggerImpl {
public:
    virtual void log(const std::string& message) = 0;
    virtual ~LoggerImpl() = default;
};

// 具体日志记录器实现 - 文件日志记录器
class FileLogger : public LoggerImpl {
public:
    void log(const std::string& message) override {
        std::cout << "Logging to file: " << message << std::endl;
    }
};

// 具体日志记录器实现 - 控制台日志记录器
class ConsoleLogger : public LoggerImpl {
public:
    void log(const std::string& message) override {
        std::cout << "Logging to console: " << message << std::endl;
    }
};

// 抽象日志记录器类
class Logger {
protected:
    std::shared_ptr<LoggerImpl> impl;
public:
    Logger(std::shared_ptr<LoggerImpl> impl) : impl(impl) {}

    virtual void log(const std::string& message) = 0;
    virtual ~Logger() = default;
};

// 具体日志记录器类 - 普通日志记录器
class SimpleLogger : public Logger {
public:
    SimpleLogger(std::shared_ptr<LoggerImpl> impl) : Logger(impl) {}

    void log(const std::string& message) override {
        impl->log("[INFO] " + message);
    }
};

// 具体日志记录器类 - 警告日志记录器
class WarningLogger : public Logger {
public:
    WarningLogger(std::shared_ptr<LoggerImpl> impl) : Logger(impl) {}

    void log(const std::string& message) override {
        impl->log("[WARNING] " + message);
    }
};

int main() {
    std::shared_ptr<Logger> fileLogger = std::make_shared<SimpleLogger>(std::make_shared<FileLogger>());
    std::shared_ptr<Logger> consoleLogger = std::make_shared<WarningLogger>(std::make_shared<ConsoleLogger>());

    fileLogger->log("File log message");
    consoleLogger->log("Console log message");

    return 0;
}
```
电子设备与遥控器
```c++

#include <iostream>
#include <string>
#include <memory>

// 抽象电子设备接口
class Device {
public:
    virtual void powerOn() = 0;
    virtual void powerOff() = 0;
    virtual ~Device() = default;
};

// 具体电子设备类 - 电视
class TV : public Device {
public:
    void powerOn() override {
        std::cout << "TV is powered on." << std::endl;
    }

    void powerOff() override {
        std::cout << "TV is powered off." << std::endl;
    }
};

// 具体电子设备类 - 音响
class Stereo : public Device {
public:
    void powerOn() override {
        std::cout << "Stereo is powered on." << std::endl;
    }

    void powerOff() override {
        std::cout << "Stereo is powered off." << std::endl;
    }
};

// 抽象遥控器类
class RemoteControl {
protected:
    std::shared_ptr<Device> device;
public:
    RemoteControl(std::shared_ptr<Device> device) : device(device) {}

    virtual void powerOn() = 0;
    virtual void powerOff() = 0;
    virtual ~RemoteControl() = default;
};

// 具体遥控器类 - 简单遥控器
class SimpleRemoteControl : public RemoteControl {
public:
    SimpleRemoteControl(std::shared_ptr<Device> device) : RemoteControl(device) {}

    void powerOn() override {
        device->powerOn();
    }

    void powerOff() override {
        device->powerOff();
    }
};

int main() {
    std::shared_ptr<Device> tv = std::make_shared<TV>();
    std::shared_ptr<RemoteControl> tvRemote = std::make_shared<SimpleRemoteControl>(tv);

    tvRemote->powerOn();
    tvRemote->powerOff();

    std::shared_ptr<Device> stereo = std::make_shared<Stereo>();
    std::shared_ptr<RemoteControl> stereoRemote = std::make_shared<SimpleRemoteControl>(stereo);

    stereoRemote->powerOn();
    stereoRemote->powerOff();

    return 0;
}


```




