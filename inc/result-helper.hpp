#pragma once

#include "result/result.hpp"
#include "source-location.hpp"

template <typename result_tp>
    requires(result_type::helper::is_result_type<result_tp>)
static inline constexpr auto map_to_str(result_tp &&R, source_location &&err_loc = source_location::current())
{
    using RawResult = std::remove_cvref_t<result_tp>;
    using ErrBase   = typename RawResult::error_type;
    using ErrTp     = std::conditional_t<std::is_lvalue_reference_v<result_tp>, ErrBase &, ErrBase &&>;

    return std::forward<result_tp>(R).map_err(
        [loc = std::move(err_loc)](ErrTp err) -> std::string
        {
            std::ostringstream oss;
            oss << loc << err;
            return std::move(oss).str();
        });
}
