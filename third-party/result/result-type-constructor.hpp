#pragma once
#include "result-helper.hpp"
#include <utility>

namespace result_type
{
    template <typename T> struct [[nodiscard]] Ok : helper::check_value_type<T>
    {
        using value_type = T;

        template <typename Tp, typename Er> friend class Result;

        explicit constexpr Ok(T &&value) : value_(std::move(value)) {}
        explicit constexpr Ok(const T &value) : value_(value) {}

    private:
        T value_;
    };

    template <typename E> struct [[nodiscard]] Err : helper::check_error_type<E>
    {
        using value_type = E;

        template <typename Tp, typename Er> friend class Result;

        explicit constexpr Err(E &&value) : value_(std::move(value)) {}
        explicit constexpr Err(const E &value) : value_(value) {}

    private:
        E value_;
    };

    /////////////////////////////////////////////////////////////////////////
    // void specialization
    /////////////////////////////////////////////////////////////////////////

    template <> struct [[nodiscard]] Ok<void>
    {
        using value_type = void;

        template <typename Tp, typename Er> friend class Result;
        explicit constexpr Ok() = default;
    };
    template <typename = void> Ok() -> Ok<void>;
    template <typename T> Ok(T) -> Ok<T>;
} // namespace result_type
