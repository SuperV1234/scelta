#include "./test_utils.hpp"
#include <scelta/support.hpp>
#include <scelta/utils.hpp>

namespace test
{
    namespace impl
    {
        template <typename T>
        struct type_w_t
        {
            using type = T;
        };
    }

    template <typename... Ts>
    constexpr impl::type_w_t<Ts...> type_w{};

    namespace impl
    {
        template <typename... Ts>
        struct alternatives_t
        {
        };

        template <typename>
        struct unpack_alternatives;

        template <typename... Ts>
        struct unpack_alternatives<alternatives_t<Ts...>>
        {
            template <template <typename...> class T>
            struct applier
            {
                using type = T<Ts...>;
            };
        };

        template <typename T>
        struct maker_t
        {
            template <typename... Ts>
            constexpr auto operator()(Ts&&... xs) const
                SCELTA_RETURNS(T(FWD(xs)...))
        };
    }

    template <typename... Ts>
    constexpr impl::alternatives_t<Ts...> alternatives{};

    template <template <template <typename...> class> class TestCase,
        typename TF>
    void instantiate_with_all_variant_implementations(TF&& f)
    {
#if defined(SCELTA_SUPPORT_VARIANT_STD)
        f(TestCase<std::variant>{});
#endif

#if defined(SCELTA_SUPPORT_VARIANT_BOOST)
        f(TestCase<boost::variant>{});
#endif

#if defined(SCELTA_SUPPORT_VARIANT_EGGS)
        f(TestCase<eggs::variant>{});
#endif
    }

    template <template <template <typename...> class> class TestCase,
        typename TF>
    void instantiate_with_all_optional_implementations(TF&& f)
    {
#if defined(SCELTA_SUPPORT_OPTIONAL_STD)
        f(TestCase<std::optional>{});
#endif

#if defined(SCELTA_SUPPORT_OPTIONAL_BOOST)
        f(TestCase<boost::optional>{});
#endif
    }

    template <typename TAlternatives, typename TF>
    void with_all_variant_implementations(TAlternatives, TF&& f)
    {
        using helper = impl::unpack_alternatives<TAlternatives>;
        instantiate_with_all_variant_implementations<helper::template applier>(
            [&](auto x) { f(impl::maker_t<typename decltype(x)::type>{}); });
    }

    template <typename TAlternative, typename TF>
    void with_all_optional_implementations(TF&& f)
    {
        using helper =
            impl::unpack_alternatives<impl::alternatives_t<TAlternative>>;
        instantiate_with_all_optional_implementations<helper::template applier>(
            [&](auto x) { f(impl::maker_t<typename decltype(x)::type>{}); });
    }
}