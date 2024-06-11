## visitor
不想侵入式的修改原有的类的逻辑,但是想给这些原有的类添加新的行为。可以将这些新的行为封装到一个访问者中。（前提，原有的类实现了 accecpt 的方法)

### 1、classic visitor

```c++
// 访问者基类前置声明
struct VisitorBase;

struct Base {
  virtual void accept(VisitorBase* visitor) = 0;
};

class ElementA;
class ElementB;

struct VisitorBase {
    virtual void visit(ElementA* obj) = 0;
    virtual void visit(ElementB* obj) = 0;
};

class ElementA {
    void accecpt(VisitorBase* visitor) override {
        visitor->visit(this);
    }
};

class ElementB {
    void accecpt(VisitorBase* visitor) override {
        visitor->visit(this);
    }
};

//// 扩展功能的访问者类

class KanonVisitor: public VisitorBase {
    void visit(ElementA* obj) override {
        // 处理对应的元素
    }

    void visit(ElementB* obj) override {
        // 处理对应的元素
    }
};
```

### 2、c++17 std::visitor
集合 variant 的访问者

```c++

class BaseShape {};

class CirShape {};

class SuqShape {};

std::variant<CirShape, SuqShape> m_var;


```


