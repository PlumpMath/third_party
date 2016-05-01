// Copyright (C) 2013  Nick Ogden <nick@nickogden.org>
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once
#include <sqlite/result.h>
#include <sqlite/type_traits.h>
#include <array>
#include <map>
#include <string>
#include <vector>
#include <cstddef>

struct sqlite3;
struct sqlite3_stmt;

namespace sqlite {

class statement;

class statement {
public:
  statement(const std::shared_ptr<sqlite3_stmt>& statement);
  statement(const statement& other) = delete;
  statement(statement&& other) = default;

  statement& operator=(const statement& other) = delete;
  statement& operator=(statement&& other) = default;

  std::size_t parameter_count() const;

  void bind_null(const std::string& parameter);
  void bind_double(const std::string& parameter, double value);
  void bind_int32(const std::string& parameter, std::int32_t value);
  void bind_int64(const std::string& parameter, std::int64_t value);
  
  void bind(const std::string& parameter, const char* value, std::size_t size);
  void bind(const std::string& parameter, const std::uint8_t* data, std::size_t size);

  void bind(const std::string& parameter, std::string&& value);
  void bind(const std::string& parameter, std::vector<std::uint8_t>&& value);

  template <std::size_t N>
  void bind(const std::string& parameter, std::array<std::uint8_t, N>&& value)
  {
    bind(parameter, std::vector<std::uint8_t>(value.data(), value.data() + value.size()));
  }

  template <typename T>
  void bind(const std::string& parameter, const T& value)
  {
    type_traits<T>::bind(*this, parameter, value);
  }

  void clear_bindings();

  friend std::ostream& operator<<(std::ostream& os, const statement& statement);

private:
  void throw_on_bind_error(const int status, const std::string& parameter) const;
  friend result make_result(const statement& statement);

private:
  std::shared_ptr<sqlite3_stmt> stmt;
  std::map<std::size_t, std::string> values;
};

} // namespace sqlite