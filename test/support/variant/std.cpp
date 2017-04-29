// Copyright (c) 2017 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#include "../../test_utils.hpp"
#include <scelta/support/variant/std.hpp>

TEST_MAIN()
{
#if defined(SCELTA_SUPPORT_STD_VARIANT)
    static_assert(scelta::impl::is_supported_variant_template_v<::std::variant>);
#endif
}