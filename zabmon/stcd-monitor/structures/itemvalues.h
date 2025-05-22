#ifndef ITEMVALUES_H
#define ITEMVALUES_H

#include <stcd-monitor/structures/item.h>
#include <stcd-monitor/structures/value.h>

#include <chrono>

#include <nlohmann/json.hpp>
namespace zab::mon
{
/// @brief Структура значения
struct ItemValues {
  /// @brief Список значений
  std::vector<Value> values;
  /// @brief Структура итема
  Item item;
  /// @brief Конструктор
  explicit ItemValues(std::vector<Value> values, Item item);
  explicit ItemValues() = default;
};
}  // namespace zab::mon

#endif  // ITEMVALUES_H