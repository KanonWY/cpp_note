## 组合模式

### 1. 概念
组合模式（Composite Pattern）是一种结构型设计模式，允许你将对象组合成树形结构以表示“部分-整体”的层次结构。组合模式使得客户端能够统一地对待单个对象和组合对象。  
组合模式的核心点主要包括以下几个方面：  

- 统一接口：  
组合模式定义了一个统一的接口，使得客户端可以一致地处理单个对象和组合对象
- 递归结构：  
组合模式以递归结构表示对象的部分-整体关系，即组合对象可以包含其他组合对象或叶子对象
- Leaf（叶子）和Composite（容器）：  
组合模式通过两种对象类型来构建树形结构。Leaf 表示叶子节点，它没有子节点，而 Composite 表示容器节点，它可以包含子节点
- 透明性：  
组合模式可以实现透明性，即客户端无需区分处理单个对象还是组合对象，统一使用组件接口来操作对象
- 简化客户端代码：  
组合模式可以简化客户端代码，客户端无需关心对象的层次结构，只需通过统一的接口来操作对象即可


`组合模式的核心是通过统一的接口和递归结构来表示对象的部分-整体关系`，从而简化系统的设计和维护。

### 2. 适用范围
- 1、树形结构：  
当对象之间存在部分-整体关系，并且这些对象形成了树形结构时，组合模式是一个很好的选择。
例如，文件系统、组织架构、菜单、结构体对象等。

- 2、统一接口:  
当客户端需要统一地对待单个对象和组合对象时，可以使用组合模式。组合模式通过统一的接口来操作对象，使得客户端代码更加简洁和可维护。

- 3、递归结构：  
当对象之间存在递归结构，即对象可以包含其他对象的情况下，组合模式是很自然的选择。通过递归地组合对象，可以轻松地表示复杂的层次结构。

- 4、共享公共行为:  
当组合对象和叶子对象需要共享公共行为时，可以使用组合模式。组合模式可以在抽象类中定义公共行为，然后在具体子类中实现。

处理 JSON 或者 yaml 对象时，可以采用组合模式:
```c++
// 组件接口
class JSONValue {
public:
    virtual void print() = 0;
    virtual ~JSONValue() = default;
};

// 叶子节点：字符串
class JSONString : public JSONValue {
private:
    std::string value;
public:
    JSONString(const std::string& str) : value(str) {}
    void print() override {
        std::cout << "\"" << value << "\"" << std::endl;
    }
};

// 叶子节点：数字
class JSONNumber : public JSONValue {
private:
    double value;
public:
    JSONNumber(double num) : value(num) {}
    void print() override {
        std::cout << value << std::endl;
    }
};

// 容器节点：对象
class JSONObject : public JSONValue {
private:
    std::map<std::string, JSONValue*> properties;
public:
    void addProperty(const std::string& key, JSONValue* value) {
        properties[key] = value;
    }
    void print() override {
        std::cout << "{" << std::endl;
        for (const auto& [key, val] : properties) {
            std::cout << "\"" << key << "\": ";
            val->print();
        }
        std::cout << "}" << std::endl;
    }
};

// 容器节点：数组
class JSONArray : public JSONValue {
private:
    std::vector<JSONValue*> elements;
public:
    void addElement(JSONValue* element) {
        elements.push_back(element);
    }
    void print() override {
        std::cout << "[" << std::endl;
        for (const auto& element : elements) {
            element->print();
        }
        std::cout << "]" << std::endl;
    }
};
```
eon 内部的 object 实现采用了组合模式来组织对象数.


