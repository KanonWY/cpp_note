#ifndef __EON_ANYVALUE_H__
#define __EON_ANYVALUE_H__

#include <variant>
#include <exception>

namespace eon::coro {

template <typename T>
class AnyValue
{
public:

private:
    std::variant<std::monostate, T, std::exception> m_value;
};

} // namespace eon::coro

#endif