## 适配器

### 1. 概述
适配器模式（Adapter Pattern）是一种结构型设计模式，它用于将一个类的接口转换成客户端期望的另一个接口，使得原本由于接口不兼容而不能一起工作的类可以一起工作。适配器模式通过`引入一个新的适配器类`，使得客户端可以`通过适配器间接调用被适配的类`，从而解决接口不匹配的问题。  

关键点：  

- 目标接口：客户端所期待的使用接口
- 需要被适配的类：一个现有接口（或者老的接口），这个接口需要被适配。
- 适配器：实现目标接口，将目标接口的方法转换为被适配类的原始方法。

实现：  
适配器模式通常持有一个到被适配对象的引用，通过适配器对象将请求委托给被适配对象。

典型类图:
```bash
    目标接口
  +-----------+      +---------+
  |   Target  |      | Adaptee |
  +-----------+      +---------+
        ^                ^
        |                |
  +-----------+      +---------+
  |  Adapter  |----->| Adaptee |
  +-----------+      +---------+
     适配器         需要被适配的类

```

### 2. 简单示例
```c++
/**
 * @brief 目标接口, 
 */
class Target {
public:
    virtual void request() = 0;
    virtual ~Target() = default;
};

/**
 * @brief 旧的类或者需要被适配的类
 */
class Adaptee {
public:
    void specificRequest() {
        std::cout << "Adaptee specific request." << std::endl;
    }
};

/**
 * @brief 适配器类
 */
class Adapter : public Target {
private:
    // 持有需要被适配的对象的指针或者引用, 这里可以使用智能指针等简化管理，或者内部调用工厂生成
    Adaptee* adaptee;

public:
    Adapter(Adaptee* adaptee) : adaptee(adaptee) {}
    // 重写虚函数，调用被适配的对象的接口
    void request() override {
        adaptee->specificRequest();
    }
};
```

eon 内部为了兼容不同的中间件，采用适配器模式来适配不同的中间件接口,提供了统一的发布订阅接口。

数据库连接适配:
SQLite 适配 Mysql 的接口
```c++

#include <iostream>
#include <string>
#include <memory>

// 抽象数据库连接接口
class DatabaseConnection {
public:
    virtual void connect() = 0;
    virtual void disconnect() = 0;
    virtual void executeQuery(const std::string& query) = 0;
    virtual ~DatabaseConnection() = default;
};

// 具体MySQL数据库连接类
class MySqlConnection : public DatabaseConnection {
public:
    void connect() override {
        std::cout << "Connected to MySQL database." << std::endl;
    }

    void disconnect() override {
        std::cout << "Disconnected from MySQL database." << std::endl;
    }

    void executeQuery(const std::string& query) override {
        std::cout << "Executing query in MySQL database: " << query << std::endl;
    }
};

/**
 * @brief 需要被适配的 sqlite 类
 */ 
class SqliteConnection {
public:
    void open() {
        std::cout << "Connected to SQLite database." << std::endl;
    }

    void close() {
        std::cout << "Disconnected from SQLite database." << std::endl;
    }

    void query(const std::string& query) {
        std::cout << "Executing query in SQLite database: " << query << std::endl;
    }
};

/**
 * @brief Sqlite适配器
 */
class SqliteToMysqlAdapter : public DatabaseConnection {
private:
    // 持有 sqlite 原始接口的智能指针，其实这里独式的指针更为合适
    std::shared_ptr<SqliteConnection> sqliteConnection;
public:
    SqliteToMysqlAdapter(std::shared_ptr<SqliteConnection> sqliteConnection) : sqliteConnection(sqliteConnection) {}

    void connect() override {
        sqliteConnection->open();
    }

    void disconnect() override {
        sqliteConnection->close();
    }

    void executeQuery(const std::string& query) override {
        sqliteConnection->query(query);
    }
};

int main() {
    // 使用MySQL数据库连接
    std::shared_ptr<DatabaseConnection> mysqlConnection = std::make_shared<MySqlConnection>();
    mysqlConnection->connect();
    mysqlConnection->executeQuery("SELECT * FROM table");
    mysqlConnection->disconnect();

    // 使用SQLite数据库连接（通过适配器）
    std::shared_ptr<SqliteConnection> sqliteConnection = std::make_shared<SqliteConnection>();
    std::shared_ptr<DatabaseConnection> sqliteAsMysqlConnection = std::make_shared<SqliteToMysqlAdapter>(sqliteConnection);
    sqliteAsMysqlConnection->connect();
    sqliteAsMysqlConnection->executeQuery("SELECT * FROM table");
    sqliteAsMysqlConnection->disconnect();

    return 0;
}
```


