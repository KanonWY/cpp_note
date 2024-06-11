/**
 * @brief 侵入式的 visitor
 */

#include <memory>
#include <sstream>
#include <iostream>

struct Expression
{
    virtual void print(std::ostringstream &oss) = 0;
};

struct DoubleExpression : Expression
{
    double value;
    explicit DoubleExpression(const double v) : value(v) {}

    void print(std::ostringstream &oss) override { oss << value; }
};

struct AddtionExpression : Expression
{
    std::shared_ptr<Expression> left, right;

    AddtionExpression(std::shared_ptr<Expression> p1,
                      std::shared_ptr<Expression> p2)
        : left(p1), right(p2)
    {
    }

    void print(std::ostringstream &oss) override
    {
        oss << "(";
        left->print(oss);
        oss << "+";
        right->print(oss);
        oss << ")";
    }
};

int main()
{
    std::ostringstream oss;

    auto e = new AddtionExpression(std::make_shared<DoubleExpression>(1),
                                   std::make_shared<DoubleExpression>(2));

    e->print(oss);

    std::cout << oss.str() << std::endl;

    return 0;
}
