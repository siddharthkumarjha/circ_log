#pragma once

#include "result-type-constructor.hpp"
#include "result-type-definition.hpp"
#include "result-type-monadics.hpp"
#include "result-type-observers.hpp"

#define TRY_UTIL_JOIN_(x, y) x##_##y
#define TRY_WITH_UNIQUE_SUFFIX_(x, y) TRY_UTIL_JOIN_(x, y)
#define TRY_OK_IMPL_(TRY_OK_UNIQUE_PLACEHOLDER, ...)                                                                   \
    ({                                                                                                                 \
        static_assert(!::std::is_lvalue_reference_v<decltype((__VA_ARGS__))>,                                          \
                      "the expression: ' " #__VA_ARGS__                                                                \
                      " ' evaluates to an l-value reference, 'TRY_OK' only accepts r-values "                          \
                      "and r-value references ");                                                                      \
        decltype(__VA_ARGS__) &&TRY_OK_UNIQUE_PLACEHOLDER = (__VA_ARGS__);                                             \
        if (TRY_OK_UNIQUE_PLACEHOLDER.is_err())                                                                        \
        {                                                                                                              \
            return Err(std::move(TRY_OK_UNIQUE_PLACEHOLDER).unwrap_err());                                             \
        }                                                                                                              \
        std::move(TRY_OK_UNIQUE_PLACEHOLDER).unwrap();                                                                 \
    })

#define TRY_OK(...) TRY_OK_IMPL_(TRY_WITH_UNIQUE_SUFFIX_(TRY_OK_PLACEHOLDER, __COUNTER__), __VA_ARGS__)
