#pragma once
#include <sqlite/type_traits.h>
#include <ice/date.h>

namespace sqlite {
namespace detail {

template<typename T>
struct date_traits {
  template <typename Field>
  static T as(const Field& field)
  {
    return ice::date::parse<T>(field.as<std::string>());
  }
  template <typename Statement>
  static void bind(Statement& statement, const std::string& parameter, const T& value)
  {
    statement.bind(parameter, ice::date::format(value));
  }
};

}  // namespace detail

// ====================================================================================================================
// Durations.
// ====================================================================================================================

template <typename Rep, typename Period>
struct type_traits<std::chrono::duration<Rep, Period>> : detail::date_traits<std::chrono::duration<Rep, Period>> {};

// ====================================================================================================================
// Time of day.
// ====================================================================================================================

template <typename Duration>
struct type_traits<ice::date::time_of_day<Duration>> : detail::date_traits<ice::date::time_of_day<Duration>> {};

// ====================================================================================================================
// Day, weekday, month and year.
// ====================================================================================================================

template <>
struct type_traits<ice::date::day> : detail::date_traits<ice::date::day> {};

template <>
struct type_traits<ice::date::weekday> : detail::date_traits<ice::date::weekday> {};

template <>
struct type_traits<ice::date::month> : detail::date_traits<ice::date::month> {};

template <>
struct type_traits<ice::date::year> : detail::date_traits<ice::date::year> {};

// ====================================================================================================================
// Nth and last weekday of a month.
// ====================================================================================================================

template <>
struct type_traits<ice::date::weekday_indexed> : detail::date_traits<ice::date::weekday_indexed> {};

template <>
struct type_traits<ice::date::weekday_last> : detail::date_traits<ice::date::weekday_last> {};

// ====================================================================================================================
// Nth and last day of a specific month.
// ====================================================================================================================

template <>
struct type_traits<ice::date::month_day> : detail::date_traits<ice::date::month_day> {};

template <>
struct type_traits<ice::date::month_day_last> : detail::date_traits<ice::date::month_day_last> {};

// ====================================================================================================================
// Nth and last weekday of a specific month.
// ====================================================================================================================

template <>
struct type_traits<ice::date::month_weekday> : detail::date_traits<ice::date::month_weekday> {};

template <>
struct type_traits<ice::date::month_weekday_last> : detail::date_traits<ice::date::month_weekday_last> {};

// ====================================================================================================================
// ISO 8601 Year, month and day combinations.
// ====================================================================================================================

template <>
struct type_traits<ice::date::year_month> : detail::date_traits<ice::date::year_month> {};

template <>
struct type_traits<ice::date::year_month_day> : detail::date_traits<ice::date::year_month_day> {};

// ====================================================================================================================
// Last day of a specific year and month.
// ====================================================================================================================

template <>
struct type_traits<ice::date::year_month_day_last> : detail::date_traits<ice::date::year_month_day_last> {};

// ====================================================================================================================
// Nth and last weekday of a specific year and month.
// ====================================================================================================================

template <>
struct type_traits<ice::date::year_month_weekday> : detail::date_traits<ice::date::year_month_weekday> {};

template <>
struct type_traits<ice::date::year_month_weekday_last> : detail::date_traits<ice::date::year_month_weekday_last> {};

// ====================================================================================================================
// Timepoints.
// ====================================================================================================================

template <typename Clock, typename Duration>
struct type_traits<std::chrono::time_point<Clock, Duration>> :
  detail::date_traits<std::chrono::time_point<Clock, Duration>> {};

}  // namespace sqlite