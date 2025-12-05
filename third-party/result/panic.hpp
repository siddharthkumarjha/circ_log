#pragma once
#include <experimental/source_location>
#include <sstream>
#include <stdexcept>

constexpr std::string_view file_base_name(std::string_view const file_name)
{
    const size_t pos = file_name.find_last_of('/');
    return pos == std::string_view::npos ? file_name : file_name.substr(pos + 1);
}

template <typename... Args> struct panic
{
    panic(Args &&...args, std::experimental::source_location &&location = std::experimental::source_location::current())
    {
        std::ostringstream oss;
        oss << "PANIC: ";
        oss << "[" << file_base_name(location.file_name()) << ':' << location.line() << ':' << location.function_name()
            << "] ";
        (oss << ... << std::forward<Args>(args));
        oss << '\n';
        throw std::runtime_error(std::move(oss).str());
    }
};
template <typename... Args> panic(Args &&...) -> panic<Args...>;
