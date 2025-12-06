#pragma once

#include "fmt/base.h"
#include "result/result.hpp"
#include <memory>

extern "C"
{
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
}

using namespace result_type;

template <typename T>
concept plain_or_unbound_arr =
    !std::is_reference_v<T> && !std::is_pointer_v<T> && (std::is_unbounded_array_v<T> || (!std::is_array_v<T>));

template <typename T>
concept mmap_safe = std::is_trivially_copyable_v<T> && plain_or_unbound_arr<T>;

static inline std::string get_err_msg(int const err_code = errno)
{
    char buf[256]                  = {0};
    constexpr const size_t buf_len = std::size(buf);

#if defined __USE_XOPEN2K && !defined __USE_GNU
    if (strerror_r(err_code, buf, buf_len) == 0)
        return buf;
    else
        return "Unknown Error";
#elif defined __USE_GNU
    return strerror_r(err_code, buf, buf_len);
#else
#    error "no thread safe strerror impl available"
#endif
}

template <mmap_safe T> class mmap_wrapper
{
private:
    struct deleter
    {
        int fd_      = -1;
        size_t size_ = 0;

        void operator()(void *fptr)
        {
            if (const auto status = msync(fptr, size_, MS_SYNC); status < 0)
            {
                fmt::println(stderr, "msync failed: {}", get_err_msg());
            }
            if (const auto status = munmap(fptr, size_); status < 0)
            {
                fmt::println(stderr, "munmap failed: {}", get_err_msg());
            }

            if (fd_ < 0)
            {
                fmt::println(stderr, "fd: {} is invalid", fd_);
            }
            else
            {
                close(fd_);
            }
        }
    };

    template <typename Tp> struct ptr_cast
    {
        Tp *operator()(void *fptr) { return static_cast<Tp *>(fptr); }
    };
    template <typename Tp> struct ptr_cast<Tp[]>
    {
        Tp *operator()(void *fptr) { return static_cast<Tp *>(fptr); }
    };

public:
    using unique_ptr = std::unique_ptr<T, deleter>;

public:
    static auto new_instance(std::string_view const file_name, off_t const offset = 0, size_t const size = sizeof(T))
        -> Result<unique_ptr, std::string>
    {
        int fd = open(file_name.data(), O_RDWR | O_CREAT | O_CLOEXEC | O_NOFOLLOW, 0644);
        if (fd < 0)
        {
            return Err(get_err_msg());
        }

        if (int const status = posix_fallocate(fd, offset, size); status != 0)
        {
            close(fd);
            return Err(get_err_msg(status));
        }

        void *fptr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, offset);
        if (fptr == MAP_FAILED)
        {
            close(fd);
            return Err(get_err_msg());
        }
        return Ok(unique_ptr(ptr_cast<T>()(fptr), deleter{.fd_ = fd, .size_ = size}));
    }

    friend std::ostream &operator<<(std::ostream &oss, unique_ptr const &ptr)
    {
        oss << "mmap@" << static_cast<void *>(ptr.get());
        return oss;
    }

    mmap_wrapper()                                = delete;
    mmap_wrapper(const mmap_wrapper &)            = delete;
    mmap_wrapper &operator=(const mmap_wrapper &) = delete;
    mmap_wrapper(mmap_wrapper &&)                 = delete;
    mmap_wrapper &operator=(mmap_wrapper &&)      = delete;
};

template <mmap_safe T, typename D>
    requires(std::same_as<std::unique_ptr<T, D>, typename mmap_wrapper<T>::unique_ptr>)
struct fmt::formatter<std::unique_ptr<T, D>>
{
    constexpr auto parse(format_parse_context &ctx) -> format_parse_context::iterator { return ctx.begin(); }

    auto format(const std::unique_ptr<T, D> &mmap_handle, format_context &ctx) const -> format_context::iterator
    {
        return fmt::format_to(ctx.out(), "mmap@{}", static_cast<void *>(mmap_handle.get()));
    }
};
