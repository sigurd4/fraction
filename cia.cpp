
#include "cia.hpp"

#include <limits>

namespace sss
{
    namespace cia
    {
        template<typename T>
        constexpr std::optional<T> checked_add(const T& a, const T& b) noexcept
        {
            if(a > 0 && b > std::numeric_limits<T>::max() - a)
            {
                return {};
            }
            if(a < 0 && b < std::numeric_limits<T>::min() - a)
            {
                return {};
            }
            return a + b;
        }
        
        template<typename T>
        constexpr std::optional<T> checked_sub(const T& a, const T& b) noexcept
        {
            if(b < 0 && a > std::numeric_limits<T>::max() + b)
            {
                return {};
            }
            if(b > 0 && a < std::numeric_limits<T>::min() + b)
            {
                return {};
            }
            return a - b;
        }

        template<typename T>
        constexpr std::optional<T> checked_mul(const T& a, const T& b) noexcept
        {
            if(b > 0)
            {
                if(a > std::numeric_limits<T>::max()/b || a < std::numeric_limits<T>::min()/b)
                {
                    return {};
                }
            }
            else if(b < 0)
            {
                if(b == -1 && a == std::numeric_limits<T>::min())
                {
                    return {};
                }

                if(a < std::numeric_limits<T>::max()/b || (b != -1 && a > std::numeric_limits<T>::min()/b))
                {
                    return {};
                }
            }

            return a*b;
        }
        
        template<typename T>
        constexpr std::optional<T> checked_div(const T& a, const T& b) noexcept
        {
            if(b == 0 || (b < 0 && b == -1 && a == std::numeric_limits<T>::min()))
            {
                return {};
            }

            return a/b;
        }
        
        template<typename T>
        constexpr std::optional<T> checked_rem(const T& a, const T& b) noexcept
        {
            if(b == 0 || (b < 0 && b == -1 && a == std::numeric_limits<T>::min()))
            {
                return {};
            }

            return a%b;
        }
    }
}