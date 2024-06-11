## 装饰器模式

### 1. 概述
装饰器模式（Decorator Pattern）是一种结构型设计模式，它允许你在运行时为对象动态地添加新的功能，同时不改变其接口。装饰器模式是通过创建一个包装器来包裹原始对象，然后向这个包装器添加需要的功能来实现的.

关键点：  
- Component（抽象组件)  
定义对象的接口，可以给这些对象动态地添加职责。
- ConcreteComponent（具体组件)  
实现Component接口，定义一个可以附加职责的对象。
- Decorator（装饰器抽象类)  
继承Component，持有一个Component对象的引用，并定义一个与Component接口一致的接口。
- ConcreteDecorator（具体装饰器）  
继承Decorator类，向其持有的Component对象添加职责。

基本类图:  
```bash
      +--------------------+
      |     Component      |
      +--------------------+
               ^
               |
               |
      +--------------------+
      | ConcreteComponent  |
      +--------------------+
               ^
               |
               |
      +--------------------+
      |     Decorator      |
      +--------------------+
               ^
               |
               |
      +--------------------+
      | ConcreteDecorator  |
      +--------------------+

```

### 2. 简单实例
```c++

/**
 * @brief 组件抽象基类
 */
class VisualComponent {
public:
    virtual void draw() = 0;
    virtual ~VisualComponent() = default;
};

/**
 * @brief 文本组件
 */
class TextView : public VisualComponent {
public:
    void draw() override {
        std::cout << "Drawing text view" << std::endl;
    }
};

/**
 * @brier 装饰器抽象基类
 */
class Decorator : public VisualComponent {
protected:
   // 拥有指向组件基类指针
    VisualComponent* component;
public:
    Decorator(VisualComponent* component) : component(component) {}

    void draw() override {
        component->draw();
    }
};


/**
 * @brief 加粗装饰器
 */
class BorderDecorator : public Decorator {
public:
    BorderDecorator(VisualComponent* component) : Decorator(component) {}

    void draw() override {
        Decorator::draw();
        drawBorder();
    }

private:
    void drawBorder() {
        std::cout << "Drawing border" << std::endl;
    }
};

/**
 * @brief 滚动装饰器
 */
class ScrollDecorator : public Decorator {
public:
    ScrollDecorator(VisualComponent* component) : Decorator(component) {}

    void draw() override {
        Decorator::draw();
        drawScroll();
    }

private:
    void drawScroll() {
        std::cout << "Drawing scroll" << std::endl;
    }
};
```

### 3. 常用场景
- 图形用户界面（GUI）开发：可以使用装饰器模式来动态地添加边框、滚动条等装饰效果。
- I/O流处理：比如Java中的BufferedReader、BufferedWriter等类就是使用装饰器模式来实现缓冲功能。
- Web开发：可以使用装饰器模式来实现对请求或响应的增强，比如添加日志、验证、缓存等功能。
- 文本处理：比如添加加粗、斜体、下划线等样式装饰。
- 日志记录：可以使用装饰器模式来实现不同层次的日志记录，如简单日志、调试日志、详细日志等。
- 权限控制：可以使用装饰器模式来动态地添加权限控制功能，如身份验证、权限检查等。

这里简单写一个例子：
#### 3.1 日志处理
```c++
#include <iostream>
#include <string>

// 抽象日志记录器接口
class Logger {
public:
    virtual void log(const std::string& message) = 0;
    virtual ~Logger() = default;
};

// 具体日志记录器实现
class ConsoleLogger : public Logger {
public:
    void log(const std::string& message) override {
        std::cout << "Console Logger: " << message << std::endl;
    }
};

// 装饰器抽象类
class LoggerDecorator : public Logger {
protected:
    Logger* logger;
public:
    LoggerDecorator(Logger* logger) : logger(logger) {}

    void log(const std::string& message) override {
        logger->log(message);
    }
};

// 具体装饰器类 - 添加时间戳
class TimestampLogger : public LoggerDecorator {
public:
    TimestampLogger(Logger* logger) : LoggerDecorator(logger) {}

    void log(const std::string& message) override {
        std::string timestamp = getTimestamp();
        LoggerDecorator::log("[" + timestamp + "] " + message);
    }

private:
    std::string getTimestamp() {
        // 实现获取时间戳的逻辑，这里简化为返回固定字符串
        return "2024-06-16 10:00:00";
    }
};

// 具体装饰器类 - 添加日志级别
class LogLevelLogger : public LoggerDecorator {
private:
    std::string level;
public:
    LogLevelLogger(Logger* logger, const std::string& level) : LoggerDecorator(logger), level(level) {}

    void log(const std::string& message) override {
        LoggerDecorator::log("[" + level + "] " + message);
    }
};

int main() {
    // 创建原始日志记录器
    Logger* consoleLogger = new ConsoleLogger();

    // 添加时间戳装饰器
    Logger* timestampLogger = new TimestampLogger(consoleLogger);

    // 添加日志级别装饰器
    Logger* logLevelLogger = new LogLevelLogger(timestampLogger, "INFO");

    // 记录日志
    logLevelLogger->log("Application started.");

    // 释放资源
    delete logLevelLogger;
    delete timestampLogger;
    delete consoleLogger;

    return 0;
}
```

#### 3.2 装饰器处理流
```c++
#include <iostream>
#include <sstream>
#include <string>

// 抽象流装饰器接口
class StreamDecorator {
public:
    virtual void write(const std::string& data) = 0;
    virtual std::string read() = 0;
    virtual ~StreamDecorator() = default;
};

// 具体流装饰器实现 - 加密装饰器
class EncryptionDecorator : public StreamDecorator {
private:
    StreamDecorator* stream;
public:
    EncryptionDecorator(StreamDecorator* stream) : stream(stream) {}

    void write(const std::string& data) override {
        // 在写入数据前进行加密
        std::string encryptedData = encrypt(data);
        stream->write(encryptedData);
    }

    std::string read() override {
        // 在读取数据后进行解密
        std::string encryptedData = stream->read();
        return decrypt(encryptedData);
    }

private:
    std::string encrypt(const std::string& data) {
        // 实现加密逻辑，这里简化为反转字符串
        std::string encryptedData = data;
        std::reverse(encryptedData.begin(), encryptedData.end());
        return encryptedData;
    }

    std::string decrypt(const std::string& encryptedData) {
        // 实现解密逻辑，这里简化为反转字符串
        std::string decryptedData = encryptedData;
        std::reverse(decryptedData.begin(), decryptedData.end());
        return decryptedData;
    }
};

// 具体流装饰器实现 - 压缩装饰器
class CompressionDecorator : public StreamDecorator {
private:
    StreamDecorator* stream;
public:
    CompressionDecorator(StreamDecorator* stream) : stream(stream) {}

    void write(const std::string& data) override {
        // 在写入数据前进行压缩
        std::string compressedData = compress(data);
        stream->write(compressedData);
    }

    std::string read() override {
        // 在读取数据后进行解压缩
        std::string compressedData = stream->read();
        return decompress(compressedData);
    }

private:
    std::string compress(const std::string& data) {
        // 实现压缩逻辑，这里简化为去除空格
        std::string compressedData = data;
        compressedData.erase(std::remove(compressedData.begin(), compressedData.end(), ' '), compressedData.end());
        return compressedData;
    }

    std::string decompress(const std::string& compressedData) {
        // 实现解压缩逻辑，这里简化为添加空格
        std::string decompressedData = compressedData;
        for (size_t i = 1; i < decompressedData.size(); i += 2) {
            decompressedData.insert(i, " ");
        }
        return decompressedData;
    }
};

// 具体流装饰器实现 - 基础流
class BaseStream : public StreamDecorator {
private:
    std::stringstream stream;
public:
    void write(const std::string& data) override {
        stream << data;
    }

    std::string read() override {
        return stream.str();
    }
};

int main() {
    // 创建基础流
    StreamDecorator* baseStream = new BaseStream();

    // 添加加密装饰器
    StreamDecorator* encryptedStream = new EncryptionDecorator(baseStream);

    // 添加压缩装饰器
    StreamDecorator* compressedStream = new CompressionDecorator(encryptedStream);

    // 写入数据
    compressedStream->write("Hello, World!");

    // 读取数据
    std::cout << "Data read from stream: " << compressedStream->read() << std::endl;

    // 释放资源
    delete compressedStream;
    delete encryptedStream;
    delete baseStream;
    return 0;
}
```

#### 3.3 装饰器权限控制
```c++
#include <iostream>
#include <string>

// 抽象组件接口
class Component {
public:
    virtual void operation() = 0;
    virtual ~Component() = default;
};

// 具体组件实现
class ConcreteComponent : public Component {
public:
    void operation() override {
        std::cout << "Performing operation" << std::endl;
    }
};

// 装饰器抽象类
class Decorator : public Component {
protected:
    Component* component;
public:
    Decorator(Component* component) : component(component) {}

    void operation() override {
        if (checkPermission()) {
            component->operation();
        } else {
            std::cout << "Permission denied" << std::endl;
        }
    }

    virtual bool checkPermission() {
        // 默认实现为允许操作
        return true;
    }
};

// 具体装饰器类 - 权限控制装饰器
class PermissionDecorator : public Decorator {
private:
    bool hasPermission;
public:
    PermissionDecorator(Component* component, bool hasPermission) : Decorator(component), hasPermission(hasPermission) {}

    bool checkPermission() override {
        return hasPermission;
    }
};

int main() {
    // 创建原始组件
    Component* component = new ConcreteComponent();

    // 添加权限控制装饰器
    Component* permissionDecorator = new PermissionDecorator(component, false);

    // 执行操作
    permissionDecorator->operation();

    // 释放资源
    delete permissionDecorator;
    delete component;

    return 0;
}

```
