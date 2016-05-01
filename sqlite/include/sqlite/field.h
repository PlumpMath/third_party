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
#include <sqlite/type_traits.h>
#include <sqlite/types.h>
#include <memory>
#include <string>
#include <cstddef>

struct sqlite3_stmt;

namespace sqlite {

class field {
public:
  field(const std::shared_ptr<sqlite3_stmt>& statement, const std::size_t& parameter_index);

  bool is_null() const;
  explicit operator bool() const;

  std::string column_name() const;

  template <typename T>
  T as() const
  {
    return type_traits<std::remove_const_t<std::remove_reference_t<T>>>::as(*this);
  }

  double as_double() const;
  std::int32_t as_int32() const;
  std::int64_t as_int64() const;
  std::string as_string() const;

  std::size_t size() const;
  const std::uint8_t* data() const;

private:
  std::shared_ptr<sqlite3_stmt> stmt;
  const std::size_t index;
};

} // namespace sqlite