#include "circ_q.hpp"
#include "data_type.hpp"
#include "fmt/base.h"
#include "fmt/chrono.h"
#include "mmap_wrapper.hpp"
#include "result/result.hpp"
#include <chrono>
#include <memory>

using namespace result_type;
using namespace std::literals;

static inline constexpr auto to_chrono_ms(std::chrono::system_clock::time_point tp) -> std::chrono::milliseconds
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch());
}

template <typename Rep, typename Period>
static inline constexpr auto to_chrono_ms(std::chrono::duration<Rep, Period> dur) -> std::chrono::milliseconds
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(dur);
}

auto main_res() -> Result<void, std::string>
{
    auto cur_epoch_ms = to_chrono_ms(std::chrono::system_clock::now());

    uint8_t arr[8]    = {0x00, 0x01, 0x02, 0x03, 0x04, 0xd5, 0x06, 0xff};
    auto can_info     = CANInfo::new_instance(0x2d1, arr, cur_epoch_ms + 10ms, cur_epoch_ms);
    fmt::println("can_info: {}", can_info);

    std::unique_ptr mmap_handle = TRY_OK(mmap_wrapper<UTCTime>::new_instance("./test.dat"));
    fmt::println("mmap handle: {}", mmap_handle);

    fmt::println("read tp: {}", mmap_handle->deserialize());

    auto cur_tp = std::chrono::system_clock::now();
    fmt::println("Writing tp: {}", cur_tp);
    UTCTime utc  = UTCTime::from_tp(cur_tp).serialize();
    *mmap_handle = utc;

    return Ok();
}

int main(void)
{
    main_res().match(
        []() -> void
        {
            fmt::println("exiting the app...");
        },
        [](std::string_view err) -> void
        {
            fmt::println("err: {}", err);
        });

    return 0;
}
