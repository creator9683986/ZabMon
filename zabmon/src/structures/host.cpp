#include <structures/host.h>

using namespace zab::mon;

nlohmann::json Host::toJson(const Host &host)
{
  nlohmann::json params;
  nlohmann::json groupsForHost = nlohmann::json::array();
  for(const std::string &id : host.groupsId) {
    groupsForHost.push_back({{"groupid", id}});
  }
  params["groups"] = groupsForHost;

  nlohmann::json templatesForHost = nlohmann::json::array();

  for(const std::string &id : host.templatesId) {
    templatesForHost.push_back({{"templateid", id}});
  }
  params["templates"] = templatesForHost;

  nlohmann::json interfacesForHost = nlohmann::json::array();

  for(const Interface &inter : host.interfaces) {
    interfacesForHost.push_back({{"ip", inter.ip},
                                 {"dns", inter.dns},
                                 {"port", inter.port},
                                 {"type", inter.type},
                                 {"main", inter.main},
                                 {"useip", inter.useip}});
  }

  params["interfaces"] = interfacesForHost;

  if(!host.id.empty()) {
    params["hostid"] = host.id;
  }

  return params;
}

Host Host::fromJson(const nlohmann::json &hostObj)
{
  Host host;
  host.name = hostObj.at("name").get<std::string>();
  host.id = hostObj.at("hostid").get<std::string>();

  for(const nlohmann::json &tempObj : hostObj.at("parentTemplates")) {
    host.templatesId.emplace_back(tempObj.at("templateid").get<std::string>());
  }

  for(const nlohmann::json &groupObj : hostObj.at("groups")) {
    host.groupsId.push_back(groupObj.at("groupid").get<std::string>());
  }

  for(const nlohmann::json &interObj : hostObj.at("interfaces")) {
    Interface interface = Interface::fromJson(interObj);
    host.interfaces.emplace_back(interface);
  }

  return host;
}