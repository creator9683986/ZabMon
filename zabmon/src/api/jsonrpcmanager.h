#ifndef JSONRPCMANAGER_H
#define JSONRPCMANAGER_H

#include <nlohmann/json.hpp>

namespace zab::mon
{
/// @brief Класс отправки json запросов в zabbix api
class JsonRpcManager
{
 public:

  /// @brief Конструктор
  /// @param url - адрес zabbix сервера для запросов
  explicit JsonRpcManager(std::string url);

  /// @brief Деструктор
  ~JsonRpcManager() = default;

  /// @brief Создает JSON запрос
  /// @param method - метод zabbix для запроса
  /// @param params - параметры запроса для zabbix
  /// @param token - токен аунтификации для zabbix
  /// @return возвращает ответ zabbix (result) или ошибку
  [[nodiscard]] nlohmann::json send(const std::string &method,
                                    const nlohmann::json &params,
                                    const std::string &token) const;

 private:
  /// @brief Вспомогательный метод для создания JSON запроса
  static size_t writeCallback(void *contents, size_t size, size_t nmemb,
                              std::string *userp);
  std::string m_url;
};
}  // namespace zab::mon

#endif  // JSONRPCMANAGER_H