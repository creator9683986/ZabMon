#include <stcd-monitor/structures/hostgroup.h>

using namespace zab::mon;

HostGroup::HostGroup(std::string name, std::string id,
                     std::vector<std::string> templatesIds)
    : name(std::move(name))
    , id(std::move(id))
    , templatesIds(std::move(templatesIds))
{
}

HostGroup HostGroup::fromJson(const nlohmann::json &groupObj)
{
  std::string name = groupObj.at("name").get<std::string>();
  std::string id = groupObj.at("groupid").get<std::string>();
  std::vector<std::string> templIds{};
  for(const nlohmann::json &templateObj : groupObj.at("templates")) {
    std::string templId = templateObj.at("templateid").get<std::string>();
    templIds.emplace_back(templId);
  }
  return HostGroup(name, id, templIds);
}