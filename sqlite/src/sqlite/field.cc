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

#include <sqlite/field.h>
#include <sqlite/error.h>
#include <sqlite3.h>
#include <algorithm>
#include <vector>
#include <cstdint>

namespace sqlite {

field::field(const std::shared_ptr<sqlite3_stmt>& statement, const std::size_t& parameter_index)
  : stmt(statement), index(parameter_index)
{
  if (!statement) {
    throw error("null statement error");
  }
}

bool field::is_null() const
{
  return sqlite3_column_type(stmt.get(), static_cast<int>(index)) == SQLITE_NULL;
}

field::operator bool() const
{
  return !is_null();
}

std::string field::column_name() const
{
  const char* name(sqlite3_column_name(stmt.get(), static_cast<int>(index)));
  return name ? std::string(name) : std::to_string(index);
}

double field::as_double() const
{
  if (is_null()) {
    throw error("null to double cast error");
  }
  return sqlite3_column_double(stmt.get(), static_cast<int>(index));
}

std::int32_t field::as_int32() const
{
  if (is_null()) {
    throw error("null to int32 cast error");
  }
  return static_cast<std::int32_t>(sqlite3_column_int(stmt.get(), static_cast<int>(index)));
}

std::int64_t field::as_int64() const
{
  if (is_null()) {
    throw error("null to int64 cast error");
  }
  return static_cast<std::int64_t>(sqlite3_column_int64(stmt.get(), static_cast<int>(index)));
}

std::string field::as_string() const
{
  if (is_null()) {
    throw error("null to string cast error");
  }
  return reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), static_cast<int>(index)));
}

std::size_t field::size() const
{
  auto size = sqlite3_column_bytes(stmt.get(), static_cast<int>(index));
  if (size < 0) {
    throw error("negative size error");
  }
  return static_cast<std::size_t>(size);
}

const std::uint8_t* field::data() const
{
  auto data = sqlite3_column_blob(stmt.get(), static_cast<int>(index));
  if (!data) {
    throw error("null blob error");
  }
  return static_cast<const std::uint8_t*>(data);
}

} // namespace sqlite