#pragma once
#include <ostream>
#include <string_view>

struct source_location
{
    static constexpr source_location current(const std::string_view file = __builtin_FILE(),
                                             const std::string_view func = __builtin_FUNCTION(),
                                             const size_t line           = __builtin_LINE()) noexcept
    {
        source_location loc;
        loc.file_name_ = file;
        loc.func_name_ = func;
        loc.line_num_  = line;

        return loc;
    }

    constexpr source_location() noexcept : file_name_("unknown"), func_name_(file_name_), line_num_(0) {}

    constexpr size_t line() const noexcept { return line_num_; }
    constexpr std::string_view file_name() const noexcept { return file_name_; }
    constexpr std::string_view function_name() const noexcept { return func_name_; }
    constexpr std::string_view file_base_name() const noexcept
    {
        const size_t pos = file_name_.find_last_of('/');
        return pos == std::string_view::npos ? file_name_ : file_name_.substr(pos + 1);
    }

    friend std::ostream &operator<<(std::ostream &oss, const source_location &loc)
    {
        oss << "[" << loc.file_base_name() << ":" << loc.line() << ':' << loc.function_name() << "] ";
        return oss;
    }

private:
    std::string_view file_name_;
    std::string_view func_name_;
    size_t line_num_;
};
