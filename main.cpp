#include <iostream>

#include "fraction.hpp"

template<typename A, typename B>
void assert_eq(const A& a, const B& b)
{
    if(!(a == b) || a != b)
    {
        throw "Unequal";
    }
}

template<typename T>
void test()
{
    assert_eq(sss::fraction<T>{2, 4u}, sss::fraction<T>{2, 4});
    assert_eq(sss::fraction<T>{4}, sss::fraction<T>{4, 1});
    assert_eq(sss::fraction<T>{}, sss::fraction<T>{0});
    assert_eq(sss::fraction<T>{4, 3}.get_numer(), 4);
    assert_eq(sss::fraction<T>{4, 3}.get_denom(), 3);
    assert_eq(sss::fraction<T>{0, 0}.is_nan(), true);
    assert_eq(sss::fraction<T>{1, 0}.is_infinite(), true);
    if(std::is_signed<T>::value)
    {
        assert_eq(sss::fraction<std::make_signed_t<T>>{-1, 0}.is_infinite(), true);
    }
    assert_eq(sss::fraction<T>{1, 0}, sss::fraction<T>{4, 0});
    assert_eq(sss::fraction<T>{0}.is_zero(), true);
    assert_eq(sss::fraction<T>{4, 4}.is_one(), true);
    if(std::is_signed<T>::value)
    {
        assert_eq(sss::fraction<std::make_signed_t<T>>{-4, 3}.abs(), sss::fraction<std::make_unsigned_t<T>>{4, 3});
        assert_eq(sss::fraction<std::make_signed_t<T>>{-4, 3}.signum(), -1);
        assert_eq(sss::fraction<std::make_signed_t<T>>{4, 3}.copysign({-5, 7}), sss::fraction<std::make_signed_t<T>>{-4, 3});
        assert_eq(sss::fraction<std::make_signed_t<T>>{4, 3}.copysign(-5), sss::fraction<std::make_signed_t<T>>{-4, 3});
    }
    assert_eq(sss::fraction<T>{4, 3}.recip(), sss::fraction<T>{3, 4});
    assert_eq(sss::fraction<T>{4, 3}.floor(), 1);
    if(std::is_signed<T>::value)
    {
        assert_eq(sss::fraction<std::make_signed_t<T>>{-4, 3}.floor(), -2);
    }
    assert_eq(sss::fraction<T>{4, 3}.ceil(), 2);
    if(std::is_signed<T>::value)
    {
        assert_eq(sss::fraction<std::make_signed_t<T>>{-4, 3}.ceil(), -1);
    }
    assert_eq(sss::fraction<T>{4, 3}.round(), 1);
    if(std::is_signed<T>::value)
    {
        assert_eq(sss::fraction<std::make_signed_t<T>>{-4, 3}.round(), -1);
    }
    assert_eq(sss::fraction<T>{4, 3}.trunc(), 1);
    if(std::is_signed<T>::value)
    {
        assert_eq(sss::fraction<std::make_signed_t<T>>{-4, 3}.trunc(), -1);
    }
    assert_eq(sss::fraction<T>{4, 3}.fract(), sss::fraction<T>{1, 3});
    if(std::is_signed<T>::value)
    {
        assert_eq(sss::fraction<std::make_signed_t<T>>{-4, 3}.fract(), sss::fraction<std::make_signed_t<T>>{-1, 3});
    }
    assert_eq(sss::fraction<T>{4, 3}.pow(2), sss::fraction<T>{16, 9});
    if(std::is_signed<T>::value)
    {
        assert_eq(sss::fraction<std::make_signed_t<T>>{-4, 3}.pow(3), sss::fraction<std::make_signed_t<T>>{-64, 27});
    }
    else
    {
        assert_eq(sss::fraction<T>{4, 3}.pow(3), sss::fraction<T>{64, 27});
    }
    assert_eq(static_cast<T>(sss::fraction<T>{4, 3}), sss::fraction<T>{4, 3}.trunc());
    assert_eq(static_cast<float>(sss::fraction<T>{4, 3}), 4.0f/3.0f);
    assert_eq(static_cast<double>(sss::fraction<T>{4, 3}), 4.0/3.0);
    assert_eq(static_cast<std::string>(sss::fraction<T>{4, 3}), "4/3");
    assert_eq(sss::fraction<T>{4, 3} + sss::fraction<T>{1, 3}, sss::fraction<T>{5, 3});
    assert_eq(sss::fraction<T>{4, 3} - sss::fraction<T>{1, 3}, 1);
    assert_eq(sss::fraction<T>{4, 3}%sss::fraction<T>{5, 7}, sss::fraction<T>{(4*7) % (5*3), 3*7});
    assert_eq(sss::fraction<T>{4, 3}*sss::fraction<T>{5, 7}, sss::fraction<T>{4*5, 3*7});
    assert_eq(sss::fraction<T>{4, 3}/sss::fraction<T>{5, 7}, sss::fraction<T>{4*7, 3*5});
    sss::fraction<T> a = {4, 3};
    a += {1, 3};
    assert_eq(a, sss::fraction<T>{5, 3});
    a = {4, 3};
    a -= {1, 3};
    assert_eq(a, 1);
    a = {4, 3};
    a %= {5, 7};
    assert_eq(a, sss::fraction<T>{(4*7) % (5*3), 3*7});
    a = {4, 3};
    a *= {5, 7};
    assert_eq(a, sss::fraction<T>{4*5, 3*7});
    a = {4, 3};
    a /= {5, 7};
    assert_eq(a, sss::fraction<T>{4*7, 3*5});
    assert_eq(sss::fraction<T>{4, 3} + 2, sss::fraction<T>{10, 3});
    if(std::is_signed<T>::value)
    {
        assert_eq(sss::fraction<std::make_signed_t<T>>{4, 3} - 2, sss::fraction<std::make_signed_t<T>>{-2, 3});
    }
    else
    {
        assert_eq(sss::fraction<T>{4, 3} - 1, sss::fraction<T>{1, 3});
    }
    assert_eq(sss::fraction<T>{4, 3}%1, sss::fraction<T>{1, 3});
    assert_eq(sss::fraction<T>{4, 3}*2, sss::fraction<T>{8, 3});
    assert_eq(sss::fraction<T>{4, 3}/2, sss::fraction<T>{2, 3});
    assert_eq(static_cast<T>(2) + sss::fraction<T>{4, 3}, sss::fraction<T>{10, 3});
    assert_eq(static_cast<T>(2) - sss::fraction<T>{4, 3}, sss::fraction<T>{2, 3});
    assert_eq(static_cast<T>(2)%sss::fraction<T>{4, 3}, sss::fraction<T>{2, 3});
    assert_eq(static_cast<T>(2)*sss::fraction<T>{4, 3}, sss::fraction<T>{8, 3});
    assert_eq(static_cast<T>(2)/sss::fraction<T>{4, 3}, sss::fraction<T>{3, 2});
    a = {4, 3};
    a += 2;
    assert_eq(a, sss::fraction<T>{10, 3});
    if(std::is_signed<T>::value)
    {
        a = {4, 3};
        a -= 2;
        assert_eq(static_cast<sss::fraction<std::make_signed_t<T>>>(a), sss::fraction<std::make_signed_t<T>>{-2, 3});
    }
    else
    {
        a = {4, 3};
        a -= 1;
        assert_eq(a, sss::fraction<T>{1, 3});
    }
    a = {4, 3};
    a %= 1;
    assert_eq(a, sss::fraction<T>{1, 3});
    a = {4, 3};
    a *= 2;
    assert_eq(a, sss::fraction<T>{8, 3});
    a = {4, 3};
    a /= 2;
    assert_eq(a, sss::fraction<T>{2, 3});
    assert_eq(-sss::fraction<T>{4, 3}, sss::fraction<std::make_signed_t<T>>{-4, 3});
    assert_eq(sss::fraction<T>{4, 3} > 1, true);
    assert_eq(sss::fraction<T>{4, 3} == sss::fraction<T>{4, 3}, true);
    assert_eq(sss::fraction<T>{4, 3} < sss::fraction<T>{5, 3}, true);
    assert_eq(sss::fraction<T>{0, 0} != sss::fraction<T>{0, 0}, true);
    assert_eq(sss::fraction<T>{4, 3} > 0, true);
    assert_eq(sss::fraction<T>{0} == 0, true);
    assert_eq(sss::fraction<T>{4, 3} != 0, true);
    if(std::is_signed<T>::value)
    {
        assert_eq(sss::fraction<std::make_signed_t<T>>{-4, 3} < 0, true);
    }
    assert_eq(std::numeric_limits<sss::fraction<T>>::min() != 0, true);
    assert_eq(std::numeric_limits<sss::fraction<T>>::min()/2, 0);
    assert_eq(std::numeric_limits<sss::fraction<T>>::max().is_finite(), true);
    assert_eq((std::numeric_limits<sss::fraction<T>>::max() + 1), sss::fraction<T>{1, 0});
    assert_eq(std::numeric_limits<sss::fraction<T>>::lowest().is_finite(), true);
    if(std::is_signed<T>::value)
    {
        assert_eq(
            (std::numeric_limits<sss::fraction<std::make_signed_t<T>>>::lowest() - 1),
            sss::fraction<std::make_signed_t<T>>{-1, 0}
        );
    }
    else
    {
        assert_eq((std::numeric_limits<sss::fraction<T>>::lowest() - 1).is_nan(), true);
    }
    assert_eq((static_cast<T>(1) + std::numeric_limits<sss::fraction<T>>::epsilon()) != 1, true);
    assert_eq((static_cast<T>(1) - std::numeric_limits<sss::fraction<T>>::epsilon()) != 1, true);
    assert_eq(
        (static_cast<T>(1) + (std::numeric_limits<sss::fraction<T>>::epsilon()
            - std::numeric_limits<sss::fraction<T>>::min())),
        1
    );
    
    assert_eq((sss::fraction<T>{0, 0} + sss::fraction<T>{1}).is_nan(), true);
    assert_eq(sss::fraction<T>{1, 0} + sss::fraction<T>{1, 0}, sss::fraction<T>{1, 0});
    if(std::is_signed<T>::value)
    {
        assert_eq(
            sss::fraction<std::make_signed_t<T>>{-1, 0} + sss::fraction<std::make_signed_t<T>>{-1, 0},
            sss::fraction<std::make_signed_t<T>>{-1, 0}
        );
        assert_eq(
            (sss::fraction<std::make_signed_t<T>>{1, 0} + sss::fraction<std::make_signed_t<T>>{-1, 0}).is_nan(),
            true
        );
        assert_eq(
            (sss::fraction<std::make_signed_t<T>>{-1, 0} + sss::fraction<std::make_signed_t<T>>{1, 0}).is_nan(),
            true
        );
    }
    assert_eq((sss::fraction<T>{0, 0} - sss::fraction<T>{1}).is_nan(), true);
    assert_eq((sss::fraction<T>{1, 0} - sss::fraction<T>{1, 0}).is_nan(), true);
    if(std::is_signed<T>::value)
    {
        assert_eq(
            sss::fraction<std::make_signed_t<T>>{1, 0} - sss::fraction<std::make_signed_t<T>>{-1, 0},
            sss::fraction<std::make_signed_t<T>>{1, 0}
        );
        assert_eq(
            sss::fraction<std::make_signed_t<T>>{-1, 0} - sss::fraction<std::make_signed_t<T>>{1, 0},
            sss::fraction<std::make_signed_t<T>>{-1, 0}
        );
        assert_eq(
            (sss::fraction<std::make_signed_t<T>>{-1, 0} - sss::fraction<std::make_signed_t<T>>{-1, 0}).is_nan(),
            true
        );
    }
    assert_eq((sss::fraction<T>{3, 4} % sss::fraction<T>{0}).is_nan(), true);
    assert_eq((static_cast<T>(1) % sss::fraction<T>{0}).is_nan(), true);
    assert_eq((sss::fraction<T>{1, 0} % sss::fraction<T>{1}).is_nan(), true);
    if(std::is_signed<T>::value)
    {
        assert_eq(
            (sss::fraction<std::make_signed_t<T>>{-3, 4} % sss::fraction<std::make_signed_t<T>>{0}).is_nan(),
            true
        );
        assert_eq((static_cast<std::make_signed_t<T>>(-1) % sss::fraction<std::make_signed_t<T>>{0}).is_nan(), true);
        assert_eq(
            (sss::fraction<std::make_signed_t<T>>{-1, 0} % sss::fraction<std::make_signed_t<T>>{1}).is_nan(),
            true
        );
    }
    assert_eq((sss::fraction<T>{1, 0} * sss::fraction<T>{0}).is_nan(), true);
    assert_eq(sss::fraction<T>{1, 0} * sss::fraction<T>{1}, sss::fraction<T>{1, 0});
    assert_eq(sss::fraction<T>{1, 0} * sss::fraction<T>{1, 0}, sss::fraction<T>{1, 0});
    if(std::is_signed<T>::value)
    {
        assert_eq(
            sss::fraction<std::make_signed_t<T>>{1, 0} * sss::fraction<std::make_signed_t<T>>{-1, 0},
            sss::fraction<std::make_signed_t<T>>{-1, 0}
        );
        assert_eq(
            sss::fraction<std::make_signed_t<T>>{-1, 0} * sss::fraction<std::make_signed_t<T>>{-1, 0},
            sss::fraction<std::make_signed_t<T>>{1, 0}
        );
        assert_eq(
            sss::fraction<std::make_signed_t<T>>{-1, 0} * sss::fraction<std::make_signed_t<T>>{1, 0},
            sss::fraction<std::make_signed_t<T>>{-1, 0}
        );
    }
    assert_eq(sss::fraction<T>{1} / sss::fraction<T>{0}, sss::fraction<T>{1, 0});
    assert_eq(sss::fraction<T>{1, 0} / sss::fraction<T>{0}, sss::fraction<T>{1, 0});
    assert_eq((sss::fraction<T>{0} / sss::fraction<T>{0}).is_nan(), true);
    if(std::is_signed<T>::value)
    {
        assert_eq(
            sss::fraction<std::make_signed_t<T>>{-1} / sss::fraction<std::make_signed_t<T>>{0},
            sss::fraction<std::make_signed_t<T>>{-1, 0}
        );
        assert_eq(
            sss::fraction<std::make_signed_t<T>>{-1, 0} / sss::fraction<std::make_signed_t<T>>{0},
            sss::fraction<std::make_signed_t<T>>{-1, 0}
        );
    }
}

template<typename T>
void test_all()
{
    for(T n {std::numeric_limits<T>::min()}; n != std::numeric_limits<T>::max(); ++n)
    {
        for(
            std::make_unsigned_t<T> d {std::numeric_limits<std::make_unsigned_t<T>>::min()};
            d != std::numeric_limits<std::make_unsigned_t<T>>::max();
            ++d
        )
        {
            sss::fraction<T> f {n, d};
            if(f.get_numer() == n && f.get_denom() == d)
            {
                (void)f.recip();
                (void)f.floor();
                (void)f.ceil();
                (void)f.round();
                (void)f.trunc();
                (void)f.fract();
                (void)f.pow(2);
                (void)(-f);
                for(T n2 {std::numeric_limits<T>::min()}; n2 != std::numeric_limits<T>::max(); ++n2)
                {
                    (void)(f + n2);
                    (void)(f - n2);
                    (void)(f % n2);
                    (void)(f * n2);
                    (void)(f / n2);
                    (void)(n2 + f);
                    (void)(n2 - f);
                    (void)(n2 % f);
                    (void)(n2 * f);
                    (void)(n2 / f);
                    for(
                        std::make_unsigned_t<T> d2 {std::numeric_limits<std::make_unsigned_t<T>>::min()};
                        d2 != std::numeric_limits<std::make_unsigned_t<T>>::max();
                        ++d2
                    )
                    {
                        sss::fraction<T> t {n2, d2};
                        if(t.get_numer() == n2 && t.get_denom() == d2)
                        {
                            (void)(f + t);
                            (void)(f - t);
                            (void)(f % t);
                            (void)(f * t);
                            (void)(f / t);
                        }
                    }
                }
            }
        }
    }
}

int main()
{
    test<char>();
    test<unsigned char>();
    test<short>();
    test<unsigned short>();
    test<int>();
    test<unsigned int>();
    test<long>();
    test<unsigned long>();
    test<long long>();
    test<unsigned long long>();

    //test_all<char>();
    
    constexpr sss::fraction<int> A {3, 5};
    constexpr sss::fraction<int> B {-2, 6};
    constexpr sss::fraction<int> C {A%B};

    constexpr int I {-3%(-2)};
}