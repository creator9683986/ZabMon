
#include <api/zabbixapi.h>
#include <exceptions.h>

#include <curl/curl.h>

#include <nlohmann/json.hpp>

using namespace zab::mon;

ZabbixApi::ZabbixApi(std::shared_ptr<JsonRpcManager> jsonManager)
    : m_jsonManager(std::move(jsonManager))

{
}

std::string ZabbixApi::getAuthToken(const std::string &user,
                                    const std::string &password) const
{
  nlohmann::json params;
  params["user"] = user;
  params["password"] = password;
  std::string method = "user.login";

  const nlohmann::json response = m_jsonManager->send(method, params, "");
  if(!response.is_string()) {
    throw InvalidResponseError("invalid auth token response");
  }

  return response.get<std::string>();
}

std::string ZabbixApi::createHost(const Host &host,
                                  const std::string &token) const
{
  nlohmann::json params{};
  params = Host::toJson(host);
  params["host"] = host.name;
  std::string method = "host.create";
  const nlohmann::json response = m_jsonManager->send(method, params, token);
  if(!response.is_object()) {
    throw InvalidResponseError("invalid create Host response");
  }
  std::vector<std::string> ids =
      response.at("hostids").get<std::vector<std::string>>();
  if(ids.size() != 1) {
    throw InvalidResponseError("Invalid host ids for create host");
  }
  return ids.front();
}

std::string ZabbixApi::createGroup(const HostGroup &hostGroup,
                                   const std::string &token) const
{
  nlohmann::json params{};
  params["name"] = hostGroup.name;
  std::string method = "hostgroup.create";
  const nlohmann::json response = m_jsonManager->send(method, params, token);
  if(!response.is_object()) {
    throw InvalidResponseError("Invalid type of response");
  }
  std::vector<std::string> ids =
      response.at("groupids").get<std::vector<std::string>>();
  if(ids.size() != 1) {
    throw InvalidResponseError("Invalid group ids for create group");
  }
  return ids.front();
}

std::string ZabbixApi::createTemplate(const Template &temp,
                                      const std::string &token) const
{
  nlohmann::json params{};
  params = Template::toJson(temp);
  params["host"] = temp.name;
  std::string method = "template.create";
  const nlohmann::json response = m_jsonManager->send(method, params, token);
  if(!response.is_object()) {
    throw InvalidResponseError("invalid create Template response");
  }
  std::vector<std::string> ids =
      response.at("templateids").get<std::vector<std::string>>();
  if(ids.size() != 1) {
    throw InvalidResponseError("Invalid template ids for create template");
  }
  return ids.front();
}

std::string ZabbixApi::createItem(const Item &item,
                                  const std::string &token) const
{
  nlohmann::json params{};
  params = Item::toJson(item);
  std::string method = "item.create";
  const nlohmann::json response = m_jsonManager->send(method, params, token);
  if(!response.is_object()) {
    throw InvalidResponseError("invalid create Item response");
  }
  std::vector<std::string> ids =
      response.at("itemids").get<std::vector<std::string>>();
  if(ids.size() != 1) {
    throw InvalidResponseError("Invalid item ids for create item");
  }
  return ids.front();
}

bool ZabbixApi::deleteHost(const std::string &hostId,
                           const std::string &token) const
{
  nlohmann::json params = std::vector<std::string>{hostId};
  std::string method = "host.delete";
  const nlohmann::json response = m_jsonManager->send(method, params, token);
  if(!response.is_object()) {
    throw InvalidResponseError("invalid delete Host response");
  }
  std::vector<std::string> ids =
      response.at("hostids").get<std::vector<std::string>>();

  return (ids.size() == 1 && ids.front() == hostId);
}

bool ZabbixApi::deleteItem(const std::string &itemId,
                           const std::string &token) const
{
  nlohmann::json params = std::vector<std::string>{itemId};
  std::string method = "item.delete";
  const nlohmann::json response = m_jsonManager->send(method, params, token);
  if(!response.is_object()) {
    throw InvalidResponseError("invalid delete Item response");
  }
  std::vector<std::string> ids =
      response.at("itemids").get<std::vector<std::string>>();

  return (ids.size() == 1 && ids.front() == itemId);
}

bool ZabbixApi::deleteHostGroup(const std::string &hostGroupId,
                                const std::string &token) const
{
  nlohmann::json params = std::vector<std::string>{hostGroupId};
  std::string method = "hostgroup.delete";
  const nlohmann::json response = m_jsonManager->send(method, params, token);
  if(!response.is_object()) {
    throw InvalidResponseError("invalid delete HostGroup response");
  }
  std::vector<std::string> ids =
      response.at("groupids").get<std::vector<std::string>>();

  return (ids.size() == 1 && ids.front() == hostGroupId);
}

bool ZabbixApi::deleteTemplate(const std::string &templateId,
                               const std::string &token) const
{
  nlohmann::json params = std::vector<std::string>{templateId};
  std::string method = "template.delete";
  const nlohmann::json response = m_jsonManager->send(method, params, token);
  if(!response.is_object()) {
    throw InvalidResponseError("invalid delete Template response");
  }
  std::vector<std::string> ids =
      response.at("templateids").get<std::vector<std::string>>();

  return (ids.size() == 1 && ids.front() == templateId);
}

std::string ZabbixApi::updateHost(const Host &host,
                                  const std::string &token) const
{
  nlohmann::json params{};
  params = Host::toJson(host);
  params.erase("interfaces");
  params["hostid"] = host.id;
  std::string method = "host.update";
  const nlohmann::json response = m_jsonManager->send(method, params, token);
  if(!response.is_object()) {
    throw InvalidResponseError("invalid update Host response");
  }
  for(const nlohmann::json &obj : response.at("hostids")) {
    return obj.get<std::string>();
  }
  throw InvalidResponseError("Invalid host ids for update host");
}

std::vector<HostGroup> ZabbixApi::getAllGroups(const std::string &token) const
{
  nlohmann::json params;
  params["output"] = std::vector<std::string>{"name", "groupid"};
  params["selectTemplates"] = std::vector<std::string>{"templateid"};
  std::string method = "hostgroup.get";
  const nlohmann::json response = m_jsonManager->send(method, params, token);
  if(!response.is_array()) {
    throw InvalidResponseError("invalid get all groups response");
  }

  std::vector<HostGroup> allGroups;
  for(const nlohmann::json &groupObj : response) {
    allGroups.emplace_back(HostGroup::fromJson(groupObj));
  }
  return allGroups;
}

std::vector<Host> ZabbixApi::getAllHosts(
    const std::string &token, const std::vector<std::string> &groupsId) const
{
  nlohmann::json params;
  params["output"] = std::vector<std::string>{"name", "hostid"};
  if(!groupsId.empty()) {
    params["groupids"] = groupsId;
  }
  params["selectParentTemplates"] = {"templateid"};
  params["selectGroups"] = std::vector<std::string>{"groupid"};
  params["selectInterfaces"] = std::vector<std::string>{
      "ip", "port", "dns", "type", "main", "useip", "interfaceid"};
  params["selectItems"] = {"name", "itemid"};
  std::string method = "host.get";
  const nlohmann::json response = m_jsonManager->send(method, params, token);
  if(!response.is_array()) {
    throw InvalidResponseError("invalid get all Hosts response");
  }
  std::vector<Host> allHosts;
  for(const nlohmann::json &hostObj : response) {
    Host host = Host::fromJson(hostObj);
    allHosts.emplace_back(host);
  }
  return allHosts;
}

std::vector<Template> ZabbixApi::getAllTemplates(
    const std::string &token, const std::vector<std::string> &groupsId) const
{
  nlohmann::json params;
  params["output"] = std::vector<std::string>{"templateid", "name"};
  if(!groupsId.empty()) {
    params["groupids"] = groupsId;
  }
  params["selectGroups"] = std::vector<std::string>{"groupid"};
  std::string method = "template.get";
  const nlohmann::json response = m_jsonManager->send(method, params, token);
  if(!response.is_array()) {
    throw InvalidResponseError("invalid get all templates response");
  }
  std::vector<Template> allTemplates;
  for(const nlohmann::json &tempObj : response) {
    Template temp = Template::fromJson(tempObj);
    allTemplates.push_back(temp);
  }
  return allTemplates;
}

std::vector<Item> ZabbixApi::getAllItems(
    const std::string &token, const std::vector<std::string> &groupsId,
    const std::vector<std::string> &tempId,
    const std::vector<std::string> &hostId) const
{
  nlohmann::json params;
  params["output"] = std::vector<std::string>{"name",       "key_",  "type",
                                              "value_type", "delay", "itemid"};
  if(!groupsId.empty()) {
    params["groupids"] = groupsId;
  }
  if(!tempId.empty()) {
    params["templateids"] = tempId;
  }
  if(!hostId.empty()) {
    params["hostids"] = hostId;
  }
  params["selectHosts"] = std::vector<std::string>{"hostid"};
  std::string method = "item.get";
  const nlohmann::json response = m_jsonManager->send(method, params, token);
  if(!response.is_array()) {
    throw InvalidResponseError("invalid get all items response");
  }
  std::vector<Item> allItems;
  for(const nlohmann::json &itemObj : response) {
    Item item = Item::fromJson(itemObj);
    allItems.emplace_back(item);
  }
  return allItems;
}

ItemValues ZabbixApi::getHistoryValue(const Item &item, const int &limit,
                                      const std::string &token) const
{
  nlohmann::json params;
  params["output"] = "extend";
  params["history"] = static_cast<int32_t>(item.valueType);
  params["sortfield"] = "clock";
  params["sortorder"] = "DESC";
  params["limit"] = limit;
  params["itemids"] = {std::stoi(item.id)};

  std::string method = "history.get";
  const nlohmann::json response = m_jsonManager->send(method, params, token);
  if(!response.is_array()) {
    throw InvalidResponseError("invalid get history value response");
  }
  ItemValues history;
  for(const nlohmann::json &valueObj : response) {
    Value value = Value::fromJson(valueObj);
    history.values.emplace_back(value);
  }
  history.item = item;
  return history;
}

ItemStatus ZabbixApi::checkItem(const std::string &itemId,
                                const std::string &token) const
{
  nlohmann::json params;
  params["output"] = std::vector<std::string>{"status"};
  params["itemids"] = std::stoi(itemId);
  std::string method = "item.get";
  const nlohmann::json response = m_jsonManager->send(method, params, token);
  if(!response.is_array()) {
    throw InvalidResponseError("invalid check item response");
  }
  if(response.size() != 1) {
    throw InvalidResponseError("Invalid item status for check item");
  }
  std::string statusStr = response.front().at("status").get<std::string>();
  return static_cast<ItemStatus>(std::stoi(statusStr));
}