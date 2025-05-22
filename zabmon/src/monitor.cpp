#include <exceptions.h>
#include <monitor.h>
#include <structures/host.h>
#include <structures/itemstatus.h>

using namespace zab::mon;

Monitor::Monitor(std::shared_ptr<ZabbixApi> zabbix, std::string user,
                 std::string password)
    : m_zabbix(std::move(zabbix))
    , m_user(std::move(user))
    , m_password(std::move(password))
{
}

std::string Monitor::createHost(const Host &host) const
{
  std::string token = m_zabbix->getAuthToken(m_user, m_password);
  return m_zabbix->createHost(host, token);
}

std::string Monitor::createGroup(const HostGroup &hostGroup) const
{
  std::string token = m_zabbix->getAuthToken(m_user, m_password);
  return m_zabbix->createGroup(hostGroup, token);
}

std::string Monitor::createTemplate(const Template &temp) const
{
  std::string token = m_zabbix->getAuthToken(m_user, m_password);
  return m_zabbix->createTemplate(temp, token);
}

std::string Monitor::createItem(const Item &item) const
{
  std::string token = m_zabbix->getAuthToken(m_user, m_password);
  return m_zabbix->createItem(item, token);
}

std::string Monitor::updateHost(const Host &host) const
{
  std::string token = m_zabbix->getAuthToken(m_user, m_password);
  return m_zabbix->updateHost(host, token);
}

bool Monitor::deleteHost(const std::string &hostId) const
{
  std::string token = m_zabbix->getAuthToken(m_user, m_password);
  return m_zabbix->deleteHost(hostId, token);
}

bool Monitor::deleteTemplate(const std::string &hostId) const
{
  std::string token = m_zabbix->getAuthToken(m_user, m_password);
  return m_zabbix->deleteTemplate(hostId, token);
}

bool Monitor::deleteHostGroup(const std::string &groupId) const
{
  std::string token = m_zabbix->getAuthToken(m_user, m_password);
  return m_zabbix->deleteHostGroup(groupId, token);
}

bool Monitor::deleteItem(const std::string &itemId) const
{
  std::string token = m_zabbix->getAuthToken(m_user, m_password);
  return m_zabbix->deleteItem(itemId, token);
}

std::vector<ItemValues> Monitor::getListValues(const std::vector<Item> &items,
                                               int32_t limit) const
{
  std::string token = m_zabbix->getAuthToken(m_user, m_password);
  std::vector<ItemValues> listValues;
  for(const Item &item : items) {
    if(m_zabbix->checkItem(item.id, token) != ItemStatus::Active) {
      throw ItemStatusError(
          "Cant get value of item " + item.name+"because it stopepd");
    }
    listValues.push_back(m_zabbix->getHistoryValue(item, limit, token));
  }
  return listValues;
}

std::vector<HostGroup> Monitor::getAllGroups() const
{
  std::string token = m_zabbix->getAuthToken(m_user, m_password);
  return m_zabbix->getAllGroups(token);
}

std::vector<Host> Monitor::getAllHosts(
    const std::vector<std::string> &filter) const
{
  std::string token = m_zabbix->getAuthToken(m_user, m_password);
  return m_zabbix->getAllHosts(token, filter);
}

std::vector<Item> Monitor::getAllItems(
    const std::vector<std::string> &filterGroups,
    const std::vector<std::string> &filterTemplate,
    const std::vector<std::string> &filterHosts) const
{
  std::string token = m_zabbix->getAuthToken(m_user, m_password);
  return m_zabbix->getAllItems(token, filterGroups, filterTemplate,
                               filterHosts);
}

std::vector<Template> Monitor::getAllTemplates(
    const std::vector<std::string> &filter) const
{
  std::string token = m_zabbix->getAuthToken(m_user, m_password);
  return m_zabbix->getAllTemplates(token, filter);
}
