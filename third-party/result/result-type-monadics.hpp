#pragma once
#include "result-helper.hpp"
#include "result-type-definition.hpp"
#include <functional>

namespace result_type
{
    template <typename T, typename E>
    template <typename F>
    constexpr auto Result<T, E>::and_then(F &&f) & -> helper::monadic_enable_t<E, F, E &, T &>
    {
        using U = helper::fn_eval_result<F, T &>;

        static_assert(helper::is_result_type<U>);
        static_assert(std::is_same_v<typename U::error_type, E>);

        if (is_ok())
        {
            return std::invoke(std::forward<F>(f), std::get<detail::ResultKind::Ok>(result_variant_));
        }
        else
        {
            return make_err<typename U::value_type, typename U::error_type>(
                std::get<detail::ResultKind::Err>(result_variant_));
        }
    }
    template <typename T, typename E>
    template <typename F>
    constexpr auto Result<T, E>::and_then(F &&f) const & -> helper::monadic_enable_t<E, F, const E &, const T &>
    {
        using U = helper::fn_eval_result<F, const T &>;

        static_assert(helper::is_result_type<U>);
        static_assert(std::is_same_v<typename U::error_type, E>);

        if (is_ok())
        {
            return std::invoke(std::forward<F>(f), std::get<detail::ResultKind::Ok>(result_variant_));
        }
        else
        {
            return make_err<typename U::value_type, typename U::error_type>(
                std::get<detail::ResultKind::Err>(result_variant_));
        }
    }
    template <typename T, typename E>
    template <typename F>
    constexpr auto Result<T, E>::and_then(F &&f) && -> helper::monadic_enable_t<E, F, E, T &&>
    {
        using U = helper::fn_eval_result<F, T &&>;

        static_assert(helper::is_result_type<U>);
        static_assert(std::is_same_v<typename U::error_type, E>);

        if (is_ok())
        {
            return std::invoke(std::forward<F>(f), std::get<detail::ResultKind::Ok>(std::move(result_variant_)));
        }
        else
        {
            return make_err<typename U::value_type, typename U::error_type>(
                std::get<detail::ResultKind::Err>(std::move(result_variant_)));
        }
    }
    template <typename T, typename E>
    template <typename F>
    constexpr auto Result<T, E>::and_then(F &&f) const && -> helper::monadic_enable_t<E, F, const E, const T &&>
    {
        using U = helper::fn_eval_result<F, const T &&>;

        static_assert(helper::is_result_type<U>);
        static_assert(std::is_same_v<typename U::error_type, E>);

        if (is_ok())
        {
            return std::invoke(std::forward<F>(f), std::get<detail::ResultKind::Ok>(std::move(result_variant_)));
        }
        else
        {
            return make_err<typename U::value_type, typename U::error_type>(
                std::get<detail::ResultKind::Err>(std::move(result_variant_)));
        }
    }

    template <typename T, typename E>
    template <typename F>
    constexpr auto Result<T, E>::or_else(F &&f) & -> helper::monadic_enable_t<T, F, T &, E &>
    {
        using G = helper::fn_eval_result<F, E &>;

        static_assert(helper::is_result_type<G>);
        static_assert(std::is_same_v<typename G::value_type, T>);

        if (is_err())
        {
            return std::invoke(std::forward<F>(f), std::get<detail::ResultKind::Err>(result_variant_));
        }
        else
        {
            return make_ok<typename G::value_type, typename G::error_type>(
                std::get<detail::ResultKind::Ok>(result_variant_));
        }
    }
    template <typename T, typename E>
    template <typename F>
    constexpr auto Result<T, E>::or_else(F &&f) const & -> helper::monadic_enable_t<T, F, const T &, const E &>
    {
        using G = helper::fn_eval_result<F, const E &>;

        static_assert(helper::is_result_type<G>);
        static_assert(std::is_same_v<typename G::value_type, T>);

        if (is_err())
        {
            return std::invoke(std::forward<F>(f), std::get<detail::ResultKind::Err>(result_variant_));
        }
        else
        {
            return make_ok<typename G::value_type, typename G::error_type>(
                std::get<detail::ResultKind::Ok>(result_variant_));
        }
    }
    template <typename T, typename E>
    template <typename F>
    constexpr auto Result<T, E>::or_else(F &&f) && -> helper::monadic_enable_t<T, F, T, E &&>
    {
        using G = helper::fn_eval_result<F, E &&>;

        static_assert(helper::is_result_type<G>);
        static_assert(std::is_same_v<typename G::value_type, T>);

        if (is_err())
        {
            return std::invoke(std::forward<F>(f), std::get<detail::ResultKind::Err>(std::move(result_variant_)));
        }
        else
        {
            return make_ok<typename G::value_type, typename G::error_type>(
                std::get<detail::ResultKind::Ok>(std::move(result_variant_)));
        }
    }
    template <typename T, typename E>
    template <typename F>
    constexpr auto Result<T, E>::or_else(F &&f) const && -> helper::monadic_enable_t<T, F, const T, const E &&>
    {
        using G = helper::fn_eval_result<F, const E &&>;

        static_assert(helper::is_result_type<G>);
        static_assert(std::is_same_v<typename G::value_type, T>);

        if (is_err())
        {
            return std::invoke(std::forward<F>(f), std::get<detail::ResultKind::Err>(std::move(result_variant_)));
        }
        else
        {
            return make_ok<typename G::value_type, typename G::error_type>(
                std::get<detail::ResultKind::Ok>(std::move(result_variant_)));
        }
    }

    template <typename T, typename E>
    template <typename F>
    constexpr auto Result<T, E>::map(F &&f) & -> helper::transform_enable_t<E, F, E &, T &>
    {
        using U = helper::fn_eval_result_xform<F, T &>;

        if (is_ok())
        {
            if constexpr (std::is_void_v<U>)
            {
                std::invoke(std::forward<F>(f), std::get<detail::ResultKind::Ok>(result_variant_));
                return make_ok<void, E>();
            }
            else
            {
                return make_ok<U, E>(
                    std::invoke(std::forward<F>(f), std::get<detail::ResultKind::Ok>(result_variant_)));
            }
        }
        else
        {
            return make_err<U, E>(std::get<detail::ResultKind::Err>(result_variant_));
        }
    }
    template <typename T, typename E>
    template <typename F>
    constexpr auto Result<T, E>::map(F &&f) const & -> helper::transform_enable_t<E, F, const E &, const T &>
    {
        using U = helper::fn_eval_result_xform<F, const T &>;

        if (is_ok())
        {
            if constexpr (std::is_void_v<U>)
            {
                std::invoke(std::forward<F>(f), std::get<detail::ResultKind::Ok>(result_variant_));
                return make_ok<void, E>();
            }
            else
            {
                return make_ok<U, E>(
                    std::invoke(std::forward<F>(f), std::get<detail::ResultKind::Ok>(result_variant_)));
            }
        }
        else
        {
            return make_err<U, E>(std::get<detail::ResultKind::Err>(result_variant_));
        }
    }
    template <typename T, typename E>
    template <typename F>
    constexpr auto Result<T, E>::map(F &&f) && -> helper::transform_enable_t<E, F, E, T>
    {
        using U = helper::fn_eval_result_xform<F, T>;

        if (is_ok())
        {
            if constexpr (std::is_void_v<U>)
            {
                std::invoke(std::forward<F>(f), std::get<detail::ResultKind::Ok>(std::move(result_variant_)));
                return make_ok<void, E>();
            }
            else
            {
                return make_ok<U, E>(
                    std::invoke(std::forward<F>(f), std::get<detail::ResultKind::Ok>(std::move(result_variant_))));
            }
        }
        else
        {
            return make_err<U, E>(std::get<detail::ResultKind::Err>(std::move(result_variant_)));
        }
    }
    template <typename T, typename E>
    template <typename F>
    constexpr auto Result<T, E>::map(F &&f) const && -> helper::transform_enable_t<E, F, const E, const T>
    {
        using U = helper::fn_eval_result_xform<F, const T>;

        if (is_ok())
        {
            if constexpr (std::is_void_v<U>)
            {
                std::invoke(std::forward<F>(f), std::get<detail::ResultKind::Ok>(std::move(result_variant_)));
                return make_ok<void, E>();
            }
            else
            {
                return make_ok<U, E>(
                    std::invoke(std::forward<F>(f), std::get<detail::ResultKind::Ok>(std::move(result_variant_))));
            }
        }
        else
        {
            return make_err<U, E>(std::get<detail::ResultKind::Err>(std::move(result_variant_)));
        }
    }

    template <typename T, typename E>
    template <typename F>
    constexpr auto Result<T, E>::map_err(F &&f) & -> helper::transform_err_enable_t<T, F, T &, E &>
    {
        using G = helper::fn_eval_result_xform<F, E &>;

        if (is_err())
        {
            return make_err<T, G>(std::invoke(std::forward<F>(f), std::get<detail::ResultKind::Err>(result_variant_)));
        }
        else
        {
            return make_ok<T, G>(std::get<detail::ResultKind::Ok>(result_variant_));
        }
    }
    template <typename T, typename E>
    template <typename F>
    constexpr auto Result<T, E>::map_err(F &&f) const & -> helper::transform_err_enable_t<T, F, const T &, const E &>
    {
        using G = helper::fn_eval_result_xform<F, const E &>;

        if (is_err())
        {
            return make_err<T, G>(std::invoke(std::forward<F>(f), std::get<detail::ResultKind::Err>(result_variant_)));
        }
        else
        {
            return make_ok<T, G>(std::get<detail::ResultKind::Ok>(result_variant_));
        }
    }
    template <typename T, typename E>
    template <typename F>
    constexpr auto Result<T, E>::map_err(F &&f) && -> helper::transform_err_enable_t<T, F, T, E &&>
    {
        using G = helper::fn_eval_result_xform<F, E &&>;

        if (is_err())
        {
            return make_err<T, G>(
                std::invoke(std::forward<F>(f), std::get<detail::ResultKind::Err>(std::move(result_variant_))));
        }
        else
        {
            return make_ok<T, G>(std::get<detail::ResultKind::Ok>(std::move(result_variant_)));
        }
    }
    template <typename T, typename E>
    template <typename F>
    constexpr auto Result<T, E>::map_err(F &&f) const && -> helper::transform_err_enable_t<T, F, const T, const E &&>
    {
        using G = helper::fn_eval_result_xform<F, const E &&>;

        if (is_err())
        {
            return make_err<T, G>(
                std::invoke(std::forward<F>(f), std::get<detail::ResultKind::Err>(std::move(result_variant_))));
        }
        else
        {
            return make_ok<T, G>(std::get<detail::ResultKind::Ok>(std::move(result_variant_)));
        }
    }

    /////////////////////////////////////////////////////////////////////////
    // void specialization
    /////////////////////////////////////////////////////////////////////////

    template <typename E>
    template <typename F>
    constexpr auto Result<void, E>::and_then(F &&f) & -> helper::monadic_no_arg_enable_t<E, F, E &>
    {
        using U = helper::fn_eval_result_no_arg<F>;

        static_assert(helper::is_result_type<U>);
        static_assert(std::is_same_v<typename U::error_type, E>);

        if (is_ok())
        {
            return std::invoke(std::forward<F>(f));
        }
        else
        {
            return make_err<typename U::value_type, typename U::error_type>(
                std::get<detail::ResultKind::Err>(result_variant_));
        }
    }
    template <typename E>
    template <typename F>
    constexpr auto Result<void, E>::and_then(F &&f) const & -> helper::monadic_no_arg_enable_t<E, F, const E &>
    {
        using U = helper::fn_eval_result_no_arg<F>;

        static_assert(helper::is_result_type<U>);
        static_assert(std::is_same_v<typename U::error_type, E>);

        if (is_ok())
        {
            return std::invoke(std::forward<F>(f));
        }
        else
        {
            return make_err<typename U::value_type, typename U::error_type>(
                std::get<detail::ResultKind::Err>(result_variant_));
        }
    }
    template <typename E>
    template <typename F>
    constexpr auto Result<void, E>::and_then(F &&f) && -> helper::monadic_no_arg_enable_t<E, F, E>
    {
        using U = helper::fn_eval_result_no_arg<F>;

        static_assert(helper::is_result_type<U>);
        static_assert(std::is_same_v<typename U::error_type, E>);

        if (is_ok())
        {
            return std::invoke(std::forward<F>(f));
        }
        else
        {
            return make_err<typename U::value_type, typename U::error_type>(
                std::get<detail::ResultKind::Err>(std::move(result_variant_)));
        }
    }
    template <typename E>
    template <typename F>
    constexpr auto Result<void, E>::and_then(F &&f) const && -> helper::monadic_no_arg_enable_t<E, F, const E>
    {
        using U = helper::fn_eval_result_no_arg<F>;

        static_assert(helper::is_result_type<U>);
        static_assert(std::is_same_v<typename U::error_type, E>);

        if (is_ok())
        {
            return std::invoke(std::forward<F>(f));
        }
        else
        {
            return make_err<typename U::value_type, typename U::error_type>(
                std::get<detail::ResultKind::Err>(std::move(result_variant_)));
        }
    }

    template <typename E>
    template <typename F>
    constexpr auto Result<void, E>::or_else(F &&f) & -> helper::fn_eval_result<F, E &>
    {
        using G = helper::fn_eval_result<F, E &>;

        static_assert(helper::is_result_type<G>);
        static_assert(std::is_same_v<typename G::value_type, void>);

        if (is_err())
        {
            return std::invoke(std::forward<F>(f), std::get<detail::ResultKind::Err>(result_variant_));
        }
        else
        {
            return make_ok<void, typename G::error_type>();
        }
    }
    template <typename E>
    template <typename F>
    constexpr auto Result<void, E>::or_else(F &&f) const & -> helper::fn_eval_result<F, const E &>
    {
        using G = helper::fn_eval_result<F, const E &>;

        static_assert(helper::is_result_type<G>);
        static_assert(std::is_same_v<typename G::value_type, void>);

        if (is_err())
        {
            return std::invoke(std::forward<F>(f), std::get<detail::ResultKind::Err>(result_variant_));
        }
        else
        {
            return make_ok<void, typename G::error_type>();
        }
    }
    template <typename E>
    template <typename F>
    constexpr auto Result<void, E>::or_else(F &&f) && -> helper::fn_eval_result<F, E &&>
    {
        using G = helper::fn_eval_result<F, E &&>;

        static_assert(helper::is_result_type<G>);
        static_assert(std::is_same_v<typename G::value_type, void>);

        if (is_err())
        {
            return std::invoke(std::forward<F>(f), std::get<detail::ResultKind::Err>(std::move(result_variant_)));
        }
        else
        {
            return make_ok<void, typename G::error_type>();
        }
    }
    template <typename E>
    template <typename F>
    constexpr auto Result<void, E>::or_else(F &&f) const && -> helper::fn_eval_result<F, const E &&>
    {
        using G = helper::fn_eval_result<F, const E &&>;

        static_assert(helper::is_result_type<G>);
        static_assert(std::is_same_v<typename G::value_type, void>);

        if (is_err())
        {
            return std::invoke(std::forward<F>(f), std::get<detail::ResultKind::Err>(std::move(result_variant_)));
        }
        else
        {
            return make_ok<void, typename G::error_type>();
        }
    }

    template <typename E>
    template <typename F>
    constexpr auto Result<void, E>::map(F &&f) & -> helper::transform_no_arg_enable_t<E, F, E &>
    {
        using U = helper::fn_eval_result_xform_no_arg<F>;

        if (is_ok())
        {
            return make_ok<U, E>(std::invoke(std::forward<F>(f)));
        }
        else
        {
            return make_err<U, E>(std::get<detail::ResultKind::Err>(result_variant_));
        }
    }
    template <typename E>
    template <typename F>
    constexpr auto Result<void, E>::map(F &&f) const & -> helper::transform_no_arg_enable_t<E, F, const E &>
    {
        using U = helper::fn_eval_result_xform_no_arg<F>;

        if (is_ok())
        {
            return make_ok<U, E>(std::invoke(std::forward<F>(f)));
        }
        else
        {
            return make_err<U, E>(std::get<detail::ResultKind::Err>(result_variant_));
        }
    }
    template <typename E>
    template <typename F>
    constexpr auto Result<void, E>::map(F &&f) && -> helper::transform_no_arg_enable_t<E, F, E>
    {
        using U = helper::fn_eval_result_xform_no_arg<F>;

        if (is_ok())
        {
            return make_ok<U, E>(std::invoke(std::forward<F>(f)));
        }
        else
        {
            return make_err<U, E>(std::get<detail::ResultKind::Err>(std::move(result_variant_)));
        }
    }
    template <typename E>
    template <typename F>
    constexpr auto Result<void, E>::map(F &&f) const && -> helper::transform_no_arg_enable_t<E, F, const E>
    {
        using U = helper::fn_eval_result_xform_no_arg<F>;

        if (is_ok())
        {
            return make_ok<U, E>(std::invoke(std::forward<F>(f)));
        }
        else
        {
            return make_err<U, E>(std::get<detail::ResultKind::Err>(std::move(result_variant_)));
        }
    }

    template <typename E>
    template <typename F>
    constexpr auto Result<void, E>::map_err(F &&f) & -> Result<void, helper::fn_eval_result_xform<F, E &>>
    {
        using G = helper::fn_eval_result_xform<F, E &>;

        if (is_err())
        {
            return make_err<void, G>(
                std::invoke(std::forward<F>(f), std::get<detail::ResultKind::Err>(result_variant_)));
        }
        else
        {
            return make_ok<void, G>();
        }
    }
    template <typename E>
    template <typename F>
    constexpr auto Result<void, E>::map_err(F &&f) const & -> Result<void, helper::fn_eval_result_xform<F, const E &>>
    {
        using G = helper::fn_eval_result_xform<F, const E &>;

        if (is_err())
        {
            return make_err<void, G>(
                std::invoke(std::forward<F>(f), std::get<detail::ResultKind::Err>(result_variant_)));
        }
        else
        {
            return make_ok<void, G>();
        }
    }
    template <typename E>
    template <typename F>
    constexpr auto Result<void, E>::map_err(F &&f) && -> Result<void, helper::fn_eval_result_xform<F, E &&>>
    {
        using G = helper::fn_eval_result_xform<F, E &&>;

        if (is_err())
        {
            return make_err<void, G>(
                std::invoke(std::forward<F>(f), std::get<detail::ResultKind::Err>(std::move(result_variant_))));
        }
        else
        {
            return make_ok<void, G>();
        }
    }
    template <typename E>
    template <typename F>
    constexpr auto Result<void, E>::map_err(F &&f) const && -> Result<void, helper::fn_eval_result_xform<F, const E &&>>
    {
        using G = helper::fn_eval_result_xform<F, const E &&>;

        if (is_err())
        {
            return make_err<void, G>(
                std::invoke(std::forward<F>(f), std::get<detail::ResultKind::Err>(std::move(result_variant_))));
        }
        else
        {
            return make_ok<void, G>();
        }
    }
} // namespace result_type
