#pragma once
#include <sqlite/error.h>
#include <ice/optional.h>
#include <ice/uuid.h>
#include <algorithm>
#include <array>
#include <string>
#include <vector>
#include <cstdint>

namespace sqlite {

enum null_t {
  null
};

template <typename T>
class type_traits {};

template <>
class type_traits<null_t> {
public:
  template <typename Field>
  static std::int8_t as(const Field& field)
  {
    return null_t();
  }
  template <typename Statement>
  static void bind(Statement& statement, const std::string& parameter, const std::null_t&)
  {
    statement.bind_null(parameter);
  }
};

template <>
class type_traits<bool> {
public:
  template <typename Field>
  static bool as(const Field& field)
  {
    return field.as_int32() ? true : false;
  }
  template <typename Statement>
  static void bind(Statement& statement, const std::string& parameter, const bool& value)
  {
    statement.bind_int32(parameter, value ? 1 : 0);
  }
};

template <>
class type_traits<float> {
public:
  template <typename Field>
  static float as(const Field& field)
  {
    return static_cast<float>(field.as_double());
  }
  template <typename Statement>
  static void bind(Statement& statement, const std::string& parameter, const float& value)
  {
    statement.bind_double(parameter, static_cast<double>(value));
  }
};

template <>
class type_traits<std::int8_t> {
public:
  template <typename Field>
  static std::int8_t as(const Field& field)
  {
    return static_cast<std::int8_t>(field.as_int32());
  }
  template <typename Statement>
  static void bind(Statement& statement, const std::string& parameter, const std::int8_t& value)
  {
    statement.bind_int32(parameter, static_cast<std::int32_t>(value));
  }
};

template <>
class type_traits<std::uint8_t> {
public:
  template <typename Field>
  static std::uint8_t as(const Field& field)
  {
    return static_cast<std::uint8_t>(field.as_int32());
  }
  template <typename Statement>
  static void bind(Statement& statement, const std::string& parameter, const std::uint8_t& value)
  {
    statement.bind_int32(parameter, static_cast<std::uint8_t>(value));
  }
};

template <>
class type_traits<std::int16_t> {
public:
  template <typename Field>
  static std::int16_t as(const Field& field)
  {
    return static_cast<std::int16_t>(field.as_int32());
  }
  template <typename Statement>
  static void bind(Statement& statement, const std::string& parameter, const std::int16_t& value)
  {
    statement.bind_int32(parameter, static_cast<std::int16_t>(value));
  }
};

template <>
class type_traits<std::uint16_t> {
public:
  template <typename Field>
  static std::uint16_t as(const Field& field)
  {
    return static_cast<std::uint16_t>(field.as_int32());
  }
  template <typename Statement>
  static void bind(Statement& statement, const std::string& parameter, const std::uint16_t& value)
  {
    statement.bind_int32(parameter, static_cast<std::uint16_t>(value));
  }
};

template <>
class type_traits<std::int32_t> {
public:
  template <typename Field>
  static std::int32_t as(const Field& field)
  {
    return static_cast<std::int32_t>(field.as_int32());
  }
  template <typename Statement>
  static void bind(Statement& statement, const std::string& parameter, const std::int32_t& value)
  {
    statement.bind_int32(parameter, static_cast<std::int32_t>(value));
  }
};

template <>
class type_traits<std::uint32_t> {
public:
  template <typename Field>
  static std::uint32_t as(const Field& field)
  {
    return static_cast<std::uint32_t>(field.as_int32());
  }
  template <typename Statement>
  static void bind(Statement& statement, const std::string& parameter, const std::uint32_t& value)
  {
    statement.bind_int32(parameter, static_cast<std::uint32_t>(value));
  }
};

template <>
class type_traits<std::int64_t> {
public:
  template <typename Field>
  static std::int64_t as(const Field& field)
  {
    return static_cast<std::int64_t>(field.as_int64());
  }
  template <typename Statement>
  static void bind(Statement& statement, const std::string& parameter, const std::int64_t& value)
  {
    statement.bind_int64(parameter, static_cast<std::int64_t>(value));
  }
};

template <>
class type_traits<std::uint64_t> {
public:
  template <typename Field>
  static std::uint64_t as(const Field& field)
  {
    return static_cast<std::uint64_t>(field.as_int64());
  }
  template <typename Statement>
  static void bind(Statement& statement, const std::string& parameter, const std::uint64_t& value)
  {
    statement.bind_int64(parameter, static_cast<std::uint64_t>(value));
  }
};

template <>
class type_traits<std::string> {
public:
  template <typename Field>
  static std::string as(const Field& field)
  {
    return field.as_string();
  }
  template <typename Statement>
  static void bind(Statement& statement, const std::string& parameter, const std::string& value)
  {
    statement.bind(parameter, value.data(), value.size());
  }
};

template <>
class type_traits<std::vector<std::uint8_t>> {
public:
  template <typename Field>
  static std::vector<std::uint8_t> as(const Field& field)
  {
    if (field.is_null()) {
      return std::vector<std::uint8_t>();
    }
    auto data = field.data();
    return std::vector<std::uint8_t>(data, data + field.size());
  }
  template <typename Statement>
  static void bind(Statement& statement, const std::string& parameter, const std::vector<std::uint8_t>& value)
  {
    statement.bind(parameter, value.data(), value.size());
  }
};

template <std::size_t N>
class type_traits<std::array<std::uint8_t, N>> {
public:
  template <typename Field>
  static std::array<std::uint8_t, N> as(const Field& field)
  {
    std::array<std::uint8_t, N> value;
    if (field.is_null()) {
      return value;
    }
    auto data = field.data();
    std::copy(data, data + std::min(value.size(), field.size()), value.data());
    return value;
  }
  template <typename Statement>
  static void bind(Statement& statement, const std::string& parameter, const std::array<std::uint8_t, N>& value)
  {
    statement.bind(parameter, value.data(), value.size());
  }
};

// UUID as a 16 byte binary blob.
template <>
class type_traits<ice::uuid> {
public:
  template <typename Field>
  static ice::uuid as(const Field& field)
  {
    if (field.is_null()) {
      return ice::uuid();
    }
    return ice::uuid(field.data(), field.size());
  }
  template <typename Statement>
  static void bind(Statement& statement, const std::string& parameter, const ice::uuid& value)
  {
    auto data = value.data();
    statement.bind(parameter, data.data(), data.size());
  }
};

// Support for optional values.
template <typename T>
class type_traits<ice::optional<T>> {
public:
  template <typename Field>
  static ice::optional<T> as(const Field& field)
  {
    if (field.is_null()) {
      return ice::optional<T>();
    }
    return field.as<T>();
  }
  template <typename Statement>
  static void bind(Statement& statement, const std::string& parameter, const ice::optional<T>& value)
  {
    if (value) {
      statement.bind(parameter, value.value());
    } else {
      statement.bind_null(parameter);
    }
  }
};

}  // namespace sqlite