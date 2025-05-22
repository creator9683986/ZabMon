#ifndef INTERFACE_H
#define INTERFACE_H

#include <nlohmann/json.hpp>

namespace zab::mon
{
/// @brief Структура интерфейса
struct Interface {
  /// @brief ip агента
  std::string ip;
  /// @brief dns агента
  std::string dns;
  /// @brief порт агента
  std::string port;
  /// @brief тип интерфейса, 1 - agent, 2 - SNMP, 3 - IPMI, 4 - JMX
  int type = 1;
  /// @brief использовать ли дефолтный интерфейс, 0 - not default, 1 - default
  int main = 1;
  /// @brief способ подключения по ip, 1 - да, 0 - нет
  int useip = 1;
  /// @brief id интерфейса
  std::string id;
  /// @brief Конструктор
  /// @param ip ip агента
  /// @param dns dns агента
  /// @param port порт агента
  /// @param type тип интерфейса
  /// @param main использовать ли дефолтный интерфейс
  /// @param useip способ подключения
  /// @param id id интерфейса
  explicit Interface(std::string ip, std::string dns, std::string port,
                     int type, int main, int useip, std::string id);
  /// @brief Метод конвертации из json в структуру интерфейса
  /// @param interfaceObj json формат интерфейса
  /// @return структуру интерфейса
  static Interface fromJson(const nlohmann::json &interfaceObj);
};
}  // namespace zab::mon

#endif  // INTERFACE_H