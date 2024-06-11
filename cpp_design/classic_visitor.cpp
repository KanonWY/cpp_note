#include <iostream>
#include <memory>
#include <sstream>

struct ExpressionVisitor;
struct DoubleExpression;
struct AddtionExpression;

/**
 * @brief 各种访问者基类
 */
struct ExpressionVisitor
{
    virtual void visit(DoubleExpression* de) = 0;
    virtual void visit(AddtionExpression* ae) = 0;
};

/**
 * @brief 被访问者抽象类
 */
struct Expression
{
    /**
     * @brief 被访问者接受访问者基类型
     */
    virtual void accept(ExpressionVisitor* visitor) = 0;
};

/**
 * @brief 被访问者实际类
 */
struct DoubleExpression : public Expression
{
    double value;
    DoubleExpression(int v) : value(v) {}
    void accept(ExpressionVisitor* visitor) override { visitor->visit(this); }
};

/**
 * @brief 被访问者实际类
 */

struct AddtionExpression : public Expression
{
    std::shared_ptr<Expression> m_left, m_right;

    AddtionExpression(std::shared_ptr<Expression> p1,
                      std::shared_ptr<Expression> p2)
        : m_left(p1), m_right(p2)
    {
    }

    void accept(ExpressionVisitor* visitor) override { visitor->visit(this); }
};

struct ExpressPrint : public ExpressionVisitor
{
    std::ostringstream oss;
    std::string str() { return oss.str(); }
    void visit(DoubleExpression* p) { oss << p; }

    void visit(AddtionExpression* p)
    {
        oss << "(";
        p->m_left->accept(this);
        oss << "+";
        p->m_right->accept(this);
        oss << ")";
    }
};

int main()
{
    std::shared_ptr<Expression> p = std::make_shared<AddtionExpression>(
        std::make_shared<DoubleExpression>(1),
        std::make_shared<DoubleExpression>(20));

    ExpressPrint print;

    p->accept(&print);

    std::cout << print.str() << std::endl;

    return 0;
}
