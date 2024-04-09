#pragma once

#include <optional>
#include <concepts>
#include <limits>

namespace sss
{
    namespace cia
    {
        template<typename T> requires std::integral<T> && std::numeric_limits<T>::is_specialized
        constexpr std::optional<T> checked_add(const T& a, const T& b) noexcept;
        
        template<typename T> requires std::integral<T> && std::numeric_limits<T>::is_specialized
        constexpr std::optional<T> checked_sub(const T& a, const T& b) noexcept;

        template<typename T> requires std::integral<T> && std::numeric_limits<T>::is_specialized
        constexpr std::optional<T> checked_mul(const T& a, const T& b) noexcept;
        
        template<typename T> requires std::integral<T> && std::numeric_limits<T>::is_specialized
        constexpr std::optional<T> checked_div(const T& a, const T& b) noexcept;
        
        template<typename T> requires std::integral<T> && std::numeric_limits<T>::is_specialized
        constexpr std::optional<T> checked_rem(const T& a, const T& b) noexcept;
    }
}

#include "cia.cpp"