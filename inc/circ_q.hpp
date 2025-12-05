#pragma once

#include "fmt/base.h"
#include "mmap_wrapper.hpp"
#include "result/result.hpp"

using namespace result_type;
using namespace std::literals;

template <typename T>
concept fmt_tp = fmt::is_formattable<T>::value;

template <fmt_tp T> struct circ_q_formatter;

template <fmt_tp T> class circ_q
{
    template <fmt_tp U> friend struct circ_q_formatter;

private:
    using pointer   = mmap_wrapper<T>::unique_ptr;
    using ref       = T &;
    using cref      = T const &;

    size_t size     = 0;
    size_t front    = 0;
    size_t capacity = 0;

    pointer data    = nullptr;

public:
    circ_q() = delete;
    circ_q(pointer &&array, size_t const cap) : capacity{cap}, data{std::move(array)} {}

    inline bool is_full() const noexcept { return (size == capacity); }
    inline bool is_empty() const noexcept { return (size == 0); }

    Result<void, std::string_view> push(cref val)
    {
        if (data == nullptr)
        {
            return Err("data is a `nullptr`"sv);
        }
        if (is_full())
        {
            return Err("capacity reached! overflow detected"sv);
        }

        size_t const back = (front + size) % capacity;
        data[back]        = val;
        ++size;
        return Ok();
    }

    Result<T, std::string_view> pop()
    {
        if (data == nullptr)
        {
            return Err("data is a `nullptr`"sv);
        }
        if (is_empty())
        {
            return Err("no elements to pop from buffer"sv);
        }

        ref ret_val = data[front];
        front       = (front + 1) % capacity;
        --size;

        return Ok(std::move(ret_val));
    }
};

template <fmt_tp T> struct circ_q_formatter
{
    static inline auto format_impl(circ_q<T> const &q, fmt::format_context &ctx) -> fmt::format_context::iterator
    {
        std::string_view sep = "";
        auto out             = ctx.out();

        if (q.data == nullptr)
        {
            fmt::format_to(out, "unexpected q has `nullptr` as `data`");
            return out;
        }

        if (q.size > q.capacity)
        {
            fmt::format_to(out, "unexpected q `size` > `cap`");
            return out;
        }

        fmt::format_to(out, "[");
        for (size_t i = 0; i < q.size; ++i)
        {
            const size_t idx = (q.front + i) % q.capacity;
            fmt::format_to(out, "{}{}", sep, q.data[idx]);
            sep = ", ";
        }
        fmt::format_to(out, "]");
        return out;
    }
};

template <fmt_tp T> struct fmt::formatter<circ_q<T>>
{
    constexpr auto parse(format_parse_context &ctx) -> format_parse_context::iterator { return ctx.begin(); }

    auto format(const circ_q<T> &q, format_context &ctx) const -> format_context::iterator
    {
        return circ_q_formatter<T>::format_impl(q, ctx);
    }
};
