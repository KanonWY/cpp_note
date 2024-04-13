#include <concepts>
#include <iostream>

/**
 * @brief 定制化的 concept
 */
template <typename Coll, typename T>
concept SupportsPushBack = requires(Coll c, T v) { c.push_back(v); };

/**
 * @brief 使用概念约束可以更好的编写范性代码
 */
template <typename Coll, typename T>
    requires SupportsPushBack<Coll, T>
void add(Coll& coll, const T& val)
{
    coll.push_back(val);
}

template <typename Coll, std::floating_point T>
void add(Coll& coll, const T& val)
{
    std::cout << "float special" << std::endl;
    coll.push_back(val);
}
int main() { return 0; }
