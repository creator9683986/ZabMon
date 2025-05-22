#ifndef ZABBIXAPI_H
#define ZABBIXAPI_H

#include <stcd-monitor/api/jsonrpcmanager.h>
#include <stcd-monitor/structures/host.h>
#include <stcd-monitor/structures/hostgroup.h>
#include <stcd-monitor/structures/itemstatus.h>
#include <stcd-monitor/structures/itemvalues.h>

namespace zab::mon
{
/// @brief Класс вызова методов zabbix сервера
class ZabbixApi
{
 public:
  /// @brief Конструктор
  /// @param jsonManager - менеджер JSON-RPC запросов
  explicit ZabbixApi(std::shared_ptr<JsonRpcManager> jsonManager);
  /// @brief Деструктор
  ~ZabbixApi() = default;

  /// @brief метод получения токена для аунтификации
  /// @param user логин для zabbix сервера
  /// @param password пароль для zabbix сервера
  /// @return токен аунтификации
  [[nodiscard]] std::string getAuthToken(const std::string &user,
                                         const std::string &password) const;

  /// @brief метод создания хоста
  /// @param host структура хоста, со всеми полями для создания
  /// @param token токен аунтификации
  /// @return id созданного хоста
  [[nodiscard]] std::string createHost(const Host &host,
                                       const std::string &token) const;

  /// @brief метод создания группы
  /// @param host структура группы, со всеми полями для создания
  /// @param token токен аунтификации
  /// @return id созданного группы
  [[nodiscard]] std::string createGroup(const HostGroup &hostGroup,
                                        const std::string &token) const;

  /// @brief метод создания шаблона
  /// @param temp структура шаблона, со всеми полями для создания
  /// @param token токен аунтификации
  /// @return id созданного шаблона
  [[nodiscard]] std::string createTemplate(const Template &temp,
                                           const std::string &token) const;

  /// @brief метод создания итема
  /// @param temp структура итема, со всеми полями для создания
  /// @param token токен аунтификации
  /// @return id созданного итема
  [[nodiscard]] std::string createItem(const Item &item,
                                       const std::string &token) const;

  /// @brief метод удаления хоста
  /// @param hostId id хоста для удаления
  /// @param token токен аунтификации
  /// @return результат удаления
  [[nodiscard]] bool deleteHost(const std::string &hostId,
                                const std::string &token) const;

  /// @brief метод удаления группы
  /// @param hostId id группы для удаления
  /// @param token токен аунтификации
  /// @return результат удаления
  [[nodiscard]] bool deleteHostGroup(const std::string &hostGroupId,
                                     const std::string &token) const;

  /// @brief метод удаления шаблона
  /// @param hostId id шаблона для удаления
  /// @param token токен аунтификации
  /// @return результат удаления
  [[nodiscard]] bool deleteTemplate(const std::string &templateId,
                                    const std::string &token) const;

  /// @brief метод удаления итема
  /// @param hostId id итема для удаления
  /// @param token токен аунтификации
  /// @return результат удаления
  [[nodiscard]] bool deleteItem(const std::string &itemId,
                                const std::string &token) const;

  // FIXME: Не работает с интерфейсами
  /// @brief метод обновления хоста
  /// @param hostId id хоста для обновления
  /// @param token токен аунтификации
  /// @return id обновленного хоста
  [[nodiscard]] std::string updateHost(const Host &host,
                                       const std::string &token) const;

  /// @brief метод получения списка всех групп
  /// @param token токен аунтификации
  /// @return возвращает список групп
  [[nodiscard]] std::vector<HostGroup> getAllGroups(
      const std::string &token) const;

  /// @brief метод получения списка всех хостов
  /// @param token токен аунтификации
  /// @param groupsId фильтр по группам
  /// @return возвращает список хостов
  [[nodiscard]] std::vector<Host> getAllHosts(
      const std::string &token, const std::vector<std::string> &groupsId) const;

  /// @brief метод получения списка всех шаблонов
  /// @param token токен аунтификации
  /// @param groupsId фильтр по группам
  /// @return возвращает список шаблонов
  [[nodiscard]] std::vector<Template> getAllTemplates(
      const std::string &token, const std::vector<std::string> &groupsId) const;

  /// @brief метод получения списка всех итемов
  /// @param token токен аунтификации
  /// @param groupsId фильтр по группам
  /// @param tempId фильтр по шаблонам
  /// @param hostId фильтр по хостам
  /// @return возвращает список итемов
  [[nodiscard]] std::vector<Item> getAllItems(
      const std::string &token, const std::vector<std::string> &groupsId,
      const std::vector<std::string> &tempId,
      const std::vector<std::string> &hostId) const;

  /// @brief метод получения истории значений для элемента данных
  /// @param item структура итема
  /// @param limit максимальное количество значений для получения
  /// @param token токен аунтификации
  /// @return возвращает историю значений для определенного элемента данных
  [[nodiscard]] ItemValues getHistoryValue(const Item &item, const int &limit,
                                           const std::string &token) const;

  /// @brief метод получения статуса итема
  /// @param itemId id итема
  /// @param token токен аунтификации
  /// @return возвращает 0, если итем активен, 1, если недоступен
  [[nodiscard]] ItemStatus checkItem(const std::string &itemId,
                                     const std::string &token) const;

 private:
  std::shared_ptr<JsonRpcManager> m_jsonManager;
};
}  // namespace zab::mon

#endif  // ZABBIXAPI_H