#ifndef MONITOR_H
#define MONITOR_H

#include <api/zabbixapi.h>

namespace zab::mon
{
/// @brief Класс логики
class Monitor
{
 public:

  /// @brief Конструктор
  explicit Monitor(std::shared_ptr<ZabbixApi> zabbix, std::string user,
                   std::string password);

  /// @brief Деструктор
  ~Monitor() = default;
  /// @brief Метод создания хоста
  /// @param host хост
  /// @return результат работы метода
  [[nodiscard]] std::string createHost(const Host &host) const;
  /// @brief Метод удаления хоста
  /// @param name имя хоста
  /// @return результат работы метода
  [[nodiscard]] bool deleteHost(const std::string &hostId) const;
  /// @brief Метод удаления группы
  /// @param name имя группы
  /// @return результат работы метода
  [[nodiscard]] bool deleteHostGroup(const std::string &groupId) const;
  /// @brief Метод удаления шаблона
  /// @param name имя шаблона
  /// @return результат работы метода
  [[nodiscard]] bool deleteTemplate(const std::string &hostId) const;
  /// @brief Метод удаления итема
  /// @param name имя итема
  /// @return результат работы метода
  [[nodiscard]] bool deleteItem(const std::string &itemId) const;

  /// @brief Метод обновления хоста
  /// @param host хост
  /// @return результат работы метода
  [[nodiscard]] std::string updateHost(const Host &host) const;

  /// @brief метод получения списка всех групп
  /// @return список всех групп
  [[nodiscard]] std::vector<HostGroup> getAllGroups() const;
  /// @brief метод получения всех хостов с фильтром
  /// @param filter фильтр по группам
  /// @return список всех хостов
  [[nodiscard]] std::vector<Host> getAllHosts(
      const std::vector<std::string> &filter) const;
  /// @brief метод получения всех шаблонов с фильтром
  /// @param filter фильтр
  /// @return список всех шаблонов
  [[nodiscard]] std::vector<Template> getAllTemplates(
      const std::vector<std::string> &filter) const;
  /// @brief метод получения всех итемов с фильтром
  /// @param filterGroups фильтр по группам
  /// @param filterTemplate фильтр по шаблонам
  /// @param filterHosts фильтр по хостам
  /// @return список всех итемов
  [[nodiscard]] std::vector<Item> getAllItems(
      const std::vector<std::string> &filterGroups,
      const std::vector<std::string> &filterTemplate,
      const std::vector<std::string> &filterHosts) const;
  /// @brief Метод создания группы
  /// @param hostGroup группа
  /// @return результат работы метода
  [[nodiscard]] std::string createGroup(const HostGroup &hostGroup) const;
  /// @brief Метод создания шаблона
  /// @param temp шаблон
  /// @return результат работы метода
  [[nodiscard]] std::string createTemplate(const Template &temp) const;
  /// @brief Метод создания итема
  /// @param item итем
  /// @return результат работы метода
  [[nodiscard]] std::string createItem(const Item &item) const;
  /// @brief Метод получения значений итемов
  /// @param items список итемов
  /// @param limit лимит для получения значений
  /// @return список значений итемов
  [[nodiscard]] std::vector<ItemValues> getListValues(
      const std::vector<Item> &items, int32_t limit) const;

 private:
  std::shared_ptr<ZabbixApi> m_zabbix;

  const std::string m_user;
  const std::string m_password;
};
}  // namespace zab::mon

#endif  // MONITOR_H