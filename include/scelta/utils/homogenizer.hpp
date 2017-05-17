// Copyright (c) 2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./optional_utils.hpp"
#include "./returns.hpp"
#include <type_traits>

// Usage of C++17: nested `namespace`.
namespace scelta::impl
{
    // Usage of C++17: `template <...> typename`.
    template <template <typename...> typename>
    struct homogenizer;

    template <typename>
    struct homogenizer_helper;

    template <template <typename...> class Variant, typename... Ts>
    struct homogenizer_helper<Variant<Ts...>>
    {
        using type = homogenizer<Variant>;
    };

    template <typename T>
    using homogenizer_helper_t = typename homogenizer_helper<T>::type;

    // clang-format off
    template <typename T>
    struct visitor_adapter
    {
        template <typename Tag, typename U>
        constexpr auto operator()(Tag, U&& x) const
            SCELTA_RETURNS(
                FWD(x)
            )
    };

    struct non_recursive_tag {};

    template <typename Return>
    struct recursive_tag { using return_type = Return; };

    template <typename Tag, typename Visitor, typename Variant, typename... Variants>
    constexpr auto visit_homogenizer(Tag tag,
        Visitor&& visitor, Variant&& variant, Variants&&... variants)
        SCELTA_RETURNS(
            homogenizer_helper_t<std::decay_t<Variant>>{}(
                visitor_adapter<std::decay_t<Variant>>{}(tag, FWD(visitor)),
                FWD(variant), FWD(variants)...)
        )
    // clang-format on
}

#define SCELTA_DEFINE_HOMOGENIZER_VARIANT(m_type, m_function) \
    namespace scelta::impl                                    \
    {                                                         \
        template <>                                           \
        struct homogenizer<m_type>                            \
        {                                                     \
            template <typename... Ts>                         \
            constexpr auto operator()(Ts&&... xs) const       \
                SCELTA_RETURNS(m_function(FWD(xs)...))        \
        };                                                    \
    }

#define SCELTA_DEFINE_HOMOGENIZER_OPTIONAL(m_type) \
    SCELTA_DEFINE_HOMOGENIZER_VARIANT(m_type, ::scelta::impl::visit_optional)
