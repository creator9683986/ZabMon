#include <stcd-monitor/structures/template.h>

using namespace zab::mon;

Template::Template(std::string name, std::string id,
                   std::vector<std::string> groupsId)
    : name(std::move(name))
    , id(std::move(id))
    , groupsId(std::move(groupsId))

{
}

Template Template::fromJson(const nlohmann::json &templateObj)
{
  Template result;
  result.name = templateObj.at("name").get<std::string>();
  result.id = templateObj.at("templateid").get<std::string>();

  for(const nlohmann::json &gr : templateObj.at("groups")) {
    result.groupsId.push_back(gr.at("groupid").get<std::string>());
  }
  return result;
}

nlohmann::json Template::toJson(const Template &temp)
{
  nlohmann::json params;

  nlohmann::json groupsForTemplate = nlohmann::json::array();
  for(const std::string &id : temp.groupsId) {
    groupsForTemplate.push_back({{"groupid", id}});
  }
  params["groups"] = groupsForTemplate;

  return params;
}