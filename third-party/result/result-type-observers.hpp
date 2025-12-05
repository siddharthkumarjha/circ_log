#pragma once
#include "panic.hpp"
#include "result-helper.hpp"
#include "result-type-definition.hpp"
#include <functional>
#include <type_traits>

namespace result_type
{
    template <typename T, typename E> constexpr bool Result<T, E>::is_ok() const noexcept
    {
        return result_variant_.index() == detail::ResultKind::Ok;
    }
    template <typename T, typename E> constexpr bool Result<T, E>::is_err() const noexcept
    {
        return result_variant_.index() == detail::ResultKind::Err;
    }

    template <typename T, typename E> template <typename F> constexpr bool Result<T, E>::is_ok_and(F &&f) const &
    {
        using Res = helper::fn_eval_result<F, const T &>;
        static_assert(std::is_same_v<Res, bool>);

        if (is_ok())
        {
            return std::invoke(std::forward<F>(f), std::get<detail::ResultKind::Ok>(result_variant_));
        }
        return false;
    }
    template <typename T, typename E> template <typename F> constexpr bool Result<T, E>::is_ok_and(F &&f) &&
    {
        using Res = helper::fn_eval_result<F, T &&>;
        static_assert(std::is_same_v<Res, bool>);

        if (is_ok())
        {
            return std::invoke(std::forward<F>(f), std::get<detail::ResultKind::Ok>(std::move(result_variant_)));
        }
        return false;
    }

    template <typename T, typename E> template <typename F> constexpr bool Result<T, E>::is_err_and(F &&f) const &
    {
        using Res = helper::fn_eval_result<F, const E &>;
        static_assert(std::is_same_v<Res, bool>);

        if (is_err())
        {
            return std::invoke(std::forward<F>(f), std::get<detail::ResultKind::Err>(result_variant_));
        }
        return false;
    }
    template <typename T, typename E> template <typename F> constexpr bool Result<T, E>::is_err_and(F &&f) &&
    {
        using Res = helper::fn_eval_result<F, E &&>;
        static_assert(std::is_same_v<Res, bool>);

        if (is_err())
        {
            return std::invoke(std::forward<F>(f), std::get<detail::ResultKind::Err>(std::move(result_variant_)));
        }
        return false;
    }

    template <typename T, typename E>
    template <typename OkFn, typename ErrFn>
    constexpr auto Result<T, E>::match(OkFn &&ok_fn, ErrFn &&err_fn) & -> std::invoke_result_t<OkFn, T &>
    {
        static_assert(std::is_invocable_v<OkFn, T &>);
        static_assert(std::is_invocable_v<ErrFn, E &>);
        using OkRes  = helper::fn_eval_result<OkFn, T &>;
        using ErrRes = helper::fn_eval_result<ErrFn, E &>;
        static_assert(std::is_convertible_v<ErrRes, OkRes>);
        if (is_ok())
        {
            return std::invoke(std::forward<OkFn>(ok_fn), std::get<detail::ResultKind::Ok>(result_variant_));
        }
        else
        {
            return std::invoke(std::forward<ErrFn>(err_fn), std::get<detail::ResultKind::Err>(result_variant_));
        }
    }
    template <typename T, typename E>
    template <typename OkFn, typename ErrFn>
    constexpr auto Result<T, E>::match(OkFn &&ok_fn, ErrFn &&err_fn) const & -> std::invoke_result_t<OkFn, T const &>
    {
        static_assert(std::is_invocable_v<OkFn, const T &>);
        static_assert(std::is_invocable_v<ErrFn, const E &>);
        using OkRes  = helper::fn_eval_result<OkFn, const T &>;
        using ErrRes = helper::fn_eval_result<ErrFn, const E &>;
        static_assert(std::is_convertible_v<ErrRes, OkRes>);
        if (is_ok())
        {
            return std::invoke(std::forward<OkFn>(ok_fn), std::get<detail::ResultKind::Ok>(result_variant_));
        }
        else
        {
            return std::invoke(std::forward<ErrFn>(err_fn), std::get<detail::ResultKind::Err>(result_variant_));
        }
    }
    template <typename T, typename E>
    template <typename OkFn, typename ErrFn>
    constexpr auto Result<T, E>::match(OkFn &&ok_fn, ErrFn &&err_fn) && -> std::invoke_result_t<OkFn, T &&>
    {
        static_assert(std::is_invocable_v<OkFn, T &&>);
        static_assert(std::is_invocable_v<ErrFn, E &&>);
        using OkRes  = helper::fn_eval_result<OkFn, T &&>;
        using ErrRes = helper::fn_eval_result<ErrFn, E &&>;
        static_assert(std::is_convertible_v<ErrRes, OkRes>);
        if (is_ok())
        {
            return std::invoke(std::forward<OkFn>(ok_fn), std::get<detail::ResultKind::Ok>(std::move(result_variant_)));
        }
        else
        {
            return std::invoke(std::forward<ErrFn>(err_fn),
                               std::get<detail::ResultKind::Err>(std::move(result_variant_)));
        }
    }

    template <typename T, typename E> constexpr T &Result<T, E>::unwrap() &
    {
        static_assert(std::is_copy_assignable_v<E>, "ill-formed");
        if (is_err())
        {
            panic("called `Result::unwrap()` on an `Err` value ", std::get<detail::ResultKind::Err>(result_variant_));
        }
        return std::get<detail::ResultKind::Ok>(result_variant_);
    }
    template <typename T, typename E> constexpr const T &Result<T, E>::unwrap() const &
    {
        static_assert(std::is_copy_assignable_v<E>, "ill-formed");
        if (is_err())
        {
            panic("called `Result::unwrap()` on an `Err` value ", std::get<detail::ResultKind::Err>(result_variant_));
        }
        return std::get<detail::ResultKind::Ok>(result_variant_);
    }
    template <typename T, typename E> constexpr T &&Result<T, E>::unwrap() &&
    {
        static_assert(std::is_move_assignable_v<E>, "ill-formed");
        if (is_err())
        {
            panic("called `Result::unwrap()` on an `Err` value ",
                  std::get<detail::ResultKind::Err>(std::move(result_variant_)));
        }
        return std::get<detail::ResultKind::Ok>(std::move(result_variant_));
    }
    template <typename T, typename E> constexpr const T &&Result<T, E>::unwrap() const &&
    {
        static_assert(std::is_move_assignable_v<E>, "ill-formed");
        if (is_err())
        {
            panic("called `Result::unwrap()` on an `Err` value ",
                  std::get<detail::ResultKind::Err>(std::move(result_variant_)));
        }
        return std::get<detail::ResultKind::Ok>(std::move(result_variant_));
    }

    template <typename T, typename E> constexpr E &Result<T, E>::unwrap_err() &
    {
        static_assert(std::is_copy_assignable_v<T>, "ill-formed");
        if (is_ok())
        {
            panic("called `Result::unwrap_err()` on an `Ok` value ", std::get<detail::ResultKind::Ok>(result_variant_));
        }
        return std::get<detail::ResultKind::Err>(result_variant_);
    }
    template <typename T, typename E> constexpr const E &Result<T, E>::unwrap_err() const &
    {
        static_assert(std::is_copy_assignable_v<T>, "ill-formed");
        if (is_ok())
        {
            panic("called `Result::unwrap_err()` on an `Ok` value ", std::get<detail::ResultKind::Ok>(result_variant_));
        }
        return std::get<detail::ResultKind::Err>(result_variant_);
    }
    template <typename T, typename E> constexpr E &&Result<T, E>::unwrap_err() &&
    {
        static_assert(std::is_move_assignable_v<T>, "ill-formed");
        if (is_ok())
        {
            panic("called `Result::unwrap_err()` on an `Ok` value ",
                  std::get<detail::ResultKind::Ok>(std::move(result_variant_)));
        }
        return std::get<detail::ResultKind::Err>(std::move(result_variant_));
    }
    template <typename T, typename E> constexpr const E &&Result<T, E>::unwrap_err() const &&
    {
        static_assert(std::is_move_assignable_v<T>, "ill-formed");
        if (is_ok())
        {
            panic("called `Result::unwrap_err()` on an `Ok` value ",
                  std::get<detail::ResultKind::Ok>(std::move(result_variant_)));
        }
        return std::get<detail::ResultKind::Err>(std::move(result_variant_));
    }

    template <typename T, typename E> template <class U> constexpr T Result<T, E>::unwrap_or(U &&default_value) const &
    {
        static_assert(std::is_copy_constructible_v<T>);
        static_assert(std::is_convertible_v<U, T>);

        return is_ok() ? std::get<detail::ResultKind::Ok>(result_variant_)
                       : static_cast<T>(std::forward<U>(default_value));
    }
    template <typename T, typename E> template <class U> constexpr T Result<T, E>::unwrap_or(U &&default_value) &&
    {
        static_assert(std::is_move_constructible_v<T>);
        static_assert(std::is_convertible_v<U, T>);

        return is_ok() ? std::get<detail::ResultKind::Ok>(std::move(result_variant_))
                       : static_cast<T>(std::forward<U>(default_value));
    }

    template <typename T, typename E>
    template <class G>
    constexpr E Result<T, E>::unwrap_err_or(G &&default_value) const &
    {
        static_assert(std::is_copy_constructible_v<E>);
        static_assert(std::is_convertible_v<G, E>);

        return is_err() ? std::get<detail::ResultKind::Err>(result_variant_)
                        : static_cast<E>(std::forward<G>(default_value));
    }
    template <typename T, typename E> template <class G> constexpr E Result<T, E>::unwrap_err_or(G &&default_value) &&
    {
        static_assert(std::is_move_constructible_v<E>);
        static_assert(std::is_convertible_v<G, E>);

        return is_err() ? std::get<detail::ResultKind::Err>(std::move(result_variant_))
                        : static_cast<E>(std::forward<G>(default_value));
    }

    /////////////////////////////////////////////////////////////////////////
    // void specialization
    /////////////////////////////////////////////////////////////////////////

    template <typename E> constexpr bool Result<void, E>::is_ok() const noexcept
    {
        return result_variant_.index() == detail::ResultKind::Ok;
    }
    template <typename E> constexpr bool Result<void, E>::is_err() const noexcept
    {
        return result_variant_.index() == detail::ResultKind::Err;
    }

    template <typename E> template <typename F> constexpr bool Result<void, E>::is_ok_and(F &&f)
    {
        using Res = helper::fn_eval_result_no_arg<F>;
        static_assert(std::is_same_v<Res, bool>);

        if (is_ok())
        {
            return std::invoke(std::forward<F>(f));
        }
        return false;
    }

    template <typename E> template <typename F> constexpr bool Result<void, E>::is_err_and(F &&f) const &
    {
        using Res = helper::fn_eval_result<F, const E &>;
        static_assert(std::is_same_v<Res, bool>);

        if (is_err())
        {
            return std::invoke(std::forward<F>(f), std::get<detail::ResultKind::Err>(result_variant_));
        }
        return false;
    }
    template <typename E> template <typename F> constexpr bool Result<void, E>::is_err_and(F &&f) &&
    {
        using Res = helper::fn_eval_result<F, E &&>;
        static_assert(std::is_same_v<Res, bool>);

        if (is_err())
        {
            return std::invoke(std::forward<F>(f), std::get<detail::ResultKind::Err>(std::move(result_variant_)));
        }
        return false;
    }

    template <typename E>
    template <typename OkFn, typename ErrFn>
    constexpr auto Result<void, E>::match(OkFn &&ok_fn, ErrFn &&err_fn) & -> std::invoke_result_t<OkFn>
    {
        static_assert(std::is_invocable_v<OkFn>);
        static_assert(std::is_invocable_v<ErrFn, E &>);
        using OkRes  = helper::fn_eval_result_no_arg<OkFn>;
        using ErrRes = helper::fn_eval_result<ErrFn, E &>;
        static_assert(std::is_convertible_v<ErrRes, OkRes>);
        if (is_ok())
        {
            return std::invoke(std::forward<OkFn>(ok_fn));
        }
        else
        {
            return std::invoke(std::forward<ErrFn>(err_fn), std::get<detail::ResultKind::Err>(result_variant_));
        }
    }
    template <typename E>
    template <typename OkFn, typename ErrFn>
    constexpr auto Result<void, E>::match(OkFn &&ok_fn, ErrFn &&err_fn) const & -> std::invoke_result_t<OkFn>
    {
        static_assert(std::is_invocable_v<OkFn>);
        static_assert(std::is_invocable_v<ErrFn, const E &>);
        using OkRes  = helper::fn_eval_result_no_arg<OkFn>;
        using ErrRes = helper::fn_eval_result<ErrFn, const E &>;
        static_assert(std::is_convertible_v<ErrRes, OkRes>);
        if (is_ok())
        {
            return std::invoke(std::forward<OkFn>(ok_fn));
        }
        else
        {
            return std::invoke(std::forward<ErrFn>(err_fn), std::get<detail::ResultKind::Err>(result_variant_));
        }
    }
    template <typename E>
    template <typename OkFn, typename ErrFn>
    constexpr auto Result<void, E>::match(OkFn &&ok_fn, ErrFn &&err_fn) && -> std::invoke_result_t<OkFn>
    {
        static_assert(std::is_invocable_v<OkFn>);
        static_assert(std::is_invocable_v<ErrFn, E &&>);
        using OkRes  = helper::fn_eval_result_no_arg<OkFn>;
        using ErrRes = helper::fn_eval_result<ErrFn, E &&>;
        static_assert(std::is_convertible_v<ErrRes, OkRes>);
        if (is_ok())
        {
            return std::invoke(std::forward<OkFn>(ok_fn));
        }
        else
        {
            return std::invoke(std::forward<ErrFn>(err_fn),
                               std::get<detail::ResultKind::Err>(std::move(result_variant_)));
        }
    }

    template <typename E> constexpr void Result<void, E>::unwrap() const &
    {
        static_assert(std::is_copy_constructible_v<E>);

        if (is_err())
        {
            panic("called `Result::unwrap()` on an `Err` value ", std::get<detail::ResultKind::Err>(result_variant_));
        }
    }
    template <typename E> constexpr void Result<void, E>::unwrap() &&
    {
        static_assert(std::is_copy_constructible_v<E>);
        static_assert(std::is_move_constructible_v<E>);

        if (is_err())
        {
            panic("called `Result::unwrap()` on an `Err` value ",
                  std::get<detail::ResultKind::Err>(std::move(result_variant_)));
        }
    }

    template <typename E> constexpr E &Result<void, E>::unwrap_err() &
    {
        if (is_ok())
        {
            panic("called `Result::unwrap_err()` on an void `Ok` value");
        }
        return std::get<detail::ResultKind::Err>(result_variant_);
    }
    template <typename E> constexpr const E &Result<void, E>::unwrap_err() const &
    {
        if (is_ok())
        {
            panic("called `Result::unwrap_err()` on an void `Ok` value");
        }
        return std::get<detail::ResultKind::Err>(result_variant_);
    }
    template <typename E> constexpr E &&Result<void, E>::unwrap_err() &&
    {
        if (is_ok())
        {
            panic("called `Result::unwrap_err()` on an void `Ok` value");
        }
        return std::get<detail::ResultKind::Err>(std::move(result_variant_));
    }
    template <typename E> constexpr const E &&Result<void, E>::unwrap_err() const &&
    {
        if (is_ok())
        {
            panic("called `Result::unwrap_err()` on an void `Ok` value");
        }
        return std::get<detail::ResultKind::Err>(std::move(result_variant_));
    }

    template <typename E> template <class G> constexpr E Result<void, E>::unwrap_err_or(G &&default_value) const &
    {
        static_assert(std::is_copy_constructible_v<E>);
        static_assert(std::is_convertible_v<G, E>);

        return is_err() ? std::get<detail::ResultKind::Err>(result_variant_)
                        : static_cast<E>(std::forward<G>(default_value));
    }
    template <typename E> template <class G> constexpr E Result<void, E>::unwrap_err_or(G &&default_value) &&
    {
        static_assert(std::is_move_constructible_v<E>);
        static_assert(std::is_convertible_v<G, E>);

        return is_err() ? std::get<detail::ResultKind::Err>(std::move(result_variant_))
                        : static_cast<E>(std::forward<G>(default_value));
    }
} // namespace result_type
