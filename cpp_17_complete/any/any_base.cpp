#include <any>
#include <iostream>
#include <string>

/**
 * @brief
 *  1、std::any 有性能损耗吗？
 *  2、std::any_cast 失败会如何？
 *  3、使用场景是什么?
 *  4、是否推荐使用 std::any
 */
void TEST_base_use()
{
    std::any a;
    std::any b = 4.3;
    a = 32;
    b = std::string("???");
    if (a.type() == typeid(std::string))
    {
        std::string s = std::any_cast<std::string>(a);
        std::cout << "string = " << s << std::endl;
    }
    else if (a.type() == typeid(int))
    {
        int i = std::any_cast<int>(a);
        std::cout << "int = " << i << std::endl;
    }
}

/**
 * @brief 类型为空
 * std::any c;
 * c.type() == typeid(void);
 */
void TEST_any_empty()
{
    std::any a;
    if (a.type() == typeid(void))
    {
        std::cout << "is empty()" << std::endl;
    }
}

/**
 * @brief any_cast 是被会抛出异常
 */
void TEST_any_cast_ERROR()
{
    try
    {
        std::any a;
        auto s = std::any_cast<std::string>(a);
    }
    catch (std::bad_any_cast& e)
    {
        std::cout << "EXCEPTION: " << e.what() << std::endl;
    }
}

/**
 * @brief any_cast
 * (1)
 * 因为 any_cast<std::string>(a) 会创建一个临时变量，因此可以使用转换引用
 * any_cast<std::string&>(a);
 * (2)
 * 如果不想处理异常，也可以使用指针操作
 */
void TEST_any_cast_ptr()
{
    std::any a;
    if (auto p = std::any_cast<std::string>(&a); p)
    {
        std::cout << "转换成功" << std::endl;
    }
    else
    {
        std::cout << "转换失败" << std::endl;
    }

    std::any b;
    std::string s = "xxxxxxxxx";
    b = s;
    if (auto p = std::any_cast<std::string>(&b); p)
    {
        std::cout << "转换成功: " << *p << std::endl;
    }
    else
    {
        std::cout << "转换失败" << std::endl;
    }
}

/**
 * @brief 对于大对象，使用 ref 进行处理
 */
void TEST_get_cast_ref()
{
    std::any a;
    std::string s{"xxx"};
    a = s;
    std::string& ref = std::any_cast<std::string&>(a);
    ref += "-yyy";
    if (auto p = std::any_cast<std::string>(&a); p)
    {
        std::cout << *p << std::endl;
    }
}

/**
 * @brief 清理 any 中的值
 * reset() 函数
 * 赋值为空
 */
void clear_any_value()
{
    std::any a;
    a = std::string("xxxx");
    a.reset();

    a = 33;
    if (a.has_value())
    {
        std::cout << "A: a has value!" << std::endl;
    }
    a = std::any{};

    if (a.has_value())
    {
        std::cout << "B: a has value!" << std::endl;
    }
    else
    {
        std::cout << "B: a has no value!" << std::endl;
    }
}

int main()
{
    clear_any_value();
    return 0;
}
