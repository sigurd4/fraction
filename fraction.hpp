#pragma once

#include <type_traits>
#include <string>
#include <compare>
#include <numeric>
#include <optional>
#include <concepts>

namespace sss
{
    template<typename T>
    concept nonbool_integral = std::integral<T> && (!std::same_as<T, bool>);

    template<typename T> requires nonbool_integral<T>
    class fraction
    {
        private:
            T numer;
            std::make_unsigned_t<T> denom;

        public:
            constexpr fraction(T numer, std::make_unsigned_t<T> denom) noexcept;
            constexpr fraction(T value) noexcept;
            constexpr fraction(void) noexcept;

            [[nodiscard]] constexpr T get_numer(void) const noexcept;
            [[nodiscard]] constexpr std::make_unsigned_t<T> get_denom(void) const noexcept;
            [[nodiscard]] constexpr bool is_nan(void) const noexcept;
            [[nodiscard]] constexpr bool is_infinite(void) const noexcept;
            [[nodiscard]] constexpr bool is_finite(void) const noexcept;
            [[nodiscard]] constexpr bool is_zero(void) const noexcept;
            [[nodiscard]] constexpr bool is_one(void) const noexcept;

            [[nodiscard]] constexpr fraction<std::make_unsigned_t<T>> abs(void) const noexcept;
            [[nodiscard]] constexpr T signum(void) const noexcept;
            [[nodiscard]] constexpr fraction copysign(const fraction& rhs) const noexcept;
            [[nodiscard]] constexpr fraction copysign(const T& rhs) const noexcept;
            [[nodiscard]] constexpr fraction recip(void) const noexcept;
            [[nodiscard]] constexpr T floor(void) const noexcept;
            [[nodiscard]] constexpr T ceil(void) const noexcept;
            [[nodiscard]] constexpr T round(void) const noexcept;
            [[nodiscard]] constexpr T trunc(void) const noexcept;
            [[nodiscard]] constexpr fraction fract(void) const noexcept;
            template<typename I> requires nonbool_integral<I>
            [[nodiscard]] constexpr fraction pow(const I& rhs) const noexcept;

            template<typename I> requires nonbool_integral<I>
            [[nodiscard]] constexpr explicit operator fraction<I>(void) const noexcept;
            /*template<typename I, std::enable_if_t<std::is_integral<I>::value && (!std::is_signed<T>::value || std::is_signed<I>::value) && (sizeof(I) > sizeof(T)), bool> = true>
            [[nodiscard]] constexpr operator Fraction<I>(void) const noexcept;*/
            [[nodiscard]] constexpr explicit operator T(void) const noexcept;
            template<typename I> requires nonbool_integral<I>
            [[nodiscard]] constexpr explicit operator I(void) const noexcept;
            template<typename F> requires std::floating_point<F>
            [[nodiscard]] constexpr operator F(void) const noexcept;
            [[nodiscard]] constexpr operator std::string(void) const noexcept;
            [[nodiscard]] constexpr fraction operator+(const fraction rhs) const noexcept;
            [[nodiscard]] constexpr fraction operator-(const fraction rhs) const noexcept;
            [[nodiscard]] constexpr fraction operator%(const fraction rhs) const noexcept;
            [[nodiscard]] constexpr fraction operator*(const fraction rhs) const noexcept;
            [[nodiscard]] constexpr fraction operator/(const fraction rhs) const noexcept;
            constexpr fraction& operator+=(const fraction rhs) noexcept;
            constexpr fraction& operator-=(const fraction rhs) noexcept;
            constexpr fraction& operator%=(const fraction rhs) noexcept;
            constexpr fraction& operator*=(const fraction rhs) noexcept;
            constexpr fraction& operator/=(const fraction rhs) noexcept;
            [[nodiscard]] constexpr fraction operator+(const T& rhs) const noexcept;
            [[nodiscard]] constexpr fraction operator-(const T& rhs) const noexcept;
            [[nodiscard]] constexpr fraction operator%(const T& rhs) const noexcept;
            [[nodiscard]] constexpr fraction operator*(const T& rhs) const noexcept;
            [[nodiscard]] constexpr fraction operator/(const T& rhs) const noexcept;
            template<typename U> requires nonbool_integral<U>
            friend constexpr fraction<U> operator+(const U& lhs, const fraction<U>& rhs) noexcept;
            template<typename U> requires nonbool_integral<U>
            friend constexpr fraction<U> operator-(const U& lhs, const fraction<U>& rhs) noexcept;
            template<typename U> requires nonbool_integral<U>
            friend constexpr fraction<U> operator%(const U& lhs, const fraction<U>& rhs) noexcept;
            template<typename U> requires nonbool_integral<U>
            friend constexpr fraction<U> operator*(const U& lhs, const fraction<U>& rhs) noexcept;
            template<typename U> requires nonbool_integral<U>
            friend constexpr fraction<U> operator/(const U& lhs, const fraction<U>& rhs) noexcept;
            constexpr fraction& operator+=(const T& rhs) noexcept;
            constexpr fraction& operator-=(const T& rhs) noexcept;
            constexpr fraction& operator%=(const T& rhs) noexcept;
            constexpr fraction& operator*=(const T& rhs) noexcept;
            constexpr fraction& operator/=(const T& rhs) noexcept;
            [[nodiscard]] constexpr fraction<std::make_signed_t<T>> operator-(void) const noexcept;
            [[nodiscard]] constexpr std::partial_ordering operator<=>(const fraction& rhs) const noexcept;
            [[nodiscard]] constexpr std::partial_ordering operator<=>(const T& rhs) const noexcept;
            template<typename I> requires nonbool_integral<I>
            [[nodiscard]] constexpr std::partial_ordering operator<=>(const I& rhs) const noexcept;
            [[nodiscard]] constexpr std::partial_ordering operator<=>(std::nullptr_t) const noexcept;
            template<typename U> requires nonbool_integral<U>
            friend constexpr std::partial_ordering operator<=>(const U& lhs, const fraction<U>& rhs) noexcept;
            template<typename U, typename I> requires nonbool_integral<U> && nonbool_integral<I>
            friend constexpr std::partial_ordering operator<=>(const I& lhs, const fraction<U>& rhs) noexcept;
            template<typename U> requires nonbool_integral<U>
            friend constexpr std::partial_ordering operator<=>(std::nullptr_t, const fraction<U>& x) noexcept;
            [[nodiscard]] constexpr bool operator==(const fraction& rhs) const noexcept;
            [[nodiscard]] constexpr bool operator==(const T& rhs) const noexcept;
            template<typename I> requires nonbool_integral<I>
            [[nodiscard]] constexpr bool operator==(const I& rhs) const noexcept;
            [[nodiscard]] constexpr bool operator==(std::nullptr_t) const noexcept;
            template<typename U> requires nonbool_integral<U>
            friend constexpr bool operator==(const U& lhs, const fraction<U>& rhs) noexcept;
            template<typename U, typename I> requires nonbool_integral<U> && nonbool_integral<I>
            friend constexpr bool operator==(const I& lhs, const fraction<U>& rhs) noexcept;
            template<typename U> requires nonbool_integral<U>
            friend constexpr bool operator==(std::nullptr_t, const fraction<U>& rhs) noexcept;
            [[nodiscard]] constexpr bool operator!=(const fraction& rhs) const noexcept;
            template<typename I> requires nonbool_integral<I>
            [[nodiscard]] constexpr bool operator!=(const I& rhs) const noexcept;
            [[nodiscard]] constexpr bool operator!=(const T& rhs) const noexcept;
            [[nodiscard]] constexpr bool operator!=(std::nullptr_t) const noexcept;
            template<typename U> requires nonbool_integral<U>
            friend constexpr bool operator!=(const U& lhs, const fraction<U>& rhs) noexcept;
            template<typename U, typename I> requires nonbool_integral<U> && nonbool_integral<I>
            friend constexpr bool operator!=(const I& lhs, const fraction<U>& rhs) noexcept;
            template<typename U> requires nonbool_integral<U>
            friend constexpr bool operator!=(std::nullptr_t, const fraction<U>& rhs) noexcept;

        private:
            constexpr void reduce(void) noexcept;
            
            [[nodiscard]] constexpr std::optional<fraction> checked_add(const fraction& rhs) const noexcept;
            [[nodiscard]] constexpr std::optional<fraction> checked_add(const T& rhs) const noexcept;
            [[nodiscard]] constexpr std::optional<fraction> checked_sub(const fraction& rhs) const noexcept;
            [[nodiscard]] constexpr std::optional<fraction> checked_sub(const T& rhs) const noexcept;
            [[nodiscard]] constexpr std::optional<fraction> checked_lsub(const T& lhs) const noexcept;
            [[nodiscard]] constexpr std::optional<fraction> checked_rem(const fraction& rhs) const noexcept;
            [[nodiscard]] constexpr std::optional<fraction> checked_rem(const T& rhs) const noexcept;
            [[nodiscard]] constexpr std::optional<fraction> checked_lrem(const T& lhs) const noexcept;
            [[nodiscard]] constexpr std::optional<fraction> checked_mul(const fraction& rhs) const noexcept;
            [[nodiscard]] constexpr std::optional<fraction> checked_mul(const T& rhs) const noexcept;
            [[nodiscard]] constexpr std::optional<fraction> checked_div(const fraction& rhs) const noexcept;
            [[nodiscard]] constexpr std::optional<fraction> checked_div(const T& rhs) const noexcept;
            [[nodiscard]] constexpr std::optional<fraction> checked_ldiv(const T& lhs) const noexcept;
    };
}

template<typename T> requires sss::nonbool_integral<T>
class std::numeric_limits<sss::fraction<T>>
{
    public:
        static constexpr float_denorm_style has_denorm = denorm_absent;
        static constexpr bool has_denorm_loss          = false;
        static constexpr bool has_infinity             = true;
        static constexpr bool has_quiet_NaN            = true;
        static constexpr bool has_signaling_NaN        = false;
        static constexpr bool is_bounded               = false;
        static constexpr bool is_exact                 = true;
        static constexpr bool is_iec559                = false;
        static constexpr bool is_integer               = false;
        static constexpr bool is_modulo                = false;
        static constexpr bool is_signed                = std::numeric_limits<T>::is_signed;
        static constexpr bool is_specialized           = true;
        static constexpr bool tinyness_before          = false;
        static constexpr bool traps                    = std::numeric_limits<T>::traps;
        static constexpr float_round_style round_style = round_toward_zero;
        static constexpr int digits                    = std::numeric_limits<T>::digits;
        static constexpr int digits10                  = std::numeric_limits<T>::digits10;
        static constexpr int max_digits10              = std::numeric_limits<T>::max_digits10;
        static constexpr int max_exponent              = std::numeric_limits<T>::max_exponent;
        static constexpr int max_exponent10            = std::numeric_limits<T>::max_exponent10;
        static constexpr int min_exponent              = -std::numeric_limits<std::make_unsigned_t<T>>::max_exponent;
        static constexpr int min_exponent10            = -std::numeric_limits<std::make_unsigned_t<T>>::max_exponent10;
        static constexpr int radix                     = 2;
        
        [[nodiscard]] static constexpr sss::fraction<T> (min)() noexcept
        {
            return {T(1), std::numeric_limits<std::make_unsigned_t<T>>::max()};
        }

        [[nodiscard]] static constexpr sss::fraction<T> (max)() noexcept
        {
            return {std::numeric_limits<T>::max(), T(1)};
        }

        [[nodiscard]] static constexpr sss::fraction<T> lowest() noexcept
        {
            return {std::numeric_limits<T>::min(), T(1)};
        }

        [[nodiscard]] static constexpr sss::fraction<T> epsilon() noexcept
        {
            return {T(std::is_signed<T>::value ? 3 : 2), std::numeric_limits<std::make_unsigned_t<T>>::max()};
        }

        [[nodiscard]] static constexpr sss::fraction<T> round_error() noexcept
        {
            return {T(1), T(2)};
        }

        [[nodiscard]] static constexpr sss::fraction<T> denorm_min() noexcept
        {
            return std::numeric_limits<sss::fraction<T>>::min();
        }

        [[nodiscard]] static constexpr sss::fraction<T> infinity() noexcept
        {
            return {T(1), T(0)};
        }

        [[nodiscard]] static constexpr sss::fraction<T> quiet_NaN() noexcept
        {
            return {T(0), T(0)};
        }

        [[nodiscard]] static constexpr sss::fraction<T> signaling_NaN() noexcept
        {
            return {T(0), T(0)};
        }
};

#include "fraction.cpp"