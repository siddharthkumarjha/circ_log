#pragma once

#include "fmt/ranges.h"
#include <absl/time/time.h>
#include <chrono>

static constexpr uint8_t to_bcd(uint8_t v) { return ((v / 10) << 4) | (v % 10); }
static constexpr uint8_t from_bcd(uint8_t b) { return ((b >> 4) * 10) + (b & 0x0F); }

namespace detail
{
    static inline constexpr uint8_t get_lo_digits(uint16_t bytes)
    {
        return static_cast<uint8_t>(bytes % uint16_t{100});
    }
    static inline constexpr uint8_t get_hi_digits(uint16_t bytes)
    {
        return static_cast<uint8_t>(bytes / uint16_t{100});
    }
    static inline constexpr uint16_t combine_hi_lo_digits(uint8_t const hi, uint8_t const lo)
    {
        return (uint16_t{hi} * 100U) + uint16_t{lo};
    }

    static inline constexpr uint8_t get_hi_byte(uint16_t const v) { return (v >> 8); }
    static inline constexpr uint8_t get_lo_byte(uint16_t const v) { return (v & 0xFF); }
    static inline constexpr uint16_t combine_hi_lo_byte(uint8_t const hi, uint8_t const lo)
    {
        return (uint16_t{hi} << 8) | uint16_t{lo};
    }
} // namespace detail

struct UTCTime
{
    uint8_t reserve_start{0x00U};

    uint8_t record_identifier_hi{0xFFU};
    uint8_t record_identifier_lo{0xFFU};

    uint8_t year_hi;
    uint8_t year_lo;

    uint8_t month;
    uint8_t day;
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;

    uint8_t reserve_end_hi{0x00U};
    uint8_t reserve_end_lo{0x00U};

    static UTCTime from_tp(std::chrono::system_clock::time_point const tp)
    {
        auto civil_tp = absl::UTCTimeZone().At(absl::FromChrono(tp)).cs;
        return UTCTime{
            .year_hi = ::detail::get_hi_digits(static_cast<uint16_t>(civil_tp.year())),
            .year_lo = ::detail::get_lo_digits(static_cast<uint16_t>(civil_tp.year())),

            .month   = static_cast<uint8_t>(civil_tp.month()),
            .day     = static_cast<uint8_t>(civil_tp.day()),
            .hours   = static_cast<uint8_t>(civil_tp.hour()),
            .minutes = static_cast<uint8_t>(civil_tp.minute()),
            .seconds = static_cast<uint8_t>(civil_tp.second()),
        };
    }

    UTCTime serialize() &&
    {
        this->year_hi = to_bcd(this->year_hi);
        this->year_lo = to_bcd(this->year_lo);

        this->month   = to_bcd(this->month);
        this->day     = to_bcd(this->day);
        this->hours   = to_bcd(this->hours);
        this->minutes = to_bcd(this->minutes);
        this->seconds = to_bcd(this->seconds);

        return *this;
    }

    UTCTime serialize() const &
    {
        UTCTime ser_time;

        ser_time.year_hi = to_bcd(this->year_hi);
        ser_time.year_lo = to_bcd(this->year_lo);

        ser_time.month   = to_bcd(this->month);
        ser_time.day     = to_bcd(this->day);
        ser_time.hours   = to_bcd(this->hours);
        ser_time.minutes = to_bcd(this->minutes);
        ser_time.seconds = to_bcd(this->seconds);

        return ser_time;
    }

    UTCTime deserialize() const
    {
        UTCTime de_time;

        de_time.year_hi = from_bcd(this->year_hi);
        de_time.year_lo = from_bcd(this->year_lo);

        de_time.month   = from_bcd(this->month);
        de_time.day     = from_bcd(this->day);
        de_time.hours   = from_bcd(this->hours);
        de_time.minutes = from_bcd(this->minutes);
        de_time.seconds = from_bcd(this->seconds);

        return de_time;
    }
};

template <> struct fmt::formatter<UTCTime>
{
    constexpr auto parse(format_parse_context &ctx) -> format_parse_context::iterator { return ctx.begin(); }

    auto format(const UTCTime &time, format_context &ctx) const -> format_context::iterator
    {
        return fmt::format_to(ctx.out(), "{:0>4}/{:0>2}/{:0>2} - {:0>2}:{:0>2}:{:0>2}",
                              ::detail::combine_hi_lo_digits(time.year_hi, time.year_lo), time.month, time.day,
                              time.hours, time.minutes, time.seconds);
    }
};

struct CANInfo
{
    uint8_t reserve{0x00U};

    uint8_t record_identifier_hi;
    uint8_t record_identifier_lo;

    uint8_t time_info; // Resolution 0.01 s
    uint8_t can_data[8];

    static CANInfo new_instance(uint16_t const can_id, uint8_t can_data[], std::chrono::milliseconds can_id_ts,
                                std::chrono::milliseconds cur_ts)
    {
        CANInfo instance{};

        instance.record_identifier_hi = ::detail::get_hi_byte(can_id);
        instance.record_identifier_lo = ::detail::get_lo_byte(can_id);

        instance.time_info            = static_cast<uint8_t>((can_id_ts - cur_ts).count() / 10LL);

        std::memcpy(instance.can_data, can_data, 8);

        return instance;
    }
};

template <> struct fmt::formatter<CANInfo>
{
    constexpr auto parse(format_parse_context &ctx) -> format_parse_context::iterator { return ctx.begin(); }

    auto format(const CANInfo &can_info, format_context &ctx) const -> format_context::iterator
    {
        return fmt::format_to(
            ctx.out(), "[canid: {:04x}, time diff: {}, can_data: [{:02x}]]",
            ::detail::combine_hi_lo_byte(can_info.record_identifier_hi, can_info.record_identifier_lo),
            can_info.time_info, fmt::join(can_info.can_data, ", "));
    }
};
