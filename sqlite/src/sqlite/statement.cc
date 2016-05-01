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

#include <sqlite/statement.h>
#include <sqlite/error.h>
#include <sqlite3.h>
#include <limits>
#include <ostream>
#include <cstdint>

namespace sqlite {
namespace {

std::size_t find_parameter_index(const std::string& parameter, const std::shared_ptr<sqlite3_stmt>& stmt)
{
  if (!stmt) {
    throw error("bind() called on a null statement");
  }
  sqlite3_reset(stmt.get());
  auto index(sqlite3_bind_parameter_index(stmt.get(), parameter.c_str()));
  if (!index)
    throw error(stmt, "while binding parameter '" + parameter + "'");
  return index;
}

} // namespace

statement::statement(const std::shared_ptr<sqlite3_stmt>& statement) : stmt(statement)
{
  if (!statement)
    throw error("attempt to create statement with null sqlite3_stmt");
}

void statement::bind_null(const std::string& parameter)
{
  auto index(find_parameter_index(parameter, stmt));
  throw_on_bind_error(sqlite3_bind_null(stmt.get(), static_cast<int>(index)), parameter);
}

void statement::bind_double(const std::string& parameter, double value)
{
  auto index(find_parameter_index(parameter, stmt));
  throw_on_bind_error(sqlite3_bind_double(stmt.get(), static_cast<int>(index), value), parameter);
}

void statement::bind_int32(const std::string& parameter, std::int32_t value)
{
  auto index(find_parameter_index(parameter, stmt));
  throw_on_bind_error(sqlite3_bind_int(stmt.get(), static_cast<int>(index), static_cast<int>(value)), parameter);
}

void statement::bind_int64(const std::string& parameter, std::int64_t value)
{
  auto index(find_parameter_index(parameter, stmt));
  throw_on_bind_error(sqlite3_bind_int64(stmt.get(), static_cast<int>(index), static_cast<sqlite_int64>(value)), parameter);
}

void statement::bind(const std::string& parameter, const char* value, std::size_t size)
{
  if (size > static_cast<std::size_t>(std::numeric_limits<int>::max())) {
    throw error(stmt, "string size out of range while binding parameter '" + parameter + "'");
  }
  auto index(find_parameter_index(parameter, stmt));
  auto status(sqlite3_bind_text(stmt.get(), static_cast<int>(index), value, static_cast<int>(size), SQLITE_STATIC));
  throw_on_bind_error(status, parameter);
}

void statement::bind(const std::string& parameter, const std::uint8_t* data, std::size_t size)
{
  if (size > static_cast<std::size_t>(std::numeric_limits<int>::max())) {
    throw error(stmt, "blob size out of range while binding parameter '" + parameter + "'");
  }
  auto index(find_parameter_index(parameter, stmt));
  std::string str(reinterpret_cast<const char*>(data), size);
  auto it = values.find(index);
  if (it != values.end()) {
    it->second.assign(std::move(str));
  } else {
    it = values.emplace(index, std::move(str)).first;
  }
  auto status(sqlite3_bind_blob(stmt.get(), static_cast<int>(index), it->second.data(), static_cast<int>(it->second.size()), SQLITE_STATIC));
  throw_on_bind_error(status, parameter);
}

void statement::bind(const std::string& parameter, std::string&& value)
{
  auto index(find_parameter_index(parameter, stmt));
  auto it = values.find(index);
  if (it != values.end()) {
    it->second.assign(std::move(value));
  } else {
    it = values.emplace(index, std::move(value)).first;
  }
  bind(parameter, it->second.data(), it->second.size());
}

void statement::bind(const std::string& parameter, std::vector<std::uint8_t>&& value)
{
  auto index(find_parameter_index(parameter, stmt));
  auto it = values.find(index);
  if (it != values.end()) {
    it->second.assign(reinterpret_cast<const char*>(value.data()), value.size());
  } else {
    it = values.emplace(index, std::string(reinterpret_cast<const char*>(value.data()), value.size())).first;
  }
  bind(parameter, reinterpret_cast<const std::uint8_t*>(it->second.data()), it->second.size());
}

void statement::clear_bindings()
{
  if (!stmt)
    throw error("clear_bindings() called on null sqlite::statement");
  auto status(sqlite3_clear_bindings(stmt.get()));
  if (status != SQLITE_OK)
    throw error(status);
}

std::size_t statement::parameter_count() const
{
  if (!stmt)
    throw error("parameter_count() called on null sqlite::statement");
  return sqlite3_bind_parameter_count(stmt.get());
}

void statement::throw_on_bind_error(const int status, const std::string& parameter) const
{
  if (status != SQLITE_OK)
    throw error(stmt, "while binding parameter '" + parameter + "'");
}

std::ostream& operator<<(std::ostream& os, const statement& statement)
{
  os << "statement:\n"
        "  sql: "
     << sqlite3_sql(statement.stmt.get()) << "\n";
  return os;
}

result make_result(const statement& statement)
{
  sqlite3_reset(statement.stmt.get());
  return statement.stmt;
}

} // namespace sqlite