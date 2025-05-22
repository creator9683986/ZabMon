#ifndef TEMPLATE_H
#define TEMPLATE_H

#include <nlohmann/json.hpp>

namespace zab::mon
{
/// @brief Структура шаблона
struct Template {
  /// @brief имя
  std::string name;
  /// @brief id
  std::string id;
  /// @brief Список id групп, к которым привязан шаблон
  std::vector<std::string> groupsId;
  /// @brief Конструктор
  explicit Template(std::string name, std::string id,
                    std::vector<std::string> groupsId);
  explicit Template() = default;
  /// @brief Метод конвертации из json в структуру
  static Template fromJson(const nlohmann::json &templateObj);
  static nlohmann::json toJson(const Template &temp);
};
}  // namespace zab::mon

#endif  // TEMPLATE_H