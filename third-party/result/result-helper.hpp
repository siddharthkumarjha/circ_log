#pragma once
#include <type_traits>

namespace result_type
{
    template <typename T, typename E> class Result;
}

namespace result_type::helper
{

    template <class T> struct remove_cvref
    {
        using type = std::remove_cv_t<std::remove_reference_t<T>>;
    };
    template <class T> using remove_cvref_t                                       = typename remove_cvref<T>::type;

    template <typename T> constexpr bool is_result_type                           = false;
    template <typename T, typename E> constexpr bool is_result_type<Result<T, E>> = true;

    template <typename F, typename T> using fn_eval_result       = remove_cvref_t<std::invoke_result_t<F &&, T &&>>;
    template <typename F, typename T> using fn_eval_result_xform = std::remove_cv_t<std::invoke_result_t<F &&, T &&>>;

    template <typename F> using fn_eval_result_no_arg            = remove_cvref_t<std::invoke_result_t<F &&>>;
    template <typename F> using fn_eval_result_xform_no_arg      = std::remove_cv_t<std::invoke_result_t<F &&>>;

    template <typename T, typename Fn, typename Tp, typename FnArg>
    using monadic_enable_t = std::enable_if_t<std::is_constructible_v<T, Tp>, fn_eval_result<Fn, FnArg>>;

    template <typename T, typename Fn, typename Tp>
    using monadic_no_arg_enable_t = std::enable_if_t<std::is_constructible_v<T, Tp>, fn_eval_result_no_arg<Fn>>;

    template <typename E, typename Fn, typename Ep, typename FnArg>
    using transform_enable_t =
        std::enable_if_t<std::is_constructible_v<E, Ep>, Result<fn_eval_result_xform<Fn, FnArg>, E>>;

    template <typename E, typename Fn, typename Ep>
    using transform_no_arg_enable_t =
        std::enable_if_t<std::is_constructible_v<E, Ep>, Result<fn_eval_result_xform_no_arg<Fn>, E>>;

    template <typename T, typename Fn, typename Tp, typename FnArg>
    using transform_err_enable_t =
        std::enable_if_t<std::is_constructible_v<T, Tp>, Result<T, fn_eval_result_xform<Fn, FnArg>>>;

    template <typename T>
    constexpr bool movable = std::is_object<T>::value && std::is_move_constructible<T>::value &&
                             std::is_assignable<T &, T>::value && std::is_swappable<T>::value;

    template <typename T> struct check_value_type
    {
        static_assert(movable<T>, "Value type `T` for `Result`, `Ok` and `Err` must be movable");
        static_assert(!std::is_reference_v<T>,
                      "Cannot use a reference for value type `T` for `Result`, `Ok` and `Err`, To "
                      "prevent subtleties use type wrappers like std::reference_wrapper instead");
        static_assert(std::is_nothrow_move_constructible_v<T>, "T must be nothrow move constructible");
    };

    template <typename E> struct check_error_type
    {
        static_assert(movable<E>, "Value type `E` for `Result`, `Ok` and `Err` must be movable");
        static_assert(!std::is_reference_v<E>,
                      "Cannot use a reference for value type `E` for `Result`, `Ok` and `Err`, To "
                      "prevent subtleties use type wrappers like std::reference_wrapper instead");
        static_assert(std::is_nothrow_move_constructible_v<E>, "E must be nothrow move constructible");
    };
} // namespace result_type::helper
