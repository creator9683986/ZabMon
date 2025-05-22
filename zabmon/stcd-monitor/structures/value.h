#ifndef VALUE_H
#define VALUE_H

#include <chrono>

#include <nlohmann/json.hpp>

namespace zab::mon
{
/// @brief Структура значения
struct Value {
  /// @brief string значение
  std::string stringValue = {};
  /// @brief секунды, для временной метки
  std::chrono::seconds clock = {};
  /// @brief наносекунды для временной метки
  std::chrono::nanoseconds ns = {};

  /// @brief Конструктор
  explicit Value() = default;
  explicit Value(std::string stringValue, std::chrono::seconds clock,
                 std::chrono::nanoseconds ns);
  static Value fromJson(const nlohmann::json& valObj);
};
}  // namespace zab::mon

#endif  // VALUE_H