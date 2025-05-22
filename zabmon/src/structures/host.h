#ifndef HOST_H
#define HOST_H

#include <structures/hostgroup.h>
#include <structures/interfaces.h>
#include <structures/item.h>
#include <structures/template.h>

#include <nlohmann/json.hpp>

namespace zab::mon
{
/// @brief Структура хоста
struct Host {
  /// @brief Имя
  std::string name;
  /// @brief id
  std::string id;
  /// @brief Список id групп, к которым привязан этот хост
  std::vector<std::string> groupsId;
  /// @brief Список id темплейтов, к которым привязан этот хост
  std::vector<std::string> templatesId;
  /// @brief Список интерфейсов для этого хоста
  std::vector<Interface> interfaces;
  /// @brief Конструктор
  explicit Host() = default;
  /// @brief Метод конвертации структуры в json формат
  /// @param host хост
  /// @return хост в json формате для дальнейших запросов
  static nlohmann::json toJson(const Host &host);
  static Host fromJson(const nlohmann::json &hostObj);
};
}  // namespace zab::mon

#endif  // HOST_H