#include "fraction.hpp"

#include <algorithm>
#include <format>
#include <bit>
#include "cia.hpp"

namespace sss
{
    template<typename T> requires nonbool_integral<T>
    constexpr fraction<T>::fraction(T numer, std::make_unsigned_t<T> denom) noexcept:
        numer{numer},
        denom{denom}
    {
        this->reduce();
    }
    template<typename T> requires nonbool_integral<T>
    constexpr fraction<T>::fraction(T value) noexcept:
        fraction{value, 1}
    {
        
    }
    template<typename T> requires nonbool_integral<T>
    constexpr fraction<T>::fraction(void) noexcept:
        fraction{0, 1}
    {

    }

    template<typename T> requires nonbool_integral<T>
    constexpr T fraction<T>::get_numer(void) const noexcept
    {
        return this->numer;
    }
    template<typename T> requires nonbool_integral<T>
    constexpr std::make_unsigned_t<T> fraction<T>::get_denom(void) const noexcept
    {
        return this->denom;
    }
    template<typename T> requires nonbool_integral<T>
    constexpr bool fraction<T>::is_nan() const noexcept
    {
        return this->numer == 0 && this->denom == 0;
    }
    template<typename T> requires nonbool_integral<T>
    constexpr bool fraction<T>::is_infinite() const noexcept
    {
        return this->numer != 0 && this->denom == 0;
    }
    template<typename T> requires nonbool_integral<T>
    constexpr bool fraction<T>::is_finite() const noexcept
    {
        return this->denom != 0;
    }
    template<typename T> requires nonbool_integral<T>
    constexpr bool fraction<T>::is_zero() const noexcept
    {
        return this->numer == 0 && this->denom != 0;
    }
    template<typename T> requires nonbool_integral<T>
    constexpr bool fraction<T>::is_one() const noexcept
    {
        return this->numer > 0 && static_cast<std::make_unsigned_t<T>>(this->numer) == this->denom;
    }

    template<typename T> requires nonbool_integral<T>
    constexpr fraction<std::make_unsigned_t<T>> fraction<T>::abs(void) const noexcept
    {
        if(this->numer < 0)
        {
            return {
                static_cast<std::make_unsigned_t<T>>(static_cast<std::make_unsigned_t<T>>(T(-1) - this->numer)
                    + std::make_unsigned_t<T>(1)),
                this->denom
            };
        }
        return {static_cast<std::make_unsigned_t<T>>(this->numer), this->denom};
    }
    template<typename T> requires nonbool_integral<T>
    constexpr T fraction<T>::signum(void) const noexcept
    {
        if(this->numer < 0)
        {
            return -1;
        }
        if(this->numer > 0)
        {
            return 1;
        }
        return 0;
    }
    template<typename T> requires nonbool_integral<T>
    constexpr fraction<T> fraction<T>::copysign(const fraction& rhs) const noexcept
    {
        return this->copysign(rhs.numer);
    }
    template<typename T> requires nonbool_integral<T>
    constexpr fraction<T> fraction<T>::copysign(const T& rhs) const noexcept
    {
        if(rhs == 0)
        {
            return 0;
        }
        if((this->numer < 0) != (rhs < 0))
        {
            return static_cast<fraction<T>>(-*this);
        }
        return *this;
    }
    template<typename T> requires nonbool_integral<T>
    constexpr fraction<T> fraction<T>::recip(void) const noexcept
    {
        if(this->numer < 0)
        {
            return {
                static_cast<T>(-static_cast<T>(this->denom)),
                static_cast<std::make_unsigned_t<T>>(-this->numer)
            };
        }
        return {static_cast<T>(this->denom), static_cast<std::make_unsigned_t<T>>(this->numer)};
    }
    template<typename T> requires nonbool_integral<T>
    constexpr T fraction<T>::floor(void) const noexcept
    {
        T x = this->numer;
        T y = static_cast<T>(this->denom);
        return x/y - (x % y != 0) * ((x < 0) ^ (y < 0));
    }
    template<typename T> requires nonbool_integral<T>
    constexpr T fraction<T>::ceil(void) const noexcept
    {
        T x = this->numer;
        T y = static_cast<T>(this->denom);
        return x/y + (x % y != 0) * !((x > 0) ^ (y > 0));
    }
    template<typename T> requires nonbool_integral<T>
    constexpr T fraction<T>::trunc(void) const noexcept
    {
        T x = this->numer;
        T y = static_cast<T>(this->denom);
        return x/y;
    }
    template<typename T> requires nonbool_integral<T>
    constexpr T fraction<T>::round(void) const noexcept
    {
        T x = this->numer;
        T y = static_cast<T>(this->denom);
        T r = x/y;
        T m = x % y;
        T h = y/2;
        return r + (!(x < 0) & (m > h)) - ((x < 0) & ((m + h) < 0));
    }
    template<typename T> requires nonbool_integral<T>
    constexpr fraction<T> fraction<T>::fract(void) const noexcept
    {
        if(this->denom > static_cast<std::make_unsigned_t<T>>(std::numeric_limits<T>::max()))
        {
            return *this;
        }
        return {static_cast<T>(this->numer % static_cast<T>(this->denom)), this->denom};
    }
    template<typename T> requires nonbool_integral<T>
    template<typename I> requires nonbool_integral<I>
    constexpr fraction<T> fraction<T>::pow(const I& rhs) const noexcept
    {
        I n {rhs};
        fraction r {1};
        fraction x {n < 0 ? this->recip() : *this};

        if(n & 1 == 1)
        {
            r = x;
        }

        for(;;)
        {
            n = n >> 1;
            if(n == 0)
            {
                return r;
            }
            x *= x;
            if(n & 1 == 1)
            {
                r *= x;
            }
        }
    }

    template<typename T> requires nonbool_integral<T>
    template<typename I> requires nonbool_integral<I>
    constexpr fraction<T>::operator fraction<I>(void) const noexcept
    {
        if(this->numer < 0 && !std::is_signed<I>::value)
        {
            return fraction<I>{0, 0};
        }
        if(sizeof(I) > sizeof(T))
        {
            return fraction<I>{static_cast<I>(this->numer), static_cast<std::make_unsigned_t<I>>(this->denom)};
        }
        if(sizeof(I) == sizeof(T))
        {
            if(std::is_signed<I>::value == std::is_signed<T>::value || !std::is_signed<I>::value)
            {
                return fraction<I>{static_cast<I>(this->numer), static_cast<std::make_unsigned_t<I>>(this->denom)};
            }
        }
        fraction<T> x {*this};
        for(T i {1};;)
        {
            if(
                x.numer <= static_cast<T>(std::numeric_limits<I>::max())
                && x.numer >= static_cast<T>(std::numeric_limits<I>::min())
                && x.denom <= static_cast<std::make_unsigned_t<T>>(std::numeric_limits<std::make_unsigned_t<I>>::max())
            )
            {
                return fraction<I>{static_cast<I>(this->numer), static_cast<std::make_unsigned_t<I>>(this->denom)};
            }
            x = {
                static_cast<T>(this->numer/++i),
                static_cast<std::make_unsigned_t<T>>(this->denom/static_cast<std::make_unsigned_t<T>>(i))
            };
        }
    }
    template<typename T> requires nonbool_integral<T>
    constexpr fraction<T>::operator T(void) const noexcept
    {
        return this->trunc();
    }
    template<typename T> requires nonbool_integral<T>
    template<typename I> requires nonbool_integral<I>
    constexpr fraction<T>::operator I(void) const noexcept
    {
        return static_cast<I>(this->trunc());
    }
    template<typename T> requires nonbool_integral<T>
    template<typename F> requires std::floating_point<F>
    constexpr fraction<T>::operator F(void) const noexcept
    {
        return static_cast<F>(this->numer)/static_cast<F>(this->denom);
    }
    template<typename T> requires nonbool_integral<T>
    constexpr fraction<T>::operator std::string(void) const noexcept
    {
        if(this->is_zero() || this->abs().is_one())
        {
            return std::format("{:d}", this->numer);
        }
        return std::format("{:d}/{:d}", this->numer, this->denom);
    }

    template<typename T> requires nonbool_integral<T>
    constexpr fraction<T> fraction<T>::operator+(const fraction<T> rhs) const noexcept
    {
        fraction<T> a {*this};
        fraction<T> b {rhs};
        for(T i {1}, j {1};;)
        {
            std::optional<fraction<T>> y = a.checked_add(b);
            if(y.has_value())
            {
                return y.value();
            }
            std::make_unsigned_t<T> a_numer = static_cast<std::make_unsigned_t<T>>(a.numer < 0 ? -1 - a.numer : a.numer)
                + (a.numer < 0 ? 1 : 0);
            std::make_unsigned_t<T> b_numer = static_cast<std::make_unsigned_t<T>>(b.numer < 0 ? -1 - b.numer : b.numer)
                + (b.numer < 0 ? 1 : 0);
            std::make_unsigned_t<T> n {(std::max(a_numer, a.denom))};
            std::make_unsigned_t<T> m {(std::max(b_numer, b.denom))};
            if(n > m)
            {
                ++i;
                a = {
                    static_cast<T>(this->numer/i),
                    static_cast<std::make_unsigned_t<T>>(this->denom/static_cast<std::make_unsigned_t<T>>(i))
                };
            }
            else
            {
                b = {
                    static_cast<T>(rhs.numer/++j),
                    static_cast<std::make_unsigned_t<T>>(rhs.denom/static_cast<std::make_unsigned_t<T>>(j))
                };
            }
        }
    }
    template<typename T> requires nonbool_integral<T>
    constexpr fraction<T> fraction<T>::operator-(const fraction<T> rhs) const noexcept
    {
        fraction<T> a {*this};
        fraction<T> b {rhs};
        for(T i {1}, j {1};;)
        {
            std::optional<fraction<T>> y = a.checked_sub(b);
            if(y.has_value())
            {
                return y.value();
            }
            std::make_unsigned_t<T> a_numer = static_cast<std::make_unsigned_t<T>>(a.numer < 0 ? -1 - a.numer : a.numer)
                + (a.numer < 0 ? 1 : 0);
            std::make_unsigned_t<T> b_numer = static_cast<std::make_unsigned_t<T>>(b.numer < 0 ? -1 - b.numer : b.numer)
                + (b.numer < 0 ? 1 : 0);
            std::make_unsigned_t<T> n {(std::max(a_numer, a.denom))};
            std::make_unsigned_t<T> m {(std::max(b_numer, b.denom))};
            if(n > m)
            {
                a = {
                    static_cast<T>(this->numer/++i),
                    static_cast<std::make_unsigned_t<T>>(this->denom/static_cast<std::make_unsigned_t<T>>(i))
                };
            }
            else
            {
                b = {
                    static_cast<T>(rhs.numer/++j),
                    static_cast<std::make_unsigned_t<T>>(rhs.denom/static_cast<std::make_unsigned_t<T>>(j))
                };
            }
        }
    }
    template<typename T> requires nonbool_integral<T>
    constexpr fraction<T> fraction<T>::operator%(const fraction<T> rhs) const noexcept
    {
        fraction<T> a {*this};
        fraction<T> b {rhs};
        for(T i {1}, j {1};;)
        {
            std::optional<fraction<T>> y = a.checked_rem(b);
            if(y.has_value())
            {
                return y.value();
            }
            std::make_unsigned_t<T> a_numer = static_cast<std::make_unsigned_t<T>>(a.numer < 0 ? -1 - a.numer : a.numer)
                + (a.numer < 0 ? 1 : 0);
            std::make_unsigned_t<T> b_numer = static_cast<std::make_unsigned_t<T>>(b.numer < 0 ? -1 - b.numer : b.numer)
                + (b.numer < 0 ? 1 : 0);
            std::make_unsigned_t<T> n {(std::max(a_numer, a.denom))};
            std::make_unsigned_t<T> m {(std::max(b_numer, b.denom))};
            if(n > m)
            {
                a = {
                    static_cast<T>(this->numer/++i),
                    static_cast<std::make_unsigned_t<T>>(this->denom/static_cast<std::make_unsigned_t<T>>(i))
                };
            }
            else
            {
                b = {
                    static_cast<T>(rhs.numer/++j),
                    static_cast<std::make_unsigned_t<T>>(rhs.denom/static_cast<std::make_unsigned_t<T>>(j))
                };
            }
        }
    }
    template<typename T> requires nonbool_integral<T>
    constexpr fraction<T> fraction<T>::operator*(const fraction<T> rhs) const noexcept
    {
        fraction<T> a {*this};
        fraction<T> b {rhs};
        for(T i {1}, j {1};;)
        {
            std::optional<fraction<T>> y = a.checked_mul(b);
            if(y.has_value())
            {
                return y.value();
            }
            std::make_unsigned_t<T> a_numer = static_cast<std::make_unsigned_t<T>>(a.numer < 0 ? -1 - a.numer : a.numer)
                + (a.numer < 0 ? 1 : 0);
            std::make_unsigned_t<T> b_numer = static_cast<std::make_unsigned_t<T>>(b.numer < 0 ? -1 - b.numer : b.numer)
                + (b.numer < 0 ? 1 : 0);
            std::make_unsigned_t<T> n {(std::max(a_numer, a.denom))};
            std::make_unsigned_t<T> m {(std::max(b_numer, b.denom))};
            if(n > m)
            {
                a = {
                    static_cast<T>(this->numer/++i),
                    static_cast<std::make_unsigned_t<T>>(this->denom/static_cast<std::make_unsigned_t<T>>(i))
                };
            }
            else
            {
                b = {
                    static_cast<T>(rhs.numer/++j),
                    static_cast<std::make_unsigned_t<T>>(rhs.denom/static_cast<std::make_unsigned_t<T>>(j))
                };
            }
        }
    }
    template<typename T> requires nonbool_integral<T>
    constexpr fraction<T> fraction<T>::operator/(const fraction<T> rhs) const noexcept
    {
        fraction<T> a {*this};
        fraction<T> b {rhs};
        for(T i {1}, j {1};;)
        {
            std::optional<fraction<T>> y = a.checked_div(b);
            if(y.has_value())
            {
                return y.value();
            }
            std::make_unsigned_t<T> a_numer = static_cast<std::make_unsigned_t<T>>(a.numer < 0 ? -1 - a.numer : a.numer)
                + (a.numer < 0 ? 1 : 0);
            std::make_unsigned_t<T> b_numer = static_cast<std::make_unsigned_t<T>>(b.numer < 0 ? -1 - b.numer : b.numer)
                + (b.numer < 0 ? 1 : 0);
            std::make_unsigned_t<T> n {(std::max(a_numer, a.denom))};
            std::make_unsigned_t<T> m {(std::max(b_numer, b.denom))};
            if(n > m)
            {
                a = {
                    static_cast<T>(this->numer/++i),
                    static_cast<std::make_unsigned_t<T>>(this->denom/static_cast<std::make_unsigned_t<T>>(i))
                };
            }
            else
            {
                b = {
                    static_cast<T>(rhs.numer/++j),
                    static_cast<std::make_unsigned_t<T>>(rhs.denom/static_cast<std::make_unsigned_t<T>>(j))
                };
            }
        }
    }
    template<typename T> requires nonbool_integral<T>
    constexpr fraction<T>& fraction<T>::operator+=(const fraction<T> rhs) noexcept
    {
        return *this = *this + rhs;
    }
    template<typename T> requires nonbool_integral<T>
    constexpr fraction<T>& fraction<T>::operator-=(const fraction<T> rhs) noexcept
    {
        return *this = *this - rhs;
    }
    template<typename T> requires nonbool_integral<T>
    constexpr fraction<T>& fraction<T>::operator%=(const fraction<T> rhs) noexcept
    {
        return *this = *this % rhs;
    }
    template<typename T> requires nonbool_integral<T>
    constexpr fraction<T>& fraction<T>::operator*=(const fraction<T> rhs) noexcept
    {
        return *this = *this * rhs;
    }
    template<typename T> requires nonbool_integral<T>
    constexpr fraction<T>& fraction<T>::operator/=(const fraction<T> rhs) noexcept
    {
        return *this = *this / rhs;
    }
    template<typename T> requires nonbool_integral<T>
    constexpr fraction<T> fraction<T>::operator+(const T& rhs) const noexcept
    {
        std::optional<fraction<T>> y {this->checked_add(rhs)};
        if(y.has_value())
        {
            return y.value();
        }
        return *this + fraction{rhs};
    }
    template<typename T> requires nonbool_integral<T>
    constexpr fraction<T> fraction<T>::operator-(const T& rhs) const noexcept
    {
        std::optional<fraction<T>> y {this->checked_sub(rhs)};
        if(y.has_value())
        {
            return y.value();
        }
        return *this - fraction{rhs};
    }
    template<typename T> requires nonbool_integral<T>
    constexpr fraction<T> fraction<T>::operator%(const T& rhs) const noexcept
    {
        std::optional<fraction<T>> y {this->checked_rem(rhs)};
        if(y.has_value())
        {
            return y.value();
        }
        return *this % fraction{rhs};
    }
    template<typename T> requires nonbool_integral<T>
    constexpr fraction<T> fraction<T>::operator*(const T& rhs) const noexcept
    {
        std::optional<fraction<T>> y {this->checked_mul(rhs)};
        if(y.has_value())
        {
            return y.value();
        }
        return *this * fraction{rhs};
    }
    template<typename T> requires nonbool_integral<T>
    constexpr fraction<T> fraction<T>::operator/(const T& rhs) const noexcept
    {
        std::optional<fraction<T>> y {this->checked_div(rhs)};
        if(y.has_value())
        {
            return y.value();
        }
        return *this / fraction{rhs};
    }
    template<typename T> requires nonbool_integral<T>
    [[nodiscard]] constexpr fraction<T> operator+(const T& lhs, const fraction<T>& rhs) noexcept
    {
        return rhs + lhs;
    }
    template<typename T> requires nonbool_integral<T>
    [[nodiscard]] constexpr fraction<T> operator-(const T& lhs, const fraction<T>& rhs) noexcept
    {
        std::optional<fraction<T>> y {rhs.checked_lsub(lhs)};
        if(y.has_value())
        {
            return y.value();
        }
        return fraction{lhs} - rhs;
    }
    template<typename T> requires nonbool_integral<T>
    [[nodiscard]] constexpr fraction<T> operator%(const T& lhs, const fraction<T>& rhs) noexcept
    {
        std::optional<fraction<T>> y {rhs.checked_lrem(lhs)};
        if(y.has_value())
        {
            return y.value();
        }
        return fraction{lhs} % rhs;
    }
    template<typename T> requires nonbool_integral<T>
    [[nodiscard]] constexpr fraction<T> operator*(const T& lhs, const fraction<T>& rhs) noexcept
    {
        return rhs*lhs;
    }
    template<typename T> requires nonbool_integral<T>
    [[nodiscard]] constexpr fraction<T> operator/(const T& lhs, const fraction<T>& rhs) noexcept
    {
        std::optional<fraction<T>> y {rhs.checked_ldiv(lhs)};
        if(y.has_value())
        {
            return y.value();
        }
        return fraction{lhs} / rhs;
    }
    template<typename T> requires nonbool_integral<T>
    constexpr fraction<T>& fraction<T>::operator+=(const T& rhs) noexcept
    {
        return *this = *this + rhs;
    }
    template<typename T> requires nonbool_integral<T>
    constexpr fraction<T>& fraction<T>::operator-=(const T& rhs) noexcept
    {
        return *this = *this - rhs;
    }
    template<typename T> requires nonbool_integral<T>
    constexpr fraction<T>& fraction<T>::operator%=(const T& rhs) noexcept
    {
        return *this = *this % rhs;
    }
    template<typename T> requires nonbool_integral<T>
    constexpr fraction<T>& fraction<T>::operator*=(const T& rhs) noexcept
    {
        return *this = *this*rhs;
    }
    template<typename T> requires nonbool_integral<T>
    constexpr fraction<T>& fraction<T>::operator/=(const T& rhs) noexcept
    {
        return *this = *this/rhs;
    }
    template<typename T> requires nonbool_integral<T>
    constexpr fraction<std::make_signed_t<T>> fraction<T>::operator-(void) const noexcept
    {
        if(this->numer > static_cast<T>(std::numeric_limits<std::make_signed_t<T>>::max()))
        {
            return {-1, 0};
        }
        if(this->numer == 0)
        {
            return {0, this->denom};
        }
        return {
            static_cast<std::make_signed_t<T>>(std::make_signed_t<T>(-1)
                - static_cast<std::make_signed_t<T>>(this->numer - 1)),
            this->denom
        };
    }
    template<typename T> requires nonbool_integral<T>
    constexpr std::partial_ordering fraction<T>::operator<=>(const fraction<T>& rhs) const noexcept
    {
        if(this->is_nan() || rhs.is_nan())
        {
            return std::partial_ordering::unordered;
        }
        return static_cast<std::partial_ordering>(
            this->numer*static_cast<T>(rhs.denom) <=> rhs.numer*static_cast<T>(this->denom)
        );
    }
    template<typename T> requires nonbool_integral<T>
    constexpr std::partial_ordering fraction<T>::operator<=>(const T& rhs) const noexcept
    {
        if(this->is_nan())
        {
            return std::partial_ordering::unordered;
        }
        return static_cast<std::partial_ordering>(
            this->numer <=> rhs*static_cast<T>(this->denom)
        );
    }
    template<typename T> requires nonbool_integral<T>
    template<typename I> requires nonbool_integral<I>
    constexpr std::partial_ordering fraction<T>::operator<=>(const I& rhs) const noexcept
    {
        if(this->is_nan())
        {
            return std::partial_ordering::unordered;
        }
        if((*this < static_cast<T>(0)) && !(rhs < static_cast<I>(0)))
        {
            return std::partial_ordering::less;
        }
        if(!(*this < static_cast<T>(0)) && (rhs < static_cast<I>(0)))
        {
            return std::partial_ordering::greater;
        }
        if(sizeof(I) > sizeof(T))
        {
            return static_cast<fraction<I>>(*this) <=> rhs;
        }
        return *this <=> static_cast<T>(rhs);
    }
    template<typename T> requires nonbool_integral<T>
    constexpr std::partial_ordering fraction<T>::operator<=>(std::nullptr_t) const noexcept
    {
        if(this->is_nan())
        {
            return std::partial_ordering::unordered;
        }
        return static_cast<std::partial_ordering>(
            this->numer <=> 0
        );
    }
    template<typename T> requires nonbool_integral<T>
    [[nodiscard]] constexpr std::partial_ordering operator<=>(const T& lhs, const fraction<T>& rhs) noexcept
    {
        if(rhs.is_nan())
        {
            return std::partial_ordering::unordered;
        }
        return static_cast<std::partial_ordering>(
            lhs*static_cast<T>(rhs.denom) <=> rhs.numer
        );
    }
    template<typename T, typename I> requires nonbool_integral<T> && nonbool_integral<I>
    [[nodiscard]] constexpr std::partial_ordering operator<=>(const I& lhs, const fraction<T>& rhs) noexcept
    {
        if(rhs.is_nan())
        {
            return std::partial_ordering::unordered;
        }
        if((rhs < static_cast<T>(0)) && !(lhs < static_cast<I>(0)))
        {
            return std::partial_ordering::greater;
        }
        if(!(rhs < static_cast<T>(0)) && (lhs < static_cast<I>(0)))
        {
            return std::partial_ordering::less;
        }
        if(sizeof(I) > sizeof(T))
        {
            return lhs <=> static_cast<fraction<I>>(rhs);
        }
        return static_cast<T>(lhs) <=> rhs;
    }
    template<typename T> requires nonbool_integral<T>
    [[nodiscard]] constexpr std::partial_ordering operator<=>(std::nullptr_t, const fraction<T>& rhs) noexcept
    {
        if(rhs.is_nan())
        {
            return std::partial_ordering::unordered;
        }
        return static_cast<std::partial_ordering>(
            0 <=> rhs.numer
        );
    }
    template<typename T> requires nonbool_integral<T>
    constexpr bool fraction<T>::operator==(const fraction<T>& rhs) const noexcept
    {
        if(this->is_nan() || rhs.is_nan())
        {
            return false;
        }
        return this->numer*static_cast<T>(rhs.denom) == rhs.numer*static_cast<T>(this->denom);
    }
    template<typename T> requires nonbool_integral<T>
    constexpr bool fraction<T>::operator==(const T& rhs) const noexcept
    {
        if(this->is_nan())
        {
            return false;
        }
        return this->numer == rhs*static_cast<T>(this->denom);
    }
    template<typename T> requires nonbool_integral<T>
    template<typename I> requires nonbool_integral<I>
    constexpr bool fraction<T>::operator==(const I& rhs) const noexcept
    {
        if(this->is_nan())
        {
            return false;
        }
        if((*this < static_cast<T>(0)) != (rhs < static_cast<I>(0)))
        {
            return false;
        }
        if(sizeof(I) > sizeof(T))
        {
            return static_cast<fraction<I>>(*this) == rhs;
        }
        return *this == static_cast<T>(rhs);
    }
    template<typename T> requires nonbool_integral<T>
    constexpr bool fraction<T>::operator==(std::nullptr_t) const noexcept
    {
        if(!this->is_finite())
        {
            return false;
        }
        return this->numer == 0;
    }
    template<typename T> requires nonbool_integral<T>
    [[nodiscard]] constexpr bool operator==(const T& lhs, const fraction<T>& rhs) noexcept
    {
        if(rhs.is_nan())
        {
            return false;
        }
        return lhs*static_cast<T>(rhs.denom) == rhs.numer;
    }
    template<typename T, typename I> requires nonbool_integral<T> && nonbool_integral<I>
    [[nodiscard]] constexpr bool operator==(const I& lhs, const fraction<T>& rhs) noexcept
    {
        if(rhs.is_nan())
        {
            return false;
        }
        if((rhs < static_cast<T>(0)) != (lhs < static_cast<I>(0)))
        {
            return false;
        }
        if(sizeof(I) > sizeof(T))
        {
            return lhs == static_cast<fraction<I>>(rhs);
        }
        return static_cast<T>(lhs) == rhs;
    }
    template<typename T> requires nonbool_integral<T>
    [[nodiscard]] constexpr bool operator==(std::nullptr_t, const fraction<T>& rhs) noexcept
    {
        if(!rhs.is_finite())
        {
            return false;
        }
        return 0 == rhs.numer;
    }
    template<typename T> requires nonbool_integral<T>
    constexpr bool fraction<T>::operator!=(const fraction<T>& rhs) const noexcept
    {
        if(this->is_nan() || rhs.is_nan())
        {
            return true;
        }
        return this->numer*static_cast<T>(rhs.denom) != rhs.numer*static_cast<T>(this->denom);
    }
    template<typename T> requires nonbool_integral<T>
    constexpr bool fraction<T>::operator!=(const T& rhs) const noexcept
    {
        if(this->is_nan())
        {
            return true;
        }
        return this->numer != rhs*static_cast<T>(this->denom);
    }
    template<typename T> requires nonbool_integral<T>
    template<typename I> requires nonbool_integral<I>
    constexpr bool fraction<T>::operator!=(const I& rhs) const noexcept
    {
        if(this->is_nan())
        {
            return true;
        }
        if((*this < static_cast<T>(0)) != (rhs < static_cast<I>(0)))
        {
            return true;
        }
        if(sizeof(I) > sizeof(T))
        {
            return static_cast<fraction<I>>(*this) != rhs;
        }
        return *this != static_cast<T>(rhs);
    }
    template<typename T> requires nonbool_integral<T>
    constexpr bool fraction<T>::operator!=(std::nullptr_t) const noexcept
    {
        if(!this->is_finite())
        {
            return true;
        }
        return this->numer != 0;
    }
    template<typename T> requires nonbool_integral<T>
    [[nodiscard]] constexpr bool operator!=(const T& lhs, const fraction<T>& rhs) noexcept
    {
        if(rhs.is_nan())
        {
            return true;
        }
        return lhs*static_cast<T>(rhs.denom) != rhs.numer;
    }
    template<typename T, typename I> requires nonbool_integral<T> && nonbool_integral<I>
    [[nodiscard]] constexpr bool operator!=(const I& lhs, const fraction<T>& rhs) noexcept
    {
        if(rhs.is_nan())
        {
            return true;
        }
        if((rhs < static_cast<T>(0)) != (lhs < static_cast<I>(0)))
        {
            return true;
        }
        if(sizeof(I) > sizeof(T))
        {
            return lhs != static_cast<fraction<I>>(rhs);
        }
        return static_cast<T>(lhs) != rhs;
    }
    template<typename T> requires nonbool_integral<T>
    [[nodiscard]] constexpr bool operator!=(std::nullptr_t, const fraction<T>& rhs) noexcept
    {
        if(!rhs.is_finite())
        {
            return true;
        }
        return 0 != rhs.numer;
    }
    template<typename T, typename O> requires nonbool_integral<T>
    constexpr O&& operator<<(O&& o, const fraction<T>& x) noexcept
    {
        return o << static_cast<std::string>(x);
    }

    // Private ---------------------------------------------------------------------------------------------------------

    template<typename T> requires nonbool_integral<T>
    constexpr void fraction<T>::reduce(void) noexcept
    {
        std::make_unsigned_t<T> gcd {static_cast<std::make_unsigned_t<T>>(std::gcd<T, std::make_unsigned_t<T>>(
            static_cast<T>(this->numer),
            static_cast<std::make_unsigned_t<T>>(this->denom)
        ))};
        if(gcd != 0 && gcd != 1)
        {
            this->numer /= static_cast<T>(gcd);
            this->denom /= gcd;
        }
    }

    template<typename T> requires nonbool_integral<T>
    constexpr std::optional<fraction<T>> fraction<T>::checked_add(const fraction<T>& rhs) const noexcept
    {
        if(rhs.is_zero() || this->is_nan())
        {
            return *this;
        }
        if(this->is_zero() || rhs.is_nan())
        {
            return rhs;
        }
        if(this->denom == rhs.denom)
        {
            std::optional<T> numer = cia::checked_add<T>(this->numer, rhs.numer);
            if(!numer.has_value())
            {
                return std::nullopt;
            }
            return fraction{numer.value(), this->denom};
        }
        std::make_unsigned_t<T> lcm {std::lcm(this->denom, rhs.denom)};
        if(lcm == 0)
        {
            if(this->denom == 0 && rhs.denom == 0)
            {
                std::optional<T> numer {cia::checked_add(this->numer, rhs.numer)};
                return fraction{numer.value_or(0), 0};
            }
            if(this->denom == 0)
            {
                return *this;
            }
            if(rhs.denom == 0)
            {
                return rhs;
            }
            return fraction{0, 0};
        }
        std::make_unsigned_t<T> lcm_div_lhs_denom = lcm/this->denom;
        if(lcm_div_lhs_denom > static_cast<std::make_unsigned_t<T>>(std::numeric_limits<T>::max()))
        {
            return std::nullopt;
        }
        std::optional<T> lhs_numer = cia::checked_mul<T>(this->numer, static_cast<T>(lcm_div_lhs_denom));
        if(!lhs_numer.has_value())
        {
            return std::nullopt;
        }
        std::make_unsigned_t<T> lcm_div_rhs_denom = lcm/rhs.denom;
        if(lcm_div_rhs_denom > static_cast<std::make_unsigned_t<T>>(std::numeric_limits<T>::max()))
        {
            return std::nullopt;
        }
        std::optional<T> rhs_numer = cia::checked_mul<T>(rhs.numer, static_cast<T>(lcm_div_rhs_denom));
        if(!rhs_numer.has_value())
        {
            return std::nullopt;
        }
        std::optional<T> numer = cia::checked_add<T>(lhs_numer.value(), rhs_numer.value());
        if(!numer.has_value())
        {
            return std::nullopt;
        }
        return fraction{numer.value(), lcm};
    }
    template<typename T> requires nonbool_integral<T>
    constexpr std::optional<fraction<T>> fraction<T>::checked_add(const T& rhs) const noexcept
    {
        if(rhs == 0 || this->is_nan())
        {
            return *this;
        }
        if(this->is_zero())
        {
            return rhs;
        }
        if(this->denom > static_cast<std::make_unsigned_t<T>>(std::numeric_limits<T>::max()))
        {
            return std::nullopt;
        }
        std::optional<T> rhs_numer = cia::checked_mul<T>(rhs, static_cast<T>(this->denom));
        if(!rhs_numer.has_value())
        {
            return std::nullopt;
        }
        std::optional<T> numer = cia::checked_add<T>(this->numer, rhs_numer.value());
        if(!numer.has_value())
        {
            return std::nullopt;
        }
        return fraction{numer.value(), this->denom};
    }
    template<typename T> requires nonbool_integral<T>
    constexpr std::optional<fraction<T>> fraction<T>::checked_sub(const fraction<T>& rhs) const noexcept
    {
        if(rhs.is_zero() || this->is_nan())
        {
            return *this;
        }
        if(rhs.is_nan())
        {
            return rhs;
        }
        if(this->denom == rhs.denom)
        {
            std::optional<T> numer = cia::checked_sub<T>(this->numer, rhs.numer);
            if(!numer.has_value())
            {
                return std::nullopt;
            }
            return fraction{numer.value(), this->denom};
        }
        std::make_unsigned_t<T> lcm {std::lcm(this->denom, rhs.denom)};
        if(lcm == 0)
        {
            if(this->denom == 0 && rhs.denom == 0)
            {
                std::optional<T> numer {cia::checked_sub(this->numer, rhs.numer)};
                return fraction{numer.value_or(0), 0};
            }
            if(this->denom == 0)
            {
                return *this;
            }
            if(rhs.denom == 0)
            {
                std::optional<T> numer {cia::checked_sub(T(0), rhs.numer)};
                return fraction{numer.value_or(0), 0};
            }
            return fraction{0, 0};
        }
        std::make_unsigned_t<T> lcm_div_lhs_denom = lcm/this->denom;
        if(lcm_div_lhs_denom > static_cast<std::make_unsigned_t<T>>(std::numeric_limits<T>::max()))
        {
            return std::nullopt;
        }
        std::optional<T> lhs_numer = cia::checked_mul<T>(this->numer, static_cast<T>(lcm_div_lhs_denom));
        if(!lhs_numer.has_value())
        {
            return std::nullopt;
        }
        std::make_unsigned_t<T> lcm_div_rhs_denom = lcm/rhs.denom;
        if(lcm_div_rhs_denom > static_cast<std::make_unsigned_t<T>>(std::numeric_limits<T>::max()))
        {
            return std::nullopt;
        }
        std::optional<T> rhs_numer = cia::checked_mul<T>(rhs.numer, static_cast<T>(lcm_div_rhs_denom));
        if(!rhs_numer.has_value())
        {
            return std::nullopt;
        }
        std::optional<T> numer = cia::checked_sub<T>(lhs_numer.value(), rhs_numer.value());
        if(!numer.has_value())
        {
            return std::nullopt;
        }
        return fraction{numer.value(), lcm};
    }
    template<typename T> requires nonbool_integral<T>
    constexpr std::optional<fraction<T>> fraction<T>::checked_sub(const T& rhs) const noexcept
    {
        if(rhs == 0 || this->is_nan())
        {
            return *this;
        }
        if(this->denom > static_cast<std::make_unsigned_t<T>>(std::numeric_limits<T>::max()))
        {
            return std::nullopt;
        }
        std::optional<T> rhs_numer = cia::checked_mul<T>(rhs, static_cast<T>(this->denom));
        if(!rhs_numer.has_value())
        {
            return std::nullopt;
        }
        std::optional<T> numer = cia::checked_sub<T>(this->numer, rhs_numer.value());
        if(!numer.has_value())
        {
            return std::nullopt;
        }
        return fraction{numer.value(), this->denom};
    }
    template<typename T> requires nonbool_integral<T>
    constexpr std::optional<fraction<T>> fraction<T>::checked_lsub(const T& lhs) const noexcept
    {
        if(this->is_nan())
        {
            return *this;
        }
        if(this->is_zero())
        {
            return lhs;
        }
        if(this->denom > static_cast<std::make_unsigned_t<T>>(std::numeric_limits<T>::max()))
        {
            return std::nullopt;
        }
        std::optional<T> lhs_numer = cia::checked_mul<T>(lhs, static_cast<T>(this->denom));
        if(!lhs_numer.has_value())
        {
            return std::nullopt;
        }
        std::optional<T> numer = cia::checked_sub<T>(lhs_numer.value(), this->numer);
        if(!numer.has_value())
        {
            return std::nullopt;
        }
        return fraction{numer.value(), this->denom};
    }
    template<typename T> requires nonbool_integral<T>
    constexpr std::optional<fraction<T>> fraction<T>::checked_rem(const fraction<T>& rhs) const noexcept
    {
        if(rhs.numer == 0 || this->is_nan())
        {
            return fraction{0, 0};
        }
        if(rhs.is_infinite())
        {
            return *this;
        }
        if(this->denom == rhs.denom)
        {
            std::optional<T> numer = cia::checked_rem<T>(this->numer, rhs.numer);
            if(!numer.has_value())
            {
                return std::nullopt;
            }
            return fraction{numer.value(), this->denom};
        }
        std::make_unsigned_t<T> lcm {std::lcm(this->denom, rhs.denom)};
        if(lcm == 0)
        {
            return fraction{0, 0};
        }
        std::make_unsigned_t<T> lcm_div_lhs_denom = lcm/this->denom;
        if(lcm_div_lhs_denom > static_cast<std::make_unsigned_t<T>>(std::numeric_limits<T>::max()))
        {
            return std::nullopt;
        }
        std::optional<T> lhs_numer = cia::checked_mul<T>(this->numer, static_cast<T>(lcm_div_lhs_denom));
        if(!lhs_numer.has_value())
        {
            return std::nullopt;
        }
        std::make_unsigned_t<T> lcm_div_rhs_denom = lcm/rhs.denom;
        if(lcm_div_rhs_denom > static_cast<std::make_unsigned_t<T>>(std::numeric_limits<T>::max()))
        {
            return std::nullopt;
        }
        std::optional<T> rhs_numer = cia::checked_mul<T>(rhs.numer, static_cast<T>(lcm_div_rhs_denom));
        if(!rhs_numer.has_value())
        {
            return std::nullopt;
        }
        std::optional<T> numer = cia::checked_rem<T>(lhs_numer.value(), rhs_numer.value());
        if(!numer.has_value())
        {
            return std::nullopt;
        }
        return fraction{numer.value(), lcm};
    }
    template<typename T> requires nonbool_integral<T>
    constexpr std::optional<fraction<T>> fraction<T>::checked_rem(const T& rhs) const noexcept
    {
        if(rhs == 0 || this->is_nan())
        {
            return fraction{0, 0};
        }
        if(this->denom > static_cast<std::make_unsigned_t<T>>(std::numeric_limits<T>::max()))
        {
            return std::nullopt;
        }
        std::optional<T> rhs_numer = cia::checked_mul<T>(rhs, static_cast<T>(this->denom));
        if(!rhs_numer.has_value())
        {
            return std::nullopt;
        }
        std::optional<T> numer = cia::checked_rem<T>(this->numer, rhs_numer.value());
        if(!numer.has_value())
        {
            return std::nullopt;
        }
        return fraction{numer.value(), this->denom};
    }
    template<typename T> requires nonbool_integral<T>
    constexpr std::optional<fraction<T>> fraction<T>::checked_lrem(const T& lhs) const noexcept
    {
        if(this->numer == 0)
        {
            return fraction{0, 0};
        }
        if(this->denom > static_cast<std::make_unsigned_t<T>>(std::numeric_limits<T>::max()))
        {
            return std::nullopt;
        }
        std::optional<T> lhs_numer = cia::checked_mul<T>(lhs, static_cast<T>(this->denom));
        if(!lhs_numer.has_value())
        {
            return std::nullopt;
        }
        std::optional<T> numer = cia::checked_rem<T>(lhs_numer.value(), this->numer);
        if(!numer.has_value())
        {
            return std::nullopt;
        }
        return fraction{numer.value(), this->denom};
    }
    template<typename T> requires nonbool_integral<T>
    constexpr std::optional<fraction<T>> fraction<T>::checked_mul(const fraction<T>& rhs) const noexcept
    {
        std::make_unsigned_t<T> gcd_ad {std::gcd<std::make_unsigned_t<T>, std::make_unsigned_t<T>>(
            static_cast<std::make_unsigned_t<T>>(this->numer < 0 ? -1 - this->numer : this->numer)
                + (this->numer < 0 ? 1 : 0),
            rhs.denom
        )};
        if(gcd_ad == 0)
        {
            gcd_ad = 1;
        }
        if(gcd_ad > static_cast<std::make_unsigned_t<T>>(std::numeric_limits<T>::max()))
        {
            return std::nullopt;
        }
        std::make_unsigned_t<T> gcd_bc {std::gcd<std::make_unsigned_t<T>, std::make_unsigned_t<T>>(
            static_cast<std::make_unsigned_t<T>>(rhs.numer < 0 ? -1 - rhs.numer : rhs.numer)
                + (rhs.numer < 0 ? 1 : 0),
            this->denom
        )};
        if(gcd_bc == 0)
        {
            gcd_bc = 1;
        }
        if(gcd_bc > static_cast<std::make_unsigned_t<T>>(std::numeric_limits<T>::max()))
        {
            return std::nullopt;
        }
        std::optional<T> numer {cia::checked_mul<T>(
            this->numer/static_cast<T>(gcd_ad),
            static_cast<T>(rhs.numer/gcd_bc)
        )};
        if(!numer.has_value())
        {
            return std::nullopt;
        }
        std::optional<std::make_unsigned_t<T>> denom {cia::checked_mul<std::make_unsigned_t<T>>(
            this->denom/gcd_bc,
            rhs.denom/gcd_ad
        )};
        if(!denom.has_value())
        {
            return std::nullopt;
        }
        return fraction{numer.value(), denom.value()};
    }
    template<typename T> requires nonbool_integral<T>
    constexpr std::optional<fraction<T>> fraction<T>::checked_mul(const T& rhs) const noexcept
    {
        std::make_unsigned_t<T> gcd {std::gcd<std::make_unsigned_t<T>, std::make_unsigned_t<T>>(
            this->denom,
            static_cast<std::make_unsigned_t<T>>(rhs < 0 ? -1 - rhs : rhs) + (rhs < 0 ? 1 : 0)
        )};
        if(gcd > static_cast<std::make_unsigned_t<T>>(std::numeric_limits<T>::max()))
        {
            return std::nullopt;
        }
        std::optional<T> numer {cia::checked_mul<T>(this->numer, rhs/static_cast<T>(gcd))};
        if(!numer.has_value())
        {
            return std::nullopt;
        }
        return fraction{numer.value(), static_cast<std::make_unsigned_t<T>>(this->denom/gcd)};
    }
    template<typename T> requires nonbool_integral<T>
    constexpr std::optional<fraction<T>> fraction<T>::checked_div(const fraction<T>& rhs) const noexcept
    {
        std::make_unsigned_t<T> gcd_ac {std::gcd<std::make_unsigned_t<T>, std::make_unsigned_t<T>>(
            static_cast<std::make_unsigned_t<T>>(this->numer < 0 ? -1 - this->numer : this->numer)
                + (this->numer < 0 ? 1 : 0),
            static_cast<std::make_unsigned_t<T>>(rhs.numer < 0 ? -1 - rhs.numer : rhs.numer)
                + (rhs.numer < 0 ? 1 : 0)
        )};
        if(gcd_ac == 0)
        {
            gcd_ac = 1;
        }
        if(gcd_ac > static_cast<std::make_unsigned_t<T>>(std::numeric_limits<T>::max()))
        {
            return std::nullopt;
        }
        std::make_unsigned_t<T> gcd_bd {std::gcd<std::make_unsigned_t<T>, std::make_unsigned_t<T>>(
            this->denom,
            rhs.denom
        )};
        if(gcd_bd == 0)
        {
            gcd_bd = 1;
        }
        if(rhs.denom/gcd_bd > static_cast<std::make_unsigned_t<T>>(std::numeric_limits<T>::max()))
        {
            return std::nullopt;
        }
        if(rhs.numer < 0)
        {
            std::optional<T> numer {cia::checked_mul<T>(
                this->numer/static_cast<T>(gcd_ac),
                -static_cast<T>(rhs.denom/gcd_bd)
            )};
            if(!numer.has_value())
            {
                return std::nullopt;
            }
            std::optional<std::make_unsigned_t<T>> denom {cia::checked_mul<std::make_unsigned_t<T>>(
                this->denom/gcd_bd,
                (static_cast<std::make_unsigned_t<T>>(-1 - rhs.numer) + 1)/gcd_ac
            )};
            if(!denom.has_value())
            {
                return std::nullopt;
            }
            return fraction{numer.value(), denom.value()};
        }
        std::optional<T> numer {cia::checked_mul<T>(
            this->numer/static_cast<T>(gcd_ac),
            static_cast<T>(rhs.denom/gcd_bd)
        )};
        if(!numer.has_value())
        {
            return std::nullopt;
        }
        std::optional<std::make_unsigned_t<T>> denom {cia::checked_mul<std::make_unsigned_t<T>>(
            this->denom/gcd_bd,
            static_cast<std::make_unsigned_t<T>>(rhs.numer)/gcd_ac
        )};
        if(!denom.has_value())
        {
            return std::nullopt;
        }
        return fraction{numer.value(), denom.value()};
    }
    template<typename T> requires nonbool_integral<T>
    constexpr std::optional<fraction<T>> fraction<T>::checked_div(const T& rhs) const noexcept
    {
        std::make_unsigned_t<T> gcd {std::gcd<std::make_unsigned_t<T>, std::make_unsigned_t<T>>(
            static_cast<std::make_unsigned_t<T>>(this->numer < 0 ? -1 - numer : numer) + (this->numer < 0 ? 1 : 0),
            static_cast<std::make_unsigned_t<T>>(rhs < 0 ? -1 - rhs : rhs) + (rhs < 0 ? 1 : 0)
        )};
        if(gcd > static_cast<std::make_unsigned_t<T>>(std::numeric_limits<T>::max()))
        {
            return std::nullopt;
        }
        if(rhs < 0)
        {
            T numer {static_cast<T>(this->numer/static_cast<T>(gcd))};
            if(numer == std::numeric_limits<T>::min())
            {
                return std::nullopt;
            }
            std::optional<std::make_unsigned_t<T>> denom {cia::checked_mul<std::make_unsigned_t<T>>(
                this->denom,
                (static_cast<std::make_unsigned_t<T>>(-1 - rhs) + 1)/gcd
            )};
            if(!denom.has_value())
            {
                return std::nullopt;
            }
            return fraction{static_cast<T>(-numer), denom.value()};
        }
        std::optional<std::make_unsigned_t<T>> denom {cia::checked_mul<std::make_unsigned_t<T>>(
            this->denom,
            static_cast<std::make_unsigned_t<T>>(rhs)/gcd
        )};
        if(!denom.has_value())
        {
            return std::nullopt;
        }
        return fraction{static_cast<T>(this->numer/static_cast<T>(gcd)), denom.value()};
    }
    template<typename T> requires nonbool_integral<T>
    constexpr std::optional<fraction<T>> fraction<T>::checked_ldiv(const T& lhs) const noexcept
    {
        std::make_unsigned_t<T> gcd {std::gcd<std::make_unsigned_t<T>, std::make_unsigned_t<T>>(
            static_cast<std::make_unsigned_t<T>>(
                    this->numer < 0 ? std::make_unsigned_t<T>(-1) - this->numer : this->numer
                )
                + std::make_unsigned_t<T>(this->numer < 0 ? 1 : 0),
            static_cast<std::make_unsigned_t<T>>(lhs < 0 ? std::make_unsigned_t<T>(-1) - lhs : lhs)
                + std::make_unsigned_t<T>(lhs < 0 ? 1 : 0)
        )};
        if(gcd > static_cast<std::make_unsigned_t<T>>(std::numeric_limits<T>::max()))
        {
            return std::nullopt;
        }
        if(this->denom > static_cast<std::make_unsigned_t<T>>(std::numeric_limits<T>::max()))
        {
            return std::nullopt;
        }
        if(this->numer < 0)
        {
            std::optional<T> numer {cia::checked_mul<T>(
                lhs/static_cast<T>(gcd),
                -static_cast<T>(this->denom)
            )};
            if(!numer.has_value())
            {
                return std::nullopt;
            }
            return fraction{
                numer.value(),
                static_cast<std::make_unsigned_t<T>>((static_cast<std::make_unsigned_t<T>>(T(-1) - this->numer)
                    + T(1))/gcd)
            };
        }
        std::optional<T> numer {cia::checked_mul<T>(
            lhs/static_cast<T>(gcd),
            static_cast<T>(this->denom)
        )};
        if(!numer.has_value())
        {
            return std::nullopt;
        }
        return fraction{
            numer.value(),
            static_cast<std::make_unsigned_t<T>>(static_cast<std::make_unsigned_t<T>>(this->numer)/gcd)
        };
    }
}