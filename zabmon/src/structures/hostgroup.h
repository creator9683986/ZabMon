#ifndef HOSTGROUP_H
#define HOSTGROUP_H

#include <structures/template.h>

namespace zab::mon
{
/// @brief Структура группы
struct HostGroup {
  /// @brief имя группы
  std::string name;
  /// @brief id группы
  std::string id;
  /// @brief список шаблонов в группу
  std::vector<std::string> templatesIds;
  /// @brief Конструктор
  /// @param name имя группы
  /// @param id id группы
  /// @param templates список шаблонов в группу
  explicit HostGroup(std::string name, std::string id,
                     std::vector<std::string> templatesIds);
  explicit HostGroup() = default;
  static HostGroup fromJson(const nlohmann::json &groupObj);
};
}  // namespace zab::mon

#endif  // HOSTGROUP_H